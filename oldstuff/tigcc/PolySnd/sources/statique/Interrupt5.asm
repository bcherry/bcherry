	section ".data"
	xdef	pSnd_Interrupt5

pSnd_Interrupt5:
	move.l	a0,interrupt_handler
	rts	