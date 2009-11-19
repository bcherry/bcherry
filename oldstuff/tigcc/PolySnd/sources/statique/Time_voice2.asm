	section ".data"
	xdef	pSnd_Time_voice2

pSnd_Time_voice2:
;5000=5000 Hz
	move.l	time_counter_voice2,d0
	divu	interrupt5rate,d0
	rts