from flask import Blueprint, jsonify, session, request, redirect, url_for
from flask_session import Session
from flask import render_template
import requests
import json
import KspReturns
import KspMongo
import time
import uuid

attendance_blueprint = Blueprint("attendance", __name__)
attendance_prefix = "/attendance"



kspMongo = KspMongo.KspMongo()

@attendance_blueprint.route(attendance_prefix + "/front", methods=['GET', 'POST'])
def attendance_front():
    if request.method == 'GET':
        return render_template("attendance_front.html")
    card_uuid = request.form.get("card_uuid", None)
    voornaam_letter = request.form.get("voornaam_letter",None)
    achternaam = request.form.get("achternaam", None)

    url_exists = url_for("attendance.attendance_exists", _external=True)
    url_exists += "?card_uuid={}".format(card_uuid)
    resp = requests.get(url_exists, verify=False).json()
    if resp['success'] == 'False':
        return KspReturns.return_front_message("Fout!", str(resp), 2, url_for("attendance.attendance_front"))
    if resp['data'] == 'False':
        #card not known
        url_new_approach = url_for("attendance.attendance_new_approach", _external=True)
        url_new_approach += "?card_uuid={}&voornaam_letter={}&achternaam={}".format(card_uuid, voornaam_letter, achternaam)
        resp = requests.get(url_new_approach, verify=False).json()
        if resp['success'] == 'True':
            msg = "Hallo!"
            if "klok_uit" in resp['data']:
                msg = "Doei! ({}s)".format(resp['data'].split("|")[1])
            return KspReturns.return_front_message("{}".format(msg), "{}.{} (sessie duurde: {}s)".format(voornaam_letter, achternaam,resp['data'].split("|")[1]),1, url_for("attendance.attendance_front"))
        else:
            return KspReturns.return_front_message("Fout!", str(resp), 1, url_for("attendance.attendance_front"))
    else:
        #card is known
        url_approach = url_for("attendance.attendance_approach", _external=True)
        url_approach += "?card_uuid={}".format(card_uuid)
        resp = requests.get(url_approach, verify=False).json()
        #return KspReturns.return_front_message("")
        if resp['success'] == 'True':
            msg = "Hallo!"
            if "klok_uit" in resp['data']:
                msg = "Doei! ({}s)".format(resp['data'].split("|")[1])
            return KspReturns.return_front_message("{}".format(msg), "{}.{}".format(voornaam_letter, achternaam),1, url_for("attendance.attendance_front"))
        else:
            return KspReturns.return_front_message("Fout!", str(resp), 1, url_for("attendance.attendance_front"))

    

@attendance_blueprint.route(attendance_prefix + "/connection")
def attendance_connection():
    return KspReturns.return_api(True, data="Connection ok!")

@attendance_blueprint.route(attendance_prefix + "/approach", methods=['GET'])
def attendance_approach():
    card_uuid = request.args.get("card_uuid", None)
    if card_uuid is None or len(card_uuid) == 0:
        return KspReturns.return_api(False, error="invalid card_uuid!")

    last_log = kspMongo.get_last_log(card_uuid)
    print(last_log)
    action = "klok_in"
    logged_time = ""
    if last_log is not None:
        if last_log['log_type'] == 'klok_in':
            action = "klok_uit"
            logged_time = str(round(time.time() - int(last_log['log_time'])))
            
    
    if not kspMongo.add_log(card_uuid, action, time.time()):
        return KspReturns.return_api(False, error="Something went wrong while adding log!")
    return KspReturns.return_api(True, data="{}|{}".format(action, logged_time))


@attendance_blueprint.route(attendance_prefix + "/exists")
def attendance_exists():
    card_uuid = request.args.get("card_uuid", None)
    if card_uuid is None or len(card_uuid) == 0:
        return KspReturns.return_api(False, error="invalid card_uuid!")

    if kspMongo._check_if_leerling_exists(card_uuid=card_uuid):
        return KspReturns.return_api(True, data="True")
    return KspReturns.return_api(True, data="False")

@attendance_blueprint.route(attendance_prefix + "/new_approach")
def attendance_new_approach():
    card_uuid = request.args.get("card_uuid", None)
    voornaam_letter = request.args.get("voornaam_letter", None)
    achternaam = request.args.get("achternaam", None)

    if card_uuid is None or \
        voornaam_letter is None or \
            achternaam is None:
            return KspReturns.return_api(False, error="Missing data!")
    
    if not kspMongo.add_leerling(voornaam_letter, achternaam, uuid.uuid4(), card_uuid):
        return  KspReturns.return_api(False, error="Error while adding user!")
    
    return redirect(url_for("attendance.attendance_approach") + "?card_uuid="+card_uuid)
