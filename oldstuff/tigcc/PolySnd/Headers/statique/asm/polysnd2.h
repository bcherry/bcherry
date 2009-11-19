;PolySnd v2.0
;------------
;(c) 2002-2003 by Geoffrey Anneheim
;geoffrey.anneheim@wanadoo.fr

_NOVOICE 	EQU	0
_VOICE1		EQU	1
_VOICE2		EQU	2
_ALLVOICES 	EQU	3

_BEEP_VOICE1 	EQU	1
_BEEP_VOICE2 	EQU	16
_FX_VOICE1 	EQU	2
_FX_VOICE2 	EQU	32
_SOUND_VOICE1 	EQU	8
_SOUND_VOICE2 	EQU	128
_WAV_VOICE1	EQU	4
_WAV_VOICE2	EQU	64

_MONO		EQU	0
_STEREO		EQU	1


polysnd2::EnableSound		equ		pSnd_EnableSound
polysnd2::DisableSound		equ		pSnd_DisableSound

polysnd2::InstallSound		equ		pSnd_InstallSound
polysnd2::UninstallSound	equ		pSnd_UninstallSound

polysnd2::Initialize		equ		pSnd_Initialize

polysnd2::SetTempo_voice1	equ		pSnd_SetTempo_voice1
polysnd2::SetTempo_voice2	equ		pSnd_SetTempo_voice2
polysnd2::GetTempo_voice1	equ		pSnd_GetTempo_voice1
polysnd2::GetTempo_voice2	equ		pSnd_GetTempo_voice2

polysnd2::SetState		equ		pSnd_SetState
polysnd2::SetLoop		equ		pSnd_SetLoop

polysnd2::SetBeep_voice1	equ		pSnd_SetBeep_voice1
polysnd2::SetBeep_voice2	equ		pSnd_SetBeep_voice2

polysnd2::PlayFX_voice1		equ		pSnd_PlayFX_voice1
polysnd2::PlayFX_voice2		equ		pSnd_PlayFX_voice2

polysnd2::GetFrequency_voice1	equ		pSnd_GetFrequency_voice1
polysnd2::GetFrequency_voice2	equ		pSnd_GetFrequency_voice2

polysnd2::PortStatus		equ		pSnd_PortStatus

polysnd2::Interrupt5		equ		pSnd_Interrupt5

polysnd2::PlaySound_voice1	equ		pSnd_PlaySound_voice1
polysnd2::PlaySound_voice2	equ		pSnd_PlaySound_voice2

polysnd2::GetLength_voice1	equ		pSnd_GetLength_voice1
polysnd2::GetLength_voice2	equ		pSnd_GetLength_voice2

polysnd2::GetTime		equ		pSnd_GetTime
polysnd2::GetState		equ		pSnd_GetState
polysnd2::GetMode		equ		pSnd_GetMode
polysnd2::GetLoop		equ		pSnd_GetLoop

polysnd2::_Time_voice1		equ		pSnd_Time_voice1
polysnd2::_Time_voice2		equ		pSnd_Time_voice2
polysnd2::SetTime_voice1	equ		pSnd_SetTime_voice1
polysnd2::SetTime_voice2	equ		pSnd_SetTime_voice2

polysnd2::VoiceState		equ		pSnd_VoiceState
polysnd2::SetVoiceState		equ		pSnd_SetVoiceState

polysnd2::SetIntMask		equ		pSnd_SetIntMask

polysnd2::AddNotes_voice1	equ		pSnd_AddNotes_voice1
polysnd2::AddNotes_voice2	equ		pSnd_AddNotes_voice2

polysnd2::PlayWAV_voice1	equ		pSnd_PlayWAV_voice1
polysnd2::PlayWAV_voice2	equ		pSnd_PlayWAV_voice2

polysnd2::Sample_voice1		equ		pSnd_Sample_voice1
polysnd2::Sample_voice2		equ		pSnd_Sample_voice2

polysnd2::GetNote_voice1	equ		pSnd_GetNote_voice1
polysnd2::GetNote_voice2	equ		pSnd_GetNote_voice2

