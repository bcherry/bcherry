	section ".data"
	xdef	pSnd_SetVoiceState

pSnd_SetVoiceState:
	move.b	d0,State_voice
	rts