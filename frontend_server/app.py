import requests
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from time import sleep

driver = webdriver.Chrome()

base_addr = "https://127.0.0.1:8080/"

def go_to_main():
    driver.get(base_addr + "attendance/front")

def enter_data(card_uuid, voornaam_letter, achternaam):
    driver.execute_script("document.getElementById(\"card_uuid\").value = \"{}\"".format(card_uuid))
    driver.execute_script("document.getElementById(\"voornaam_letter\").value = \"{}\"".format(voornaam_letter))
    driver.execute_script("document.getElementById(\"achternaam\").value = \"{}\"".format(achternaam))
    
    driver.execute_script("document.getElementById(\"form\").submit();")

def find_between( s, first, last ):
    try:
        start = s.index( first ) + len( first )
        end = s.index( last, start )
        return s[start:end]
    except ValueError:
        return ""

import kspSerial
a = kspSerial.KspSerial("COM3", 115200)
print("open serial: ".format(a.open_port()))
a.send_message("server_ready")
sleep(3)

go_to_main()
while True:
    resp = a.read_message()
    if resp != "" and resp is not False and resp != "client_ready":
        print(resp)
        data = find_between(resp, "[","]")
        data_frames = data.split("|")
        achternaam = data_frames[0].split(",")[0]
        voornaam_letter = data_frames[0].split(",")[1].replace(".","")
        card_uuid = data_frames[1]
        print("achternaam: {}".format(achternaam))
        print("voornaam: {}".format(voornaam_letter))
        print("card uuid: {}".format(card_uuid))
        enter_data(card_uuid, voornaam_letter, achternaam)
