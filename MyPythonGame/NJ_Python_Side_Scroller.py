#!/usr/bin/env python

# Author: Noemi Johnson
# Date: 5/4/2018
# Synopsis: Porting Mario to Python
# Important Disclaimer: This was based on the aliens example. Forgive the mess
# I tested this on Pycharm and ran it on the console using the following command:
#  python NJ_Python_Side_Scroller.py

# importing everything I could find
import os.path
import random

import pygame
from arcade.sprite import Sprite
from pygame.locals import *
from pynput.keyboard import Key , Controller

keyboard = Controller (  )

keyboard.press ( Key.space )
keyboard.release ( Key.space )
keyboard.press ( Key.page_down )

if not pygame.image.get_extended ( ):
    raise SystemExit ( "Sorry this didn't work" )

# constants
FRAMES_PER_SEC = 40
PLAYER_SPEED = 12
MAX_SHOTS = 2
FIREBALL_SPEED = 10
GOOMBA_SPEED = 5
GOOMBA_ODDS = 45
EXPLODE_TIME = 6
GOOMBA_RELOAD = 20
GRAVITY_CONSTANT = 6.9
VERTICAL_VELOCITY = -28.0
SCREENRECT = Rect ( 0 , 0 , 1000 , 500 )

# globals
dirtyrects = [ ]  # used for the sprites
next_tick = 0  # used for timing


class Img: pass  # container for images


main_dir = os.path.split ( os.path.abspath ( __file__ ) )[ 0 ]  # Program's diretory


def load_image ( file , transparent ):
    file = os.path.join ( main_dir , 'data' , file )
    try:
        surface = pygame.image.load ( file )
    except pygame.error:
        raise SystemExit ( 'Could not load image "%s" %s' %
                           (file , pygame.get_error ( )) )
    if transparent:
        corner = surface.get_at ( (0 , 0) )
        surface.set_colorkey ( corner , RLEACCEL )
    return surface.convert ( )


class Sprite:
    source_rect: None

    def __init__ ( self , image ):
        self.image = image
        self.rect = image.get_rect ( )
        self.source_rect = None

    def update ( self ):
        pass

    def draw ( self , screen ):
        r = screen.blit ( self.image , self.rect )
        dirtyrects.append ( r )

    def erase ( self , screen , background ):
        r = screen.blit ( background , self.rect , self.rect )
        dirtyrects.append ( r )

    def collide_rect ( left , right ):
        return left.rect.colliderect ( right.rect )


class Tube ( Sprite ):

    def __init__ ( self ):
        Sprite.__init__ ( self , Img.tube )
        self.alive = 1
        self.reloading = 0
        self.rect.centerx = SCREENRECT.centerx + 100
        self.rect.bottom = SCREENRECT.bottom - 10

        def update ( self , ):
            global SCREENRECT
            self.rect[ 0 ] = self.rect[ 0 ].clamp ( SCREENRECT )
            self.rect.bottom = SCREENRECT.bottom - 30


class Mario ( Sprite ):

    def __init__ ( self ):
        Sprite.__init__ ( self , Img.mario )
        self.gravity_constant = GRAVITY_CONSTANT
        self.vertical_velocity = VERTICAL_VELOCITY
        self.alive = 1
        self.reloading = 1
        self.rect.centerx = SCREENRECT.centerx
        self.rect.bottom = SCREENRECT.bottom - 10
        self.jumping = 0

    def move ( self , direction ):
        self.rect = self.rect.move ( direction * PLAYER_SPEED , 0 ).clamp ( SCREENRECT )

    def jump ( self , vertical_direction ):
        self.rect.top = self.rect.top - vertical_direction - PLAYER_SPEED + GRAVITY_CONSTANT

    def stop_jump ( self , vertical_direction ):
        self.rect.top = self.rect.bottom - vertical_direction + 30
        self.rect = self.rect.clamp ( SCREENRECT )


class Goomba ( Sprite ):

    def __init__ ( self ):
        Sprite.__init__ ( self , Img.goomba )
        self.alive = 1
        self.reloading = 0
        self.rect.centerx = SCREENRECT.centerx + 400
        self.rect.bottom = SCREENRECT.bottom - 10
        self.facing = random.choice ( (-1 , 1) ) * GOOMBA_SPEED
        if self.facing < 0:

            self.rect.centerx += 5;
            self.rect.bottom = SCREENRECT.bottom
        else:
            self.rect.centerx -= 50;
            self.rect.bottom = SCREENRECT.bottom

    def update ( self ):
        global SCREENRECT
        self.rect[ 0 ] = self.rect[ 0 ] + self.facing
        if not SCREENRECT.contains ( self.rect ):
            self.facing = -self.facing;
            self.rect.bottom = SCREENRECT.bottom - 30


