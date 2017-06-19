"""
fetch to/ insert from mongodb

no authentication for now.
"""

import pymongo


class DB:
    def __init__(self, url, port, db_name):
        self.url = url
        self.port = port
        self.db_name = db_name
        self.conn = pymongo.MongoClient(url, port)
        self.db = self.conn.get_database(self.db_name)

    def find(self, collection, key, value):
        col = self.db.get_collection(collection)
        docs = col.find({key: value})
        return docs

    def insert(self, collection, json_doc):
        col = self.db.get_collection(collection)
        col.insert(json_doc)
        return
