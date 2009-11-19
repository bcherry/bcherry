====================================================================================

         A	      L            IIIIIIII    EEEEE    N       N
      A   A	      L	      I         EEE         N N   N
   A         A      L	      I         EEE         N    NN	INVASION	By: Jason Ho
AA        AA   LLLLL   IIIIIIII    EEEEE    N       N

"You must kill to live...they live to kill"
====================================================================================

* This help file is best viewed with Font 10, Times New Roman.

Updates
=========
- Version 1.5
  * Saved 506 bytes by using better coding for pictures.
  * Changed code around by defining prototypes.
  * Fixed a memory leak problem (in v1.4 and lower you lost 4k of ram everytime you ran Alien Invasion)
  * Easier scrolling in the menus. If you are selecting the last item of the menu and you press down, you go back to the first item, and vice versa.
  * Default speed is now 65 (instead of 63).
- Version 1.4
  * New Alien Invasion Website!
  * New Directors Cut Version! Find it at my profile at www.ticalc.org
  * Added a restriction to the level for cluster bombs, and lasers. The max level for both of them is now 20. My intentions for making better upgrades in the higher levels were not for people to be able to clear 4 levels with 1 cluster bomb, rather i wanted it so you would reach the max level faster.
  * Made the program smaller by 3840 bytes using pointers.
  * Made the program smaller by 1434 bytes by not using LCD_WIDTH and LCD_HEIGHT (rather by #define)
  * Shows you the version number of Alien Invasion in the introduction.
- Version 1.3
  * Improved bullet collision detection.
  * Upgrades get better as you get to the higher levels! This is in response to the complaints that once you die in the high levels, theres almost no way to recover.
  * Max level for heat seaker is now 5.
  * Chances of getting recharge in shield is higher.
  * Fixed a laser bug that froze your calculator when you got the level too high.
  * APPS does not work anymore, instead you have HOME which will let you quickly escape out of the program while saving your state
      (!!!) Note this function does *not* work if the program is archived!!!.
- Version 1.2
  * Fixed all highscore bugs.
- Version 1.1
  * Fixed a highscore bug you had when you died (didnt go to the "CONGRATULATIONS YOU MADE IT INTO HIGH SCORES" screen).
- Version 1.0
  * First Release!

* AI is a no stub program, but since it is big, youll need to run either AISTART() (included in the zip file), install doorsos and just run AI(), or use the hardware patch and run AI().

ALIEN INVASION ONLINE
==============================================
http://www.geocities.com/dakron15/AI

This is the official Alien Invasion Website. Go here if you want to get the very latest news about Alien Invasion or the very latest version of Alien Invasion. The Alien Invasion version will be more updated than at calc.org or at ticalc.org; sometimes if i change a little in the game, but not enough to make a new version, I will still update it onto this webpage. This page should be up by 2-17-01, so keep checking for updates!

What is Alien Invasion?
====================
Alien Invasion is an advanced shooting game for the TI-89 programmed completely in C, that can run up to 70 frames per second (fps). Instead of just mowing down the enemies by holding down fire, Alien Invasion requires careful planning, fast thinking, and good judgement. You are provided with a normal gun, and many special items and upgrades to destroy the enemy. As each level passes, the difficulty progressively increases. 

Controls
========
The controls are very simple, basically the ARROWS, 2nd, and F1-F5. Heres a more detailed description:

2nd - Fire bullets
LEFT - Move Left
RIGHT - Move Right
UP - Activate Shields
DOWN - Teleport to the other side.
F1 - Shoot/Detonate Cluster Bomb
F2 - Heat Seeker
F3 - Lasers
F4 - Disruption Field
F5 - Check item levels
HOME - Quick Exit
ESC - Main Menu/Quit Game

Fire bullets
=========
Fires the typical gun installed on all ally ships. These guns can be upgraded to triple fire, and spreader fire. You will start finding spreader fire upgrades after level 20. You can never run out of these bullets. Press 2nd to fire these bullets.

Cluster Bomb
===========
Sends out a powerful bomb that must be manually detonated. Press F1 to send it out, and F1 again to detonated it. The blast radius may get very large, but your ship will not be harmed by the blast. The maximum level for Cluster Bombs is level 20.

Heat Seeker
=========
This special type of bullet detects heat emitted from other enemies. It then targets the enemy, follows its prey, and detonates upon impact. To use it press F2. Heat seakers can only be upgraded to level 5.

Lasers
======
Shoots a very powerful laser that would easily burn through any ship. To use the lasers, press F3. The maximum level for lasers is level 20.

Disruption Field
=============
Sends out powerful waves that temporarily disables enemy thrusters. Note that because this only works for electronical enemies, the biological enemies will not be affected by this skill. Disable time will vary. Press F4 to use this skill.

Shields
======
Activates the magnetic protection shields that will shield you from any type of damage. In times of trouble, these shields are vital to your survival. Because of the large amount of energy required to maintain the shield, it must be recharged. Projectiles cannot be fired when the shields are active. Your shield bar is indicated at the bottom right hand corner of the screen. To use the shields hold down the UP button. To toggle the item/shield display, press the SHIFT button.

Teleport
=======
Using advanced technology, teleporting breaks you down into little particles, sends your particles through a laser to the other side, and then reassembles the particles. Teleporting in and out of trouble is a vital strategy for survival. To teleport, press the DOWN button.

Temporary Invincibility
===================
Temporary Invincibility is cast whenever you die and come back in, or when you teleport from one end to the other. This is to prevent the possibility of uncontrollable death.

Upgrades
========
After completing each level, whats left of the enemy's energy will be transformed into in the form of a ruby. Grab as many of these as you can to receive all sorts of upgrades, including extra special items, enhancing of special items (cluster bombs get larger, lasers get longer, and heat seakers follow better), full shield recharge, and much more. Press F5 to check your current item levels.

Making A MOD of Alien Invasion
===========================
If you would like to make a MOD of my game, download the Alien Invasion Source at http://www.ticalc.org
In there you will find the conditions at which you may modify my game.
Even if you dont know how to program in C, you can still make a MOD of this game, it is all explained in the README of the Alien Invasion Source.

Other Things
===========
If you really like my game, please send me an email or Instant Message and tell me what you think about it; that would really make my day. 
To download more of my programs, visit my profile at:  http://www.ticalc.org/archives/files/authors/31/3179.html
Other comments, suggestions, bug reports, game ideas, complaints - email me or send me an instant message.

Thanks
=======
Thanks to...
- James J. Marshall for suggesting many new features, bugs, and for continual support.
- Sebastian Reicheit for being patient and answering my frequent questions. 
- The guy who answered me on how to use rowread at tict.ticalc.org (Dark Angel)
- Thomas Nussbaumer for helping me with many problems.
- Peter Rowe for helping me with many problems, and for suggesting the "bosskey" (quick exit).
- Paul Froissart and Olle Hedman for showing me how to use pointers.
- Paul Froissart for suggesting to not use LCD_WIDTH and LCD_HEIGHT.
- David K. for finding the memory leak bug.
- My friend Will Roche for the support, and for designing a few pictures for me. (http://www.ticalc.org/archives/files/authors/45/4556.html)

======================================================
Name: Jason Ho
Email: dakron15@netzero.net or cho@mindspring.com
(Im trying to get used to using dakron15@netzero.net, so send it there, but if that doesnt work send it to cho@mindspring.com)
AIM: Bravo585
URL: http://www.geocities.com/dakron15
======================================================