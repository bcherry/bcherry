	section ".data"
	xdef	pSnd_SetInterrupt5Rate
	
pSnd_SetInterrupt5Rate:
;HW1=11569 Hz
;HW2=8192 Hz
	lea	counter_frequency_int5,a0					;Adresse de counter_frequency_int5 dans a0
	clr.w	(a0)								;Efface counter_frequency_int5					
	move.w	d0,30(a0)							;Déplace a0 dans interrupt5rate
	move.w	#8192,d1
	cmp.b	#1,Hardware
	beq.s	SetInterruptRate_HW1
suite_SetInterruptRate_HW1:	
	divu.w	d0,d1
	mulu.w	#32768,d1
	move.w	d1,-26(a0)							;Nouvelle valeur dans add_frequency_int5
	rts

SetInterruptRate_HW1:
	move.w	#11569,d1
	bra.s	suite_SetInterruptRate_HW1