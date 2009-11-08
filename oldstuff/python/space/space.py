#!/usr/bin/env python

import time
import string
import random
import pygame
from pygame.locals import *
from ship import Ship
from starfield import Starfield
from pygame.color import THECOLORS

def main():
	WINSIZE = 640,480	
	NUMBER_OF_STARS = 250
	STARTING_ANGLE = 180
  	CLOSEST_STAR_COLOR = (255, 255, 255)
	STAR_SIZE_IN_PIXELS = 1
	#_V_ = 1
	pygame.init()
	pygame.key.set_repeat(1,1)
	screen = pygame.display.set_mode(WINSIZE,pygame.SWSURFACE)
	stars = Starfield(screen,screen.get_rect(),NUMBER_OF_STARS,STARTING_ANGLE,(40,4),STAR_SIZE_IN_PIXELS,CLOSEST_STAR_COLOR)
	
	ship1 = Ship(screen)
	
	done = False
	while not done:
		screen.fill(THECOLORS["black"])
		stars.update()
		ship1.draw()
		ship1.resetSprt()
		pygame.display.update()
		events = pygame.event.get()
		for e in events:
			if(e.type == QUIT):
				done = True
				break
			elif (e.type == KEYDOWN):
				if(e.key == K_ESCAPE):
					done = True
					break
				#Test for movement
				dx,dy = 0,0
				if(e.key == K_DOWN):
					dy += 1
				if(e.key == K_UP):
					dy -= 1
				if(e.key == K_LEFT):
					dx -= 1
				if(e.key == K_RIGHT):
					dx += 1
				ship1.move(dx,dy)
				#if(e.key == K_PLUS):
					#stars.set_speed((
	print "Exiting!"
	
	return

if __name__ == "__main__":
	main()
	