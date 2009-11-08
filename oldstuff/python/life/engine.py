#################################################################
##  Game of Life
##  Mike Steder
##
##  Simulation Engine Object
##  engine.py
##
##  "this object generates a field/universe for our creatures
##   and distributes them.  A call to a step method timesteps
##   the entire 'civilization' of creatures once.  
##
##   Currently the model follows the following rules:
##
##   1).  The field is randomly populated with creatures to a certain percentage of
##          the available space
##   2).  Creatures with 3 or more neighbors(diagonals count):
##          (  This creature has no neighbors:
##            O O O
##            O X O
##            O O O
##            This creature has 3 neighbors
##            X X O
##            O X X
##            O O O )
##          will mate with a nearby neighbor and reproduce
##          into one of their open adjacent spots (diagonals count)
##  3).  A creature with 2 neighbors will just sit still and wait for something exciting to happen.
##  4).  A creature with fewer then 2 neighbors will get lonely and die.
#################################################################

# Import my module first
#import _engine


import sys
import random
import time

try:
    import Numeric
except ImportError:
    print 'Couldn\'t locate Numeric Python Module.'
    print 'which is required to run this application'

# Coords = [UP, UP-RIGHT, RIGHT, etc] clockwise
NEIGHBORS = [
                            (0,1), # up
                            (0,-1), # down
                            (1,0), # left
                            (-1,0), # right
                            (1,1), # upper left
                            (-1,1), # upper right
                            (-1,-1), # lower right
                            (1,-1), # lower left
                            ]

class Simulation:
    def __init__(self,xdim=10, ydim=10,
                 # colors will all be real palette values
                 plantcolor=0, herbcolor=0,
                 carncolor=0, emptycolor=0,
                 numcreatures=10,
                 static=2, # number of neighbors necessary to be in balance
                ):
        # The percentage chance of dying increases every deathsteps steps
        # and a creature is checked to see if they die every deathsteps steps
        # eatsteps is max number of days a creature can go without eating
        # Initialize and Store Simulation Variables
        self.xdim = xdim
        self.ydim = ydim
        self.xrange = range(0,self.xdim)
        self.yrange = range(0, self.ydim )
        self.range = range(0, self.xdim * self.ydim)
        self.plantcolor=plantcolor
        self.herbcolor=herbcolor
        self.carncolor=carncolor
        self.emptycolor=emptycolor
        self.numcreatures = numcreatures
        self.static = static
        self.environment = None
        self.lut = None
        self.pixel_data = Numeric.zeros( (xdim,ydim), 'i' )
        self.init_environment()
        
    def init_environment(self):
        self.environment = Numeric.zeros((self.xdim,self.ydim),'i')

        print "Setting up pixel_data array:"
        for i in xrange(self.xdim):
            for j in xrange(self.ydim):
                try:
                    self.pixel_data[i][j] = self.emptycolor
                except IndexError:
                    print "Tried to set pixel_data[%s][%s]! (xdim=%s,ydim=%s)"%(i,j,self.xdim,self.ydim)
                except TypeError:
                    print "i=%s,j=%s,self.emptycolor=%s"%(i,j,self.emptycolor)

        print "Setting up Lookup Table:"
        self.lut = {}
                    
        print 'Setting up creatures'
        i = 0 # self.numplants
        iterations = 0
        while i < self.numcreatures:
            x = self.randomx()
            y = self.randomy()
            if self.environment[x][y] == 0:
               self.environment[x][y] = 1
               self.pixel_data[x][y] = self.plantcolor # GREEN!
               if( i % 100 == 0 ):
                   print i,"- (",x,",",y,")","- Created!"
               i+=1
               iterations += 1
               self.lut[(x,y)] = (x,y)
            else:
                # Duplicate location, something's already living here.
                # Try again!
                iterations += 1
                continue
        print "%s Creatures setup in %s iterations."%(len(self.lut),iterations)
        # End of initenvironment
        return

    ### Determine random starting locations
    def randomx(self):
        x = random.randrange(0,self.xdim-1)
        return x
    def randomy(self):
        y = random.randrange(0,self.ydim-1)
        return y

    def timestep(self):
        #print 'timestep'
        # Set move options and call move
        values = self.lut.values()
        moves = len(values)
        avgmove = 0.0
        startstep = time.time()
        for coords in values:
            startmove = time.time()
            self.move( coords )
            endmove = time.time()
            avgmove += (endmove - startmove)
        endstep = time.time()
        #print "timestep2: avgmove = %s, step = %s"%( avgmove/moves, endstep-startstep)
        return

    def countNeighbors( self, coords ):
        myNeighbors = 0
        freelist = []
        for n in NEIGHBORS:
            nx,ny = (coords[0] + n[0])%self.xdim, (coords[1] + n[1])%self.ydim
            if( self.environment[nx][ny] ):
                myNeighbors += 1
            else:
                freelist.append( n )
        return myNeighbors, freelist

    def move( self, coords ):
        x,y = coords[0],coords[1]
        # numNeighbors,freeList = self.countNeighbors(coords)
        numNeighbors,freeList = _engine.count_neighbors( x, y, self.environment )
        # print "neighbors, freelist = %s, %s"%(numNeighbors,freeList)
        if( numNeighbors > self.static and numNeighbors < 8 ):
            random.shuffle(freeList)
            item = freeList[0]
            nx = (x + item[0])%self.xdim
            ny = (y + item[1])%self.ydim
            self.environment[nx][ny] = 1
            self.pixel_data[nx][ny] = self.plantcolor
            self.lut[(nx,ny)] = (nx,ny)
            self.numcreatures+=1
        elif( numNeighbors < self.static ):
            # Die
            self.environment[x][y] = 0
            self.pixel_data[x][y] = self.emptycolor
            del self.lut[(x,y)] 
            self.numcreatures -=1
        else:
            # Do nothing, wait for your neighbors to do something
            pass
        
        return
        

if __name__=='__main__':
    model = Simulation()
    print 'Simulation Initialized!'
##    i = 0
##    for list in model.environment:
##        for item in list:
##            print i,item.type
##            i+=1
    print 'Begin Timestepping!'
    i = 0
    if len(sys.argv) > 1:
        steps = sys.argv[1]
    else:
        steps = 365
    while i < steps:
        model.timestep()
        i+=1
    print 'Complete'
