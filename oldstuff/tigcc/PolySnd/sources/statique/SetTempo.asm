	section ".data"
	xdef	pSnd__SetTempo_
	
;Fonction _Set_Tempo
pSnd__SetTempo_:
;HW1=11569 Hz
;HW2=8192 Hz
;15360=(HW2) 8192*60=32*120*132
;21692=(HW1) 11569*60=32*120*181

;15360=(8192*60)/32
;21692=(11569*60)/32

;Change un tempo en bpm en cycle tempo pour PolySnd. 
	cmp.b	#1,Hardware
	beq.s	__SetTempo_HW1
	move.l	#15360,d1
suite_set_tempo:
	divu	d0,d1
	
	tst.b	d2
	beq.s	_SetTempo_voice1_
	move.w	d1,unittps_voice2
	rts
	
__SetTempo_HW1:
	move.l	#21692,d1
	bra	suite_set_tempo

_SetTempo_voice1_:
	move.w	d1,unittps_voice1
	rts
