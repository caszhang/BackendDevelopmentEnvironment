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

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
import os, sys
sys.path.append("..")
from conf.conf import http_config as http_config

from tornado.options import define, options
define("port", default=http_config["port"], help="run on the given port", type=int)

from application import Application
import logging 
from log import init_log

if __name__ == '__main__':
    init_log()
    foxea_logger = logging.getLogger('foxea')  
    foxea_logger.info('foxea torando web start') 

    tornado.options.parse_command_line()
    application = Application()
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
