	section ".data"
	xdef	pSnd_SkipData_voice2
	
pSnd_SkipData_voice2:
	move.l	a0,-(a7)
	lea	_fcounter2,a0
	clr.w	(a0)
	clr.w	2(a0)
	clr.w	10(a0)
	clr.l	32(a0)
	
	clr.b	-53(a0)				;lenght_voice2
	clr.b	-51(a0)				;_lcounter2
	clr.b	-42(a0)				;note_voice2
	
	bclr.b	#1,($60000E)
	move.l	(a7)+,a0
	rts
