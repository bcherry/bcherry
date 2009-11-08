	section ".data"
	xdef	int5_handler

;/////////////////////////////////////////////////////////
;Méthode pour jouer des fréquences sur 2 voies avec unittps
;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
;------
;Voice1
play_voice1:
	lea	_fcounter1,a0							;Pointeur de _fcounter1 dans a0
	move.w	frequency_voice1,d0						;Frequency_voice1 dans d0
	beq.s	son_1								;Si égale à 0 alors aller à son_1
	add.w	(a0),d0								;d0=_fcounter1+d0
	bcc.s	son_1								;Si dépasse 16 bits alors aller son_1
	sub.w	play_frequency,d0						;d0=play_frequency-d0

_sound_:
	move.w	d0,(a0)								;Rempace _fcounter1 par le contenu de d0
	move.b	playmode,d0							;Déplace playmode dans d0
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
	lea	_fcounter2,a0							;Identique que play_voice1
	move.w	frequency_voice2,d0
	beq.s	son_2
	add.w	(a0),d0
	bcc.s	son_2
	sub.w	play_frequency,d0
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
	
	move.w	mask_int5,sr						;Déplace le mask dans sr
 
;Interruption de gestion audio
execute_int5_handler:
	move.w	state_and_mode,d0						;state_and_mode dans d0
	
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
	move.w	state_and_mode,d0						;state_and_mode dans d0
		
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
	lea	counter_frequency_int5,a0					;pointeur de counter_frequency_int dans a0
	move.w	add_frequency_int5,d0					;add_frequency_int5 dans d0
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
	move.l	interrupt_handler,-(a7)					;Execute le contenu d'interrupt5
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
	lea	_ucounter1,a0						;pointeur de _ucounter1 dans a0
	
	subq.b	#1,-50(a0)							;Soustrait de 1 _lcounter1
	tst.b	_lcounter1							;test de _lcounter1
	beq.s	done_length_beep_voice1						;Si égale à 0 done_length_beep_voice1
	move.w	unittps_voice1,(a0)						;Initailise le compteur _ucounter1
	bra	done_voice1							;Fin de la voie 1

done_length_beep_voice1:
	clr.l	22(a0)								;Efface time_counter_voice1
	bsr	pSnd_SkipData_voice1						;Efface d'autre variables de base
	
	btst.b	#0,-46(a0)							;Si mode loop voice 1
	bne.s	init_unittps_voice1						;Egale à 1 alors on intialise tout
	bclr.b	#0,-48(a0)							;Efface le mode state_voice
	andi.w	#%1111110000000000,18(a0)					;Efface le mode beep voice1
	bra	done_voice1							;Fin de la voie 1

init_unittps_voice1:
	move.w	unittps_voice1,(a0)						;Initialise _ucounter1
	move.b	length_voice1,-50(a0)					;Initialise _lcounter1
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
	lea	_ucounter2,a0

	subq.b	#1,-53(a0)							;_lcounter2
	tst.b	_lcounter2
	beq.s	done_length_beep_voice2
	move.w	unittps_voice1,(a0)
	bra	done_voice2

done_length_beep_voice2:
	clr.l	26(a0)
	bsr	pSnd_SkipData_voice2
		
	btst.b	#1,-50(a0)							;loop
	bne.s	init_unittps_voice2
	bclr.b	#1,-51(a0)							;state_voice
	andi.w	#%1000000011111000,14(a0)
	bra	done_voice2

init_unittps_voice2:
	move.w	unittps_voice2,(a0)
	move.b	length_voice2,-53(a0)
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
	lea	_ucounter1,a0						;pointeur de _ucounter1 dans a0

	tst.b	_lcounter1							;Test _lcounter1
	beq.s	done_length_playfx_voice1					;Si égale à 0
	subq.b	#1,-50(a0)							;Soustrait de 1 _lcounter1
	move.w	unittps_voice1,(a0)						;Initialise _ucounter1
	bra	done_voice1							;Fin de la voie 1
	
done_length_playfx_voice1:
	move.w	unittps_voice1,(a0)						;Initialise _ucounter1
	
