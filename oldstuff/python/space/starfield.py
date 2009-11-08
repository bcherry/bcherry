#! /usr/bin/env python

import random, pygame, math

class Star:
    """ Star is a crippled particle class - it can draw itself, keep track
    of its speed, position and direction, color and size. """
    
    def __init__(self, position_x, position_y, distance, angle, speed, size,
                 color):
        self.position_x = position_x
        self.position_y = position_y
        self.distance = distance
        self.size = size
        self.angle = angle
        self.speed = speed
        self._calculate_axis_speeds()
        self.color = (int(distance * color[0]),
                      int(distance * color[1]),
                      int(distance * color[2]))

    def draw(self, display):
        display.fill(self.color, (self.position_x, self.position_y,
                                  self.size, self.size))
        return

    def erase(self, display):
        display.fill((0,0,0), (self.position_x, self.position_y,
                                        self.size, self.size))
        return

    def move(self, elapsed_time):
        self.position_x += (self.speed_x * elapsed_time)
        self.position_y += (self.speed_y * elapsed_time)
        return

    def set_angle(self, new_angle):
        self.angle = new_angle
        self._calculate_axis_speeds()

    def set_speed(self, new_speed):
        self.speed = new_speed
        self._calculate_axis_speeds()

    def _calculate_axis_speeds(self):
        # This is a pretty slow function - performance is poor because of
        # the math. If you had a game where the starfield could only move
        # in certain directions, or at certain speeds, you might be able to
        # increase performance by precalculating the values you need, and
        # feeding them in as constants.
        self.speed_x = math.cos(self.angle / (180 / math.pi)) * self.speed
        self.speed_y = math.sin(self.angle / (180 / math.pi)) * self.speed
        

class Starfield:
    """
    Starfield(display, rect, number_of_stars, angle, speed_sequence,
              size, color)
    
    The starfield class uses the following arguments:

        display            -- A pygame surface
        rect               -- A pygame.Rect on that surface - the starfield
                              might only take up a certain area on your
                              display, leaving room for a hud, etc.
        number_of_stars    -- An integer - the more stars, the slower the
                              animation.
        angle              -- From 0 to 360 in degrees
        speed_sequence     -- A sequence of two integers. The first is the
                              speed of the slowest (most distant) star, the
                              second is the speed of the fastest (closest)
                              star. Both values are in pixels per second.
        size               -- The size of the stars in pixels. One is the
                              default.
        color              -- An RGB tuple of your stars' color. The
                              color of individual stars will vary from this
                              for the closest, to nearly black for the most
                              distant. This is optional - if it's omitted,
                              the closest stars will be bright white.

    The starfield can have its angle and speed changed on the fly.
    """
                                                  
    
    def __init__(self, display, rect, number_of_stars, angle,
                 speed_sequence, size = 1, color = (255, 255, 255)):
        self.display_surface = display
        self.display_rect = rect
        self.angle = angle
        self.fastest_star_speed = speed_sequence[0]
        self.slowest_star_speed = speed_sequence[1]
        self.brightest_color = color
        self.number_of_stars = number_of_stars
        self.timer = Timer()
        

        # create our stars
        
        self.stars = []
        for index in range(number_of_stars):
            x_pos = self._random_x()
            y_pos = self._random_y()
            distance = random.random()
            speed = ((distance * \
                      (self.fastest_star_speed - self.slowest_star_speed)) + \
                     self.slowest_star_speed)
            
            my_star = Star(x_pos, y_pos, distance, angle, speed, size, color)
            self.stars.append(my_star)
        
                           
    def update(self):
        self.erase()
        self.move()
        self.draw()
    
    def draw(self):
        for my_star in self.stars:
            my_star.draw(self.display_surface)
        return

    def erase(self):
        for my_star in self.stars:
            my_star.erase(self.display_surface)
        return

    def move(self):
        elapsed_time = self.timer.get_elapsed_time()
        for my_star in self.stars:
            my_star.move(elapsed_time)
            # Check if the star has moved off a screen edge. If so, put it
            # back on the opposite screen edge at a random position.
            if my_star.position_x <= self.display_rect.left:
                my_star.position_x = self.display_rect.right
                my_star.position_y = self._random_y()
            elif my_star.position_x >= self.display_rect.right:
                my_star.position_x = self.display_rect.left
                my_star.position_y = self._random_y()
            if my_star.position_y <= self.display_rect.top:
                my_star.position_y = self.display_rect.bottom
                my_star.position_x = self._random_x()
            elif my_star.position_y >= self.display_rect.bottom:
                my_star.position_y = self.display_rect.top
                my_star.position_x = self._random_x()
        return

    def set_angle(self, new_angle):
        self.angle = new_angle
        for star in self.stars:
            star.set_angle(new_angle)
        return

    def set_speeds(self, new_speed_sequence):
        self.fastest_star_speed = new_speed_sequence[0]
        self.slowest_star_speed = new_speed_sequence[1]
        for star in self.stars:
            new_speed = ((star.distance * \
                         (self.fastest_star_speed-self.slowest_star_speed)) + \
                         self.slowest_star_speed)
            star.set_speed(new_speed)
        return

    def _random_x(self):
        return float(random.randint(self.display_rect.left,
                                      self.display_rect.right))

    def _random_y(self):
        return float(random.randint(self.display_rect.top,
                                      self.display_rect.bottom))

           
        
    
class Timer:
    """ This class lets me scale a star's movement based on how much time
    has passed since the previous frame.  That results in smoother, more
    constant motion, even if the frame rate varies. """
    def __init__(self):
        self.current_time = pygame.time.get_ticks()

    def get_elapsed_time(self):
        now = pygame.time.get_ticks()
        elapsed_time = (now - self.current_time) * 0.001
        self.current_time = now
        return elapsed_time


if __name__ == "__main__":
    pygame.init()

    # Change these for different effects

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
    
    while 1:
        my_starfield.update()
        pygame.display.update()

        # Check for quit, and process user input (arrow keys)
        
        for event in pygame.event.get():
            if event.type is pygame.QUIT:
                raise SystemExit
            elif event.type is pygame.KEYDOWN:
                if event.key is pygame.K_ESCAPE:
                    raise SystemExit
                elif event.key == pygame.K_RIGHT:
                    new_angle = my_starfield.angle + 10
                    if new_angle > 360:
                        new_angle -= 360
                    my_starfield.set_angle(new_angle)
                elif event.key == pygame.K_LEFT:
                    new_angle = my_starfield.angle - 10
                    if new_angle < 0:
                        new_angle += 360
                    my_starfield.set_angle(new_angle)
                elif event.key == pygame.K_UP:
                    if current_speed != len(preset_speeds) - 1:
                        current_speed += 1
                        new_speed = preset_speeds[current_speed]
                        my_starfield.set_speeds(new_speed)
                elif event.key == pygame.K_DOWN:
                    if current_speed != 0:
                        current_speed -= 1
                        new_speed = preset_speeds[current_speed]
                        my_starfield.set_speeds(new_speed)

                                               
        
        
