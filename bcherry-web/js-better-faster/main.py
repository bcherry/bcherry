#!/usr/bin/env python
import os
import wsgiref.handlers
from google.appengine.ext import webapp
from google.appengine.ext.webapp import template

class MainHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'index.html')
		self.response.out.write(template.render(path, {}))

class SlidesHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'slides.html')
		self.response.out.write(template.render(path, {}))

def main():
	application = webapp.WSGIApplication([
		('/js-better-faster/slides', SlidesHandler),
		('/js-better-faster', MainHandler),
	], debug=True)
	wsgiref.handlers.CGIHandler().run(application)

if __name__ == '__main__':
	main()

