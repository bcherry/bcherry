	section ".data"
	xdef	pSnd_GetFrequency_voice1

pSnd_GetFrequency_voice1:
	move.w	frequency_voice1,d0
	rts