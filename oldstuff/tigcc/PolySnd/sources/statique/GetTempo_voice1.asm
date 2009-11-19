	section ".data"
	xdef	pSnd_GetTempo_voice1

pSnd_GetTempo_voice1:
	move.w	unittps_voice1,d1
	cmp.b	#1,Hardware
	beq.s	_set_tempo_voice1_HW1
	move.w	#15360,d0
	divu	d1,d0
	rts
	
_set_tempo_voice1_HW1:
	move.w	#21692,d0
	divu	d1,d0
	rts