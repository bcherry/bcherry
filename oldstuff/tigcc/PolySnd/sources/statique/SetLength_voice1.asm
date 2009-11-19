	section ".data"
	xdef	pSnd_SetLength_voice1
	
pSnd_SetLength_voice1:
	move.w	unittps_voice1,_ucounter1
	move.b	d0,length_voice1
	move.b	d0,_lcounter1
	rts