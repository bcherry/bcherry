	section ".data"
	xdef	pSnd_note_time_counter_voice1
	
pSnd_note_time_counter_voice1:
	tst.l	d0
	beq.s	return_note_time_voice1
	move.l	d0,note_time_voice1
	rts
	
return_note_time_voice1:
	move.l	note_time_voice1,d0
	rts