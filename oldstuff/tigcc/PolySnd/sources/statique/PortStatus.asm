	section ".data"
	xdef	pSnd_PortStatus

pSnd_PortStatus:
	move.b	($60000E),d0
	rts