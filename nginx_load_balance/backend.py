#coding=utf-8

import re
import time
import hashlib
import requests
import json
import urllib2
import tornado.web
import tornado.ioloop
import logging
from tornado.options import define, options, parse_command_line

define('port', default=8989, help='run on the port', type=int)

class HelloHandler(tornado.web.RequestHandler):
	def get(self):
		greeting = self.get_argument('greeting', 'Hello')
		print 'get:', greeting
		self.write("hello, world\n")

	def post(self):
		d = self.request.body
		print 'post:', d
		self.write("hello, world\n")

class PingHandler(tornado.web.RequestHandler):
	def get(self):
		arg = self.get_argument('ping', 'PING')
		print 'ping:', arg 
		self.write("pong\n")

	def post(self):
		d = self.request.body
		print 'post:', d 
		self.write("pong\n")

if __name__ == '__main__':
    parse_command_line()
    app = tornado.web.Application([(r'/hello', HelloHandler), (r'/ping', PingHandler), ], )
    app.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
