;	   			 -------------------------------------
;	     			  PolySnd v2.0 par Geoffrey ANNEHEIM
;	       			   http://perso.wanadoo.fr/tisofts/
;	         		     geoffrey.anneheim@wanadoo.fr
;	             			Compilé avec TI-GCC
;	                 		    07/04/2003
;	    			 -------------------------------------



;************************************************************************************************************
;Juste une remarque, je distribue les sources de PolySnd v2.0 pour permettre une mise à jour constante et
;permettre aux débutants d'améliorer leur niveau.
;Vous n'êtes pas autorisé à modifier et à distribuer ce programme sans mon accord. Si vous avez à modifier
;les sources de PolySnd v2.0 pour vos propres programmes, vous devez m'envoyer vos modifications par mail.
;************************************************************************************************************
;************************************************************************************************************
;Just a thing, I distribute the sources of PolySnd v2.0 in order to allow a constant update and to allow
;beginners to increase their knowledge.
;You are not allowed to modify and distribute this program without my permission. If you have to modify the
;sources of PolySnd v2.0 for your own programs, you must mail me your modifications.
;************************************************************************************************************

	include "doorsos.h"

	xdef _library
	xdef _ti89
	xdef _ti92plus

;Fonctions sonores simple de PolySnd v2.0
	xdef polysnd2@0000
	xdef polysnd2@0001
	xdef polysnd2@0002
	xdef polysnd2@0003
	xdef polysnd2@0004
	xdef polysnd2@0005
	xdef polysnd2@0006
	xdef polysnd2@0007
	xdef polysnd2@0008
	xdef polysnd2@0009
	xdef polysnd2@000A
	xdef polysnd2@000B
	xdef polysnd2@000C
	xdef polysnd2@000D
	xdef polysnd2@000E
	xdef polysnd2@000F
	xdef polysnd2@0010
	xdef polysnd2@0011
	xdef polysnd2@0012
	xdef polysnd2@0013
	xdef polysnd2@0014
	xdef polysnd2@0015
	xdef polysnd2@0016
	xdef polysnd2@0017
	xdef polysnd2@0018
	xdef polysnd2@0019
	xdef polysnd2@001A
	xdef polysnd2@001B
	xdef polysnd2@001C
	xdef polysnd2@001D
	xdef polysnd2@001E
	xdef polysnd2@001F
	xdef polysnd2@0020
	xdef polysnd2@0021
	xdef polysnd2@0022
	xdef polysnd2@0023
	xdef polysnd2@0028
	xdef polysnd2@0029
	xdef polysnd2@002A
	xdef polysnd2@002B
	xdef polysnd2@002C
	xdef polysnd2@002D
	xdef polysnd2@002E
	xdef polysnd2@002F
	xdef polysnd2@0030
	xdef polysnd2@0031
	xdef polysnd2@0032
	xdef polysnd2@0033
	xdef polysnd2@0034
	xdef polysnd2@0035
	
;Fonctions sonores numérique
	xdef polysnd2@0024
	xdef polysnd2@0025
	xdef polysnd2@0026
	xdef polysnd2@0027
	

;**************************************************
;Active l'utilisation du port I/O pour faire du son
;**************************************************
polysnd2@0000:
EnableSound:
; move.b #0,xxx.l = 8 bytes; clr.b xxx.l = 6 bytes.
	clr.b	($60000C)					;Accès au port I/O en bas niveau
	andi.b	#%11111100,($60000E)				;Met à état bas le port I/O
	rts


;**********************************************
;Remet à la normal l'utilisation du port I/O	
;**********************************************
polysnd2@0001:
DisableSound:
	andi.b	#%11111100,($60000E)				;Met à état bas le port I/O				
	move.b	#$8D,($60000C)					;Accès normal au port I/O
	rts


;************************************
;Installation de l'interruption audio
;************************************	
polysnd2@0002:
InstallSound:
	bsr	GetHardware					;Récupére version Hardawre (1=HW1, 2=HW2)
	bsr	_Initialize_PolySnd				;Initialise les varaibles de PolySnd
	
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

	
;***************************************
;Desinstallation de l'interruption audio
;***************************************	
polysnd2@0003:
UninstallSound:
	;Désactive l'interruption n°5
	;move.w	#$0700,d0					;Désactive toutes les interuptions
	;trap	#1						;Effectué par trap #1
	move.b	backup_600015(PC),($600015)			;Restaure le port 600015
	cmp.b	#1,Hardware					;Si HW1
	beq.s	uninstall_hw1
	move.b	#$CC,($600017)					;HW2
suite_uninstall:
	bclr.b	#2,($600001)
	move.l	old_int5(PC),$74				;Remet l'ancienne interruption
	bset.b	#2,($600001)

	;moveq	#0,d0						;Réactive toutes les interuptions
	;trap	#1						;Effectué par trap #1
	rts
	
uninstall_hw1:
	move.b	#$B2,($600017)					;HW1
	bra.s	suite_uninstall


;*************************************
;Initialise variables audio
;*************************************
polysnd2@0004:
_Initialize_PolySnd:
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

;Fonction _Set_Tempo
_SetTempo_:
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


;**************************************
;Réglage des tempo (unittps) //Voice1\\
;
;INPUT d0.w = Tempo
;
;DEL: d0, d1, d2
;**************************************
polysnd2@0005:
SetTempo_voice1:
	moveq	#0,d2							;voice 0
	bsr.s	_SetTempo_
	rts
	
	
;**************************************
;Réglage des tempo (unittps) //Voice2\\
;
;INPUT: d0.w = Tempo
;
;DEL: d0, d1, d2
;**************************************
polysnd2@0006:
SetTempo_voice2:
	moveq	#1,d2							;voice 1
	bsr.s	_SetTempo_
	rts
	
	
;*************************************
;Renvoie le tempo (unittps) //Voice1\\
;
;OUTPUT: d0
;
;DEL: d0, d1
;*************************************
polysnd2@0007:
GetTempo_voice1:
	move.w	unittps_voice1(PC),d1
	cmp.b	#1,Hardware
	beq.s	_set_tempo_voice1_HW1
	move.w	#15360,d0
	divu	d1,d0
	rts
	
_set_tempo_voice1_HW1:
	move.w	#21692,d0
	divu	d1,d0
	rts
	
	
;*************************************
;Renvoie le tempo (unittps) //Voice2\\
;
;OUTPUT: d0
;
;DEL: d0, d1
;*************************************
polysnd2@0008:
GetTempo_voice2:
	move.w	unittps_voice2(PC),d1
	cmp.b	#1,Hardware
	beq.s	_set_tempo_voice2_HW1
	move.w	#15360,d0
	divu	d1,d0
	rts
	
_set_tempo_voice2_HW1:
	move.w	#21692,d0
	divu	d1,d0
	rts
	
	
