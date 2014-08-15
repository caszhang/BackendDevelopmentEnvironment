'''
/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-03-02 16:05
 * Filename :        main.py
 * Description :     
 * *****************************************************************************/
 '''
#! /usr/bin/python
# -*- coding:utf-8 -*-
import tornado.httpclient 
import tornado.gen
import urllib 
import json 
import datetime
import time
import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
import logging

import os
import re
import sys

from config import config
from mysql import Connection
from index_handler import IndexHandler

class Application(tornado.web.Application):
    def __init__(self):
        #self.db = Mysql(config["db_host"], config["db_user"], config["db_password"], config["db_port"], config["db_database"])
        self.db = Connection('%s:%d' % (config["db_host"], config["db_port"]), config["db_database"], config["db_user"], config["db_password"])


        handlers = [
            (r'/', IndexHandler),
        ]

        settings = {
            'template_path': 'templates',
            'static_path': 'static',
            #'ui_modules' :  {'Book' : BookModule},
            'debug' : True,
            "cookie_secret": "DK2Y33AGTDeVX19jf4Lvtw6Bx3+BEU4QqwCXB5hXMLs=",
        }

        tornado.web.Application.__init__(self, handlers, **settings)

