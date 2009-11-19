	section ".data"
	xdef	pSnd_SetLoop

pSnd_SetLoop:
	move.b	d0,loop
	rts