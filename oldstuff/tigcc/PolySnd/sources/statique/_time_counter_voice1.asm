	section ".data"
	xdef	pSnd_time_counter_voice1
	
pSnd_time_counter_voice1:
	tst.l	d0
	beq.s	return_time_voice1
	move.l	d0,time_counter_voice1
	rts
	
return_time_voice1:
	move.l	time_counter_voice1,d0
	rts