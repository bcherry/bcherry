	section ".data"
	xdef	pSnd_SetTime_voice2

pSnd_SetTime_voice2:
	mulu	interrupt5rate,d0
	move.l	d0,time_counter_voice2
	rts