	section ".data"
	xdef	pSnd_Ptridle_voice1
	
pSnd_Ptridle_voice1:
	move.l	a0,d0
	tst.l	d0
	beq.s	return_ptr_voice1
	move.l	d0,ptridle_voice1
	bsr	pSnd_SkipData_voice1
	suba.l	a0,a0
	rts
	
return_ptr_voice1:
	move.l	ptridle_voice1,a0
	rts