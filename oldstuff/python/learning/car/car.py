# INTIALISATION
import pygame, math, sys
from pygame.locals import *
screen = pygame.display.set_mode((1024, 768))
car = pygame.image.load('car.png')
clock = pygame.time.Clock()
k_up = k_down = k_left = k_right = 0
speed = direction = 0
position = (100, 100)
TURN_SPEED = 5
ACCELERATION = 2
MAX_FORWARD_SPEED = 10
MAX_REVERSE_SPEED = ­5
BLACK = (0,0,0)
while 1:
    # USER INPUT
    clock.tick(30)
    for event in pygame.event.get():
        if not hasattr(event, 'key'): continue
        down = event.type == KEYDOWN     # key down or up?
        if event.key == K_RIGHT: k_right = down * ­5
        elif event.key == K_LEFT: k_left = down * 5
        elif event.key == K_UP: k_up = down * 2
        elif event.key == K_DOWN: k_down = down * ­2
        elif event.key == K_ESCAPE: sys.exit(0)     # quit the game
    screen.fill(BLACK)
    # SIMULATION
    # .. new speed and direction based on acceleration and turn
    speed += (k_up + k_down)
    if speed > MAX_FORWARD_SPEED: speed = MAX_FORWARD_SPEED
    if speed < MAX_REVERSE_SPEED: speed = MAX_REVERSE_SPEED
    direction += (k_right + k_left)
    # .. new position based on current position, speed and direction
    x, y = position
    rad = direction * math.pi / 180
    x += ­speed*math.sin(rad)
    y += ­speed*math.cos(rad)
    position = (x, y)
    # RENDERING
    # .. rotate the car image for direction
    rotated = pygame.transform.rotate(car, direction)
    # .. position the car on screen
    rect = rotated.get_rect()
    rect.center = position
    # .. render the car to screen
    screen.blit(rotated, rect)
    pygame.display.flip()