class Burn ( Sprite ):

    def __init__ ( self , sprite ):
        Sprite.__init__ ( self , Img.burn )
        self.life = EXPLODE_TIME
        self.rect.center = sprite.rect.center

    def update ( self ):
        self.life = self.life - 1


class Fireball ( Sprite ):

    def __init__ ( self , mario ):
        Sprite.__init__ ( self , Img.fireball )
        self.rect.centerx = mario.rect.centerx
        self.rect.bottom = mario.rect.centerx - 10

    def update ( self ):
        self.rect.centerx = self.rect.centerx + FIREBALL_SPEED
        self.rect.bottom = SCREENRECT.bottom - 10


def main ():
    global dirtyrects

    pygame.init ( )
    screen = pygame.display.set_mode ( SCREENRECT.size , 0 )
    clock = pygame.time.Clock ( )

    # Load the Resources

    Img.background = load_image ( 'level_sky.png' , 0 )
    Img.fireball = load_image ( 'fireball.png' , 1 )
    Img.goomba = load_image ( 'goomba.png' , 1 )
    Img.mario = load_image ( 'mario0.png' , 1 )
    Img.burn = load_image ( 'goomba_fire.png' , 1 )
    Img.tube = load_image ( 'tube.png' , 1 )
    Img.mario = load_image ( 'mario0.png' , 1 )

    # Using the level_sky.png made in Paint for the background:
    background = pygame.Surface ( SCREENRECT.size )
    for x in range ( 0 , SCREENRECT.width , Img.background.get_width ( ) ):
        background.blit ( Img.background , (x , 0) )
        screen.blit ( background , (0 , 0) )
        pygame.display.flip ( )

    # Initializing Sprites:
    mario = Mario ( )
    goombas = [ Goomba ( ) ]
    fireballs = [ ]
    burns = [ ]
    tubes = [ Tube ( ) ]

    # Main loop
    while mario.alive or burns:
        clock.tick ( FRAMES_PER_SEC )
        all_imgs = {}
        pygame.event.pump ( )
        keystate = pygame.key.get_pressed ( )
        if keystate[ K_ESCAPE ] or pygame.event.peek ( QUIT ):
            break

        # Clearing the screen
        for sprite in [ mario ] + goombas + fireballs + burns + tubes:
            sprite.erase ( screen , background )
            sprite.update ( )

        # Cleaning cooked goombas and Fireballs
        for e in burns:
            if e.life <= 0:
                burns.remove ( e )
        for s in fireballs:
            if s.rect.top <= 0:
                fireballs.remove ( s )

        # Moving Mario:
        #   mario.can_jump = False
        direction = keystate[ K_RIGHT ] - keystate[ K_LEFT ]
        mario.move ( direction )

        vertical_direction = keystate[ K_SPACE ]
        mario.jump ( vertical_direction )

        if not mario.jumping:
            mario.jumping = keystate[ K_SPACE ]
            mario.stop_jump ( vertical_direction )

        if mario.jumping and keystate[ K_SPACE ]:

            mario.jumping = keystate[ K_SPACE ]


        elif mario.jumping:

            mario.jumping = keystate[ K_SPACE ]
            mario.stop_jump ( vertical_direction )

        # Mario gets more fireballs:

        if not mario.reloading and keystate[ K_RCTRL ] and len ( fireballs ) < MAX_SHOTS:
            fireballs.append ( Fireball ( mario ) )
        mario.reloading = keystate[ K_RCTRL ]

        # Create new goomba
        if not int ( random.random ( ) * GOOMBA_ODDS ):
            goombas.append ( Goomba ( ) )

        # Detect collisions
        goombasrects = [ ]
        for a in goombas:
            goombasrects.append ( a.rect )

        hit = mario.rect.collidelist ( goombasrects )

        if hit != -1:
            goomba = goombas[ hit ]
            burns.append ( Burn ( goomba ) )
            burns.append ( Burn ( mario ) )
            goombas.remove ( goomba )
            mario.alive = 0
        for fireball in fireballs:
            hit = fireball.rect.collidelist ( goombas )
            if hit != -1:
                goomba = goombas[ hit ]
                burns.append ( Burn ( goomba ) )
                fireballs.remove ( fireball )
                goombas.remove ( goomba )
                break
        # Drawing everything
        for sprite in [ mario ] + goombas + fireballs + burns + tubes:
            sprite.draw ( screen )

        pygame.display.update ( dirtyrects )
        dirtyrects = [ ]

    pygame.time.wait ( 50 )


#The int main() of Python:
if __name__ == '__main__':
    main ( )
