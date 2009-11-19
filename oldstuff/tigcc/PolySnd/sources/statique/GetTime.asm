	section ".data"
	xdef	pSnd_GetTime

pSnd_GetTime:
	moveq.l	#0,d0								;Efface d0
	moveq.l	#0,d1								;Efface d1
	
loop1:										;Boucle
	cmpi.b	#$FF,(a0)+							;jusqu'� ce que la fin des donn�es
	beq.s	end_loop							;Alors fin boulce
	
	move.b	(a0)+,d1							;D�place l'octet en courds dans d1
	add.l	d1,d0								;Ajoute la dur�e en cycles dans d0
	bra.s	loop1								;Boucle
	
end_loop:
; Tr�s lent...
	divu	#32,d0								;Divise par 32 (32 cycles=1 temps)
	mulu	#60,d0								;Multiplie par 60 (60 secondes)
	
	divu.w	d2,d0								;Renvoie en fonction du tempo, le temps en seconde des donn�es
	rts