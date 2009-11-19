// C Source File
// Created 21/Oct/03; 16:35:25
/*
    Slider Puzzle 2.0
    Copyright (C) 2000-2002 John David Ratliff

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
// Created 12/20/00; 4:01:49 PM
// Updated 03/08/01; 6:15:00 AM
// Updated 8/23/2002; 2:21:39 AM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "slider.h"            // include the slider puzzle header file

void initPuzzle(int *puzzle) {
      int loop;

      // initialize the puzzle positions
      for (loop = 0; loop < 15; loop++) {
            puzzle[loop] = loop;
      }
      
      // make the last piece the blank piece
      puzzle[15] = -1;
}

void randomizePuzzle(int *puzzle) {
      int loop, randNum, temp;

      // randomize the puzzle pieces
      for (loop = 0; loop < 16; loop++) {
            // select a random puzzle piece
            randNum = random(16);

            // replace the old piece with the new piece
            temp = puzzle[loop];
            puzzle[loop] = puzzle[randNum];
            puzzle[randNum] = temp;
      }
}

void drawPuzzle(int *x, int *y, int *puzzle) {
      int xLoop, yLoop, piece = 0;
      
      // randomize the puzzle before drawing
      randomizePuzzle(puzzle);

      // loop through the rows of the puzzle to draw them
      for (yLoop = 0; yLoop < (16 * 4); yLoop += 16) {
            for (xLoop = 0; xLoop < (16 * 4); xLoop += 16) {
                  // only attempt to draw valid pieces
                  if (puzzle[piece] != -1) {
                        Sprite16(xLoop, yLoop, PIECE_HEIGHT, 
                                    pieces[puzzle[piece]], LCD_MEM, SPRT_XOR);
                        
                        // mark the position of the piece
                        x[puzzle[piece]] = xLoop;
                        y[puzzle[piece]] = yLoop;
                  }
                  
                  piece++;
            }
      }
}

inline void drawScreen(void) {
      // clear the screen
      ClrScr();
      
      // draw the game control strings
      DrawStr(0, 70, "Use Arrows to Move Pieces", A_NORMAL);
      DrawStr(0, 80, "Press ENTER to Slide Piece", A_NORMAL);
      DrawStr(0, 90, "Press ESC to Quit", A_NORMAL);
      
      // draw the game information strings
      DrawStr(67, 0, "Slider 2.0", A_NORMAL);
      
      FontSetSys(F_4x6);
      DrawStr(80, 10, "Copyright (C) 2001", A_NORMAL);
      DrawStr(80, 18, "John David Ratliff", A_NORMAL);
      
      DrawStr(67, 30, "Created by Techno-Plaza", A_NORMAL);
      DrawStr(75, 38, "for Lesson Review 2", A_NORMAL);
      DrawStr(67, 50, "Learn more at our website", A_NORMAL);
      DrawStr(70, 58, "www.technoplaza.net", A_NORMAL);

      // reset font to normal size
      FontSetSys(F_6x8);
}

void newGame(int *x, int *y, int *puzzle) {
      int key, loop;
      int position = 0, currentPiece = 0, on = 1, done = 0, winner = 0;

      // draw the background screen
      drawScreen();

      // draw a new puzzle
      drawPuzzle(x,y,puzzle);
      
      // set the first piece to be piece 0
      currentPiece = puzzle[position];
      
      // loop until the puzzle is completed, or until the
      // user presses ESC
      while (!done) {
            // set the timer interval to 1/2 second
            OSFreeTimer(USER_TIMER);
            OSRegisterTimer(USER_TIMER, 10);
            
            // wait for the timer to expire, or the user to press a key
            while (!OSTimerExpired(USER_TIMER) && !kbhit());
            
            if (kbhit()) {
                  // if the user pressed a key, grab it
                  key = ngetchx();
            } else {
                  // otherwise, erase old keystrokes
                  key = 0;
            }
            
            // handle keystrokes
            if (key == KEY_ESC) {
                  // ESC means they quit
                  done = 1;
            } else if (key == KEY_ENTER) {
                  if (position+1 <= 16) {
                        // try to move the piece right
                        // if the right position is vacant
                        if (puzzle[position+1] == -1 && x[currentPiece] != (16*4-16)) {
                              // make sure that the old piece is still drawn
                              if (!on) {
                                    Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                                pieces[currentPiece], LCD_MEM, SPRT_XOR);
                                    on = 1;
                              }

                              // erase the piece
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // move the piece right 1 position (16 pixels)
                              x[currentPiece]+=16;
                        
                              // reset the puzzle position holders
                              puzzle[position+1] = puzzle[position];
                              puzzle[position] = -1;
                              position++;
                              currentPiece = puzzle[position];

                              // draw the piece at the new position                              
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // don't try to move any other direction
                              continue;
                        }
                  } 
                  
                  if (position-1 >= 0) {
                        // try to move the piece left
                        // if the left position is vacant, and we haven't already moved
                        if (puzzle[position-1] == -1 && x[currentPiece] != 0) {
                              // make sure the piece is drawn, so it can be properly erased
                              if (!on) {
                                    Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                                pieces[currentPiece], LCD_MEM, SPRT_XOR);
                                    on = 1;
                              }
                              
                              // erase the piece
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // move the piece 1 position left (16 pixels)
                              x[currentPiece]-=16;
                              
                              // reset the puzzle position holders
                              puzzle[position-1] = puzzle[position];
                              puzzle[position] = -1;
                              position--;
                              currentPiece = puzzle[position];
                              
                              // draw the piece at the new location
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // don't try to move any other direction
                              continue;
                        }
                  } 
                  
                  if (position-4 >= 0) {
                        // try to move the piece up
                        // if the up position is vacant, and we haven't already moved
                        if (puzzle[position-4] == -1 && y[currentPiece] != 0) {
                              // make sure the piece is drawn, so it can be properly erased
                              if (!on) {
                                    Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                                pieces[currentPiece], LCD_MEM, SPRT_XOR);
                                    on = 1;
                              }
                              
                              // erase the piece                              
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // move the piece 1 position up (16 pixels)
                              y[currentPiece]-=16;
                              
                              // reset the puzzle position holders
                              puzzle[position-4] = puzzle[position];
                              puzzle[position] = -1;
                              position-=4;
                              currentPiece = puzzle[position];
                              
                              // redraw the piece at the new location
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // don't try to move any other direction
                              continue;
                        }
                  }
                  
                  if (position+4 <= 15) {
                        // try to move the piece down
                        // if the down position is vacant, and we haven't already moved
                        if (puzzle[position+4] == -1 && y[currentPiece] != (16*4-16)) {
                              // make sure the piece is drawn, so it can be properly erased
                              if (!on) {
                                    Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                                pieces[currentPiece], LCD_MEM, SPRT_XOR);
                                    on = 1;
                              }
                              
                              // erase the piece
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // move the piece one position down (16 pixels)
                              y[currentPiece]+=16;
                              
                              // reset the puzzle position holders
                              puzzle[position+4] = puzzle[position];
                              puzzle[position] = -1;
                              position+=4;
                              currentPiece = puzzle[position];
                              
                              // redraw the piece at the new location
                              Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                          pieces[currentPiece], LCD_MEM, SPRT_XOR);
                              
                              // no need to continue since there are no other directions...
                        }
                  }
            } else if (key == KEY_LEFT) {
                  // make sure the piece is still drawn, so it's still there when we move
                  if (!on) {
                        Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                    pieces[currentPiece], LCD_MEM, SPRT_XOR);
                        on = 1;
                  }

                  // if there is a left position to move to                  
                  if (position > 0) {
                        // move left
                        position--;
                        
                        // if we are at the empty slot, move left again
                        if (puzzle[position] == -1 && position > 0) {
                              position--;
                        
                        // if we can't move left anymore, start at the end
                        } else if (puzzle[position] == -1 && position == 0) {
                              position = 15;
                        }
                  } else {
                        // set the position to the end
                        position = 15;
                        
                        // if we found the empty slot, move left one
                        if (puzzle[position] == -1) {
                              position--;
                        }
                  }

                  // reset the puzzle position holder
                  currentPiece = puzzle[position];
            } else if (key == KEY_RIGHT) {
                  // make sure the piece is drawn, so we can move away safely
                  if (!on) {
                        Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                    pieces[currentPiece], LCD_MEM, SPRT_XOR);
                        on = 1;
                  }
                  
                  // if there is a position to move right to
                  if (position < 15) {
                        // move one position to the right
                        position++;
                        
                        // if we hit the empty slot, move right again
                        if (puzzle[position] == -1 && position < 15) {
                              position++;
                        
                        // if we hit the empty slot and the edge, start over
                        } else if (puzzle[position] == -1 && position == 15) {
                              position = 0;
                        }
                  // else, we need to start over at the beginning
                  } else {
                        // move to the beginning
                        position = 0;
                        
                        // if we hit the empty slot, move right again
                        if (puzzle[position] == -1) {
                              position++;
                        }
                  }
                  
                  // reset the puzzle position holder
                  currentPiece = puzzle[position];
            } else if (key == KEY_UP) {
                  // make sure the piece is drawn, so we can move safely
                  if (!on) {
                        Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                    pieces[currentPiece], LCD_MEM, SPRT_XOR);
                        on = 1;
                  }
                  
                  // if there is an up position to move to
                  if ((position - 4) >= 0) {
                        // move up one position
                        position-=4;
                        
                        // if we hit the empty slot
                        if (puzzle[position] == -1) {
                              // if there is another up position, go there
                              if ((position - 4) >= 0) {
                                    position-=4;
                              // otherwise, run to the bottom row
                              } else {
                                    position = 16 - (4 - position);
                              }
                        }
                  // else, move to the bottom row
                  } else {
                        // set position at the bottom row
                        position = 16 - (4 - position);
                        
                        // if we hit the empty slot, move up again
                        if (puzzle[position] == -1) {
                              position-=4;
                        }
                  }
                  
                  // reset the puzzle position holder
                  currentPiece = puzzle[position];
            } else if (key == KEY_DOWN) {
                  // make sure the piece is still drawn, so we can move safely
                  if (!on) {
                        Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                                    pieces[currentPiece], LCD_MEM, SPRT_XOR);
                        on = 1;
                  }
                  
                  // if there is a down position to move to
                  if ((position + 4) <= 15) {
                        // move down one position
                        position+=4;
                        
                        // if we hit the empty slot
                        if (puzzle[position] == -1) {
                              // if there is another down position
                              if ((position + 4) <= 15) {
                                    // move to the next down position
                                    position+=4;
                              // otherwise, goto the top row
                              } else {
                                    position%=4;
                              }
                        }
                  // else, move to the top row
                  } else {
                        // move to the top row
                        position%=4;
                        
                        // if we hit the empty slot
                        if (puzzle[position] == -1) {
                              // move down one more time
                              position+=4;
                        }
                  }
                  
                  // reset the puzzle position holder
                  currentPiece = puzzle[position];
            }
            
            // blink the current piece so we know which one it is
            on = !on;
            Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                        pieces[currentPiece], LCD_MEM, SPRT_XOR);
            
            // set the winner to one, then try to disqualify it with
            // the for loop. it's easier to reach a false conclusion than
            // a true conclusion, so start true, and work towards false
            winner = 1;

            // test for winner
            // loop through all game pieces, and see if their positions
            // match their piece numbers. If they do, then the player won
            // if we find any piece that is not right, we have no winner
            // and we stop the for loop
            for (loop = 0; loop < 15; loop++) {
                  if (y[loop] != ((loop / 4) * 16) || x[loop] != ((loop % 4) * 16)) {
                        winner = 0;
                        break;
                  }
            }

            // if we have a winner, make sure the piece is drawn and
            // set done to yes
            if (winner) {
                  // if we win, then we are done, obviously
                  done = 1;
                  
                  // make sure the piece is drawn so it looks nice
                  if (!on) {
                        Sprite16(x[currentPiece], y[currentPiece], PIECE_HEIGHT, 
                              pieces[currentPiece], LCD_MEM, SPRT_XOR);
                        on = 1;
                  }
                  
                  // tell the player they won
                  DlgMessage("Winner", "You have completed the puzzle!", BT_OK, BT_NONE);
            }
      }
}

void _main(void) {
      int *x = NULL, *y = NULL, *puzzle = NULL;
      const char *title = "DMA Error", *error = "Unable to allocate array space";
      
      // allocate memory for the x position array
      if ((x = (int *)calloc(16,sizeof(int))) == NULL) {
            DlgMessage(title,error,BT_OK,BT_NONE);
            return;
      }
      
      // allocate memory for the y position array
      if ((y = (int *)calloc(16,sizeof(int))) == NULL) {
            DlgMessage(title,error,BT_OK,BT_NONE);
            
            // free the memory used by the x positions
            free(x);
            
            return;
      }
      
      // allocate memory fro the puzzle piece position array
      if ((puzzle = (int *)calloc(16,sizeof(int))) == NULL) {
            DlgMessage(title,error,BT_OK,BT_NONE);
            
            // free the memory used by the position arrays
            free(x);
            free(y);
            
            return;
      }
      
      // seed the random number generator
      randomize();
      
      // initialize the puzzle
      initPuzzle(puzzle);
      
      // loop and play the game
      while ((DlgMessage("Slider Puzzle 2.0","Press ENTER to play, ESC to quit.",
                        BT_OK,BT_NONE)) == KEY_ENTER) {
            newGame(x,y,puzzle);
      }
      
      // free the memory allocated for the arrays
      free(x);
      free(y);
      free(puzzle);
}
