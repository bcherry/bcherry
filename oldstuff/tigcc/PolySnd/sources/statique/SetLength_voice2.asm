	section ".data"
	xdef	pSnd_SetLength_voice2

pSnd_SetLength_voice2:
	move.w	unittps_voice2,_ucounter2
	move.b	d0,length_voice2
	move.b	d0,_lcounter2
	rts