polysnd2::_Ptridle_voice1	equ		pSnd_Ptridle_voice1
polysnd2::_Ptridle_voice2	equ		pSnd_Ptridle_voice2
polysnd2::_time_counter_voice1	equ		pSnd_time_counter_voice1
polysnd2::_time_counter_voice2	equ		pSnd_time_counter_voice2
polysnd2::_note_time_counter_voice1 equ		pSnd_note_time_counter_voice1
polysnd2::_note_time_counter_voice2 equ		pSnd_note_time_counter_voice2
polysnd2::_SkipData_voice1	equ		pSnd_SkipData_voice1
polysnd2::_SkipData_voice2	equ		pSnd_SkipData_voice2

polysnd2::PlayMode		equ		pSnd_PlayMode

polysnd2::SetLength_voice1	equ		pSnd_SetLength_voice1
polysnd2::Setlength_voice2	equ		pSnd_SetLength_voice2

polysnd2::SetInterrupt5Rate	equ		pSnd_SetInterrupt5Rate

	
;*******************************
;Enumération des notes pour MIDI
;*******************************

;------------
;| OCTAVE 0 |
;------------
_c0		EQU	1
_cd0		EQU	2
_db0		EQU	2
_d0		EQU	3
_dd0		EQU	4
_eb0		EQU	4
_e0		EQU	5
_f0		EQU	6
_fd0		EQU	7
_gb0		EQU	7
_g0		EQU	8
_gd0		EQU	9
_ab0		EQU	9
_a0		EQU	10
_ad0		EQU	11
_bb0		EQU	11
_b0		EQU	12 


;------------
;| OCTAVE 1 |
;------------
_c1		EQU	13
_cd1		EQU	14
_db1		EQU	14
_d1		EQU	15
_dd1		EQU	16
_eb1		EQU	16
_e1		EQU	17
_f1		EQU	18
_fd1		EQU	19
_gb1		EQU	19
_g1		EQU	20
_gd1		EQU	21
_ab1		EQU	21
_a1		EQU	22
_ad1		EQU	23
_bb1		EQU	23
_b1		EQU	24 


;------------
;| OCTAVE 2 |
;------------
_c2		EQU	25
_cd2		EQU	26
_db2		EQU	26
_d2		EQU	27
_dd2		EQU	28
_eb2		EQU	28
_e2		EQU	29
_f2		EQU	30
_fd2		EQU	31
_gb2		EQU	31
_g2		EQU	32
_gd2		EQU	33
_ab2		EQU	33
_a2		EQU	34
_ad2		EQU	35
_bb2		EQU	35
_b2		EQU	36 


;------------
;| OCTAVE 3 |
;------------
_c3		EQU	37
_cd3		EQU	38
_db3		EQU	38
_d3		EQU	39
_dd3		EQU	40
_eb3		EQU	40
_e3		EQU	41
_f3		EQU	42
_fd3		EQU	43
_gb3		EQU	43
_g3		EQU	44
_gd3		EQU	45
_ab3		EQU	45
_a3		EQU	46
_ad3		EQU	47
_bb3		EQU	47
_b3		EQU	48 


;------------
;| OCTAVE 4 |
;------------
_c4		EQU	49
_cd4		EQU	50
_db4		EQU	50
_d4		EQU	51
_dd4		EQU	52
_eb4		EQU	52
_e4		EQU	53
_f4		EQU	54
_fd4		EQU	55
_gb4		EQU	55
_g4		EQU	56
_gd4		EQU	57
_ab4		EQU	57
_a4		EQU	58
_ad4		EQU	59
_bb4		EQU	59
_b4		EQU	60 


;------------
;| OCTAVE 5 |
;------------
_c5		EQU	61
_cd5		EQU	62
_db5		EQU	62
_d5		EQU	63
_dd5		EQU	64
_eb5		EQU	64
_e5		EQU	65
_f5		EQU	66
_fd5		EQU	67
_gb5		EQU	67
_g5		EQU	68
_gd5		EQU	69
_ab5		EQU	69
_a5		EQU	70
_ad5		EQU	71
_bb5		EQU	71
_b5		EQU	72
 

;------------
;| OCTAVE 6 |
;------------
_c6		EQU	73
_cd6		EQU	74
_db6		EQU	74
_d6		EQU	75
_dd6		EQU	76
_eb6		EQU	76
_e6		EQU	77
_f6		EQU	78
_fd6		EQU	79
_gb6		EQU	79
_g6		EQU	80
_gd6		EQU	81
_ab6		EQU	81
_a6		EQU	82
_ad6		EQU	83
_bb6		EQU	83
_b6		EQU	84 


