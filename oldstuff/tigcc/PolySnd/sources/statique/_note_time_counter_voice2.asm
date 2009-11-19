	section ".data"
	xdef	pSnd_note_time_counter_voice2
	
pSnd_note_time_counter_voice2:
	tst.l	d0
	beq.s	return_note_time_voice2
	move.l	d0,note_time_voice2
	rts
	
return_note_time_voice2:
	move.l	note_time_voice2,d0
	rts