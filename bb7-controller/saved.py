
arm_zero_pos = 50
weight_side_diff = 50
delay = 500
bb7.zero(arm_zero_pos).send()
#time.sleep(delay)
#conta i passi in avanti
for i in range(2):
	#porta il peso indietro a sinistra
	bb7.armBackRight(arm_zero_pos-80).armBackLeft(arm_zero_pos-80-weight_side_diff) \
	.delay(delay) \
	#muove a gamba anteriore destra per il passo
	.armFrontRight(-60) \
	#time.sleep(delay)
	.shoulderFrontRight(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.delay(delay) \
	#porta il peso in avanti a destra
	.armFrontRight(arm_zero_pos-90-weight_side_diff).armFrontLeft(arm_zero_pos-90) \
	.delay(delay) \
	#muove a gamba posteriore sinistra per il passo
	.armBackLeft(-60) \
	.delay(delay) \
	.shoulderBackLeft(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armBackLeft(arm_zero_pos).armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
	.delay(delay) \
	#effettua la spinta per il passo
	.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
	.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.shoulderFrontRight(0).shoulderFrontLeft(-40) \
	.shoulderBackLeft(0).shoulderBackRight(-40) \
	.delay(delay) \

	
	#porta il peso indietro a destra
	.armBackRight(arm_zero_pos-80-weight_side_diff).armBackLeft(arm_zero_pos-80) \
	.delay(delay) \
	#muove a gamba anteriore sinistra per il passo
	.armFrontLeft(-60) \
	.delay(delay) \
	.shoulderFrontLeft(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armFrontLeft(arm_zero_pos) \
	.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.delay(delay) \
	#porta il peso in avanti a sinistra
	.armFrontRight(arm_zero_pos-90).armFrontLeft(arm_zero_pos-90-weight_side_diff) \
	.delay(delay) \
	#muove a gamba posteriore destra per il passo
	.armBackRight(-60) \
	.delay(delay) \
	.shoulderBackRight(60) \
	.delay(delay) \
	#reimposta l'assetto gambe
	.armFrontRight(arm_zero_pos).armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.delay(delay) \
	#effettua la spinta per il passo
	.armFrontRight(arm_zero_pos).armFrontLeft(arm_zero_pos) \
	.armBackRight(arm_zero_pos).armBackLeft(arm_zero_pos) \
	.shoulderFrontRight(-60).shoulderFrontLeft(0) \
	.shoulderBackLeft(-60).shoulderBackRight(0) \
	.delay(delay) \
	.send()
	
time.sleep(10)