;------------
;| OCTAVE 7 |
;------------
_c7		EQU	85
_cd7		EQU	86
_db7		EQU	86
_d7		EQU	87
_dd7		EQU	88
_eb7		EQU	88
_e7		EQU	89
_f7		EQU	90
_fd7		EQU	91
_gb7		EQU	91
_g7		EQU	92
_gd7		EQU	93
_ab7		EQU	93
_a7		EQU	94
_ad7		EQU	95
_bb7		EQU	95
_b7		EQU	96


;------------
;| OCTAVE 8 |
;------------
_c8		EQU	97
_cd8		EQU	98
_db8		EQU	98
_d8		EQU	99
_dd8		EQU	100
_eb8		EQU	100
_e8		EQU	101
_f8		EQU	102
_fd8		EQU	103
_gb8		EQU	103
_g8		EQU	104
_gd8		EQU	105
_ab8		EQU	105
_a8		EQU	106
_ad8		EQU	107
_bb8		EQU	107
_b8		EQU	108


;------------
;| OCTAVE 9 |
;------------
_c9		EQU	109
_cd9		EQU	110
_db9		EQU	110
_d9		EQU	111
_dd9		EQU	112
_eb9		EQU	112
_e9		EQU	113
_f9		EQU	114
_fd9		EQU	115
_gb9		EQU	115
_g9		EQU	116
_gd9		EQU	117
_ab9		EQU	117
_a9		EQU	118
_ad9		EQU	119
_bb9		EQU	119
_b9		EQU	120
  

;*************************************
;Enumération des notes norme française
;*************************************

;------------
;| OCTAVE 0 |
;------------
do_0		EQU	1
dod_0		EQU	2
reb_0		EQU	2
re_0		EQU	3
red_0		EQU	4
mib_0		EQU	4
mi_0		EQU	5
fa_0		EQU	6
fad_0		EQU	7
solb_0		EQU	7
sol_0		EQU	8
sold_0		EQU	9
lab_0		EQU	9
la_0		EQU	10
lad_0		EQU	11
sib_0		EQU	11
si_0		EQU	12 


;------------
;| OCTAVE 1 |
;------------
do_1		EQU	13
dod_1		EQU	14
reb_1		EQU	14
re_1		EQU	15
red_1		EQU	16
mib_1		EQU	16
mi_1		EQU	17
fa_1		EQU	18
fad_1		EQU	19
solb_1		EQU	19
sol_1		EQU	20
sold_1		EQU	21
lab_1		EQU	21
la_1		EQU	22
lad_1		EQU	23
sib_1		EQU	23
si_1		EQU	24 


;------------
;| OCTAVE 2 |
;------------
do_2		EQU	25
dod_2		EQU	26
reb_2		EQU	26
re_2		EQU	27
red_2		EQU	28
mib_2		EQU	28
mi_2		EQU	29
fa_2		EQU	30
fad_2		EQU	31
solb_2		EQU	31
sol_2		EQU	32
sold_2		EQU	33
lab_2		EQU	33
la_2		EQU	34
lad_2		EQU	35
sib_2		EQU	35
si_2		EQU	36 


;------------
;| OCTAVE 3 |
;------------
do_3		EQU	37
dod_3		EQU	38
reb_3		EQU	38
re_3		EQU	39
red_3		EQU	40
mib_3		EQU	40
mi_3		EQU	41
fa_3		EQU	42
fad_3		EQU	43
solb_3		EQU	43
sol_3		EQU	44
sold_3		EQU	45
lab_3		EQU	45
la_3		EQU	46
lad_3		EQU	47
sib_3		EQU	47
si_3		EQU	48 


;------------
;| OCTAVE 4 |
;------------
do_4		EQU	49
dod_4		EQU	50
reb_4		EQU	50
re_4		EQU	51
red_4		EQU	52
mib_4		EQU	52
mi_4		EQU	53
fa_4		EQU	54
fad_4		EQU	55
solb_4		EQU	55
sol_4		EQU	56
sold_4		EQU	57
lab_4		EQU	57
la_4		EQU	58
lad_4		EQU	59
sib_4		EQU	59
si_4		EQU	60 


