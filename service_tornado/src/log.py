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
import os
import logging  
import logging.config

def init_log():
    logging.config.fileConfig('../conf/logging.conf') 

