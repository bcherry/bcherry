#!/usr/bin/env python
import os
import wsgiref.handlers
from google.appengine.ext import webapp
from google.appengine.ext.webapp import template

class MainHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'index.html')
		self.response.out.write(template.render(path, {}))

class TalkHandler(webapp.RequestHandler):
	def get(self, talk_name):
		path = os.path.join(os.path.dirname(__file__), 'talk.html')
		self.response.out.write(template.render(path, dict(talk_name=talk_name)))

class SlidesHandler(webapp.RequestHandler):
	def get(self, talk_name):
		path = os.path.join(os.path.dirname(__file__), talk_name + '/slides.html')
		self.response.out.write(template.render(path, {}))

def main():
	application = webapp.WSGIApplication([
		('/talks/(.+)/slides/?', SlidesHandler),
		('/talks/(.+)', TalkHandler),
		('/talks/?', MainHandler),
	], debug=True)
	wsgiref.handlers.CGIHandler().run(application)

if __name__ == '__main__':
	main()

