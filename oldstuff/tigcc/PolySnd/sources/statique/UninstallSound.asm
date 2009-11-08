	section ".data"
	xdef	pSnd_UninstallSound

pSnd_UninstallSound:
	;Désactive l'interruption n°5
	;move.w	#$0700,d0					;Désactive toutes les interuptions
	;trap	#1						;Effectué par trap #1
	move.b	backup_600015,($600015)			;Restaure le port 600015
	cmp.b	#1,Hardware					;Si HW1
	beq.s	uninstall_hw1
	move.b	#$CC,($600017)					;HW2
suite_uninstall:
	bclr.b	#2,($600001)
	move.l	old_int5,$74					;Remet l'ancienne interruption
	bset.b	#2,($600001)

	;moveq	#0,d0						;Réactive toutes les interuptions
	;trap	#1						;Effectué par trap #1
	rts
	
uninstall_hw1:
	move.b	#$B2,($600017)					;HW1
	bra.s	suite_uninstall