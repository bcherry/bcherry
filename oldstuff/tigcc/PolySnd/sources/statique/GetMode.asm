	section ".data"
	xdef	pSnd_GetMode

pSnd_GetMode:
	move.w	state_and_mode,d0
	move.w	d0,d1
	;Extrait modes voice1
	andi.w	#%0000000001111000,d0
	lsr.w	#3,d0
	;Extrait modes voice2
	andi.w	#%0011110000000000,d1
	lsr.w	#6,d1
	or.w	d1,d0
	rts