	section ".data"
	xdef	pSnd_PlayMode
	
pSnd_PlayMode:
	move.w	state_and_mode,d1
	andi.w	#%0111110011111000,d1					;Efface le bit PlayMode dans state_and_mode
	move.b	#3,playmode						;Par défaut PlayMode=Mono
	tst.b	d0							;Si d0=0
	beq.s	end_playmode						;Alors passe
	ori.w	#$8000,d1						;Met le bit 15 de state_and_mode à 1
	move.b	#1,playmode

end_playmode:
	move.w	d1,state_and_mode					;enregistre d1 dans state_and_mode
	rts