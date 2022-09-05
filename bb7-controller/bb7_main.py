
import time
from enum import Enum

#application states
class States(Enum):
    START   = 0
    WORKING = 1
    STOP  	= 2

#local properties	
state = States.START 
inLife = True

#main
setup()
while(inLife):
	loop()
	time.sleep(1)
	
#setup enviroment
def setup():
	return
	
#loop enviroment
def loop():
	match state:
		case States.START:
			#...
		case States.WORKING:
			#...
		case States.STOP:
			#...
		case _:
			#...
	return
