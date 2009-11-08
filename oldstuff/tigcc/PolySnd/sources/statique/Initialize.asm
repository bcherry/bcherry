	section ".data"
	xdef	pSnd_Initialize

pSnd_Initialize:
;103=Size of table
;51=(104/2)-1
	moveq	#51,d0
	lea	length_voice1(PC),a0
_loop_erase:
	clr.w	(a0)+
	dbf	d0,_loop_erase	

	lea	_fcounter1+20(PC),a0
	move.w	#$8000,(a0)
	move.w	#8000,20(a0)
	move.w	#8000,22(a0)
	move.w	#1,32(a0)
	rts