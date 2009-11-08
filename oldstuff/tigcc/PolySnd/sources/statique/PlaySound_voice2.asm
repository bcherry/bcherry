	section ".data"
	xdef	pSnd_PlaySound_voice2

pSnd_PlaySound_voice2:
	bset.b	#1,State_voice
	move.l	a0,ptrstart_voice2
	move.l	a0,ptridle_voice2
	
	bsr	pSnd_SkipData_voice2
	clr.l	time_counter_voice2
	
	move.w	state_and_mode,d0
	andi.w	#%1100000011111000,d0						;Mode PlayFx voice 2
	ori.w	#$2000,d0
	move.w	d0,state_and_mode
	rts