	section ".data"
	xdef	pSnd_DisableSound

pSnd_DisableSound:
	andi.b	#%11111100,($60000E)				;Met � �tat bas le port I/O				
	move.b	#$8D,($60000C)					;Acc�s normal au port I/O
	rts