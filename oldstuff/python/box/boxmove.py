import time
import string
import random

import pygame
from pygame.locals import *
from pygame.color import THECOLORS

import box
from starfield import *
#from box import *

def main():
	WINSIZE = 640,480
	pygame.init()
	#screen = pygame.display.set_mode(WINSIZE,0,8)
	#pygame.display.set_caption('Move the Box!')
	
	NUMBER_OF_STARS = 250
	STARTING_ANGLE = 180
  	CLOSEST_STAR_COLOR = (255, 255, 255)
	STAR_SIZE_IN_PIXELS = 2

	# A few preset speeds for the user to see.
	
	preset_speeds = ( (10, 1), (20, 2), ( 40, 4), (60, 6), (80, 8) )
	current_speed = 2
	
	# For fullscreen, replace pygame.SWSURFACE with pygame.FULLSCREEN
	
	display = pygame.display.set_mode((640, 480), pygame.SWSURFACE)
	
	my_starfield = Starfield(display, display.get_rect(), NUMBER_OF_STARS,
				STARTING_ANGLE, preset_speeds[current_speed],
				STAR_SIZE_IN_PIXELS, CLOSEST_STAR_COLOR)
		
		#screen.fill(THECOLORS["black"])
	
	box1 = box.Box(display,(32,32),(1,1),THECOLORS["black"],THECOLORS["white"])
	
	done = False
	while not done:
		my_starfield.update()
		box1.draw()
		pygame.display.update()
		events = pygame.event.get()
		for e in events:
			if(e.type == QUIT):
				done = True
				break
			elif (e.type == KEYDOWN):
				dx,dy = 0,0
				
				if(e.key == K_ESCAPE):
					done = True
					break
				if(e.key == K_f):
					pygame.display.toggle_fullscreen()
				if(e.key == K_DOWN):
					dy += 1
				if(e.key == K_UP):
					dy -= 1
				if(e.key == K_RIGHT):
					dx += 1
				if(e.key == K_LEFT):
					dx -= 1
				
				box1.chV(dx,dy)
	print "Exiting!"
	
	return

if __name__ == "__main__":
	main()
	