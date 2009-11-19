;--------------------------------
;Geoffrey ANNEHEIM
;08/08/03

;Simple exemple for PolySnd v2.0
;--------------------------------

	include		"doorsos.h"			;Incus Doorsos, ici on programme en Kernel
	include		"polysnd2.h"			;Inclus PolySnd v2.0 (ici Dynamique ASM)
	
	xdef		_ti89
	xdef		_ti92plus
	xdef		_main
	xdef		_comment
	
_main: 
 	move.w	#$0700,d0				;Désactive toutes les interruptions
	trap	#1					;Appel routine du TIOS
	bclr.b	#2,($600001)				;Retire la protection de la table vectorielle
	move.l	$64,int1				;Sauvegarde l'adresse Int1
	move.l	$68,int2				;Sauvegarde l'adresse Int2
	move.l	#empty_handler,$64			;Redirige vers empty_handler
	move.l	#empty_handler2,$68			;Redirige vers ampty_handler2, inhibe clavier
	bset.b	#2,($600001)				;Remet protection mémoire
	moveq.w	#0,d0					;Active toutes les interruptions
	trap	#1					;Effectué par d0
	
 	bsr	GetCalculator				;Renvoie le model de TI dans calc
 	
 	jsr	polysnd2::EnableSound			;Met le port I/O en son
	jsr	polysnd2::InstallSound			;Install l'interurption audio
 
 	moveq.b  #_MONO,d0				;Met PolySnd en mode MONO
 	jsr	polysnd2::PlayMode			;Effectué par PlayMode
 	 
 	move.w	#8192,d0			
 	jsr	polysnd2::Sample_voice1	
 	 
 	lea	sound(PC),a0
	move.w	#fin_sound-sound,d0
	jsr	polysnd2::PlayWAV_voice1
	 	 	 	
 	moveq.b  #_VOICE1,d0				;Pour toutes les voies
 	jsr	polysnd2::SetState			;Par défaut PolySnd et en mode pause pour éviter
 	
 	
loop_key:						;Boucle clavier	
 	jsr	polysnd2::GetState
 	tst.b	d0
 	beq	fin
 	
 	cmp.b	#0,calc					;Si TI89
	beq	key_89					;key_89
	bne	key_92					;key_92 focntionne aussi sur V200	

key_89: move.w	#6,d0					;Repére ESC
	bsr	masque
	btst.b	#0,d0
	beq	fin					;Si pressé fin
	bne	loop_key				;Sinon loop

key_92: move.w	#8,d0					;Repéré ESC
	bsr	masque	
	btst.b	#6,d0
	beq	fin					;Si pressé fin
	bra	loop_key				;sinon loop
 
fin:	
 	jsr	polysnd2::UninstallSound		;Desinstall l'interurption audio et la remet à son état courant
	jsr	polysnd2::DisableSound			;Idem pour l'interruption du port I/O
	
	move.w	#$0700,d0				;Désactive interruptions
	trap	#1
	bclr.b	#2,($600001)
	move.l	int1(PC),$64				;Remet à son état courant l'auto int 1 et 2
	move.l	int2(PC),$68
	bset.b	#2,$600001
	moveq.w	#0,d0
	trap	#1
 	rts	


empty_handler:
	rte
	
empty_handler2:
	sf.b $60001b 					;Permet d'inhiber le clavier
	rte 
	
;----------------------------
;Detection de la calculatrice
;----------------------------
GetCalculator:
	movem.l	a1,-(a7)			;Sauvegarde a1
	move.l $C8,a0				;Addresse pour vérifier si TI-89 ou TI-92 Plus
	move.b	#1,calc				;Prédéfinis comme TI-92 Plus
	move.l	a0,d1				;Prépare d1
	and.l #$400000,d1			;Si ~0 alors TI-92 Plus sinon TI-89 ou V200
	bne	__calculator			;Dans cas TI-92 Plus alors Fin
	move.b	#0,calc				;Prédéfinis comme TI-89
	move.l $2F*4(a0),a1			;Prépare condition
	cmp.b	#200,2(a1)			;Si 200 alors TI-89
	bcs	__calculator			;Donc Fin
	move.b	#2,calc				;Donc V200
