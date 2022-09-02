from bb7 import BB7
import time
import random

class BB7Motion:

	def __init__(self, bb7):
		self.bb7 = bb7
		
	#il robot e' contento
	def happy(self):
		#si abbassa in avanti e guarda in su
		self.bb7.armBackRight(60).armBackRight(60).armFrontRight(0).armFrontLeft(0).head(10).send();
		time.sleep(0.4)

		#nuove il dietro come un cane
		for degree in [30, -30, 30, -30, 30, -30, 30, -30, 0]:
			self.bb7.shoulderBackRight(-degree).shoulderBackLeft(degree).send();
			time.sleep(0.2)


	#standup iniziale
	def standup(self):
		arm_zero_pos = 50
		self.bb7.zero(arm_zero_pos).send()
		time.sleep(0.5)

		#effettua leggeri movimenti random
		for _ in range(50):
			mov = random.choice([0, 15, -15])
			
			self.bb7.armFrontRight(arm_zero_pos-mov).armFrontLeft(arm_zero_pos+mov)
			self.bb7.armBackRight(arm_zero_pos-mov).armBackLeft(arm_zero_pos+mov)
			self.bb7.head(random.choice([0, 3, -3, 0, 2, -2]))
			self.bb7.neck(random.choice([0, 4, -4, 0, 2, -2]))
			self.bb7.send()
			
			time.sleep(1)
			
		time.sleep(4)
		self.bb7.relax()


	#prova i vari passi: avanti, indietro, ruota a destra, ruota a sinistra
	def steps(self):
		#va avanti
		self.stepForward(2)
		time.sleep(1)
		#va indietro
		self.stepBack(2)
		time.sleep(1)
		#gira a sinistra
		self.stepTurnLeft(2)
		time.sleep(1)
		#gira a destra
		self.stepTurnRight(2)
		time.sleep(1)
		#fine
		self.bb7.zero(50)
		time.sleep(2)
		self.bb7.zero()
		time.sleep(0.3)
		self.bb7.relax()
		
	def stepForward(self, stepCount=1, delay=0.1, arm_zero_pos=50):
		self.bb7.zero(arm_zero_pos)
		self.delay(delay)
		#conta i passi in avanti
		for i in range(stepCount):
			#porta il peso indietro a sinistra
			self.bb7.armBackRight(arm_zero_pos-50).armBackLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba anteriore destra per il passo
			self.bb7.armFrontRight(-40)
			self.delay(delay)
			self.bb7.shoulderFrontRight(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso in avanti a destra
			self.bb7.armFrontRight(arm_zero_pos-90).armFrontLeft(arm_zero_pos-20)
			self.delay(delay)
			#muove a gamba posteriore sinistra per il passo
			self.bb7.armBackLeft(-40)
			self.delay(delay)
			self.bb7.shoulderBackLeft(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.shoulderFrontRight(0).shoulderFrontLeft(-40)
			self.bb7.shoulderBackLeft(0).shoulderBackRight(-40)
			self.delay(delay)

			#porta il peso indietro a destra
			self.bb7.armBackRight(arm_zero_pos-70).armBackLeft(arm_zero_pos-50)
			self.delay(delay)
			#muove a gamba anteriore sinistra per il passo
			self.bb7.armFrontLeft(-40)
			self.delay(delay)
			self.bb7.shoulderFrontLeft(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontLeft(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso in avanti a sinistra
			self.bb7.armFrontRight(arm_zero_pos-50).armFrontLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba posteriore destra per il passo
			self.bb7.armBackRight(-40)
			self.delay(delay)
			self.bb7.shoulderBackRight(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.shoulderFrontRight(-40).shoulderFrontLeft(0)
			self.bb7.shoulderBackLeft(-40).shoulderBackRight(0)
			self.delay(delay)
			self.bb7.send()

	#step back
	def stepBack(self, stepCount=1, delay=0.1, arm_zero_pos=50):
		self.bb7.zero(arm_zero_pos)
		self.delay(delay)
		#conta i passi in avanti
		for i in range(stepCount):
			#porta il peso in avanti a sinistra
			self.bb7.armFrontRight(arm_zero_pos-50).armFrontLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba posteriore destra per il passo
			self.bb7.armBackRight(-40)
			self.delay(delay)
			self.bb7.shoulderBackRight(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armBackRight(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso indietro a destra
			self.bb7.armBackRight(arm_zero_pos-90).armBackLeft(arm_zero_pos-20)
			self.delay(delay)
			#muove a gamba anteriore sinistra per il passo
			self.bb7.armFrontLeft(-40)
			self.delay(delay)
			self.bb7.shoulderFrontLeft(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontLeft(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) 
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) 
			self.bb7.shoulderBackRight(0).shoulderBackLeft(40) 
				.shoulderFrontLeft(0).shoulderFrontRight(40)
			self.delay(delay)

			#porta il peso in avanti a destra
			self.bb7.armFrontRight(arm_zero_pos-70).armFrontLeft(arm_zero_pos-50)
			self.delay(delay)
			#muove a gamba posteriore sinistra per il passo
			self.bb7.armBackLeft(-40)
			self.delay(delay)
			self.bb7.shoulderBackLeft(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso indietro a sinistra
			self.bb7.armBackRight(arm_zero_pos-50).armBackLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba anteriore destra per il passo
			self.bb7.armFrontRight(-40)
			self.delay(delay)
			self.bb7.shoulderFrontRight(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armBackRight(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.shoulderBackRight(40).shoulderBackLeft(0)
			self.bb7.shoulderFrontLeft(40).shoulderFrontRight(0)
			self.delay(delay)
			self.bb7.send()

	#step turn left
	def stepTurnLeft(self, stepCount=1, delay=0.1, arm_zero_pos=50):
		self.bb7.zero(arm_zero_pos)
		self.delay(delay)
		#conta i passi in avanti
		for i in range(stepCount):
			#porta il peso indietro a sinistra
			self.bb7.armBackRight(arm_zero_pos-50).armBackLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba anteriore destra per il passo
			self.bb7.armFrontRight(-40)
			self.delay(delay)
			self.bb7.shoulderFrontRight(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso in avanti a destra
			self.bb7.armFrontRight(arm_zero_pos-90).armFrontLeft(arm_zero_pos-20)
			self.delay(delay)
			#muove a gamba posteriore sinistra per il passo
			self.bb7.armBackLeft(-40)
			self.delay(delay)
			self.bb7.shoulderBackLeft(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.shoulderFrontRight(0).shoulderFrontLeft(40)
			self.bb7.shoulderBackLeft(0).shoulderBackRight(-40)
			self.delay(delay)

			#porta il peso indietro a destra
			self.bb7.armBackRight(arm_zero_pos-70).armBackLeft(arm_zero_pos-50)
			self.delay(delay)
			#muove a gamba anteriore sinistra per il passo
			self.bb7.armFrontLeft(-40)
			self.delay(delay)
			self.bb7.shoulderFrontLeft(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontLeft(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso in avanti a sinistra
			self.bb7.armFrontRight(arm_zero_pos-50).armFrontLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba posteriore destra per il passo
			self.bb7.armBackRight(-40)
			self.delay(delay)
			self.bb7.shoulderBackRight(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.shoulderFrontRight(-40).shoulderFrontLeft(0)
			self.bb7.shoulderBackLeft(40).shoulderBackRight(0)
			self.delay(delay)
			self.bb7.send()

	#step turn right
	def stepTurnRight(self, stepCount=1, delay=0.1, arm_zero_pos=50):
		self.bb7.zero(arm_zero_pos)
		self.delay(delay)
		#conta i passi in avanti
		for i in range(stepCount):
			#porta il peso indietro a sinistra
			self.bb7.armBackRight(arm_zero_pos-50).armBackLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba anteriore destra per il passo
			self.bb7.armFrontRight(-40)
			self.delay(delay)
			self.bb7.shoulderFrontRight(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso in avanti a destra
			self.bb7.armFrontRight(arm_zero_pos-90).armFrontLeft(arm_zero_pos-20)
			self.delay(delay)
			#muove a gamba posteriore sinistra per il passo
			self.bb7.armBackLeft(-40)
			self.delay(delay)
			self.bb7.shoulderBackLeft(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.shoulderFrontRight(0).shoulderFrontLeft(-40)
			self.bb7.shoulderBackLeft(0).shoulderBackRight(40)
			self.delay(delay)

			#porta il peso indietro a destra
			self.bb7.armBackRight(arm_zero_pos-70).armBackLeft(arm_zero_pos-50)
			self.delay(delay)
			#muove a gamba anteriore sinistra per il passo
			self.bb7.armFrontLeft(-40)
			self.delay(delay)
			self.bb7.shoulderFrontLeft(40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontLeft(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#porta il peso in avanti a sinistra
			self.bb7.armFrontRight(arm_zero_pos-50).armFrontLeft(arm_zero_pos-70)
			self.delay(delay)
			#muove a gamba posteriore destra per il passo
			self.bb7.armBackRight(-40)
			self.delay(delay)
			self.bb7.shoulderBackRight(-40)
			self.delay(delay)
			#reimposta l'assetto gambe
			self.bb7.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.delay(delay)
			#effettua la spinta per il passo
			self.bb7.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos)
			self.bb7.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos)
			self.bb7.shoulderFrontRight(40).shoulderFrontLeft(0)
			self.bb7.shoulderBackLeft(-40).shoulderBackRight(0)
			self.delay(delay)
			self.bb7.send()
			