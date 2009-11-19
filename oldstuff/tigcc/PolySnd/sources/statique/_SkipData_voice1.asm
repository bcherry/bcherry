	section ".data"
	xdef	pSnd_SkipData_voice1
	
pSnd_SkipData_voice1:
	move.l	a0,-(a7)
	lea	_fcounter1,a0
	clr.w	(a0)					;_fcounter1
	clr.w	2(a0)					;_ucounter1
	clr.w	8(a0)					;frequency_voice1
	clr.l	28(a0)					;note_time_voice1
	
	clr.b	-50(a0)					;lenght_voice1
	clr.b	-48(a0)					;_lcounter1
	clr.b	-39(a0)					;note_voice1	
		
	bclr.b	#0,($60000E)
	move.l	(a7)+,a0
	rts