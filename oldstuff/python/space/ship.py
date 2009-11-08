import pygame
from pygame.locals import *
import os
#from pygame.color import THECOLORS

VX,VY = 2,2

class Ship:
	def __init__(self,screen):
		self.screen = screen
		screensize = self.screen.get_size()
		self.sprt_still = pygame.image.load(os.path.join("img","ship.gif"))
		self.sprt_up = pygame.image.load(os.path.join("img","ship_up.gif"))
		self.sprt_dn = pygame.image.load(os.path.join("img","ship_dn.gif"))
		self.sprite = self.sprt_still
		self.rect = self.sprite.get_rect(center=(screensize[0]/2,screensize[1]/2))
	
	def draw(self):
		#pygame.draw.rect(self.screen,THECOLORS["black"],self.rect)
		self.screen.blit(self.sprite,self.rect)
		for e in extras:
			e.draw()
	
	def resetSprt(self):
		self.sprite = self.sprt_still
	
	def move(self,dx,dy):
		if(dy<0):
			self.sprite = self.sprt_up
		if(dy>0):
			self.sprite = self.sprt_dn
		new_rect = self.rect.move(VX*dx,VY*dy)
		if(self.screen.get_rect().contains(new_rect)):
			self.rect = new_rect
	
	def fire(self):
		e.extend(
		
