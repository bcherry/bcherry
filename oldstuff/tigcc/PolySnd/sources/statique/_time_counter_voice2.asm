	section ".data"
	xdef	pSnd_time_counter_voice2
	
pSnd_time_counter_voice2:
	tst.l	d0
	beq.s	return_time_voice2
	move.l	d0,time_counter_voice2
	rts
	
return_time_voice2:
	move.l	time_counter_voice2,d0
	rts