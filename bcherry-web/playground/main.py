#!/usr/bin/env python
import os
import wsgiref.handlers
from google.appengine.ext import webapp
from google.appengine.ext.webapp import template
import time

class MainHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'index.html')
		self.response.out.write(template.render(path, {}))

class SetTimeoutHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'settimeout.html')
		self.response.out.write(template.render(path, {}))

class ComparingComparisonsHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'comparisonperformance.html')
		self.response.out.write(template.render(path, {}))

class DefaultValuesHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'defaultvalues.html')
		self.response.out.write(template.render(path, {}))

class ChainingHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'chaining.html')
		self.response.out.write(template.render(path, {}))

class SpyConstructorsHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'spying-constructors.html')
		self.response.out.write(template.render(path, {}))


class KeyboardShortcutsHandler(webapp.RequestHandler):
	def get(self):
		path = os.path.join(os.path.dirname(__file__), 'keyboardshortcuts.html')
		self.response.out.write(template.render(path, {}))

class SaneHTML5HistoryHandler(webapp.RequestHandler):
	def get(self, *args):
		path = os.path.join(os.path.dirname(__file__), 'html5history.html')
		self.response.out.write(template.render(path, {}))

class PushStateImgHandler(webapp.RequestHandler):
  def get(self):
    time.sleep(1)
    self.response.headers["Content-Type"] = "image/jpeg"
    self.response.out.write('')

class AnyHandler(webapp.RequestHandler):
  def get(self, path):
    file_path = os.path.join(os.path.dirname(__file__), path + '.html')
    self.response.out.write(open(file_path).read())


def main():
	application = webapp.WSGIApplication([
		('/playground', MainHandler),
		('/playground/settimeout', SetTimeoutHandler),
		('/playground/comparisons', ComparingComparisonsHandler),
		('/playground/defaultvalues', DefaultValuesHandler),
		('/playground/spying-constructors', SpyConstructorsHandler),
		('/playground/keyboard-shortcuts', KeyboardShortcutsHandler),
		('/playground/pushstate.jpg', PushStateImgHandler),
    ('/playground/sanerhtml5history(/(.+)?)?', SaneHTML5HistoryHandler),
		('/playground/(.*)', AnyHandler),
	], debug=True)
	wsgiref.handlers.CGIHandler().run(application)

if __name__ == '__main__':
	main()