extract_voice1:
	clr.w	(a0)								;Efface _ucounter1
	clr.b	-50(a0)								;Efface _lcounter1
	clr.l	26(a0)								;Efface note_tume_counter_voice1
	
	move.l	ptridle_voice1,a0						;Pointeur ptridle_voice1 dans a0
	move.w	(a0)+,frequency_voice1						;Extrait short dans frequency_voice1
	move.w	(a0),d0								;Extrait short suivant dans d0
	move.b	d0,_lcounter1							;Déplace que les 8 bits dans _lcounter1
	move.b	d0,length_voice1						;Idem dans length_voice1

	;Prepare compatibilité avec HW1 et HW2
	lea	_fcounter1,a0						;pointeur _fcounter1 dans a0
	move.w	frequency_voice1,d0						;frequency_voice1 dans d0
	lsr.w	#1,d0								;Rotation d'un bit vers la droite
	sub.w	play_frequency,d0						;d0=play_frequency-d0
	move.w	d0,(a0)								;Déplace d0 dans _fcounter1
	
	addq.l	#4,10(a0)							;Ajoute 4 au pointeur ptridle_voice1
	
	cmpi.w	#$FFFF,frequency_voice1						;Si frequency_voice1 = 0xFFFF
	beq.s	end_playfx_voice1						;Alors end_playfx_voice1
	bra	done_voice1							;Sinon fin voies 1

end_playfx_voice1:
	lea	state_and_mode,a0						;Pointeur state_and_mode dans a0
	clr.l	4(a0)								;Efface time_counter_voice1
	bsr	pSnd_SkipData_voice1						;Efface d'autre variables

	btst.b	#0,-64(a0)							;Si mode loop voie 1
	bne.s	init_playfx_voice1						;=1 alors initialise
	andi.b	#2,-66(a0)							;Efface state_voice1
	andi.w	#%1111110000000000,(a0)						;Efface mode PlayFX voice1
	bra	done_voice1							;Fin de la voie 1

init_playfx_voice1:
	move.l	ptrstart_voice1,-10(a0)					;ptrstart_voice1 dans ptridle_voice1
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
	lea	_ucounter2,a0

	tst.b	_lcounter2
	beq.s	done_length_playfx_voice2
	subq.b	#1,-53(a0)							;_lcounter2
	move.w	unittps_voice2,(a0)
	bra	done_voice2
	
done_length_playfx_voice2:
	move.w	unittps_voice2,(a0)
	
extract_voice2:
	clr.w	(a0)
	clr.b	-53(a0)								;_lcounter2
	clr.l	30(a0)
	
	move.l	ptridle_voice2,a0
	move.w	(a0)+,frequency_voice2
	move.w	(a0),d0
	move.b	d0,_lcounter2
	move.b	d0,length_voice2
	
	;Prepare compatibilité avec HW1 et HW2
	lea	_fcounter2,a0
	move.w	frequency_voice2,d0
	lsr.w	#1,d0
	sub.w	play_frequency,d0
	move.w	d0,(a0)
		
	addq.l	#4,12(a0)							;ptridle_voice2
	
	cmpi.w	#$FFFF,frequency_voice2
	beq.s	end_playfx_voice2
	bra	done_voice2	

end_playfx_voice2:
	lea	state_and_mode,a0
	clr.l	12(a0)
	bsr	pSnd_SkipData_voice2
		
	btst.b	#1,-64(a0)							;loop
	bne.s	init_playfx_voice2
	andi.b	#1,-66(a0)							;state_voice
	andi.w	#%1000000011111000,(a0)
	bra	done_voice2

init_playfx_voice2:
	move.l	ptrstart_voice2,-4(a0)
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
	lea	_ucounter1,a0						;Pointeur _ucounter1 dans a0
	tst.b	_lcounter1							;Test _lcounter1
	beq.s	done_length_sound_voice1					;Si égale à 0
	
	subq.b	#1,-50(a0)							;Soustrait de 1 _lcounter1
	move.w	unittps_voice1,(a0)						;Initialise _ucounter1
	bra	done_voice1							;Fin de la voie 1

done_length_sound_voice1:
	move.w	unittps_voice1,(a0)						;initialise _ucounter1
	
