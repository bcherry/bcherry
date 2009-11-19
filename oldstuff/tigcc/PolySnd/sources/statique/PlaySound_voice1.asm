	section ".data"
	xdef	pSnd_PlaySound_voice1

pSnd_PlaySound_voice1:
	bset.b	#0,State_voice
	move.l	a0,ptrstart_voice1
	move.l	a0,ptridle_voice1
	
	bsr	pSnd_SkipData_voice1	
	clr.l	time_counter_voice1
	
	move.w	state_and_mode,d0
	andi.w	#%1111110010000000,d0						;Mode PlaySound voice 1
	ori.w	#$0040,d0
	move.w	d0,state_and_mode
	rts