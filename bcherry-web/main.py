#!/usr/bin/env python
import os
import wsgiref.handlers
from google.appengine.api import xmpp
from google.appengine.ext import webapp
from google.appengine.ext.webapp import template

class MainHandler(webapp.RequestHandler):
	def get(self):
		#user_address = 'bcherry@gmail.com'
		#msg = "hello from web"
		#status_code = xmpp.send_message(user_address, msg)
		#chat_message_sent = (status_code != xmpp.NO_ERROR)
		#self.response.out.write("status code is %s, chat_message_sent is %s, presence is %s" % (status_code, chat_message_sent, xmpp.get_presence(user_address)))
		
		path = os.path.join(os.path.dirname(__file__), 'index.html')
		self.response.out.write(template.render(path, {}))

def main():
	application = webapp.WSGIApplication([('/', MainHandler)], debug=True)
	wsgiref.handlers.CGIHandler().run(application)

if __name__ == '__main__':
	main()

