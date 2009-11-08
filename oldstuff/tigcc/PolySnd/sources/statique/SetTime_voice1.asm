	section ".data"
	xdef	pSnd_SetTime_voice1

pSnd_SetTime_voice1:
	mulu	interrupt5rate,d0
	move.l	d0,time_counter_voice1
	rts