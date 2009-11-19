	section ".data"
	xdef	pSnd_Time_voice1
	
pSnd_Time_voice1:
;5000=5000 Hz
	move.l	time_counter_voice1,d0
	divu	interrupt5rate,d0
	rts
