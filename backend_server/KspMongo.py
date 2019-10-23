import pymongo
from enum import Enum
from datetime import datetime
leerling_entries = ["voornaam_letter", "achternaam", "uuid", "card_uuid"]
log_entries = ["card_uuid", "log_type", "time"]


class KspMongo:
    def __init__(self):
        self.client = pymongo.MongoClient("mongodb://localhost:27017")
        self.database = self.client['tech_attendance_system']

        self.leerlingen = self.database["leerlingen"]
        self.logs = self.database['logs']

    def add_log(self, card_uuid, log_type, log_time):
        log_dict = {
            "card_uuid": card_uuid,
            "log_type": log_type,
            "log_time": log_time,

            "year": datetime.fromtimestamp(log_time).year,
            "month": datetime.fromtimestamp(log_time).month,
            "day": datetime.fromtimestamp(log_time).day,
            
        }

        self.logs.insert_one(log_dict)
        return True

    def get_all_logs(self, card_uuid):
        query = {'card_uuid': card_uuid}
        return self.logs.find(query).sort("log_time", -1)

    def get_logs_from(self, card_uuid, year, month, day):
        query = {'year':year,'month':month, 'day':day}
        if card_uuid is not None:
            query['card_uuid'] = card_uuid
        return self.logs.find(query).sort("log_time", -1)

    def get_last_log(self, card_uuid=None):
        if card_uuid is None:
            return False

        query = {'card_uuid': card_uuid}

        for i in self.logs.find(query).sort("log_time",-1):
            return i

    def remove_leerling(self, uuid=None, card_uuid=None):
        if card_uuid is None and uuid is None:
            return False

        if card_uuid is not None:
            query = {'card_uuid': card_uuid}
        if uuid is not None:
            query = {'uuid': uuid}

        self.leerlingen.delete_one(query)
        return True

    def add_leerling_from_dict(self, add_dict):
        return self.add_leerling(add_dict['voornaam_letter'], add_dict['achternaam'], add_dict['uuid'], add_dict['card_uuid'])

    def add_leerling(self, voornaam_letter, achternaam, uuid, card_uuid):
        leerling_dict = {
            "voornaam_letter": voornaam_letter,
            "achternaam": achternaam,
            "uuid": str(uuid),
            "card_uuid": str(card_uuid)
        }

        if self._check_if_leerling_exists(card_uuid=card_uuid, uuid=uuid) is True:
            return False

        self.leerlingen.insert_one(leerling_dict)
        return True

    def update_leerling_from_dict(self, update_dict):
        return self.update_leerling(update_dict['voornaam_letter'], update_dict['achternaam'], update_dict['uuid'], update_dict['card_uuid'])

    def update_leerling(self, voornaam_letter, achternaam, uuid, card_uuid):
        query = {"uuid": uuid}
        update_dict = {}
        if voornaam_letter is not None:
            update_dict["voornaam_letter"] = voornaam_letter
        if achternaam is not None:
            update_dict["achternaam"] = achternaam
        if uuid is None:
            return False

        if card_uuid is not None:
            update_dict["card_uuid"] = card_uuid
        new_values = {"$set": update_dict}

        self.leerlingen.update_one(query, new_values)
        return True

    def get_leerling(self, card_uuid=None, uuid=None):
        if card_uuid is None and uuid is None:
            return None

        if uuid is not None:
            query = {"uuid": str(uuid)}
        if card_uuid is not None:
            query = {"card_uuid": str(card_uuid)}

        for i in self.leerlingen.find(query):
            return i

    def get_all_leerlingen(self):
        return self.leerlingen.find()

    def _check_if_leerling_exists(self, card_uuid=None, uuid=None):
        if card_uuid is None and uuid is None:
            return None

        if uuid is not None:
            query = {"uuid": str(uuid)}
        if card_uuid is not None:
            query = {"card_uuid": str(card_uuid)}

        print(self.leerlingen.find(query).count())
        if self.leerlingen.find(query).count() > 0:
            return True
        return False
