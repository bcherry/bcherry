	section ".data"
	xdef	pSnd_DisableSound

pSnd_DisableSound:
	andi.b	#%11111100,($60000E)				;Met à état bas le port I/O				
	move.b	#$8D,($60000C)					;Accès normal au port I/O
	rts