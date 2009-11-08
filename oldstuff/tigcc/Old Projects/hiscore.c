// C Source File
// Created 21/Oct/03; 22:04:48
/*
    Nibbles 68k v. 4.1 -- Nibbles Snake Game Clone
    Copyright (C) 2000-2002 John David Ratliff
    Based on Stilgar's Nibbles for Fargo

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// C Source File
// Created 7/1/2001; 6:57:05 AM
// Updated 8/23/2002; 8:57:44 PM

#include <stdio.h>		// FILE I/O methods
#include <estack.h>		// file tags
#include <mem.h>		// Memory functions (i.e. memset)

#include "nibbles.h"

// the high score file data
static const char *scorefile = "nibblehi";

// loadHiScores - loads the hiscores from file into the hiscore table
// 	returns TRUE if hiscores were read, FALSE otherwise
short int loadHiScores(SCORE *hiscores) {
	short int success = TRUE;
	FILE *f;

	// open file for reading and check for errors
	if ((f = fopen(scorefile,"rb")) == NULL) {
		success = FALSE;
	} else {
		// read the hiscore table and check for I/O errors
		if (fread(hiscores,sizeof(SCORE),MAX_HISCORES,f) != MAX_HISCORES) {
			success = FALSE;
		}

		fclose(f);
	}

	return success;
}

// saveHiScores - saves the hiscore table to the hiscore file
//	returns TRUE if the scores were saved to file, FALSE otherwise
short int saveHiScores(SCORE *hiscores) {
	const char *filetype = "N68K";
	short int success = TRUE;
	FILE *f;

	// delete any old scorefile before recreating
	unlink(scorefile);

	// open the hiscore file
	if ((f = fopen(scorefile,"wb")) == NULL) {
		success = FALSE;
	} else {
		// write the hiscore table to disk - check for I/O errors
		if (fwrite(hiscores,sizeof(SCORE),MAX_HISCORES,f) != MAX_HISCORES) {
			success = FALSE;
		}

		// write the file tag
		fputc(0,f);
		fputs(filetype,f);
		fputc(0,f);
		fputc(OTH_TAG,f);

		fclose(f);
	}

	return success;
}

// createHiScoreTable - create a blank hiscore table and write it to file
// 	return the result of the disk save
short int createHiScoreTable(void) {
	SCORE board[MAX_HISCORES];

	// set all the entries in the table to blank
	memset(board,0,sizeof(SCORE) * MAX_HISCORES);

	return (saveHiScores(board));
}

// needScoreFile - checks whether a hiscore file already exists
// 	returns TRUE if there is no hiscore file, FALSE otherwise
short int needScoreFile(void) {
	FILE *f = fopen(scorefile,"rb");
	short int needed = FALSE;

	// if we cannot open the file, we must need to create it
	if (f == NULL) {
		needed = TRUE;
	} else {
		fclose(f);
	}

	return needed;
}