;------------
;| OCTAVE 5 |
;------------
do_5		EQU	61
dod_5		EQU	62
reb_5		EQU	62
re_5		EQU	63
red_5		EQU	64
mib_5		EQU	64
mi_5		EQU	65
fa_5		EQU	66
fad_5		EQU	67
solb_5		EQU	67
sol_5		EQU	68
sold_5		EQU	69
lab_5		EQU	69
la_5		EQU	70
lad_5		EQU	71
sib_5		EQU	71
si_5		EQU	72
 

;------------
;| OCTAVE 6 |
;------------
do_6		EQU	73
dod_6		EQU	74
reb_6		EQU	74
re_6		EQU	75
red_6		EQU	76
mib_6		EQU	76
mi_6		EQU	77
fa_6		EQU	78
fad_6		EQU	79
solb_6		EQU	79
sol_6		EQU	80
sold_6		EQU	81
lab_6		EQU	81
la_6		EQU	82
lad_6		EQU	83
sib_6		EQU	83
si_6		EQU	84 


;------------
;| OCTAVE 7 |
;------------
do_7		EQU	85
dod_7		EQU	86
reb_7		EQU	86
re_7		EQU	87
red_7		EQU	88
mib_7		EQU	88
mi_7		EQU	89
fa_7		EQU	90
fad_7		EQU	91
solb_7		EQU	91
sol_7		EQU	92
sold_7		EQU	93
lab_7		EQU	93
la_7		EQU	94
lad_7		EQU	95
sib_7		EQU	95
si_7		EQU	96


;------------
;| OCTAVE 8 |
;------------
do_8		EQU	97
dod_8		EQU	98
reb_8		EQU	98
re_8		EQU	99
red_8		EQU	100
mib_8		EQU	100
mi_8		EQU	101
fa_8		EQU	102
fad_8		EQU	103
solb_8		EQU	103
sol_8		EQU	104
sold_8		EQU	105
lab_8		EQU	105
la_8		EQU	106
lad_8		EQU	107
sib_8		EQU	107
si_8		EQU	108


;------------
;| OCTAVE 9 |
;------------
do_9		EQU	109
dod_9		EQU	110
reb_9		EQU	110
re_9		EQU	111
red_9		EQU	112
mib_9		EQU	112
mi_9		EQU	113
fa_9		EQU	114
fad_9		EQU	115
solb_9		EQU	115
sol_9		EQU	116
sold_9		EQU	117
lab_9		EQU	117
la_9		EQU	118
lad_9		EQU	119
sib_9		EQU	119
si_9		EQU	120
  
pause		EQU	$00
rest		EQU	$00


;*****************************************************************************
;Voici la liste des durées. Cette liste peut être modifiée.
;Par défaut 32=noire.
;Durées étalonnées avec précision en fonction du tempo.
;
;Ronde = 4 temps
;Blanche = 2 temps
;Blanche pointée = 2+1/2
;Noire pointée = 1+1/2 temps
;Noire = 1 temps
;Croche pointée = 1/2+1/4 temps
;Croche = 1/2 temps
;Double croche = 1/4 temps
;Triple croche = 1/8 temps
;Triolet = 1/3 temps
;*****************************************************************************

_r		EQU	128
_bp		EQU	80
_b		EQU	64
_np		EQU	48	
_n		EQU	32
_cp		EQU	24
_c		EQU	16
_cc		EQU	8
_ccc		EQU	4
_cccc		EQU	2
_tri		EQU	11

ronde		EQU	128
blanche_pointee	EQU	80
blanche		EQU	64
noire_pointee	EQU	48	
noire		EQU	32
croche_pointee	EQU	24
croche		EQU	16
double_croche	EQU	8
triple_croche	EQU	4
quadruple_croche EQU	2
triolet		EQU	11

_soupir		EQU	32
_soupirp	EQU	48
soupir		EQU	32
soupir_pointe	EQU	48

whole		EQU	128	;Ronde
half		EQU	80	;Blanche
quarter		EQU	34	;Noire
eighth		EQU	16	;Croche
sixteenth	EQU	8	;Double croche
thirtysecond	EQU	4	;Triple croche
sixtyfourth	EQU	2	;Quadruple croche

half3		EQU	21
quarter3	EQU	10
eighth3		EQU	5
sixteenth3	EQU	2
thirtysecond3	EQU	1
sixtyfourth3	EQU	0