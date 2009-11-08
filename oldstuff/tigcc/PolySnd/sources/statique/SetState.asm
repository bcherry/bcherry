	section ".data"	
	xdef	pSnd_SetState

pSnd_SetState:
	move.w	state_and_mode,d1
	andi.w	#%1011110001111000,d1					;Masque, extrait state voice1 et state voice2
	btst.b	#0,d0							;Si bit 0 de d0 est égale à 0
	beq.s	suite_setstate_voice1					;Passer à la suite
	ori.w	#$0080,d1						;state voice1 à 1
	
suite_setstate_voice1:			
	btst.b	#1,d0							;Si bit 1 de d0 est égale à 0
	beq.s	suite_setstate_voice2					;Passer à la suite
	ori.w	#$4000,d1						;state voice2 à 1

suite_setstate_voice2:
	move.w	d1,state_and_mode					;Enregistre la trasnformation dans state_and_mode
	rts