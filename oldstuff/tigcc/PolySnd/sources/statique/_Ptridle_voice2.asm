	section ".data"
	xdef	pSnd_Ptridle_voice2
	
pSnd_Ptridle_voice2:
	move.l	a0,d0
	tst.l	d0
	beq.s	return_ptr_voice2
	move.l	d0,ptridle_voice2
	bsr	pSnd_SkipData_voice2
	suba.l	a0,a0
	rts
	
return_ptr_voice2:
	move.l	ptridle_voice2,a0
	rts