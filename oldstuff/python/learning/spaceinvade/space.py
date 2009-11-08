#!/usr/bin/env python
import random, os
import pygame
from pygame.locals import *

def load_image(file_name,colorkey=None):
	full_name = os.path.join('data',file_name)
	try:
		image = pygame.image.load(full_name)
	except pygame.error, message:
		print 'Cannot load image:',full_name
		raise SystemExit,message
	image = image.convert()
	if colorkey is not None:
		if colorkey is -1:
			colorkey = image.get_at((0,0))
		image.set_colorkey(colorkey,RLEACCEL)
	return image,image.get_rect()

def load_sound(name):
	class No_Sound:
		def play(self): pass
	if not pygame.mixer or not pygame.mixer.get_init():
		return No_Sound
	full_name = os.path.join('data',name)
	if os.path.exists(full_name) == False:
		sound = pygame.mixer.Sound(full_name)
	else:
		print 'File does not exist:',full_name
		return No_Sound
	return sound

class Ship(pygame.sprite.Sprite):
	"""This class is for the player's ship"""
	def __init__(self):
		pygame.sprite.Sprite.__init__(self)
		self.image,self.rect = load_image('ship.bmp',-1)
		self.rect.center = (320,450)
		self.x_velocity = 0
		self.y_velocity = 0
	
	def update(self):
		self.rect.move_ip((self.x_velocity,self.y_velocity))
		if self.rect.left < 0:
			self.rect.left = 0
		elif self.rect.right > 640:
			self.rect.right = 640
		if self.rect.top <= 260:
			self.rect.top = 260
		elif self.rect.bottom >= 480:
			self.rect.bottom = 480

class Enemy(pygame.sprite.Sprite):
	"""This class is for Enemy ships"""
	def __init__(self,startx):
		pygame.sprite.Sprite.__init__(self)
		self.image,self.rect = load_image('eship.bmp',-1)
		self.rect.centerx = startx
		self.rect.centery = 120
		self.distance = 0
		self.x_velocity = 0
		self.y_velocity = 0
	
	def update(self):
		if self.distance == 0:
			self.distance = random.randint(3,15)
			self.x_velocity,self.y_velocity = random.randint(-2,2),random.randint(-2,2)
		self.rect.move_ip((self.x_velocity,self.y_velocity))
		self.distance -= 1
		if self.rect.left < 0:
			self.rect.left = 0
		elif self.rect.right > 640:
			self.rect.right - 640
		if self.rect.top < 0:
			self.rect.top = 0
		elif self.rect.bottom > 220:
			self.rect.bottom = 220
		fire = random.randint(1,60)
		if fire == 1:
			ebomb_sprites.add(Ebomb(self.rect.midbottom))
			shot1.play()

class bomb(pygame.sprite.Sprite):
	"""This class is for the player's weapons"""
	def __init__(self,startpos):
		pygame.sprite.Sprite.__init__(self)
		self.image,self.rect = load_image('fire.bmp',-1)
		self.rect.center = startpos
	def update(self):
		if self.rect.bottom <= 0:
			self.kill()
		else:
			self.rect.move_ip((0,-4))

class Ebomb(pygame.sprite.Sprite):
	"""This class is for the enemy's weapons"""
	def __init__(self,startpos):
		pygame.sprite.Sprite.__init__(self)
		self.image,self.rect = load_image('efire.bmp',-1)
		self.rect.midtop = startpos
	def update(self):
		if self.rect.bottom >= 480:
			self.kill()
		else:
			self.rect.move_ip((0,4))

def main():
	random.seed()
	pygame.init()
	
	screen = pygame.display.set_mode((640,480))
	pygame.display.set_caption('Space Game')
	pygame.mouse.set_visible(False)
	
	background_image,background_rect = load_image('stars.bmp')
	screen.blit(background,(0,0))
	
	explode1 = load_sound('explode1.wav')
	explode2 = load_sound('explode2.wav')
	global shot1
	shot1 = load_sound('silent.wav')
	shot2 = load_sound('fx.wav')
	
	numberof_hits = 0
	numberof_shots = 0
	enemy_killed = 0
	
	ship = Ship()
	playership_sprite = pygame.sprite.RenderClear(ship)
	bomb_sprites = pygame.sprite.RenderClear()
	enemyship_sprites = pygame.sprite.RenderClear()
	enemyship_sprites.add(Enemy(212))
	enemyship_sprites.add(Enemy(320))
	enemyship_sprites.add(Enemy(428))
	global ebomb_sprites
	ebomb_sprites = pygame.sprite.RenderClear()
	
	running = 1
	counter = 0
	while running:
		pygame.time.delay(10)
		
		for event in pygame.event.get():
			if event.type == QUIT:
				running = 0
			elif event.type == KEYDOWN:
				if event.key == K_ESCAPE:
					running = 0
				elif event.key == K_LEFT:
					ship.x_velocity = -2
				elif event.key == K_RIGHT:
					ship.x_velocity = 2
				elif event.key == K_UP:
					ship.y_velocity = -2
				elif event.key == K_DOWN:
					ship.y_velocity = 2
				elif event.key == K_f:
					bomb_sprites.add(bomb(ship.rect.midtop))
					numberof_shots += 1
					shot2.play()
			elif event.type == KEYUP:
				if event.key == K_LEFT:
					ship.x_velocity = 0
				elif event.key == K_RIGHT:
					ship.x_velocity = 0
				elif event.key == K_UP:
					ship.y_velocity = 0
				elif event.key == K_DOWN:
					ship.y_velocity = 0
		
		counter += 1
		if counter >= 200:
			enemyship_sprites.add(Enemy(320))
			counter = 0
		
		ebomb_sprites.clear(screen,background_image)
		enemyship_sprites.clear(screen,background_image)
		bomb_sprites.clear(screen,background_image)
		playership_sprite.clear(screen,background_image)
		
		bomb_sprites.update()
		ebomb_sprites.update()
		enemyship_sprites.update()
		playership_sprite.update()
		
		for hit in pygame.sprite.groupcollide(enemyship_sprites,bomb_sprites,1,1):
			explode1.play()
			enemy_killed += 1
		
		if enemyship_sprites.sprites() == []:
			print 'You Win!!!'
			print 'Shots fired:',numberof_shots
			print 'Hits taken:',numberof_hits
			print 'Enemies killed:',enemy_killed
			running = 0
		
		for hit in pygame.sprite.groupcollide(ebomb_sprites,playership_sprite,1,0).keys():
			numberof_hits += 1
			explode2.play()
		if numberof_hits >= 3:
			print 'You lose :('
			print 'Shots fired:',numberof_shots
			print 'Hits taken:',numberof_hits
			print 'Enemies killed:',enemy_killed
			running = 0
		
		ebomb_sprites.draw(screen)
		bomb_sprites.draw(screen)
		enemyship_sprites.draw(screen)
		playership_sprite.draw(screen)
		pygame.display.flip()
	
	pygame.time.delay(3000)
	screen = pygame.display.set_mode((640, 480))	


if __name__ == '__main__': main()