extract_sound_voice1:	
	clr.w	(a0)								;Efface _ucounter1
	clr.b	-50(a0)								;Efface _lcounter1
	clr.l	26(a0)								;Efface note_time_counter_voice1	
	
	move.l	ptridle_voice1,a0						;Pointeur ptridle_voice1 dans a0
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
	lea	TblFreq,a0							;Charge TblFreq dans a0
	move.b	d0,note_voice1							;d0 dans note_voice1
	add.w	d0,a0								;Ajoute de d0 dans TblFreq
	move.w 	0(a0,d0.w),frequency_voice1					;Extrait la fréquence dans TblFreq en fonction de la note
		
	;Prepare compatibilité avec HW1 et HW2
	lea	_fcounter1,a0						;pointeur _fcounter1 dans a0
	move.w	frequency_voice1,d0						;frequency_voice1 dans d0
	lsr.w	#1,d0								;Rotation d'un bit vers la droite
	sub.w	play_frequency,d0						;d0=play_frequency-d0
	move.w	d0,(a0)								;Déplace d0 dans _fcounter1
	
	addq.l	#2,10(a0)							;Ajoute 2 à ptridle_voice1
	bra	done_voice1							;Fin de la voie 1

end_sound_voice1:
	lea	state_and_mode,a0						;Pointeur state_and_mode dans a0
	clr.l	4(a0)								;Efface time_counter_voice1
	bsr	pSnd_SkipData_voice1						;Efface d'autre variables
	btst.b	#0,-64(a0)							;test loop voice 1
	bne.s	init_sound_voice1						;Si =1
	andi.b	#2,-66(a0)							;Efface state_voice1
	andi.w	#%1111110000000000,(a0)						;Efface mode MIDI voice 1
	bra	done_voice1							;Fin de la voie 1

init_sound_voice1:
	move.l	ptrstart_voice1,-10(a0)					;Déplace ptrstart dans ptridle_voice1
	bra	done_voice1							;Fin de la voie 1

_add_note_voice1:
	add.b	add_note_voice1,d0						;d0=add_note_voice1+d0
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
	lea	_ucounter2,a0
	tst.b	_lcounter2
	beq.s	done_length_sound_voice2
	
	subq.b	#1,-53(a0)							;_lcounter2
	move.w	unittps_voice2,(a0)
	bra	done_voice2

done_length_sound_voice2:
	move.w	unittps_voice2,(a0)
	
extract_sound_voice2:	
	clr.w	(a0)
	clr.b	-53(a0)								;_lcounter2
	clr.l	30(a0)
	
	move.l	ptridle_voice2,a0
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
	lea	TblFreq,a0
	move.b	d0,note_voice2
	add.w	d0,a0
	move.w 	0(a0,d0.w),frequency_voice2

	;Compatibilité avec HW1 et HW2
	lea	_fcounter2,a0
	move.w	frequency_voice2,d0
	lsr.w	#1,d0
	sub.w	play_frequency,d0
	move.w	d0,(a0)
	
	addq.l	#2,12(a0)							;ptridle_voice2
	bra	done_voice2

end_sound_voice2:
	lea	state_and_mode,a0
	clr.l	12(a0)
	bsr	pSnd_SkipData_voice2
		
	btst.b	#1,-64(a0)							;loop
	bne.s	init_sound_voice2
	andi.b	#1,-66(a0)							;state_voice
	andi.w	#%1000000011111000,(a0)
	bra	done_voice2

init_sound_voice2:
	move.l	ptrstart_voice2,-4(a0)
	bra	done_voice2	
	
_add_note_voice2:
	add.b	add_note_voice2,d0
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
	move.w	wav_counter_sample_voice1,d0
	cmp.w	d0,d1
	blt	end_wav_voice1

execute_play_wav_voice1:
	subi	#$8000,wav_counter_sample_voice1
	
	move.l 	wav_ptr_voice1,a0 							;wav_ptr_voice1 dans a0
	move.b 	wav_bit_voice1,d0 							;wav_bit_voice1 dans d0

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
	mulu	wav_sample_voice1,d1
	add	d1,wav_counter_sample_voice1
	subi	#32768,d1
	move.w	wav_counter_sample_voice1,d0
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
	mulu	wav_sample_voice2,d1
	add	d1,wav_counter_sample_voice2
	subi	#32768,d1
	move.w	wav_counter_sample_voice2,d0
	cmp.w	d0,d1
	blt.s	end_wav_voice2

execute_play_wav_voice2:
	subi	#$8000,wav_counter_sample_voice2
	
	move.l 	wav_ptr_voice2,a0 
	move.b 	wav_bit_voice2,d0 

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
	mulu	wav_sample_voice2,d1
	add	d1,wav_counter_sample_voice2
	subi	#32768,d1
	move.w	wav_counter_sample_voice2,d0
	cmp.w	d0,d1
	blt.s	end_wav_voice2
	bra	execute_play_wav_voice2