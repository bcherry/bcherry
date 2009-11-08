	section ".data"
	xdef	pSnd_SetBeep_voice2

pSnd_SetBeep_voice2:
	bset.b	#1,State_voice						;State_voice voice 1 à 1
	clr.l	time_counter_voice2					;Efface le compteur de temps voice 2
	
	move.b	d1,length_voice2
	move.w	d0,frequency_voice2
	
	move.w	unittps_voice2,_ucounter2
	move.w	d0,_fcounter2
	move.b	d1,_lcounter2
	
	;Prepare compatibilité avec HW1 et HW2
	lsr.w	#1,d0
	sub.w	play_frequency,d0
	move.w	d0,_fcounter2
	
	move.w	state_and_mode,d0			
	andi.w	#%1100000011111000,d0					;Activge le mode beep voice 2 dans state_and_mode
	ori.w	#$0400,d0	
	move.w	d0,state_and_mode
	
	clr.l	note_time_voice2
	rts