// C Source File
// Created 3/13/2005; 4:48:17 PM

#include <tigcclib.h>
#include "mapdata.h"
#include "gfx.h"

// Main Function
void _main(void)
{
	ST_helpMsg("Making metmaps.MET...");
	FILE* f = fopen("metmaps", "wb");
	if(f == NULL) {
		clrscr();
		printf("metmaps.MET: File Open Failed!\n");
		ngetchx();
		exit(0);
	} else {
		if(fwrite(maps,sizeof(maps),1,f)!=1) {
			clrscr();
			printf("metmaps.MET: Writing Failed!\n");
			fclose(f);
			ngetchx();
			exit(0);
		} else {
			fputc(0,f);
			fputs("MET",f);
			fputc(0,f);
			fputc(OTH_TAG,f);
			fclose(f);
		}
	}
	ST_helpMsg("Making metgfx.MET...");
	f = fopen("metgfx", "wb");
	if(f == NULL) {
		clrscr();
		printf("metgfx.MET: File Open Failed!\n");
		ngetchx();
		exit(0);
	} else {
		if(fwrite(&g,sizeof(struct graphics),1,f)!=1) {
			clrscr();
			printf("metgfx.MET: Writing Failed!\n");
			fclose(f);
			ngetchx();
			exit(0);
		} else {
			fputc(0,f);
			fputs("MET",f);
			fputc(0,f);
			fputc(OTH_TAG,f);
			fclose(f);
		}
	}
}