__calculator:
	movem.l	(a7)+,a1			;Restaure a1
	rts
	
	
;--------------------------------------------
;Masque pour la lecture du clavier	
;********************************************
;********************************************
;masque une ligne du clavier
;--------------------------------------------
;d0=ligne -> d0=réponse
;********************************************
masque:
	movem.l	d1,-(a7)
	move.w	#$FFFE,d1
	rol.w	d0,d1
	clr.w	d0
	move.w	d1,($600018)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	move.b	($60001B),d0
	movem.l	(a7)+,d1
	RTS
	
		
_comment dc.b	"Yahoo exemple WAV",0
calc	 dc.b	0

int1	dc.l	0
int2	dc.l	0
	EVEN
	
	
;********************************************************************
;Données WAV
;********************************************************************
sound:
	dc.b	$33,$91,$FF,$FF,$FF,$FF,$07,$C0,$13,$7E
	dc.b	$80,$1F,$FC,$03,$FE,$83,$01,$FE,$03,$04
	dc.b	$1E,$80,$70,$00,$70,$00,$1F,$80,$13,$04
	dc.b	$1E,$9C,$27,$80,$73,$80,$01,$FC,$83,$0F
	dc.b	$E2,$1C,$24,$90,$63,$38,$81,$03,$23,$08
	dc.b	$02,$3C,$C0,$21,$FE,$70,$82,$03,$07,$13
	dc.b	$1C,$39,$98,$E0,$C0,$C1,$04,$07,$0E,$06
	dc.b	$38,$F8,$30,$C0,$C1,$07,$01,$0C,$3C,$38
	dc.b	$E0,$F0,$C1,$01,$87,$0F,$1E,$38,$78,$30
	dc.b	$C0,$C1,$07,$07,$0E,$3E,$38,$F0,$F0,$C1
	dc.b	$00,$07,$1F,$04,$3F,$F8,$60,$F8,$C1,$07
	dc.b	$83,$0F,$7E,$10,$FC,$F0,$83,$E1,$01,$3F
	dc.b	$08,$0E,$F8,$41,$70,$C0,$0F,$86,$07,$FC
	dc.b	$20,$38,$F0,$0F,$C3,$01,$7E,$10,$1C,$F0
	dc.b	$83,$E1,$C0,$3F,$18,$0E,$F2,$C3,$E1,$E0
	dc.b	$1F,$1C,$06,$F0,$C1,$70,$90,$1F,$0C,$07
	dc.b	$F9,$C1,$70,$90,$1F,$04,$07,$F8,$E1,$70
	dc.b	$80,$1F,$0E,$07,$FF,$E1,$70,$F0,$0F,$0E
	dc.b	$07,$E7,$60,$70,$78,$0E,$86,$83,$E7,$E0
	dc.b	$38,$78,$0E,$86,$83,$E7,$20,$38,$08,$06
	dc.b	$87,$C1,$70,$70,$1C,$0C,$07,$C3,$C1,$7F
	dc.b	$38,$1C,$9C,$83,$C3,$E0,$39,$38,$0E,$92
	dc.b	$83,$E0,$E0,$1C,$0C,$07,$FF,$41,$70,$F0
	dc.b	$1C,$04,$07,$F9,$E0,$30,$98,$0F,$8E,$83
	dc.b	$FC,$20,$18,$CC,$07,$C7,$C1,$73,$30,$1C
	dc.b	$24,$07,$C3,$60,$30,$38,$0E,$82,$83,$E3
	dc.b	$30,$3C,$38,$8E,$E3,$C1,$E1,$18,$1F,$1C
	dc.b	$06,$F1,$C1,$71,$10,$1F,$1C,$07,$F1,$C1
	dc.b	$71,$90,$1F,$0C,$07,$F1,$C1,$71,$10,$1E
	dc.b	$1C,$06,$E1,$81,$E3,$10,$3C,$38,$88,$C3
	dc.b	$03,$07,$31,$7C,$70,$30,$C7,$0F,$0E,$1E
	dc.b	$FC,$C1,$C1,$01,$7F,$30,$18,$F2,$0F,$1C
	dc.b	$8C,$F0,$3F,$38,$F8,$70,$78,$70,$00,$C7
	dc.b	$FF,$83,$83,$83,$87,$3F,$00,$C0,$FF,$E3
	dc.b	$79,$00,$38,$7C,$FC,$C1,$83,$03,$13,$1E
	dc.b	$38,$FC,$20,$00,$07,$CF,$73,$86,$00,$04
	dc.b	$38,$3E,$F9,$80,$03,$82,$E3,$F0,$63,$30
	dc.b	$8F,$30,$82,$41,$12,$1C,$3C,$06,$C9,$E1
	dc.b	$C1,$E1,$1C,$83,$3F,$FF,$E0,$1C,$E0,$C7
	dc.b	$03,$0C,$32,$0C,$FF,$83,$F0,$00,$21,$0C
	dc.b	$63,$18,$62,$38,$F0,$04,$C3,$0F,$C7,$01
	dc.b	$FE,$79,$00,$81,$1F,$00,$07,$1F,$00,$FC
	dc.b	$30,$F8,$01,$03,$E0,$E0,$1F,$0F,$06,$C2
	dc.b	$7F,$F8,$1F,$08,$00,$00,$FF,$7F,$18,$00
	dc.b	$E0,$8F,$FF,$03,$00,$00,$FE,$FF,$18,$00
	dc.b	$00,$C6,$FF,$7F,$00,$00,$00,$FE,$FF,$E1
	dc.b	$04,$00,$FF,$F9,$1F,$00,$FC,$03,$FF,$03
	dc.b	$1C,$00,$F0,$1F,$F8,$0F,$80,$1F,$F0,$3F
	dc.b	$10,$1F,$00,$7F,$80,$7F,$00,$FE,$C0,$FF
	dc.b	$01,$FC,$00,$FC,$01,$FE,$00,$FC,$01,$FF
	dc.b	$01,$FF,$00,$FC,$00,$FC,$00,$FE,$80,$FF
	dc.b	$00,$7F,$00,$1E,$80,$3F,$C0,$1F,$F0,$1F
	dc.b	$E0,$0F,$E0,$07,$F0,$03,$F8,$03,$FC,$03
	dc.b	$FC,$00,$FE,$00,$7E,$00,$3F,$80,$7F,$F0
	dc.b	$3F,$E0,$0F,$E0,$0F,$E0,$0F,$F0,$07,$FE
	dc.b	$07,$F8,$01,$FC,$01,$FE,$00,$7E,$C0,$FF
	dc.b	$C0,$3F,$C0,$1F,$E0,$0F,$C0,$07,$F0,$07
	dc.b	$FE,$01,$FC,$00,$FE,$80,$7F,$80,$3F,$C0
	dc.b	$1F,$C0,$1F,$E0,$0F,$F0,$07,$F0,$03,$FC
	dc.b	$07,$FE,$00,$7E,$00,$7F,$80,$1F,$C0,$1F
	dc.b	$F0,$07,$F0,$07,$F8,$01,$FC,$00,$FE,$C0
	dc.b	$7F,$C0,$1F,$C0,$0F,$F0,$07,$F0,$03,$FC
	dc.b	$01,$FF,$80,$7F,$C0,$0F,$C0,$03,$F0,$07
	dc.b	$FE,$03,$FC,$00,$FE,$C0,$1F,$80,$07,$F0
	dc.b	$3F,$3C,$00,$FC,$81,$FF,$00,$06,$00,$03
	dc.b	$F8,$3F,$F8,$00,$C0,$C0,$79,$00,$1F,$80
	dc.b	$FF,$FF,$03,$00,$00,$FE,$F3,$7F,$00,$00
	dc.b	$E0,$FF,$FF,$03,$00,$80,$FF,$FF,$1F,$00
	dc.b	$00,$FE,$0F,$7F,$00,$60,$E0,$7F,$F0,$01
	dc.b	$00,$02,$FF,$C3,$33,$00,$1F,$FC,$07,$E0
	dc.b	$01,$FC,$E0,$3F,$80,$0F,$F0,$3F,$FC,$01
	dc.b	$0E,$C0,$3F,$F0,$0F,$FC,$03,$FC,$00,$1E
	dc.b	$E0,$4F,$F8,$03,$78,$80,$FF,$C1,$07,$30
	dc.b	$00,$FC,$81,$3F,$C0,$1F,$F8,$07,$00,$00
	dc.b	$30,$F0,$1F,$FC,$03,$FC,$01,$7F,$90,$07
	dc.b	$80,$03,$7E,$C0,$1F,$E0,$0F,$FC,$01,$30
	dc.b	$00,$1F,$FE,$07,$E0,$01,$7E,$E0,$3F,$C0
	dc.b	$03,$78,$80,$7F,$C0,$07,$F8,$87,$FF,$00
	dc.b	$0E,$E0,$0F,$FF,$03,$78,$C0,$3F,$F8,$07
	dc.b	$F0,$00,$3F,$C0,$1F,$E0,$03,$FE,$03,$1F
	dc.b	$00,$07,$FC,$07,$FF,$00,$1E,$E0,$1F,$F8
	dc.b	$01,$7C,$80,$3F,$E0,$07,$F8,$03,$7F,$00
	dc.b	$00,$C0,$07,$FE,$C1,$3F,$E0,$0F,$FC,$03
	dc.b	$7C,$80,$3F,$E0,$07,$F8,$01,$FF,$C0,$0F
	dc.b	$C0,$03,$F8,$C1,$7F,$C0,$0F,$E0,$03,$FF
	dc.b	$00,$1E,$C0,$0F,$F0,$03,$FC,$87,$3F,$E0
	dc.b	$07,$E0,$01,$FE,$C3,$3F,$F0,$0F,$FC,$03
	dc.b	$F0,$E0,$1F,$E0,$07,$FC,$01,$7E,$F8,$0F
	dc.b	$E0,$01,$F8,$00,$3E,$F0,$0F,$F8,$01,$FF
	dc.b	$00,$0F,$E0,$0F,$7C,$00,$FF,$C0,$7F,$E0
	dc.b	$03,$98,$00,$7F,$C0,$1F,$F0,$03,$FE,$00
	dc.b	$1F,$80,$0F,$FC,$01,$3E,$80,$0F,$F0,$0F
	dc.b	$7C,$00,$3C,$E0,$0F,$F8,$01,$7C,$00,$3F
	dc.b	$F0,$03,$F8,$01,$7F,$00,$0F,$F0,$03,$FF
	dc.b	$81,$1F,$80,$07,$F8,$01,$7E,$80,$0F,$F0
	dc.b	$0F,$F8,$00,$7F,$E0,$1F,$C0,$03,$FC,$C1
	dc.b	$3F,$C0,$07,$F0,$01,$FE,$80,$1F,$F0,$07
	dc.b	$FC,$01,$FC,$E0,$3F,$C0,$03,$FC,$01,$7F
	dc.b	$E0,$0F,$E0,$03,$FC,$00,$3E,$E0,$0F,$F0
	dc.b	$03,$3E,$00,$3F,$F0,$07,$78,$00,$7E,$C0
	dc.b	$1F,$F8,$03,$F8,$00,$7E,$C0,$0F,$F0,$07
	dc.b	$FC,$00,$3F,$C0,$0F,$F8,$03,$78,$C0,$3F
	dc.b	$E0,$0F,$FC,$01,$7C,$80,$1F,$E0,$07,$FE
	dc.b	$01,$3F,$C0,$07,$FC,$03,$7E,$00,$1F,$E0
	dc.b	$07,$FC,$01,$7C,$80,$1F,$F0,$03,$FE,$01
	dc.b	$3F,$E0,$07,$F8,$00,$FE,$80,$0F,$C0,$0F
	dc.b	$FC,$80,$FF,$C0,$07,$F8,$01,$7E,$80,$3F
	dc.b	$F0,$07,$F8,$01,$7F,$C0,$1F,$F0,$03,$FC
	dc.b	$80,$3F,$E0,$0F,$F8,$01,$7C,$C0,$0F,$E0
	dc.b	$07,$FC,$01,$3F,$80,$0F,$E0,$03,$FE,$80
	dc.b	$1F,$C0,$07,$FC,$03,$7F,$00,$0F,$F0,$07
	dc.b	$FC,$00,$3C,$E0,$1F,$F0,$01,$78,$80,$1F
	dc.b	$F0,$07,$E0,$01,$7F,$80,$0F,$E0,$03,$FC
	dc.b	$80,$1F,$80,$07,$FC,$03,$7C,$80,$1F,$E0
	dc.b	$07,$FC,$00,$3E,$E0,$0F,$F0,$01,$78,$80
	dc.b	$1F,$F0,$03,$F8,$80,$3F,$C0,$07,$E0,$07
	dc.b	$7F,$00,$1F,$80,$07,$FE,$00,$18,$00,$07
	dc.b	$F0,$81,$78,$60,$0E,$FC,$03,$78,$40,$1E
	dc.b	$F0,$03,$E0,$00,$3F,$F0,$07,$80,$01,$7E
	dc.b	$C0,$0F,$C0,$03,$FE,$80,$1F,$80,$07,$F0
	dc.b	$80,$3F,$80,$07,$E0,$81,$7F,$00,$0E,$F8
	dc.b	$03,$7C,$00,$1F,$80,$07,$FC,$00,$3C,$E0
	dc.b	$1F,$F8,$01,$7C,$80,$1F,$F8,$03,$F8,$80
	dc.b	$3F,$E0,$07,$F8,$01,$7C,$C0,$0F,$E0,$03
	dc.b	$FE,$81,$1F,$C0,$07,$F0,$81,$3F,$80,$0F
	dc.b	$F8,$03,$7E,$80,$3F,$C0,$07,$FC,$01,$7E
	dc.b	$C0,$0F,$F8,$01,$FE,$00,$1F,$FC,$0F,$F8
	dc.b	$01,$3E,$E0,$0F,$F0,$03,$E4,$80,$3F,$E0
	dc.b	$07,$F0,$03,$FF,$E0,$0F,$F0,$01,$FE,$80
	dc.b	$3F,$C0,$07,$FE,$00,$1F,$00,$07,$FC,$01
	dc.b	$7F,$E0,$0F,$C0,$03,$FE,$81,$1F,$80,$07
	dc.b	$FE,$C1,$3F,$C0,$0F,$F8,$03,$7E,$C0,$1F
	dc.b	$F8,$07,$FC,$00,$3C,$E0,$0F,$FC,$03,$7F
	dc.b	$C0,$1F,$F0,$03,$70,$80,$3F,$E0,$0F,$F8
	dc.b	$01,$7E,$C0,$0F,$C0,$01,$FE,$E0,$3F,$F0
	dc.b	$07,$FC,$00,$1F,$E0,$07,$F8,$01,$7E,$C0
	dc.b	$0F,$F0,$01,$FC,$00,$3F,$F0,$07,$FC,$80
	dc.b	$1F,$C0,$07,$FC,$81,$7F,$E0,$1F,$F0,$03
	dc.b	$7E,$C0,$1F,$C0,$07,$FE,$80,$1F,$E0,$07
	dc.b	$FC,$01,$3F,$E0,$07,$F8,$01,$3F,$C0,$0F
	dc.b	$F8,$03,$7F,$00,$1F,$F0,$03,$FC,$80,$3F
	dc.b	$E0,$0F,$FC,$01,$3E,$00,$1F,$F0,$03,$F8
	dc.b	$C0,$3F,$E0,$07,$F0,$01,$7E,$F0,$1F,$F8
	dc.b	$07,$7E,$80,$1F,$C0,$0F,$F8,$07,$FE,$00
	dc.b	$0F,$C4,$03,$FE,$80,$3F,$F0,$1F,$FC,$01
	dc.b	$3F,$C0,$09,$F8,$03,$FE,$80,$7F,$F0,$1F
	dc.b	$FC,$03,$7F,$C0,$1F,$F0,$0F,$FC,$83,$FF
	dc.b	$C0,$3F,$90,$07,$FC,$80,$3F,$E0,$0F,$F8
	dc.b	$03,$FC,$00,$3F,$80,$0F,$F0,$0F,$FC,$03
	dc.b	$3E,$C0,$0F,$F0,$0F,$F0,$03,$FF,$10,$3E
	dc.b	$00,$00,$C0,$3F,$FF,$8F,$10,$00,$FC,$01
	dc.b	$FE,$00,$FF,$00,$FF,$F8,$21,$80,$01,$70
	dc.b	$1E,$F8,$7F,$00,$1C,$80,$3F,$00,$7E,$E0
	dc.b	$87,$03,$06,$07,$FE,$FF,$87,$07,$00,$8F
	dc.b	$81,$FF,$23,$00,$F8,$E1,$79,$00,$1C,$00
	dc.b	$FE,$7F,$C0,$7F,$00,$0C,$00,$F8,$0F,$F0
	dc.b	$7F,$00,$00,$90,$FF,$FF,$7F,$00,$00,$C4
	dc.b	$7C,$F8,$01,$FF,$81,$67,$CC,$19,$00,$00
	dc.b	$FF,$03,$FC,$F8,$1F,$00,$00,$E0,$00,$F8
	dc.b	$FF,$0F,$00,$60,$F0,$FF,$FF,$07,$02,$00
	dc.b	$7E,$78,$00,$06,$0F,$E1,$63,$7C,$38,$01
	dc.b	$02,$FF,$71,$F0,$7F,$04,$00,$00,$FF,$7F
	dc.b	$F8,$3F,$00,$40,$E0,$FF,$07,$E0,$01,$3C
	dc.b	$00,$7C,$C0,$1F,$E0,$1F,$C8,$04,$FC,$87
	dc.b	$0F,$40,$FE,$E0,$3F,$FF,$11,$00,$00,$9C
	dc.b	$FF,$E7,$07,$E2,$01,$F0,$00,$78,$80,$FF
	dc.b	$F0,$0F,$F8,$1F,$38,$04,$C0,$19,$80,$FF
	dc.b	$F8,$FF,$0F,$80,$01,$70,$80,$7F,$3C,$0F
	dc.b	$80,$1F,$33,$00,$E0,$0F,$3C,$FC,$C3,$C1
	dc.b	$0F,$F0,$7C,$00,$81,$F9,$1C,$C0,$FF,$F3
	dc.b	$18,$00,$80,$87,$83,$FF,$78,$10,$0E,$06
	dc.b	$23,$00,$3C,$FE,$0F,$C0,$83,$07,$00,$98
	dc.b	$C7,$01,$48,$3C,$18,$8F,$01,$91,$78,$30
	dc.b	$1F,$07,$10,$60,$20,$1E,$FE,$3F,$00,$00
	dc.b	$FC,$FF,$7F,$C0,$00,$C0,$CF,$FF,$04,$00
	dc.b	$70,$FC,$F1,$CF,$03,$00,$E0,$FF,$01,$00
	dc.b	$00,$FF,$FF,$FF,$07,$00,$00,$E0,$3F,$00
	dc.b	$00,$80,$FF,$FF,$7F,$00,$00,$8E,$FF,$1F
	dc.b	$00,$1C,$F0,$FF,$47,$04,$00,$E0,$F3,$7F
	dc.b	$00,$70,$C0,$FF,$7F,$7C,$00,$C0,$00,$FC
	dc.b	$00,$F8,$FF,$FF,$0F,$78,$00,$C0,$01,$FE
	dc.b	$03,$E0,$C3,$FF,$03,$FC,$00,$C0,$01,$FC
	dc.b	$00,$FC,$C7,$FF,$0F,$3C,$00,$04,$03,$FC
	dc.b	$00,$98,$1F,$FF,$1F,$7C,$00,$18,$01,$F0
	dc.b	$00,$E0,$7F,$FE,$7F,$30,$01,$00,$0C,$E0
	dc.b	$07,$C0,$FF,$FF,$1F,$00,$01,$00,$71,$00
	dc.b	$7F,$00,$FF,$F3,$FF,$01,$04,$00,$E0,$03
	dc.b	$9C,$07,$F0,$FF,$FF,$1F,$00,$08,$00,$33
	dc.b	$00,$7E,$80,$FF,$FF,$FF,$01,$00,$00,$30
	dc.b	$1F,$E0,$1F,$FE,$7F,$E0,$33,$00,$7E,$00
	dc.b	$FC,$00,$FC,$CF,$FF,$87,$E0,$03,$C0,$19
	dc.b	$80,$01,$00,$FF,$F1,$FF,$01,$0E,$00,$04
	dc.b	$01,$1C,$02,$F8,$9F,$FF,$0F,$38,$06,$80
	dc.b	$01,$80,$1F,$E0,$FF,$FF,$3F,$10,$1E,$00
	dc.b	$1E,$00,$7F,$80,$7F,$FC,$7F,$80,$71,$00
	dc.b	$00,$00,$FC,$00,$FF,$F0,$FF,$C0,$3F,$00
	dc.b	$21,$00,$7F,$00,$FF,$E0,$FF,$C1,$1F,$80
	dc.b	$1F,$00,$3F,$80,$7F,$84,$7F,$04,$1F,$C0
	dc.b	$1F,$00,$0E,$E0,$7F,$F0,$3F,$F8,$3F,$E0
	dc.b	$01,$00,$0F,$E0,$0F,$C7,$3F,$F8,$1F,$C0
	dc.b	$07,$C0,$0F,$C0,$3F,$C0,$FF,$C0,$7F,$00
	dc.b	$7F,$00,$FE,$00,$FF,$00,$FF,$03,$FF,$03
	dc.b	$F8,$03,$F8,$07,$F8,$1F,$F8,$1F,$84,$1F
	dc.b	$E0,$3F,$00,$3C,$06,$F0,$3F,$F0,$3C,$38
	dc.b	$C6,$00,$84,$01,$F0,$0F,$E1,$7F,$E0,$FF
	dc.b	$00,$FE,$01,$F0,$01,$E0,$1F,$E0,$7F,$80
	dc.b	$3F,$00,$3E,$80,$FF,$03,$FF,$07,$FE,$07
	dc.b	$86,$0F,$C0,$07,$C0,$0F,$F0,$7F,$E0,$7F
	dc.b	$00,$7E,$C0,$79,$00,$7C,$60,$FC,$01,$FC
	dc.b	$01,$F8,$01,$FC,$03,$FC,$81,$FF,$83,$FF
	dc.b	$03,$C1,$03,$CE,$01,$FC,$01,$7F,$80,$7F
	dc.b	$80,$3F,$00,$1C,$08,$1F,$C0,$3F,$00,$3F
	dc.b	$80,$3F,$C0,$0F,$80,$0F,$E0,$1F,$E0,$7F
	dc.b	$E0,$03,$E0,$01,$E0,$03,$FC,$03,$FE,$01
	dc.b	$FF,$80,$3F,$80,$3F,$01,$3F,$00,$3F,$80
	dc.b	$7F,$80,$3F,$C0,$1F,$E0,$07,$F0,$07,$E0
	dc.b	$0F,$E0,$0F,$E0,$0F,$F0,$0F,$C0,$1F,$80
	dc.b	$1F,$C0,$3F,$80,$3F,$00,$7F,$00,$7F,$00
	dc.b	$FE,$01,$FC,$03,$FC,$03,$F8,$07,$F8,$07
	dc.b	$F0,$0F,$C0,$3F,$C0,$FF,$00,$FE,$80,$F1
	dc.b	$07,$F9,$0F,$C0,$3F,$80,$7F,$00,$FF,$07
	dc.b	$FE,$0F,$CC,$1F,$C0,$3F,$80,$7F,$80,$7F
	dc.b	$00,$FE,$01,$FC,$07,$F8,$0F,$F0,$1F,$30
	dc.b	$FE,$E1,$FF,$21,$F9,$CF,$FF,$1C,$F8,$43
	dc.b	$E0,$07,$FC,$07,$FC,$03,$FC,$0F,$F8,$0F
	dc.b	$E0,$1F,$F0,$1F,$E0,$3F,$00,$7F,$E0,$1F
	dc.b	$E1,$3F,$E0,$3F,$F3,$3F,$FF,$3F,$FF,$07
	dc.b	$FE,$3F,$FC,$0F,$FC,$0F,$FC,$0F,$FC
fin_sound:
	end