;**********************************************
;SetState (Régle start ou stop sur chaque voies
;
;INPUT: d0.b = State
;
;DEL: d0, d1
;**********************************************
polysnd2@0009:
SetState:
	move.w	state_and_mode(PC),d1
	andi.w	#%1011110001111000,d1					;Masque, extrait state voice1 et state voice2
	btst.b	#0,d0							;Si bit 0 de d0 est égale à 0
	beq.s	suite_setstate_voice1					;Passer à la suite
	ori.w	#$0080,d1						;state voice1 à 1
	
suite_setstate_voice1:			
	btst.b	#1,d0							;Si bit 1 de d0 est égale à 0
	beq.s	suite_setstate_voice2					;Passer à la suite
	ori.w	#$4000,d1						;state voice2 à 1

suite_setstate_voice2:
	move.w	d1,state_and_mode					;Enregistre la trasnformation dans state_and_mode
	rts
		
	
;**********************************************
;SetLoop (Régle loop sur chaque voies
;
;INPUT: d0.b = Loop
;
;DEL: d0
;**********************************************
polysnd2@000A:
SetLoop:
	move.b	d0,loop
	rts	


;*************************************
;BEEP réglage sur voie1
;
;INPUT: d0.w = Frequence
;	d1.b = Length
;
;DEL: d0, d1
;*************************************
polysnd2@000B:
SetBeep_voice1:
	bset.b	#0,State_voice						;State_voice voice1 à 1
	clr.l	time_counter_voice1					;Efface le compteur de temps voice1
	
	move.b	d1,length_voice1					;Change la variable length voice1
	move.w	d0,frequency_voice1					;Change la variable frequency voice1
	
	move.w	unittps_voice1(PC),_ucounter1				;Initialise le compteur de tempo voice1
	move.w	d0,_fcounter1						;Initialise le compteur de fréquence
	move.b	d1,_lcounter1						;Initialise le compteur de durée
	
	;Prepare compatibilité avec HW1 et HW2				;Prépare la fonction play_voice1
	lsr.w	#1,d0								
	sub.w	play_frequency(PC),d0
	move.w	d0,_fcounter1
		
	move.w	state_and_mode(PC),d0
	andi.w	#%1111110010000000,d0					;Active le mode beep voice 1 dans state_and_mode
	ori.w	#$0008,d0
	move.w	d0,state_and_mode	
	
	clr.l	note_time_voice1					;Efface le compteur de temps de note
	rts


;*************************************
;BEEP réglage sur voie2
;
;INPUT: d0.w = Frequence
;	d1.b = Length
;
;DEL: d0, d1
;*************************************
polysnd2@000C:
SetBeep_voice2:
	bset.b	#1,State_voice						;State_voice voice 1 à 1
	clr.l	time_counter_voice2					;Efface le compteur de temps voice 2
	
	move.b	d1,length_voice2
	move.w	d0,frequency_voice2
	
	move.w	unittps_voice2(PC),_ucounter2
	move.w	d0,_fcounter2
	move.b	d1,_lcounter2
	
	;Prepare compatibilité avec HW1 et HW2
	lsr.w	#1,d0
	sub.w	play_frequency(PC),d0
	move.w	d0,_fcounter2
	
	move.w	state_and_mode(PC),d0			
	andi.w	#%1100000011111000,d0					;Activge le mode beep voice 2 dans state_and_mode
	ori.w	#$0400,d0	
	move.w	d0,state_and_mode
	
	clr.l	note_time_voice2
	rts


;*************************************
;PLAYFX réglages sur voie 1
;
;INPUT: a0.l = Data for PlayFX
;
;DEL: a0.l, d0.w
;*************************************
polysnd2@000D:
PlayFX_voice1:
	bset.b	#0,State_voice
	move.l	a0,ptrstart_voice1
	move.l	a0,ptridle_voice1
	
	bsr	_SkipData_voice1
	clr.l	time_counter_voice1
	
	move.w	state_and_mode(PC),d0
	andi.w	#%1111110010000000,d0					;Mode PlayFx voice 1
	ori.w	#$0010,d0
	move.w	d0,state_and_mode
	rts
	

;*************************************
;PLAYFX réglages sur voie 2
;
;INPUT: a0.l = Data for PlayFX
;
;DEL: a0.l, d0.w
;*************************************
polysnd2@000E:
PlayFX_voice2:
	bset.b	#1,State_voice
	move.l	a0,ptrstart_voice2
	move.l	a0,ptridle_voice2
	
	bsr	_SkipData_voice2
	clr.l	time_counter_voice2
	
	move.w	state_and_mode(PC),d0
	andi.w	#%1100000011111000,d0					;Mode PlayFx voice 2
	ori.w	#$0800,d0
	move.w	d0,state_and_mode
	rts


;***************************************************
;GetFrequency_voice1 (renvoie la fréquence voie n°1)
;
;OUTPUT: d0.w = Frequence
;
;DEL: d0.w
;***************************************************
polysnd2@000F:
GetFrequency_voice1:
	move.w	frequency_voice1(PC),d0
	rts
	
	
;***************************************************
;GetFrequency_voice2 (renvoie la fréquence voie n°2)
;
;OUTPUT: d0.w = Frequence
;
;DEL: d0.w
;***************************************************
polysnd2@0010:
GetFrequency_voice2:
	move.w	frequency_voice2(PC),d0
	rts	
	
	
;***************************************************
;PortStatus (renvoie le status du Port audio)
;
;OUTPUT: d0.b = PortStatus
;
;DEL: d0.b
;***************************************************
polysnd2@0011:
PortStatus:
	move.b	($60000E),d0
	rts		
	
	
;***************************************************
;Int_Handler 5
;
;INPUT: a0.l = Interrupt
;
;DEL: a0.l
;***************************************************	
polysnd2@0012:
Interrupt5:
	move.l	a0,interrupt_handler
	rts	


;*************************************
;Sound réglages sur voie 1
;
;INPUT: a0.l = Ptr for PlaySound
;
;DEL: a0.l, d0.w
;*************************************
polysnd2@0013:
PlaySound_voice1:
	bset.b	#0,State_voice
	move.l	a0,ptrstart_voice1
	move.l	a0,ptridle_voice1
	
	bsr	_SkipData_voice1	
	clr.l	time_counter_voice1
	
	move.w	state_and_mode(PC),d0
	andi.w	#%1111110010000000,d0						;Mode PlaySound voice 1
	ori.w	#$0040,d0
	move.w	d0,state_and_mode
	rts
	

;*************************************
;Sound réglages sur voie 2
;
;INPUT: a0.l = Ptr for PlaySound
;
;DEL: a0.l, d0.w
;*************************************
polysnd2@0014:
PlaySound_voice2:
	bset.b	#1,State_voice
	move.l	a0,ptrstart_voice2
	move.l	a0,ptridle_voice2
	
	bsr	_SkipData_voice2
	clr.l	time_counter_voice2
	
	move.w	state_and_mode(PC),d0
	andi.w	#%1100000011111000,d0						;Mode PlayFx voice 2
	ori.w	#$2000,d0
	move.w	d0,state_and_mode
	rts


;***************************************
;GetLength_voice1 renvoie la durée voie1
;
;OUTPUT: d0.b = Length
;
;DEL: d0.b
;***************************************
polysnd2@0015:
GetLength_voice1:
	move.b	length_voice1(PC),d0
	rts
	
	
;***************************************
;Getlength_voice2 renvoie la durée voie2
;
;OUTPUT: d0.b = Length
;
;DEL: d0.b
;***************************************
polysnd2@0016:
GetLength_voice2:
	move.b	length_voice2(PC),d0
	rts


;**************************************************
;GetTime renvoie le temps d'une musiques en seconde
;
;INPUT: a0.l = Ptr Datas
;	d2.w = Tempo
;
;OUTPUT: d0.w = Time
;
;DEL: d0, d1, d2, a0
;**************************************************
polysnd2@0017:
GetTime:
	moveq.l	#0,d0								;Efface d0
	moveq.l	#0,d1								;Efface d1
	
loop1:										;Boucle
	cmpi.b	#$FF,(a0)+							;jusqu'à ce que la fin des données
	beq.s	end_loop							;Alors fin boulce
	
	move.b	(a0)+,d1							;Déplace l'octet en courds dans d1
	add.l	d1,d0								;Ajoute la durée en cycles dans d0
	bra.s	loop1								;Boucle
	
end_loop:
; Très lent...
	divu	#32,d0								;Divise par 32 (32 cycles=1 temps)
	mulu	#60,d0								;Multiplie par 60 (60 secondes)
	
	divu.w	d2,d0								;Renvoie en fonction du tempo, le temps en seconde des données
	rts


;**************************************************
;GetState renvoie l'état audio	
;
;OUTPUT: d0.b = State
;
;DEL: d0, d1
;**************************************************
polysnd2@0018:
GetState:
	move.w	state_and_mode(PC),d1						;state_and_mode dans d1
	moveq	#0,d0								;Efface le contenu d0
	btst.w	#7,d1								;Si bit 7 de d1 à 0
	beq.s	noset_getstate_voice1						;Alors passe
	moveq	#1,d0								;Sinon met le bit 0 de d0 à 1

noset_getstate_voice1:
	btst.w	#14,d1								;Si bit 14 de d1 à 0
	beq.s	noset_getstate_voice2						;Alors passe2
	ori.b 	#2,d0								;Met le bit 1 de d0 à 1
	
noset_getstate_voice2:
	rts


;**************************************************
;GetMode renvoie mode	
;
;OUTPUT: d0.b = Mode
;
;DEL: d0, d1
;**************************************************
polysnd2@0019:
GetMode:
	move.w	state_and_mode(PC),d0
	move.w	d0,d1
	;Extrait modes voice1
	andi.w	#%0000000001111000,d0
	lsr.w	#3,d0
	;Extrait modes voice2
	andi.w	#%0011110000000000,d1
	lsr.w	#6,d1
	or.w	d1,d0
	rts
	

;**************************************************
;GetLoop renvoie loop
;
;OUTPUT: d0.b = Loop
;
;DEL: d0	
;**************************************************
polysnd2@001A:
GetLoop:
	move.b	loop(PC),d0
	rts
	
	
;*********************************************************
;Time
;Renvoie dans d0 le temps écoulé de la musique en secondes
;
;OUTPUT: d0.w = Time
;
;DEL: d0, d1
;*********************************************************
polysnd2@001B:
_Time_voice1:
;5000=5000 Hz
	move.l	time_counter_voice1(PC),d0
	divu	interrupt5rate(PC),d0
	rts


;*********************************************************
;Time
;Renvoie dans d0 le temps écoulé de la musique en secondes
;
;OUTPUT: d0.w = Time
;
;DEL: d0, d1
;*********************************************************
polysnd2@001C:
_Time_voice2:
;5000=5000 Hz
	move.l	time_counter_voice2(PC),d0
	divu	interrupt5rate(PC),d0 
	rts


;*********************************************************
;SetTime
;Remet à zéro le counter temps voie1 (marge d'erreur de 1)
;
;INPUT: d0.w = Time
;
;DEL: d0
;*********************************************************
polysnd2@001D:
SetTime_voice1:
	mulu	interrupt5rate(PC),d0
	move.l	d0,time_counter_voice1
	rts
	
	
;*********************************************************
;SetTime
;Remet à zéro le counter temps voie2
;
;INPUT: d0.w = Time
;
;DEL: d0
;*********************************************************
polysnd2@001E:
SetTime_voice2:
	mulu	interrupt5rate(PC),d0
	move.l	d0,time_counter_voice2
	rts
	
		
;*********************************************************
;VoiceState
;Option de synchronisation
;
;OUTPUT: d0.b = state_voice
;
;DEL: d0
;*********************************************************
polysnd2@001F:
VoiceState:
	move.b	State_voice(PC),d0
	rts


;*********************************************************
;SetVoiceState
;Réglage de state_voice
;
;INPUT: d0.b = state_voice
;
;DEL: d0
;*********************************************************
polysnd2@0020:
SetVoiceState:
	move.b	d0,State_voice
	rts


;*********************************************************
;SetIntMask
;Valeurs du masque d'interruptions
;
;INPUT: d0.w = mask_int5
;
;DEL: d0
;*********************************************************
polysnd2@0021:
SetIntMask:
	move.w	d0,mask_int5
	rts


;*************************************************************
;AddNote_voice1 Ajoute une nombre de notes dans la table voie1
;
;INPUT: d0.b = add_note
;
;DEL: d0
;*************************************************************		
polysnd2@0022:
AddNotes_voice1:
	move.b	d0,add_note_voice1
	rts


;*************************************************************
;AddNote_voice2 Ajoute une nombre de notes dans la table voie2
;
;INPUT: d0.b = add_note
;
;DEL: d0
;*************************************************************		
polysnd2@0023:
AddNotes_voice2:
	move.b	d0,add_note_voice2
	rts


;/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/
;Fonctions numérique
;\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
;*************************************************************
;PlayWAV voie 1 joue des données WAV de diverse qualitées.
;
;INPUT: a0.l = Ptr WAV
;	d0.w = Size
;
;DEL: d0, a0
;*************************************************************	
polysnd2@0024:
PlayWAV_voice1:
	move.w	d0,wav_size_voice1
	move.l	a0,wav_ptr_voice1
	clr.b	wav_bit_voice1
	
	move.w	state_and_mode(PC),d0
	andi.w	#%1111110010000000,d0					;PlayWAV voice 1
	ori.w	#$0020,d0
	move.w	d0,state_and_mode
	clr.w	wav_counter_sample_voice1
	rts
	
	
;*************************************************************
;PlayWAV voie 2 joue des données WAV de diverse qualitées.
;
;INPUT: a0.l = Ptr WAV
;	d0.w = Size
;
;DEL: d0, a0
;*************************************************************	
polysnd2@0025:
PlayWAV_voice2:
	move.w	d0,wav_size_voice2
	move.l	a0,wav_ptr_voice2
	clr.b	wav_bit_voice2
	
	move.w	state_and_mode(PC),d0
	andi.w	#%1100000011111000,d0					;PlayWAV voice 2
	ori.w	#$1000,d0
	move.w	d0,state_and_mode
	clr.w	wav_counter_sample_voice2
	rts


;**************************************************************
;Sample_voice1 régle fréquence WAV voie1
;
;INPUT: d0.w = sample
;
;DEL: d0
;**************************************************************
polysnd2@0026:
Sample_voice1:
	move.w	d0,wav_sample_voice1
	rts


;**************************************************************
;Sample_voice2 régle fréquence WAV voie2
;
;INPUT: d0.w = sample
;
;DEL: d0
;**************************************************************
polysnd2@0027:
Sample_voice2:
	move.w	d0,wav_sample_voice2
	rts

	
;**************************************************************
;GetNote_voice1 renvoie la note en cours
;
;OUTPUT: d0.b = note
;
;DEL: d0
;**************************************************************	
polysnd2@0028:
GetNote_voice1:
	move.b	note_voice1(PC),d0
	rts


;**************************************************************
;GetNote_voice2 renvoie la note en cours
;
;OUTPUT: d0.b = note
;
;DEL: d0
;**************************************************************	
polysnd2@0029:
GetNote_voice2:
	move.b	note_voice2(PC),d0
	rts

	
;**************************************************************
;_Ptridle_voice1 change ou renvoie le pointeur du fichier. NULL
;Si argument=NULL renvoie pointeur sinon renvoie NULL
;
;INPUT: a0.l = Ptr
;
;OUTPUT: a0.l = Ptr
;
;DEL: d0, a0 
;**************************************************************	
polysnd2@002A:
_Ptridle_voice1:
	move.l	a0,d0
	tst.l	d0
	beq.s	return_ptr_voice1
	move.l	d0,ptridle_voice1
	bsr	_SkipData_voice1
	suba.l	a0,a0
	rts
	
return_ptr_voice1:
	move.l	ptridle_voice1(PC),a0
	rts


;**************************************************************
;_Ptridle_voice2 change ou renvoie le pointeur du fichier. NULL
;Si argument=NULL renvoie pointeur sinon renvoie NULL
;
;INPUT: a0.l = Ptr
;
;OUTPUT: a0.l = Ptr
;
;DEL: d0, a0  
;**************************************************************	
polysnd2@002B:
_Ptridle_voice2:
	move.l	a0,d0
	tst.l	d0
	beq.s	return_ptr_voice2
	move.l	d0,ptridle_voice2
	bsr	_SkipData_voice2
	suba.l	a0,a0
	rts
	
return_ptr_voice2:
	move.l	ptridle_voice2(PC),a0
	rts


;**************************************************************	
;_time_counter_voice1
;Renvoie ou modifie le compteur de temps, cette instruction est
;différente de GetTime et SetTime car ce compteur et en cycles
;d'interruptions et dépend de SetInterrupt5Rate.
;
;INPUT: d0.l = Counter
;
;OUTPUT: d0.l = counter
;
;DEL: d0
;**************************************************************
polysnd2@002C:
_time_counter_voice1:
	tst.l	d0
	beq.s	return_time_voice1
	move.l	d0,time_counter_voice1
	rts
	
return_time_voice1:
	move.l	time_counter_voice1(PC),d0
	rts


;**************************************************************	
;_time_counter_voice2
;Renvoie ou modifie le compteur de temps, cette instruction est
;différente de GetTime et SetTime car ce compteur et en cycles
;d'interruptions et dépend de SetInterrupt5Rate.
;
;INPUT: d0.l = Counter
;
;OUTPUT: d0.l = counter
;
;DEL: d0 
;**************************************************************
polysnd2@002D:
_time_counter_voice2:
	tst.l	d0
	beq.s	return_time_voice2
	move.l	d0,time_counter_voice2
	rts
	
return_time_voice2:
	move.l	time_counter_voice2(PC),d0
	rts
	

;**************************************************************	
;_note_time_counter_voice1
;Renvoie ou modifie le compteur de note. Dépend de SetInterrupt5Rate.
;
;INPUT: d0.l = Counter
;
;OUTPUT: d0.l = counter
;
;DEL: d0  
;**************************************************************
polysnd2@002E:
_note_time_counter_voice1:
	tst.l	d0
	beq.s	return_note_time_voice1
	move.l	d0,note_time_voice1
	rts
	
return_note_time_voice1:
	move.l	note_time_voice1(PC),d0
	rts
	
	
;**************************************************************	
;_note_time_counter_voice2
;Renvoie ou modifie le compteur de note. Dépend de SetInterrupt5Rate.
;
;INPUT: d0.l = Counter
;
;OUTPUT: d0.l = counter
;
;DEL: d0 
;**************************************************************
polysnd2@002F:
_note_time_counter_voice2:
	tst.l	d0
	beq.s	return_note_time_voice2
	move.l	d0,note_time_voice2
	rts
	
return_note_time_voice2:
	move.l	note_time_voice2(PC),d0
	rts


;**************************************************************	
;_SkipData_voice1
;Cette fonction remet à zéro les compteurs de PolySnd v2.0
;**************************************************************
polysnd2@0030:
_SkipData_voice1:
	move.l	a0,-(a7)
	lea	_fcounter1(PC),a0
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
	
	
;**************************************************************	
;_SkipData_voice1
;Cette fonction remet à zéro les compteurs de PolySnd v2.0
;**************************************************************
polysnd2@0031:
_SkipData_voice2:
	move.l	a0,-(a7)
	lea	_fcounter2(PC),a0
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


;**************************************************************	
;PlayMode
;Mode MONO ou Stéréo, permet de gagner en ressources CPU
;
;INPUT: d0.b = PlayMode
;
;DEL: d0, d1
;**************************************************************
polysnd2@0032:
PlayMode:
	move.w	state_and_mode(PC),d1
	andi.w	#%0111110011111000,d1					;Efface le bit PlayMode dans state_and_mode
	move.b	#3,playmode						;Par défaut PlayMode=Mono
	tst.b	d0							;Si d0=0
	beq.s	end_playmode						;Alors passe
	ori.w	#$8000,d1						;Met le bit 15 de state_and_mode à 1
	move.b	#1,playmode

end_playmode:
	move.w	d1,state_and_mode					;enregistre d1 dans state_and_mode
	rts	


;**************************************************************	
;SetLength_voice1
;Régle length voice1
;
;INPUT: d0.b = length
;
;DEL: d0
;**************************************************************	
polysnd2@0033:
SetLength_voice1:
	move.w	unittps_voice1(PC),_ucounter1
	move.b	d0,length_voice1
	move.b	d0,_lcounter1
	rts
	

;**************************************************************	
;SetLength_voice2
;Régle length voice2
;
;INPUT: d0.b = length
;
;DEL: d0
;**************************************************************	
polysnd2@0034:
SetLength_voice2:
	move.w	unittps_voice2(PC),_ucounter2
	move.b	d0,length_voice2
	move.b	d0,_lcounter2
	rts


;***************************************************************
;SetInterrupt5Rate:
;Régle la fréquence de l'auto int personnalisé de PolySnd.
;
;INPUT: d0.w = fréquence
;
;DEL: d1, a0
;****************************************************************
polysnd2@0035:
SetInterrupt5Rate:
;HW1=11569 Hz
;HW2=8192 Hz
	lea	counter_frequency_int5(PC),a0					;Adresse de counter_frequency_int5 dans a0
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
	
					
;/////////////////////////////////////////////////////////
;Méthode pour jouer des fréquences sur 2 voies avec unittps
;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
;------
;Voice1
play_voice1:
	lea	_fcounter1(PC),a0						;Pointeur de _fcounter1 dans a0
	move.w	frequency_voice1(PC),d0						;Frequency_voice1 dans d0
	beq.s	son_1								;Si égale à 0 alors aller à son_1
	add.w	(a0),d0								;d0=_fcounter1+d0
	bcc.s	son_1								;Si dépasse 16 bits alors aller son_1
	sub.w	play_frequency(PC),d0						;d0=play_frequency-d0

_sound_:
	move.w	d0,(a0)								;Rempace _fcounter1 par le contenu de d0
	move.b	playmode(PC),d0							;Déplace playmode dans d0
	eor.b	d0,($60000E)							;XOR avec d0
	subq.w	#1,2(a0)							;Soustrait de 1 le compteur _ucounter1
	rts									;Return to subroutine
	
son_1:
	move.w	d0,(a0)								;Rempace _fcounter1 par le contenu de d0
	subq.w	#1,2(a0)							;Soustrait de 1 le compteur _ucounter1
	rts
	
;------
;Voice2
play_voice2:
	lea	_fcounter2(PC),a0						;Identique que play_voice1
	move.w	frequency_voice2(PC),d0
	beq.s	son_2
	add.w	(a0),d0
	bcc.s	son_2
	sub.w	play_frequency(PC),d0
	eori.b	#2,($60000E)
	
son_2:
	move.w	d0,(a0)
	subq.w	#1,2(a0)
	rts
	

;-------------------------------------------------
;INTERRUPTION AUDIO
;-------------------------------------------------
;*************************************************
;int_5: Interruption de gestion audio     ~8192 hz
;*************************************************
int5_handler:
	move.l	d0,-(a7)							;Sauvegarde de d0
	pea	(a0)								;Sauvegarde de a0
	
	move.w	mask_int5(PC),sr						;Déplace le mask dans sr
 
;Interruption de gestion audio
execute_int5_handler:
	move.w	state_and_mode(PC),d0						;state_and_mode dans d0
	
	add.b	d0,d0								;Bit suivant
	bcc.s	done_voice1							;Si dépasse 8 bits
        
        add.b	d0,d0								;Bit suivant
	bcs	sound_voice1							;Si dépasse 8 bits
	add.b	d0,d0								;Bit suivant
	bcs	play_wav_voice1							;Si dépasse 8 bits
	add.b	d0,d0
	bcs	data_voice1
	add.b	d0,d0
	bcs	beep_voice1
                
done_voice1:
	move.w	state_and_mode(PC),d0						;state_and_mode dans d0
		
	add.w	d0,d0								;Bit suiavnt
	bcc.s	done_voice2							;Si dépasse 16 bits	
        
        add.w	d0,d0
	bcc.s	done_voice2   
           
	add.w	d0,d0
	bcs	sound_voice2
	add.w	d0,d0
	bcs 	play_wav_voice2
	add.w	d0,d0
	bcs	data_voice2
	add.w	d0,d0
	bcs	beep_voice2
 	
done_voice2:
	;frequence HW1/HW2
	lea	counter_frequency_int5(PC),a0					;pointeur de counter_frequency_int dans a0
	move.w	add_frequency_int5(PC),d0					;add_frequency_int5 dans d0
	add.w	d0,(a0)								;Ajoute d0 à counter_frequency_int5
	bpl.s	execute_define_int5_handler					;Si dépasse 32768
	move.l	(a7)+,a0							;Restaure a0
	move.l	(a7)+,d0							;Restaure d0
	rte
     
execute_define_int5_handler:
	subi.w	#$8000,(a0)							;Soustrait de 32768 counter_frequency_int5
	addq.l	#1,2(a0)							;Ajoute 1 à time_counter_voice1
	addq.l	#1,6(a0)							;Ajoute 1 à note_time_counter_voice1
	addq.l	#1,10(a0)							;Ajoute 1 à time_counter_voice2
	addq.l	#1,14(a0)							;Ajoute 1 à note_time_counter_voice2
	move.l	(a7)+,a0							;Restaure a0
	move.l	(a7)+,d0							;Restaure d0
	move.l	interrupt_handler(PC),-(a7)					;Execute le contenu d'interrupt5
	rts									;Va à la fonction


;-*-*-*-*-*-*-*-*-*-*
;Fonction BEEP voie 1
;*-*-*-*-*-*-*-*-*-*-
beep_voice1:
	tst.w	_ucounter1							;Test compteur _ucounter1
	beq.s	done_unittps_beep_voice1					;Si égale à 0
	bsr	play_voice1							;Joue fréquence
	bra	done_voice1							;Fin de la voie 1
	
done_unittps_beep_voice1:
	lea	_ucounter1(PC),a0						;pointeur de _ucounter1 dans a0
	
	subq.b	#1,-50(a0)							;Soustrait de 1 _lcounter1
	tst.b	_lcounter1							;test de _lcounter1
	beq.s	done_length_beep_voice1						;Si égale à 0 done_length_beep_voice1
	move.w	unittps_voice1(PC),(a0)						;Initailise le compteur _ucounter1
	bra	done_voice1							;Fin de la voie 1

done_length_beep_voice1:
	clr.l	22(a0)								;Efface time_counter_voice1
	bsr	_SkipData_voice1						;Efface d'autre variables de base
	
	btst.b	#0,-46(a0)							;Si mode loop voice 1
	bne.s	init_unittps_voice1						;Egale à 1 alors on intialise tout
	bclr.b	#0,-48(a0)							;Efface le mode state_voice
	andi.w	#%1111110000000000,18(a0)					;Efface le mode beep voice1
	bra	done_voice1							;Fin de la voie 1

init_unittps_voice1:
	move.w	unittps_voice1(PC),(a0)						;Initialise _ucounter1
	move.b	length_voice1(PC),-50(a0)					;Initialise _lcounter1
	bra	done_voice1							;_lcounter1 Fin voie 1


;-*-*-*-*-*-*-*-*-*-*
;Fonction BEEP voie 2
;*-*-*-*-*-*-*-*-*-*-
beep_voice2:									;Identique que la voie 1
	tst.w	_ucounter2
	beq.s	done_unittps_beep_voice2
	bsr	play_voice2
	bra	done_voice2
	
done_unittps_beep_voice2:
	lea	_ucounter2(PC),a0

	subq.b	#1,-53(a0)							;_lcounter2
	tst.b	_lcounter2
	beq.s	done_length_beep_voice2
	move.w	unittps_voice1(PC),(a0)
	bra	done_voice2

done_length_beep_voice2:
	clr.l	26(a0)
	bsr	_SkipData_voice2
		
	btst.b	#1,-50(a0)							;loop
	bne.s	init_unittps_voice2
	bclr.b	#1,-51(a0)							;state_voice
	andi.w	#%1000000011111000,14(a0)
	bra	done_voice2

init_unittps_voice2:
	move.w	unittps_voice2(PC),(a0)
	move.b	length_voice2(PC),-53(a0)
	bra	done_voice2


;-*-*-*-*-*-*-*-*-*-*-*-*
;Extraction PLAYFX voie 1
;*-*-*-*-*-*-*-*-*-*-*-*-
data_voice1:
	tst.w	_ucounter1							;Test _ucounter1
	beq.s	done_unittps_playfx_voice1					;Si égale à 0
	bsr	play_voice1							;Joue fréquence voie1
	bra	done_voice1							;Fin de la voie 1
	
done_unittps_playfx_voice1:
	lea	_ucounter1(PC),a0						;pointeur de _ucounter1 dans a0

	tst.b	_lcounter1							;Test _lcounter1
	beq.s	done_length_playfx_voice1					;Si égale à 0
	subq.b	#1,-50(a0)							;Soustrait de 1 _lcounter1
	move.w	unittps_voice1(PC),(a0)						;Initialise _ucounter1
	bra	done_voice1							;Fin de la voie 1
	
done_length_playfx_voice1:
	move.w	unittps_voice1(PC),(a0)						;Initialise _ucounter1
	
extract_voice1:
	clr.w	(a0)								;Efface _ucounter1
	clr.b	-50(a0)								;Efface _lcounter1
	clr.l	26(a0)								;Efface note_tume_counter_voice1
	
	move.l	ptridle_voice1(PC),a0						;Pointeur ptridle_voice1 dans a0
	move.w	(a0)+,frequency_voice1						;Extrait short dans frequency_voice1
	move.w	(a0),d0								;Extrait short suivant dans d0
	move.b	d0,_lcounter1							;Déplace que les 8 bits dans _lcounter1
	move.b	d0,length_voice1						;Idem dans length_voice1

	;Prepare compatibilité avec HW1 et HW2
	lea	_fcounter1(PC),a0						;pointeur _fcounter1 dans a0
	move.w	frequency_voice1(PC),d0						;frequency_voice1 dans d0
	lsr.w	#1,d0								;Rotation d'un bit vers la droite
	sub.w	play_frequency(PC),d0						;d0=play_frequency-d0
	move.w	d0,(a0)								;Déplace d0 dans _fcounter1
	
	addq.l	#4,10(a0)							;Ajoute 4 au pointeur ptridle_voice1
	
	cmpi.w	#$FFFF,frequency_voice1						;Si frequency_voice1 = 0xFFFF
	beq.s	end_playfx_voice1						;Alors end_playfx_voice1
	bra	done_voice1							;Sinon fin voies 1

end_playfx_voice1:
	lea	state_and_mode(PC),a0						;Pointeur state_and_mode dans a0
	clr.l	4(a0)								;Efface time_counter_voice1
	bsr	_SkipData_voice1						;Efface d'autre variables

	btst.b	#0,-64(a0)							;Si mode loop voie 1
	bne.s	init_playfx_voice1						;=1 alors initialise
	andi.b	#2,-66(a0)							;Efface state_voice1
	andi.w	#%1111110000000000,(a0)						;Efface mode PlayFX voice1
	bra	done_voice1							;Fin de la voie 1

init_playfx_voice1:
	move.l	ptrstart_voice1(PC),-10(a0)					;ptrstart_voice1 dans ptridle_voice1
	bra	done_voice1							;Fin de la voie 1


;-*-*-*-*-*-*-*-*-*-*-*-*
;Extraction PLAYFX voie 2
;*-*-*-*-*-*-*-*-*-*-*-*-	
data_voice2:
	tst.w	_ucounter2
	beq.s	done_unittps_playfx_voice2
	bsr	play_voice2
	bra	done_voice2
	
done_unittps_playfx_voice2:
	lea	_ucounter2(PC),a0

	tst.b	_lcounter2
	beq.s	done_length_playfx_voice2
	subq.b	#1,-53(a0)							;_lcounter2
	move.w	unittps_voice2(PC),(a0)
	bra	done_voice2
	
done_length_playfx_voice2:
	move.w	unittps_voice2(PC),(a0)
	
extract_voice2:
	clr.w	(a0)
	clr.b	-53(a0)								;_lcounter2
	clr.l	30(a0)
	
	move.l	ptridle_voice2(PC),a0
	move.w	(a0)+,frequency_voice2
	move.w	(a0),d0
	move.b	d0,_lcounter2
	move.b	d0,length_voice2
	
	;Prepare compatibilité avec HW1 et HW2
	lea	_fcounter2(PC),a0
	move.w	frequency_voice2(PC),d0
	lsr.w	#1,d0
	sub.w	play_frequency(PC),d0
	move.w	d0,(a0)
		
	addq.l	#4,12(a0)							;ptridle_voice2
	
	cmpi.w	#$FFFF,frequency_voice2
	beq.s	end_playfx_voice2
	bra	done_voice2	

end_playfx_voice2:
	lea	state_and_mode(PC),a0
	clr.l	12(a0)
	bsr	_SkipData_voice2
		
	btst.b	#1,-64(a0)							;loop
	bne.s	init_playfx_voice2
	andi.b	#1,-66(a0)							;state_voice
	andi.w	#%1000000011111000,(a0)
	bra	done_voice2

init_playfx_voice2:
	move.l	ptrstart_voice2(PC),-4(a0)
	bra	done_voice2


;-*-*-*-*-*-*-*-*-*-*-*-*
;Sound voie 1
;*-*-*-*-*-*-*-*-*-*-*-*-
sound_voice1:
	tst.w	_ucounter1							;Test _ucounter1
	beq.s	done_unittps_sound_voice1					;Si égale à 0 alors
	bsr	play_voice1							;Joue fréquence voie 1
	bra	done_voice1							;Fin de la voie 1
	
done_unittps_sound_voice1:
	lea	_ucounter1(PC),a0						;Pointeur _ucounter1 dans a0
	tst.b	_lcounter1							;Test _lcounter1
	beq.s	done_length_sound_voice1					;Si égale à 0
	
	subq.b	#1,-50(a0)							;Soustrait de 1 _lcounter1
	move.w	unittps_voice1(PC),(a0)						;Initialise _ucounter1
	bra	done_voice1							;Fin de la voie 1

done_length_sound_voice1:
	move.w	unittps_voice1(PC),(a0)						;initialise _ucounter1
	
extract_sound_voice1:	
	clr.w	(a0)								;Efface _ucounter1
	clr.b	-50(a0)								;Efface _lcounter1
	clr.l	26(a0)								;Efface note_time_counter_voice1	
	
	move.l	ptridle_voice1(PC),a0						;Pointeur ptridle_voice1 dans a0
	move.b	(a0)+,d0							;Extrait octet dans d0
	cmpi.b	#$FF,d0								;Si octet=0xFF
	beq.s	end_sound_voice1						;End_sound_voice1
	ext.w	d0								;Convertie d0 en 16 bits
	;mulu.w	#2,d0
	
	tst.b	d0								;Test d0
	bne	_add_note_voice1						;Différent de 0 alors

return_sound_voice1:	
	move.b	(a0),_lcounter1							;Déplace octet suivant dans _lcounter1
	move.b	(a0),length_voice1						;_lcounter1 dans length_voice1
	lea	TblFreq(PC),a0							;Charge TblFreq dans a0
	move.b	d0,note_voice1							;d0 dans note_voice1
	add.w	d0,a0								;Ajoute de d0 dans TblFreq
	move.w 	0(a0,d0.w),frequency_voice1					;Extrait la fréquence dans TblFreq en fonction de la note
		
	;Prepare compatibilité avec HW1 et HW2
	lea	_fcounter1(PC),a0						;pointeur _fcounter1 dans a0
	move.w	frequency_voice1(PC),d0						;frequency_voice1 dans d0
	lsr.w	#1,d0								;Rotation d'un bit vers la droite
	sub.w	play_frequency(PC),d0						;d0=play_frequency-d0
	move.w	d0,(a0)								;Déplace d0 dans _fcounter1
	
	addq.l	#2,10(a0)							;Ajoute 2 à ptridle_voice1
	bra	done_voice1							;Fin de la voie 1

end_sound_voice1:
	lea	state_and_mode(PC),a0						;Pointeur state_and_mode dans a0
	clr.l	4(a0)								;Efface time_counter_voice1
	bsr	_SkipData_voice1						;Efface d'autre variables
	btst.b	#0,-64(a0)							;test loop voice 1
	bne.s	init_sound_voice1						;Si =1
	andi.b	#2,-66(a0)							;Efface state_voice1
	andi.w	#%1111110000000000,(a0)						;Efface mode MIDI voice 1
	bra	done_voice1							;Fin de la voie 1

init_sound_voice1:
	move.l	ptrstart_voice1(PC),-10(a0)					;Déplace ptrstart dans ptridle_voice1
	bra	done_voice1							;Fin de la voie 1

_add_note_voice1:
	add.b	add_note_voice1(PC),d0						;d0=add_note_voice1+d0
	bra	return_sound_voice1						;return_sound_voice1
	

;-*-*-*-*-*-*-*-*-*-*-*-*
;Sound voie 2
;*-*-*-*-*-*-*-*-*-*-*-*-
sound_voice2:									;Identique à sound_voice1
	tst.w	_ucounter2
	beq.s	done_unittps_sound_voice2
	bsr	play_voice2
	bra	done_voice2
	
done_unittps_sound_voice2:
	lea	_ucounter2(PC),a0
	tst.b	_lcounter2
	beq.s	done_length_sound_voice2
	
	subq.b	#1,-53(a0)							;_lcounter2
	move.w	unittps_voice2(PC),(a0)
	bra	done_voice2

done_length_sound_voice2:
	move.w	unittps_voice2(PC),(a0)
	
extract_sound_voice2:	
	clr.w	(a0)
	clr.b	-53(a0)								;_lcounter2
	clr.l	30(a0)
	
	move.l	ptridle_voice2(PC),a0
	move.b	(a0)+,d0
	cmpi.b	#$FF,d0
	beq.s	end_sound_voice2
	ext.w	d0
	;mulu.w	#2,d0
	
	tst.b	d0
	bne	_add_note_voice2

return_sound_voice2:	
	move.b	(a0),_lcounter2
	move.b	(a0),length_voice2
	lea	TblFreq(PC),a0
	move.b	d0,note_voice2
	add.w	d0,a0
	move.w 	0(a0,d0.w),frequency_voice2

	;Compatibilité avec HW1 et HW2
	lea	_fcounter2(PC),a0
	move.w	frequency_voice2(PC),d0
	lsr.w	#1,d0
	sub.w	play_frequency(PC),d0
	move.w	d0,(a0)
	
	addq.l	#2,12(a0)							;ptridle_voice2
	bra	done_voice2

end_sound_voice2:
	lea	state_and_mode(PC),a0
	clr.l	12(a0)
	bsr	_SkipData_voice2
		
	btst.b	#1,-64(a0)							;loop
	bne.s	init_sound_voice2
	andi.b	#1,-66(a0)							;state_voice
	andi.w	#%1000000011111000,(a0)
	bra	done_voice2

init_sound_voice2:
	move.l	ptrstart_voice2(PC),-4(a0)
	bra	done_voice2	
	
_add_note_voice2:
	add.b	add_note_voice2(PC),d0
	bra	return_sound_voice2
	
		
;*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
;Gestion numérique
;*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
;-----
;voie1
;-----
play_wav_voice1: 
	move.l	d1,-(a7)								;Sauvegarde de d1
	btst.b	#0,Hardware								;Si Hardware=1
	bne	counter_sample_HW1_voice1						;counter_sample_HW1_voice1
counter_sample_HW2_voice1:								;Prepare le passage de execute_play_wav_voice1 en fonction de sample
	move.w	#4,d1
	mulu	wav_sample_voice1,d1
	add	d1,wav_counter_sample_voice1
	subi	#32768,d1
	move.w	wav_counter_sample_voice1(PC),d0
	cmp.w	d0,d1
	blt	end_wav_voice1

execute_play_wav_voice1:
	subi	#$8000,wav_counter_sample_voice1
	
	move.l 	wav_ptr_voice1(PC),a0 							;wav_ptr_voice1 dans a0
	move.b 	wav_bit_voice1(PC),d0 							;wav_bit_voice1 dans d0

	btst 	d0,(a0) 								;Si bit d0 de l'octet en cours est égale à 0
	beq.s 	no_impulsion_voice1 							;no_impulsion_voice1
	bset	#0,($60000E)								;Etat haut du port 0x60000E
	cmp.b	#3,playmode
	beq.s	_mono1_wav
	bra	suite_wav_voice1							;suite_wav_voice1
	
no_impulsion_voice1:
	bclr	#0,($60000E)								;Etat bas du port 0x60000E
	cmp.b	#3,playmode
	beq.s	_mono0_wav
	
suite_wav_voice1:
	addq.b 	#1,wav_bit_voice1 							;Ajoute 1 à wav_bit_voice1
	cmpi.b 	#8,wav_bit_voice1							;Si wav_bit_voice1<8
	ble.s 	end_wav_voice1 								;end_wav_voice1

add_ptr_wav_voice1: 
	addq.l 	#1,wav_ptr_voice1 							;Ajoue 1 à wav_ptr_voice1
	clr.b 	wav_bit_voice1								;Efface wav_bit_voice1

	tst.w	wav_size_voice1								;Test wav_size_voice1
	beq.s	no_wav_voice1								;=0
	subq.w	#1,wav_size_voice1							;soustrait de 1 wav_size_voice1
	
end_wav_voice1:
	move.l	(a7)+,d1 								;Restaure d1
	bra 	done_voice1								;Fin de la voie 1
	
no_wav_voice1:
	bclr.b	#0,State_voice								;Efface state_voice1
	andi.w	#%1111110000000000,state_and_mode					;Efface mode PlayWAV voice1
	clr.l	time_counter_voice1							;Efface time_counter_voice1
	bra.s	end_wav_voice1								;end_wav_voice1
	
counter_sample_HW1_voice1:
	move.w	#3,d1									;Passage de execute_play_wav_voice1 sur HW1 en fonction de sample
	mulu	wav_sample_voice1(PC),d1
	add	d1,wav_counter_sample_voice1
	subi	#32768,d1
	move.w	wav_counter_sample_voice1(PC),d0
	cmp.w	d0,d1
	blt.s	end_wav_voice1
	bra	execute_play_wav_voice1

_mono1_wav:
	bset.b	#1,($60000E)								;Etat haut bit 1 port 0x60000E
	bra.s	suite_wav_voice1 
	
_mono0_wav:
	bclr.b	#1,($60000E)								;Etat bas bit 1 port 0x60000E
	bra	suite_wav_voice1 	

;-----
;voie2
;-----	
play_wav_voice2:
	move.l	d1,-(a7) 								;Identique à play_wav_voice1
	btst.b	#0,Hardware
	bne	counter_sample_HW1_voice2
counter_sample_HW2_voice2:
	move.w	#4,d1
	mulu	wav_sample_voice2(PC),d1
	add	d1,wav_counter_sample_voice2
	subi	#32768,d1
	move.w	wav_counter_sample_voice2(PC),d0
	cmp.w	d0,d1
	blt.s	end_wav_voice2

execute_play_wav_voice2:
	subi	#$8000,wav_counter_sample_voice2
	
	move.l 	wav_ptr_voice2(PC),a0 
	move.b 	wav_bit_voice2(PC),d0 

	btst 	d0,(a0) 
	beq.s 	no_impulsion_voice2 
	bset	#1,($60000E)
	bra.s 	suite_wav_voice2 

no_impulsion_voice2: 
	bclr 	#1,($60000E)
	
suite_wav_voice2:
	addq.b 	#1,wav_bit_voice2 
	cmpi.b 	#8,wav_bit_voice2 
	ble.s 	end_wav_voice2 

add_ptr_wav_voice2: 
	addq.l 	#1,wav_ptr_voice2 
	clr.b 	wav_bit_voice2

	tst.w	wav_size_voice2
	beq.s	no_wav_voice2
	subq.w	#1,wav_size_voice2

end_wav_voice2: 
	move.l	(a7)+,d1
	bra 	done_voice2
	
no_wav_voice2:
	bclr.b	#1,State_voice
	andi.w	#%1000000011111000,state_and_mode
	clr.l	time_counter_voice2
	bra.s	end_wav_voice2
	
counter_sample_HW1_voice2:
	move.w	#3,d1
	mulu	wav_sample_voice2(PC),d1
	add	d1,wav_counter_sample_voice2
	subi	#32768,d1
	move.w	wav_counter_sample_voice2(PC),d0
	cmp.w	d0,d1
	blt.s	end_wav_voice2
	bra	execute_play_wav_voice2

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
		
;--------------------------------------------------------------------------------------------
empty_handler:
	rte	
	even
length_voice1	dc.b	0
length_voice2	dc.b	0
_lcounter1	dc.b	0
_lcounter2	dc.b	0

State_voice	dc.b	0
Hardware	dc.b	0

;mode		dc.b	0
;state		dc.b	0
loop		dc.b	0

add_note_voice1	dc.b	0		
add_note_voice2	dc.b	0

wav_bit_voice1	dc.b	0
wav_bit_voice2	dc.b	0

note_voice1	dc.b	0
note_voice2	dc.b	0

backup_600015	dc.b	0
playmode	dc.b	0
	even
			
old_int5	dc.l	0
interrupt_handler dc.l	0

ptrstart_voice1	dc.l	0
ptrstart_voice2	dc.l	0

wav_ptr_voice1	dc.l	0
wav_ptr_voice2	dc.l	0

wav_size_voice1	dc.w	0
wav_size_voice2	dc.w	0


mask_int5	dc.w	0

add_frequency_int5 dc.w	0
play_frequency 	dc.w	0

_fcounter1	dc.w	0
_ucounter1	dc.w	0
_fcounter2	dc.w	0
_ucounter2	dc.w	0

frequency_voice1 dc.w	0
ptridle_voice1	dc.l	0
frequency_voice2 dc.w	0
ptridle_voice2	dc.l	0

state_and_mode	dc.w	0
counter_frequency_int5	dc.w	0
time_counter_voice1 dc.l	0
note_time_voice1 dc.l	0
time_counter_voice2 dc.l	0
note_time_voice2 dc.l	0

wav_sample_voice1 dc.w	0
wav_sample_voice2 dc.w	0

wav_counter_sample_voice1	dc.w	0
wav_counter_sample_voice2	dc.w	0

unittps_voice1	dc.w	0
unittps_voice2	dc.w	0

interrupt5rate	dc.w	0
		even

;Real Table of frequences for mode PlaySound	
TblFreq		dc.w	$0000 ;PAUSE
		dc.w	$0020,$0022,$0024,$0026,$0029,$002B,$002E,$0030,$0033,$0037,$003A,$003D
		dc.w	$0041,$0045,$0049,$004D,$0052,$0057,$005C,$0061,$0067,$006E,$0074,$007B
		dc.w	$0082,$008A,$0092,$009B,$00A4,$00AE,$00B8,$00C3,$00CF,$00DC,$00E9,$00F6
		dc.w	$0105,$0115,$0125,$0137,$0149,$0150,$0171,$0187,$019F,$01B8,$01D2,$01ED
		dc.w	$020B,$022A,$024B,$026E,$0293,$02BA,$02E3,$030F,$033E,$0370,$03A4,$03DB
		dc.w	$0416,$0454,$0496,$04DC,$0526,$0574,$05C7,$061F,$067D,$06E0,$0748,$07B7
		dc.w	$082D,$08A9,$092D,$09B9,$0A4D,$0AE9,$0B8F,$0C3F,$0CFA,$0DC0,$0E91,$0F6F
		dc.w	$105A,$1152,$125A,$1372,$149A,$15D3,$171F,$187F,$19F4,$1B80,$1D22,$1EDE
		dc.w	$20B4,$22A5,$24B5,$26E4,$2934,$2BA7,$2E3F,$30FF,$33E9,$3700,$3A45,$3DBC
		dc.w	$4168,$454B,$496A,$4DC8,$5268,$574E,$5C7F,$61FF,$67D3,$6E00,$748A,$7B78
	end