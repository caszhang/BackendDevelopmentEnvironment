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
import logging


logger = logging.getLogger('foxea') 

class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("hello foxea");
        logger.info("get called")
