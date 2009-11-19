	section ".data"	
	xdef	pSnd_GetLength_voice1

pSnd_GetLength_voice1:
	move.b	length_voice1,d0
	rts