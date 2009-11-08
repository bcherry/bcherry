#!/usr/bin/env python

#Import Modules
import random
import os, pygame
from pygame.locals import *



#functions to create our resources
def load_image(name, colorkey=None):
    fullname = os.path.join('data', name)
    try:
        image = pygame.image.load(fullname)
    except pygame.error, message:
        print 'Cannot load image:', fullname
        raise SystemExit, message
    image = image.convert()
    if colorkey is not None:
        if colorkey is -1:
            colorkey = image.get_at((0,0))
        image.set_colorkey(colorkey, RLEACCEL)
    return image, image.get_rect()

def load_sound(name):
    class NoneSound:
        def play(self): pass
    if not pygame.mixer or not pygame.mixer.get_init():
        return NoneSound()
    fullname = os.path.join('data', name)
    try:
        sound = pygame.mixer.Sound(fullname)
    except pygame.error, message:
        print 'Cannot load sound:', fullname
        raise SystemExit, message
    return sound

class Ship(pygame.sprite.Sprite):
    """This class is for the players ship"""
    def __init__(self):
        pygame.sprite.Sprite.__init__(self) #call Sprite intializer
        self.image, self.rect = load_image('ship.bmp', -1)
        self.rect.center = (320,450)
        self.x_velocity = 0
        self.y_velocity = 0

    def update(self):
        self.rect.move_ip((self.x_velocity, self.y_velocity))
        
        if self.rect.left < 0:
            self.rect.left = 0
        elif self.rect.right > 640:
            self.rect.right = 640
            
        if self.rect.top <= 260:
            self.rect.top = 260
        elif self.rect.bottom >= 480:
            self.rect.bottom = 480
            
class Enemy(pygame.sprite.Sprite):
    """This class is for the enemy ships"""
    def __init__(self,startx):
        pygame.sprite.Sprite.__init__(self) #call Sprite intializer
        self.image, self.rect = load_image('eship.bmp', -1)
        self.rect.centerx = startx
        self.rect.centery = 120
        self.distance = 0
        self.x_velocity = 0
        self.y_velocity = 0
        
    def update(self):
        #movment
        if self.distance == 0:
            #random distance from 3 to 15 turns
            self.distance=random.randint(3,15)
            #random x,y velocity form -2 to 2
            self.x_velocity=random.randint(-2,2)
            self.y_velocity=random.randint(-2,2)       

        self.rect.move_ip((self.x_velocity, self.y_velocity))
        if self.rect.left < 0:
            self.rect.left = 0
        elif self.rect.right > 640:
            self.rect.right = 640
            
        if self.rect.top <= 0:
            self.rect.top = 0
        elif self.rect.bottom >= 220:
            self.rect.bottom = 220


        
        self.distance -= 1
        #random 1 - 60 determines if firing
        fire=random.randint(1,60)
        if fire == 1:
            ebomb_sprites.add(Ebomb(self.rect.midbottom))
            shot1.play()

class bomb(pygame.sprite.Sprite):
    """This class is for the players weapons"""
    def __init__(self, startpos):
        pygame.sprite.Sprite.__init__(self) #call Sprite intializer
        self.image, self.rect = load_image('fire.bmp', -1)
        self.rect.center = startpos
        
    def update(self):
        if self.rect.bottom <= 0:
            self.kill()
        else:
            self.rect.move_ip((0, -4))
        
class Ebomb(pygame.sprite.Sprite):
    """This class is for the players weapons"""
    def __init__(self, startpos):
        pygame.sprite.Sprite.__init__(self) #call Sprite intializer
        self.image, self.rect = load_image('efire.bmp', -1)
        self.rect.midtop = startpos
        
    def update(self):
        if self.rect.bottom >= 480:
            self.kill()
        else:
            self.rect.move_ip((0, 4))


def main():
    """this function is called when the program starts.
       it initializes everything it needs, then runs in
       a loop until the function returns."""
    random.seed()
    pygame.init()
    screen = pygame.display.set_mode((640, 480), FULLSCREEN)
    pygame.display.set_caption('Trekie Forever')
    

    #Create The Backgound
#    background = pygame.Surface(screen.get_size())
#    background = background.convert()
    background_image, background_rect = load_image('stars.bmp')
#    background.blit(background_image, (0,0))
#    background_image.convert()
    screen.blit(background_image, (0,0))

    pygame.mouse.set_visible(False)

    #Load music
    explode1 = load_sound("explode1.wav")
    explode2 = load_sound("explode2.wav")
    global shot1
    shot1 = load_sound("silent.wav")
    shot2 = load_sound("fx.wav")


    #Prepare the game objects
    numberof_hits = 0
    numberof_shots = 0 #only used for end of game stats
    enemy_killed = 0 #only used for end of game stats


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

        #add enemy
        counter += 1
        if counter >= 200:
            enemyship_sprites.add(Enemy(320))
            counter = 0
            
        #Clear Everything
        ebomb_sprites.clear(screen, background_image)
        enemyship_sprites.clear(screen, background_image)
        bomb_sprites.clear(screen, background_image)
        playership_sprite.clear(screen, background_image)



        #Update all sprites
        bomb_sprites.update()
        playership_sprite.update()
        ebomb_sprites.update()
        enemyship_sprites.update()
        
        #See if players bombs hit any enemy ships
        for hit in pygame.sprite.groupcollide(enemyship_sprites, bomb_sprites, 1, 1):
            explode1.play()
            enemy_killed += 1
        if enemyship_sprites.sprites() == []:
            print "You Win!!!!"
            print "Shot fired:",numberof_shots
            print "Hits taken:",numberof_hits
            print "Enemy killed", enemy_killed
            running = 0;

        #See if enemy bombs hit the player
        for hit in pygame.sprite.groupcollide(ebomb_sprites, playership_sprite, 1, 0).keys():
            numberof_hits += 1
            explode2.play()
        if numberof_hits == 300000:
            print "You lose :("
            print "Shot fired:",numberof_shots
            print "Hits taken:",numberof_hits
            print "Enemy killed", enemy_killed
            running = 0;
            
        #Draw Everything
        ebomb_sprites.draw(screen)
        bomb_sprites.draw(screen)
        enemyship_sprites.draw(screen)
        playership_sprite.draw(screen)
        
        pygame.display.flip()

    #Game is finnished wait 3 seconds before exiting
    pygame.time.delay(3000)
    screen = pygame.display.set_mode((640, 480))

#this calls the 'main' function when this script is executed
if __name__ == '__main__': main()
