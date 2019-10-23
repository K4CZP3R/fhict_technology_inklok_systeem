from flask import Blueprint, jsonify, session, render_template, request, url_for, flash, redirect, send_file
from flask_session import Session
from werkzeug.utils import secure_filename
import json
import KspReturns
import KspMongo
import os
import os.path
from datetime import datetime

admin_blueprint = Blueprint("admin", __name__)
admin_prefix = "/admin"

kspMongo = KspMongo.KspMongo()

@admin_blueprint.route(admin_prefix)
def admin():
    tools = [
        {
            "title": "Leerlingen",
            "description": "Zie alle leerlingen",
            "url": url_for("admin.admin_leerlingen"),
            "img_name": "students.png"
        },
        {
            "title": "Leerling toevoegen",
            "description": "Voeg een nieuwe leerling toe",
            "url": url_for("admin.admin_add_leerling"),
            "img_name": "add.png"
        },
        {
            "title": "Aanwezigheid (vandaag)",
            "description": "Zie aanwezigheid voor vandaag",
            "url": url_for("admin.admin_today"),
            "img_name": "attendance.png"
        },
        {
            "title": "Attendance front-end",
            "description": "Inlog scherm voor leerlingen",
            "url": url_for("attendance.attendance_front"),
            "img_name": "login.png"
        }
    ]
    return render_template('dashboard.html', tools=tools)

@admin_blueprint.route(admin_prefix + "/avatar/<string:uuid>", methods=['POST', 'GET'])
def admin_avatar(uuid):
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['file']
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        if file:
            uuid = secure_filename(uuid)
            file.save(os.path.join("uploads", uuid+".png"))
            return KspReturns.return_front_message("Photo OK!", "Photo uploaded!", 1, url_for("admin.admin_leerlingen"))
    if request.method == 'GET':
        if os.path.isfile("uploads/{}.png".format(uuid)):
            return send_file('uploads/{}.png'.format(uuid), mimetype='image/png')
        return send_file('uploads/default.png', mimetype='image/png')


@admin_blueprint.route(admin_prefix + "/add_leerling", methods=['POST', 'GET'])
def admin_add_leerling():
    if request.method == 'GET':
        return render_template("leerling.html",
                               leerling={'uuid':'none'},
                               leerling_entries=KspMongo.leerling_entries,
                               photo_url=admin_prefix+"/avatar")
    if request.method == 'POST':
        for entry in KspMongo.leerling_entries:
            if entry not in KspMongo.leerling_entries:
                return KspReturns.return_front_message("invalid post data", "try again", 3, url_for("admin.admin_add_leerling"))
        if not kspMongo.add_leerling_from_dict(request.form):
            return KspReturns.return_front_message("Error", "Error while adding user!", 3, "")
        return KspReturns.return_front_message("Ok!", "Leerling is toegevoegd!", 2, url_for("admin.admin_leerlingen"))


@admin_blueprint.route(admin_prefix + "/leerlingen", methods=['POST', 'GET'])
def admin_leerlingen():
    if request.method == 'GET':
        return render_template('leerlingen.html',
                               leerlingen=kspMongo.get_all_leerlingen(),
                               edit_url=admin_prefix + "/edit_leerling",
                               add_url=admin_prefix+"/add_leerling",
                               photo_url=admin_prefix+"/avatar",
                               logs_url=admin_prefix+"/leerling_logs")
    return render_template('test.html')


@admin_blueprint.route(admin_prefix + "/remove_leerling/<string:uuid>", methods=['GET'])
def admin_remove_leerling(uuid):
    if not kspMongo.remove_leerling(uuid):
        return KspReturns.return_front_message("Error", "Error while removing user! Try again!", "2", "")
    return KspReturns.return_front_message("Removed!", ":)", 1, url_for("admin.admin_leerlingen"))


@admin_blueprint.route(admin_prefix + "/edit_leerling/<string:uuid>", methods=['POST', 'GET'])
def admin_edit_leerling(uuid):
    if request.method == 'GET':
        leerling = kspMongo.get_leerling(uuid=uuid)
        if leerling is None:
            return KspReturns.return_front_message("Error", "User does not exist!", 3, url_for("admin.admin_leerlingen"))

        return render_template("leerling.html",
                               leerling=kspMongo.get_leerling(uuid=uuid),
                               leerling_entries=KspMongo.leerling_entries,
                               remove_url=admin_prefix+"/remove_leerling",
                               photo_url=admin_prefix+"/avatar")
    if request.method == 'POST':
        for entry in request.form:
            if entry not in KspMongo.leerling_entries:
                return KspReturns.return_front_message("invalid post data", "try again", 3, url_for("admin.admin_leerlingen"))

        if not kspMongo.update_leerling_from_dict(request.form):
            return KspReturns.return_front_message("Error", "Error while updating user!", 3, "")
        return KspReturns.return_front_message("Updated!", ":)", 1, "")


@admin_blueprint.route(admin_prefix + "/leerling_logs/<string:card_uuid>")
def admin_leerling_logs(card_uuid):
    logs = kspMongo.get_all_logs(card_uuid)
    return render_template('logs.html', logs=logs)

def leerling_aanwezig(leerling, logs):
    for log in logs:
        print(log)
        if leerling['card_uuid'] == log['card_uuid']:
            return True
    return False

@admin_blueprint.route(admin_prefix + "/day/<string:year>/<string:month>/<string:day>")
def admin_day(year, month, day):
    leerlingen = kspMongo.get_all_leerlingen()
    le_list = list()
    for leerling in leerlingen:
        le_list.append(leerling)
    logs = kspMongo.get_logs_from(None, year, month, day)
    lo_list = list()
    for log in logs:
        lo_list.append(log)

    aanwezig = list()
    afwezig = list()

    for leerling in le_list:
        if(leerling_aanwezig(leerling, lo_list)):
            aanwezig.append(leerling)
        else:
            afwezig.append(leerling)
    
    return render_template('today_calendar.html', aanwezig=aanwezig, afwezig=afwezig, d=day, m=month, y=year)

@admin_blueprint.route(admin_prefix + "/today")
def admin_today():
    leerlingen = kspMongo.get_all_leerlingen()
    le_list = list()
    for leerling in leerlingen:
        le_list.append(leerling)
    
    logs = kspMongo.get_logs_from(None, datetime.now().year, datetime.now().month, datetime.now().day)
    lo_list = list()
    for log in logs:
        lo_list.append(log)
    

    aanwezig = list()
    afwezig = list()

    for leerling in le_list:
        if(leerling_aanwezig(leerling, lo_list)):
            aanwezig.append(leerling)
        else:
            afwezig.append(leerling)


    return render_template('today_calendar.html', aanwezig=aanwezig, afwezig=afwezig,d=datetime.now().day, m=datetime.now().month, y=datetime.now().year)