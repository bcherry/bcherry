	section ".data"
	xdef	pSnd_PlayWAV_voice2

pSnd_PlayWAV_voice2:
	move.w	d0,wav_size_voice2
	move.l	a0,wav_ptr_voice2
	clr.b	wav_bit_voice2
	
	move.w	state_and_mode,d0
	andi.w	#%1100000011111000,d0					;PlayWAV voice 2
	ori.w	#$1000,d0
	move.w	d0,state_and_mode
	clr.w	wav_counter_sample_voice2
	rts