	section ".data"
	xdef	pSnd_GetState

pSnd_GetState:
	move.w	state_and_mode,d1						;state_and_mode dans d1
	moveq	#0,d0								;Efface le contenu d0
	btst.w	#7,d1								;Si bit 7 de d1 à 0
	beq.s	noset_getstate_voice1						;Alors passe
	moveq	#1,d0								;Sinon met le bit 0 de d0 à 1

noset_getstate_voice1:
	btst.w	#14,d1								;Si bit 14 de d1 à 0
	beq.s	noset_getstate_voice2						;Alors passe2
	ori.b 	#2,d0								;Met le bit 1 de d0 à 1
	
noset_getstate_voice2:
	rts