	section ".data"
	xdef	pSnd_GetTempo_voice2

pSnd_GetTempo_voice2:
	move.w	unittps_voice2,d1
	cmp.b	#1,Hardware
	beq.s	_set_tempo_voice2_HW1
	move.w	#15360,d0
	divu	d1,d0
	rts
	
_set_tempo_voice2_HW1:
	move.w	#21692,d0
	divu	d1,d0
	rts