	section ".data"
	xdef	pSnd_SetTempo_voice2

pSnd_SetTempo_voice2:
	moveq	#1,d2
	bsr	pSnd__SetTempo_
	rts