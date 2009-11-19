	section ".data"
	xdef	pSnd_GetLoop

pSnd_GetLoop:
	move.b	loop,d0
	rts