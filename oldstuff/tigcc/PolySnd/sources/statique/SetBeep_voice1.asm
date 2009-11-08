	section ".data"
	xdef	pSnd_SetBeep_voice1
	
pSnd_SetBeep_voice1:
	bset.b	#0,State_voice						;State_voice voice1 à 1
	clr.l	time_counter_voice1					;Efface le compteur de temps voice1
	
	move.b	d1,length_voice1					;Change la variable length voice1
	move.w	d0,frequency_voice1					;Change la variable frequency voice1
	
	move.w	unittps_voice1,_ucounter1				;Initialise le compteur de tempo voice1
	move.w	d0,_fcounter1						;Initialise le compteur de fréquence
	move.b	d1,_lcounter1						;Initialise le compteur de durée
	
	;Prepare compatibilité avec HW1 et HW2				;Prépare la fonction play_voice1
	lsr.w	#1,d0								
	sub.w	play_frequency,d0
	move.w	d0,_fcounter1
		
	move.w	state_and_mode,d0
	andi.w	#%1111110010000000,d0					;Active le mode beep voice 1 dans state_and_mode
	ori.w	#$0008,d0
	move.w	d0,state_and_mode	
	
	clr.l	note_time_voice1					;Efface le compteur de temps de note
	rts