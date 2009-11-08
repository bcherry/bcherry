import pygame
from pygame.locals import *

class Box:
	def __init__(self,screen,size,velocities,background,boxcolor):
		self.screen = screen
		screensize = self.screen.get_size()
		self.screenwidth = screensize[0]
		self.screenheight = screensize[1]
		#Position
		#Box will start roughly middle
		self.x = screensize[0]/2
		self.y = screensize[1]/2
		self.width = size[0]
		self.height = size[1]
		#Velocity
		self.vx = velocities[0]
		self.vy = velocities[1]
		self.bgcolor = background
		self.boxcolor = boxcolor
		self.rect = pygame.rect.Rect(self.x,self.y,self.width,self.height)
	
	def draw(self):
		#erase
		pygame.draw.rect(self.screen,self.bgcolor,self.rect)
		#update pos or reverse
		#check for collision:
		nx,ny = self.x+self.vx,self.y+self.vy
		bound_x = nx + self.width
		bound_y = ny + self.height
		if((bound_x >= self.screenwidth) or (nx <= 0)):
			self.vx *= -1 * 0.9
		else:
			self.x = nx
		if((bound_y >= self.screenheight) or (ny <= 0)):
			self.vy *= -1 * 0.9
		else:
			self.y = ny
		#Draw new box
		self.rect = pygame.rect.Rect(nx,ny,self.width,self.height)
		pygame.draw.rect(self.screen,self.boxcolor,self.rect)
	
	def setV(self,x,y):
		self.vx = x
		self.vy = y
	
	def setBackgroundColor(self,color):
		self.bgcolor = color
	def setBoxColor(self, color):
		self.boxcolor = color
