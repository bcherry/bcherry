	section ".data"
	xdef	pSnd_PlayWAV_voice1

pSnd_PlayWAV_voice1:
	move.w	d0,wav_size_voice1
	move.l	a0,wav_ptr_voice1
	clr.b	wav_bit_voice1
	
	move.w	state_and_mode,d0
	andi.w	#%1111110010000000,d0					;PlayWAV voice 1
	ori.w	#$0020,d0
	move.w	d0,state_and_mode
	clr.w	wav_counter_sample_voice1
	rts