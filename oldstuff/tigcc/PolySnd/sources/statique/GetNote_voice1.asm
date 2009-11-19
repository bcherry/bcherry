	section ".data"
	xdef	pSnd_GetNote_voice1

pSnd_GetNote_voice1:
	move.b	note_voice1,d0
	rts