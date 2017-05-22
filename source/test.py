import subprocess
import pygame
from threading import Timer
import sys

def turingMachineRunner(userinput):
    args="TM.txt"
    return_code= subprocess.call(["./TM", args, userinput])

# t = Timer(int(sys.argv[1]), turingMachineRunner)
# t.start()
# userinput = ""
while True:
   userinput = input() 
   if userinput == "quit":
       break
   else:
       turingMachineRunner(userinput)

