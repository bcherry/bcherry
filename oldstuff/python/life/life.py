"""
Game of Life Engine
Mike Steder

Randomly generates Game of Life boards and displays them.
"""

# My code:
from engine import *

# 
import time
import string
import random

# PyGame Constants
import pygame
from pygame.locals import *

# for optimizing drawing:
from pygame import surfarray

def drawfield( screen, scale_surface, pixels ):
    surfarray.blit_array( scale_surface, pixels )
    temp = pygame.transform.scale(scale_surface, screen.get_size())
    screen.blit(temp, (0,0))
    return

def writeParametersToLog( xsize, ysize, starting_pop, static, avgpop ):
    pass

def main():
    print 'The game of life!'
    WINSIZE = 640,480
    SCALE = 4
    ARRAYSIZE = WINSIZE[0]/SCALE, WINSIZE[1]/SCALE
    
    # Initialize the Pygame Engine!
    pygame.init()
    screen = pygame.display.set_mode(WINSIZE,0,8)
    scale_screen = pygame.surface.Surface( ARRAYSIZE,0,8 )
    pygame.display.set_caption('Python Population Model')
    white = 255,240,200
    black = 20,20, 40
    red = 255, 20, 40
    green = 20,255,40
    blue = 20,20,255
    screen.fill(black)
    scale_screen.fill(black)
    screen.set_palette( [black, red, green, blue, white] )
    scale_screen.set_palette( [black, red, green, blue, white] )
    
    # Initialize the model:
    # 2,3,1,0 correspond to palette colors set above (0=black, 1=red, etc)
    # the order of the Simulation constructor is: plant color, herbivore color, carnivore color, empty color

    count = 1000
    static = 2
    model = Simulation(ARRAYSIZE[0],ARRAYSIZE[1],
                                          2,3,1,0,
                                          count,
                                          static, 
                                          )

    #print "Executing timestep1"
    #for i in xrange(100):
    #    model.timestep1()
    #print "Executing timestep2"
    #for i in xrange(100):
    #    model.timestep2()
    
    i = 0
    pop = 0
    popsum = 0
    popave = 0.0
    timesteps = 365
    elapsedtime = 0.0
    drawtime = 0.0
    updatetime = 0.0
    totaltime=0.0
    fps = 0.0
    sumfps = 0.0
    avgfps = 0.0
    done = False
    while not done:
        # print 'timestep> ', i, '-',drawtime, '-',updatetime,'-',elapsedtime,model.numplants,model.numherbs,model.numcarns
        #print  i, '-',drawtime, '-',updatetime,'-',elapsedtime,'-',avgfps
        print i,'-',avgfps,'-',pop,'-',popave
        
        starttime = time.time()
        drawfield( screen, scale_screen, model.pixel_data ) 
        endtime = time.time()
        
        drawtime = endtime - starttime
        starttime = time.time()
        pygame.display.update()
        endtime = time.time()
        updatetime = endtime - starttime
        
        starttime = time.time()
        model.timestep()
        endtime = time.time()
        elapsedtime = endtime-starttime
        totaltime = (drawtime + updatetime + elapsedtime)
        
        sumfps += ( 1.0/totaltime )
        i += 1    
        avgfps =  sumfps / i

        pop = model.numcreatures 
        popsum += pop
        prev_popave = popave
        popave = popsum / (i*1.0)
        # Restart if either all creatures die or the screen is filled:
        if( pop <= 0 or
             pop >= (ARRAYSIZE[0] * ARRAYSIZE[1]) ):
            print "Restarting @ pop = %s"%pop
            i = 0
            pop = 0
            popsum = 0
            popave = 0.0
            sumfps = 0.0
            model = Simulation(ARRAYSIZE[0],ARRAYSIZE[1],
                                          2,3,1,0,
                                          random.randint(100,3000),
                                          random.randint(1,7), 
                                          )
        
        # Handle someone closing the window or pressing escape
        events = pygame.event.get( )
        for e in events:
            if( e.type == QUIT ):
                done = True
                break
            elif (e.type == KEYDOWN):
                if( e.key == K_ESCAPE ):
                    done = True
                    break
                if( e.key == K_f ):
                    pygame.display.toggle_fullscreen()

    print "Exiting!"

    return
if __name__=="__main__":
    main()
    #import hotshot
    #prof = hotshot.Profile("hotshot_life_stats")
    #prof.runcall(main)
    #prof.close()
