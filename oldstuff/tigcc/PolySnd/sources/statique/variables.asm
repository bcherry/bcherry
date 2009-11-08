	section ".data"
	xdef	length_voice1
	xdef	length_voice2
	xdef	_lcounter1
	xdef	_lcounter2
	
	xdef	 State_voice
	xdef	Hardware
	
	xdef	loop
	
	xdef	add_note_voice1
	xdef	add_note_voice2
	
	xdef	wav_bit_voice1
	xdef	wav_bit_voice2

	xdef	note_voice1
	xdef	note_voice2
	
	xdef	backup_600015
	xdef	playmode
	
	;even	
	xdef	old_int5
	xdef	interrupt_handler
	
	xdef	ptrstart_voice1
	xdef	ptrstart_voice2
	
	xdef	wav_ptr_voice1
	xdef	wav_ptr_voice2
	
	xdef	wav_size_voice1
	xdef	wav_size_voice2
	
	xdef	mask_int5
	
	xdef	add_frequency_int5
	xdef	play_frequency
	
	xdef	_fcounter1
	xdef	_ucounter1
	xdef	_fcounter2
	xdef	_ucounter2
	
	xdef	frequency_voice1
	xdef	ptridle_voice1
	xdef	frequency_voice2
	xdef	ptridle_voice2
	
	xdef	state_and_mode 
	xdef	counter_frequency_int5
	xdef	time_counter_voice1
	xdef	note_time_voice1
	xdef	time_counter_voice2
	xdef	note_time_voice2

	xdef	wav_sample_voice1
	xdef	wav_sample_voice2

	xdef	wav_counter_sample_voice1
	xdef	wav_counter_sample_voice2

	xdef	unittps_voice1
	xdef	unittps_voice2
	
	xdef	interrupt5rate
	
	xdef	empty_handler
	xdef	TblFreq
	xdef	altern
	
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
		even
altern		dc.b	0