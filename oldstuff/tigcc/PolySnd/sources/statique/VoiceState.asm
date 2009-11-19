	section ".data"
	xdef	pSnd_VoiceState

pSnd_VoiceState:
	move.b	State_voice,d0
	rts