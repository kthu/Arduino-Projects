#!/usr/bin/env python
# This is a python implementation of an algorithm that will eventually be an
# arduino project. Just to make experimenting a bit easier.
import sys, time, pygame
AXISVALTICKS=10
TICKSECS=0.01
SILENTTHRESHHOLD=2
PASSWORD="GN"

alphabet = [['A','B','C','D','E'],['F','G','H','I','J'],['L','M','N','O','P'],['Q','R','S','T','U'],['V','W','X','Y','Z']]
word=""
count = 0
x = 0
y = 0

def getkey():
   for key in pygame.event.get():
      if key.type == pygame.KEYDOWN:
         if key.unicode == key.unicode:
            return key.unicode

def getaxis():
   tap = False
   silentduration = 0
   value = 0
   tickcount = 0 ;

   while True:
      key = getkey()
      
      if key == ' ':
         tap = True 

      if tickcount == AXISVALTICKS:
         tickcount = 0
         if tap == True:
            silentduration = 0
            value += 1
            tap = False
         else:
            if value > 0:
               silentduration += 1
               if silentduration >= SILENTTHRESHHOLD:
                  return value

      if value >= len(alphabet):
         return value

      tickcount += 1
      time.sleep(TICKSECS)

pygame.init()
size = width, height = 100, 100
screen = pygame.display.set_mode(size)

while True:
   x = getaxis()-1
   y = getaxis()-1
   letter = alphabet[x][y]
   print letter

   word += letter
   if len(word) > len(PASSWORD):
      word = word[-len(PASSWORD):]
   if word == PASSWORD:
      print "OK!"
      sys.exit(0)

