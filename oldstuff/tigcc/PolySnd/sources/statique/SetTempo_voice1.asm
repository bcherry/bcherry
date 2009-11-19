	section ".data"
	xdef	pSnd_SetTempo_voice1

pSnd_SetTempo_voice1:
	moveq	#0,d2
	bsr	pSnd__SetTempo_
	rts