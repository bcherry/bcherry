	section ".data"

	xdef	pSnd_EnableSound
	
pSnd_EnableSound:
; move.b #0,xxx.l = 8 bytes; clr.b xxx.l = 6 bytes.
	clr.b	($60000C)					;Acc�s au port I/O en bas niveau
	andi.b	#%11111100,($60000E)				;Met � �tat bas le port I/O
	rts