	section ".data"
	
	xdef	pSnd_InstallSound

pSnd_InstallSound:
	bsr	GetHardware					;Récupére version Hardawre (1=HW1, 2=HW2)
	bsr	pSnd_Initialize					;Initialise les varaibles de PolySnd
	
	move.w	#$2200,mask_int5				;Masque d'interruptions par défaut
	move.l	#empty_handler,interrupt_handler		;Redirection de la sortie de l'int5 sur rte

	;Active l'interruption n°5
	move.b	($600015),backup_600015				;Sauvegarde du port 600015
	
	;move.w	#$0700,d0					;Désactive toutes les interruptions
	;trap	#1						;Effectué par trap #1
	
	move.b	#40,($60000C)					;Init Full I/O Access
	move.b	#$FF,($600017)					;Reprogram timer to max
	move.b	#$0B,($600015)					;Init Timer (HW1=~11 Khz, HW2=8192 Hz)
	
	move.l	$74,old_int5					;Sauvegarde de l'ancien adresse de int5
	bclr.b	#2,($600001)
	move.l	#int5_handler,$74				;Remplace l'ancienne int5 par celle de PolySnd 
	bset.b	#2,($600001)
	
	;moveq	#0,d0						;Met 0 dans d0
	;trap	#1						;Active toutes les interruptions
	
	cmp.b	#1,Hardware					;Si HW1
	beq.s	install_HW1					;Aller à install_HW1
	move.w	#4,add_frequency_int5				;Convert 8192 Hz at 1 Hz (execute_define_int5_handler)
	move.w	#32768,play_frequency				;Convert correct frequency (play_voice)
	rts
	
install_HW1:
	move.w	#3,add_frequency_int5				;Convert ~11 Khz at 1 Hz (execute_define_int5_handler)
	move.w	#46276,play_frequency				;Convert correct frequency (play_voice)
	rts

;-------------------------------------
;Detection de la version de l'Hardware
;-------------------------------------
GetHardware:
	move.l	$C8,d0
	andi.l	#$600000,d0
	move.l	d0,a0
	move.l	$104(a0),a0
	move.b	#1,Hardware
	sub.l	a0,d0
	cmpi.l	#-$10000,d0
	bls.s	__hardware
	cmpi.w	#$16,(a0)
	bls.s	__hardware

	;move.b	#2,Hardware
	addq.b	#1,Hardware
__hardware:
	rts