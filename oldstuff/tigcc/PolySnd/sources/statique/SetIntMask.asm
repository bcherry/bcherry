	section ".data"
	xdef	pSnd_SetIntMask

pSnd_SetIntMask:
	move.w	d0,mask_int5
	rts