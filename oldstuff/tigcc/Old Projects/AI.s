	.file	"AI.c"
#NO_APP
/* Include Patch: __not_reg_relative */
	.macro __save_the_stack_pointer
		move.l %sp,__save__sp__
	.endm
	.data
tigcc_compiled.:
	.set _A_LINE,0xA000
#APP
/* Main Startup Code */
	.even
	.set __min_num_ROM_CALLs__,0x3CC
	move.l 0xC8,%a0
	/* Calculator Detection */
	moveq #1,%d0
	move.l %a0,%d1
	and.l #0x400000,%d1
	jbne __calc_in_d0__
	clr.w %d0
	move.l (%a0,0x2F*4),%a1 /* ScrRect */
	cmp.b #200,2(%a1)
	jbcs __calc_in_d0__
	moveq #3,%d0
__calc_in_d0__:
	lea __calculator(%pc),%a1
	move.w %d0,(%a1)
__calc_is_OK__:
	/* Minimum AMS Version Check */
	cmp.l #__min_num_ROM_CALLs__,(%a0,-4) /* TIOS_entries */
	jbcc __ams_version_is_OK__
__ams_version_not_OK__:
	pea.l __ams_too_low__
__display_message_and_exit__:
	move.l (%a0,0xE6*4),%a0 /* ST_helpMsg */
	jsr (%a0)
	addq.l #4,%sp
	rts
__ams_version_is_OK__:
	/* Screen Saving Support */
	pea.l (%a2)
	lea (%sp,-3840),%sp
	pea 3840
	pea 0x4C00
	pea (%sp,8)
	move.l (%a0,0x26A*4),%a2 /* memcpy */
	jsr (%a2)
	/* Support for exit and atexit */
	movem.l %d3-%d7/%a2-%a6,-(%sp)
	jbsr _main
	movem.l (%sp)+,%d3-%d7/%a2-%a6
	/* Screen Saving Support (Restoring) */
	pea 3840
	pea (%sp,16)
	pea 0x4C00
	jsr (%a2)
	lea (%sp,3864),%sp
	movea.l (%sp)+,%a2
	rts
	.xdef __save__sp__
	.even
__save__sp__:
	.long 0
	.xdef __calculator
	.even
__calculator:
	.word -1
	.even
__ams_too_low__:
	.ascii "AMS "
	.ascii "1.01"
	.asciz " or higher needed"
	.even
#NO_APP
#APP
	/* Include Patch: _detect_calc_ */
	/* Include Patch: _need_calculator_var_ */
	/* Include Patch: _MIN_AMS_required_ */
	/* Include Patch: _MIN_AMS_1_01_ */
	/* Include Patch: dont_set_file_inuse_bit */
	/* Include Patch: nostub_patch */
	/* Include Patch: nostub_optimize_rom_calls */
	/* Include Patch: nostub_save_screen */
	/* Include Patch: no_retval */
	/* Include Patch: nostub_exit_support */
	/* Include Patch: save_the_sp */
	/* Include Patch: complex_main */
	.set MT_TEXT,0x8000
	.set MT_XREF,0x9000
	.set MT_ICON,0xA000
	.set MT_CASCADE,0x4000
#NO_APP
	.globl	player_bullet_x
	.data
	.even
player_bullet_x:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.globl	player_bullet_y
	.even
player_bullet_y:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.globl	player_particles_x
	.even
player_particles_x:
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.globl	player_particles_y
	.even
player_particles_y:
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.globl	player_explosion
	.even
player_explosion:
	.word	0
	.globl	player_invincible
	.even
player_invincible:
	.word	0
	.globl	player_invincible_time
	.even
player_invincible_time:
	.word	150
	.globl	player_shield_on
	.even
player_shield_on:
	.word	0
	.globl	player_shield
	.even
player_shield:
	.word	200
	.globl	laser_x
	.even
laser_x:
	.word	0
	.globl	laser_y
	.even
laser_y:
	.word	-1
	.globl	laser_length
	.even
laser_length:
	.word	-2
	.globl	laser_press
	.even
laser_press:
	.word	0
	.globl	laser_level
	.even
laser_level:
	.word	15
	.globl	cluster_x
	.even
cluster_x:
	.word	-1
	.globl	cluster_y
	.even
cluster_y:
	.word	-1
	.globl	cluster_at_y
	.even
cluster_at_y:
	.word	-1
	.globl	cluster_radius
	.even
cluster_radius:
	.word	0
	.globl	cluster_press
	.even
cluster_press:
	.word	0
	.globl	cluster_level
	.even
cluster_level:
	.word	20
	.globl	heat_x
	.even
heat_x:
	.word	-1
	.globl	heat_y
	.even
heat_y:
	.word	-1
	.globl	heat_target
	.even
heat_target:
	.word	0
	.globl	heat_level
	.even
heat_level:
	.word	3
	.globl	heat_accuracy
	.even
heat_accuracy:
	.word	1
	.globl	disruption_press
	.even
disruption_press:
	.word	0
	.globl	item_disruption
	.even
item_disruption:
	.word	5
	.globl	item_heat
	.even
item_heat:
	.word	5
	.globl	item_cluster
	.even
item_cluster:
	.word	5
	.globl	item_laser
	.even
item_laser:
	.word	5
	.globl	item_show
	.even
item_show:
	.word	1
	.globl	enemy_particles_Vx
	.data
	.even
enemy_particles_Vx:
	.word	-3
	.word	-2
	.word	0
	.word	3
	.word	2
	.word	3
	.globl	enemy_particles_Vy
	.even
enemy_particles_Vy:
	.word	-2
	.word	2
	.word	-3
	.word	2
	.word	3
	.word	-2
	.globl	enemy_x
	.data
	.even
enemy_x:
	.word	20
	.word	40
	.word	60
	.word	80
	.word	100
	.globl	enemy_y
	.even
enemy_y:
	.word	3
	.word	3
	.word	3
	.word	3
	.word	3
	.globl	enemy_Vx
	.even
enemy_Vx:
	.word	-2
	.word	1
	.word	-1
	.word	2
	.word	1
	.globl	enemy_Vy
	.even
enemy_Vy:
	.word	-1
	.word	1
	.word	-1
	.word	1
	.word	1
	.globl	enemy_bullet_x
	.even
enemy_bullet_x:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.globl	enemy_bullet_y
	.even
enemy_bullet_y:
	.word	111
	.word	111
	.word	111
	.word	111
	.word	111
	.globl	enemy_explosion
	.even
enemy_explosion:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.globl	enemy_alive
	.even
enemy_alive:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.globl	enemy_ship
	.even
enemy_ship:
	.word	1
	.globl	enemy_explosion_show
	.even
enemy_explosion_show:
	.word	1
	.globl	enemy_low
	.even
enemy_low:
	.word	1
	.globl	enemy_bullet_accuracy
	.even
enemy_bullet_accuracy:
	.word	0
	.globl	enemy_bullet_speed
	.even
enemy_bullet_speed:
	.word	1
	.globl	upgrade_x
	.even
upgrade_x:
	.word	-1
	.globl	upgrade_y
	.even
upgrade_y:
	.word	111
	.globl	game_speed
	.even
game_speed:
	.word	6800
	.globl	game_level
	.even
game_level:
	.word	1
	.globl	game_lives
	.even
game_lives:
	.word	3
	.globl	game_quick_exit
	.even
game_quick_exit:
	.word	0
	.globl	high_name
	.even
high_name:
	.word	74
	.word	97
	.word	115
	.word	111
	.word	110
	.word	32
	.word	72
	.word	111
	.word	32
	.word	32
	.word	32
	.word	32
	.word	74
	.word	97
	.word	115
	.word	111
	.word	110
	.word	32
	.word	72
	.word	111
	.word	32
	.word	32
	.word	32
	.word	32
	.word	74
	.word	97
	.word	115
	.word	111
	.word	110
	.word	32
	.word	72
	.word	111
	.word	32
	.word	32
	.word	32
	.word	32
	.word	74
	.word	97
	.word	115
	.word	111
	.word	110
	.word	32
	.word	72
	.word	111
	.word	32
	.word	32
	.word	32
	.word	32
	.word	74
	.word	97
	.word	115
	.word	111
	.word	110
	.word	32
	.word	72
	.word	111
	.word	32
	.word	32
	.word	32
	.word	32
	.globl	high_score
	.even
high_score:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.globl	repeat1
	.even
repeat1:
	.word	0
	.globl	repeat2
	.even
repeat2:
	.word	0
	.globl	virtual
	.even
virtual:
	.long	0
	.even
player_ship:
	.word	64
	.word	224
	.word	224
	.word	304
	.word	272
	.word	792
	.word	5197
	.word	7855
	.word	5805
	.word	5869
	.word	5917
	.word	7967
	.word	3574
player_bullet:
	.byte	2
	.byte	7
	.byte	7
	.byte	2
enemy_bullet:
	.byte	2
	.byte	7
	.byte	2
	.even
heart:
	.word	238
	.word	509
	.word	509
	.word	507
	.word	246
	.word	124
	.word	56
	.word	16
	.even
shield:
	.long	17825792
	.long	32505856
	.long	30539775
	.long	30474241
	.long	32636927
	.long	14680064
	.even
upgrade:
	.word	56
	.word	116
	.word	242
	.word	511
	.word	254
	.word	124
	.word	56
particle:
	.byte	14
	.byte	31
	.byte	27
	.byte	31
	.byte	14
enemy_particle:
	.byte	7
cluster:
	.byte	28
	.byte	58
	.byte	121
	.byte	127
	.byte	127
	.byte	62
	.byte	28
heat:
	.byte	2
	.byte	5
	.byte	5
	.byte	5
	.byte	7
disruption:
	.byte	25
	.byte	63
	.byte	102
	.even
enemy1:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	28700
	.long	59450
	.long	124921
	.long	131071
	.long	131071
	.long	63550
	.long	28700
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	14336
	.long	29696
	.long	61952
	.long	65024
	.long	65024
	.long	32256
	.long	16128
	.long	896
	.long	504
	.long	244
	.long	242
	.long	254
	.long	254
	.long	124
	.long	56
	.long	0
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.long	896
	.long	896
	.long	896
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.long	0
	.long	56
	.long	116
	.long	242
	.long	254
	.long	254
	.long	252
	.long	504
	.long	896
	.long	16128
	.long	30208
	.long	61952
	.long	65024
	.long	65024
	.long	31744
	.long	14336
	.long	0
	.even
enemy2:
	.long	0
	.long	14392
	.long	29812
	.long	62194
	.long	65278
	.long	65278
	.long	32508
	.long	16376
	.long	896
	.long	16376
	.long	30452
	.long	62194
	.long	65278
	.long	65278
	.long	31868
	.long	14392
	.long	0
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	30684
	.long	60346
	.long	124921
	.long	131071
	.long	131071
	.long	64446
	.long	30556
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.even
enemy3:
	.long	1984
	.long	8176
	.long	16312
	.long	32588
	.long	65318
	.long	65430
	.long	130955
	.long	131071
	.long	131071
	.long	131071
	.long	122879
	.long	57342
	.long	61438
	.long	30716
	.long	16376
	.long	8176
	.long	1984
	.even
enemy4:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	58254
	.long	120669
	.long	131071
	.long	128991
	.long	58254
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	56
	.long	116
	.long	124
	.long	124
	.long	1016
	.long	1856
	.long	1984
	.long	1984
	.long	16256
	.long	29696
	.long	31744
	.long	31744
	.long	14336
	.long	0
	.long	0
	.long	896
	.long	1856
	.long	1984
	.long	1984
	.long	896
	.long	256
	.long	896
	.long	1856
	.long	1984
	.long	1984
	.long	896
	.long	256
	.long	896
	.long	1856
	.long	1984
	.long	1984
	.long	896
	.long	0
	.long	0
	.long	14336
	.long	29696
	.long	31744
	.long	31744
	.long	16256
	.long	1856
	.long	1984
	.long	1984
	.long	1016
	.long	116
	.long	124
	.long	124
	.long	56
	.long	0
	.long	0
	.even
enemy5:
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.long	0
	.long	0
	.long	28700
	.long	59450
	.long	124025
	.long	130175
	.long	130175
	.long	63550
	.long	28700
	.long	0
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.long	0
	.long	0
	.long	7280
	.long	15080
	.long	31204
	.long	32764
	.long	32764
	.long	16120
	.long	7280
	.long	0
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.long	0
	.long	0
	.long	896
	.long	1856
	.long	3872
	.long	4064
	.long	4064
	.long	1984
	.long	896
	.long	0
	.even
enemy6:
	.long	262152
	.long	393240
	.long	327720
	.long	166992
	.long	89760
	.long	57024
	.long	29568
	.long	127968
	.long	259056
	.long	491448
	.long	425880
	.long	191952
	.long	49344
	.long	16512
	.long	0
	.long	0
	.long	0
	.long	3072
	.long	24192
	.long	114400
	.long	455672
	.long	652276
	.long	783356
	.long	491448
	.long	425880
	.long	191952
	.long	49344
	.long	16512
	.even
enemy7:
	.long	0
	.long	0
	.long	896
	.long	1856
	.long	118567
	.long	110571
	.long	73713
	.long	34754
	.long	21396
	.long	16248
	.long	1984
	.long	1344
	.long	896
	.long	640
	.long	640
	.long	0
	.long	0
	.long	0
	.long	0
	.long	896
	.long	5968
	.long	16184
	.long	12264
	.long	12264
	.long	6096
	.long	2976
	.long	1856
	.long	1984
	.long	1344
	.long	896
	.long	1088
	.long	2080
	.long	0
	.long	0
	.data
.LC0:
	.ascii "level: %d\0"
.LC3:
	.ascii "         \0"
.LC4:
	.ascii "%d/200\0"
.LC2:
	.ascii "level: 5\0"
.LC1:
	.ascii "level: 4\0"
	.even
	.globl	_main
_main:
#APP
	move.l %sp,__save__sp__
	/* Support for OPTIMIZE_ROM_CALLS */
	move.l 0xC8,%a5
#NO_APP
	link.w %a6,#-212
	movm.l #0x1f38,-(%sp)
	clr.w -202(%a6)
	clr.w -204(%a6)
	clr.w -206(%a6)
	clr.w -208(%a6)
	clr.w -210(%a6)
	pea 3840.w
	move.l 648(%a5),%a0
	jbsr (%a0)
	move.l %a0,virtual
	addq.l #4,%sp
	cmp.w #1,game_quick_exit.l
	jbeq .L2
	jbsr intro
.L2:
	jbsr init
	cmp.w #1,game_quick_exit.l
	jbeq .L3
	jbsr start
.L3:
	clr.w game_quick_exit
.L4:
	clr.w %d3
.L22:
	addq.w #1,-206(%a6)
	cmp.w #2000,-206(%a6)
	jbeq .L234
.L11:
	cmp.w #500,-206(%a6)
	jbeq .L13
	cmp.w #1000,-206(%a6)
	jbeq .L13
	cmp.w #1500,-206(%a6)
	jbeq .L13
	tst.w -206(%a6)
	jbne .L12
.L13:
	jbsr enemyshoot
.L12:
	cmp.w #2001,-206(%a6)
	slt %d0
	ext.w %d0
	and.w %d0,-206(%a6)
	move.w %d3,%a3
	lea enemy_x,%a1
	lea (%a3,%a3.l),%a0
	lea enemy_Vx,%a2
	move.w (%a2,%a0.l),%d1
	move.w (%a1,%a0.l),%d0
	add.w %d1,%d0
	move.w %d0,(%a1,%a0.l)
	lea enemy_y,%a4
	move.l #enemy_Vy,%d4
	move.w (%a0,%d4.l),%d2
	add.w %d2,(%a4,%a0.l)
	cmp.w #127,%d0
	jble .L15
	neg.w %d1
	move.w %d1,(%a2,%a0.l)
	move.w #128,(%a1,%a0.l)
.L15:
	tst.w (%a1,%a0.l)
	jble .L235
.L16:
	move.w game_level,enemy_low
	move.w game_level,%d1
	cmp.w #49,%d1
	jble .L17
	move.w %d1,%d0
	add.w #-25,%d0
	move.w %d0,enemy_low
.L17:
	cmp.w #79,%d1
	jble .L18
	move.w %d1,%d0
	add.w #-55,%d0
	move.w %d0,enemy_low
.L18:
	cmp.w #60,enemy_low.l
	jble .L19
	move.w #60,enemy_low
.L19:
	move.w enemy_low,%d0
	add.w #23,%d0
	cmp.w (%a4,%a0.l),%d0
	jbgt .L20
	tst.w %d2
	jble .L20
	neg.w %d2
	move.w %d2,(%a0,%d4.l)
.L20:
	lea enemy_y,%a2
	lea (%a3,%a3.l),%a1
	tst.w (%a2,%a1.l)
	jble .L236
.L9:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L22
	cmp.w #10,%d1
	jbeq .L237
.L23:
	cmp.w #50,%d1
	jbeq .L238
.L24:
	cmp.w #80,%d1
	jbeq .L239
.L25:
	clr.w %d3
	move.w enemy_bullet_speed,%d4
	move.w player_x,%d2
	move.w enemy_bullet_accuracy,%d1
.L33:
	move.w %d3,%a0
	lea enemy_bullet_y,%a1
	add.l %a0,%a0
	move.w (%a1,%a0.l),%d0
	cmp.w #99,%d0
	jbgt .L28
	add.w %d4,%d0
	move.w %d0,(%a1,%a0.l)
	lea enemy_bullet_x,%a1
	move.w (%a1,%a0.l),%d0
	cmp.w %d0,%d2
	jble .L31
	add.w %d1,%d0
	move.w %d0,(%a1,%a0.l)
.L31:
	move.w (%a1,%a0.l),%d0
	cmp.w %d0,%d2
	jbge .L28
	sub.w %d1,%d0
	move.w %d0,(%a1,%a0.l)
.L28:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L33
	move.w game_fire,%d1
	cmp.w #1,%d1
	jbeq .L240
.L34:
	cmp.w #3,%d1
	jbeq .L241
.L35:
	cmp.w #5,%d1
	jbeq .L242
.L44:
	clr.w %d3
	move.w game_fire,%d1
.L65:
	move.w %d3,%a0
	lea player_bullet_x,%a1
	add.l %a0,%a0
	cmp.w #160,(%a1,%a0.l)
	jbls .L60
	clr.w (%a1,%a0.l)
.L60:
	lea player_bullet_y,%a1
	tst.w (%a1,%a0.l)
	jblt .L243
.L58:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L65
	clr.w %d3
	move.w player_shield_on,%d7
	move.w player_invincible,%d6
	move.w player_explosion,%d5
	move.w player_x,%a3
	move.w player_invincible_time,%a2
.L76:
	tst.w %d7
	jbne .L68
	tst.w %d6
	jbne .L68
	tst.w %d5
	jbne .L68
	move.w %d3,%a0
	lea (%a0,%a0.l),%a1
	move.w %a3,%d2
	move.w %a3,%d0
	subq.w #3,%d0
	lea enemy_bullet_x,%a0
	move.w (%a0,%a1.l),%d1
	cmp.w %d1,%d0
	jbgt .L68
	add.w #14,%d0
	cmp.w %d1,%d0
	jblt .L68
	lea enemy_bullet_y,%a0
	move.w (%a0,%a1.l),%d0
	cmp.w #86,%d0
	jble .L68
	cmp.w #99,%d0
	jbgt .L68
	moveq.l #1,%d5
	move.w #150,%a2
	clr.w %d4
.L75:
	move.w %d4,%a0
	add.l %a0,%a0
	lea player_particles_x,%a1
	move.w %d2,%d1
	add.w #13,%d1
	move.w %d1,(%a1,%a0.l)
	lea player_particles_y,%a1
	move.w #95,(%a1,%a0.l)
	addq.w #1,%d4
	cmp.w #5,%d4
	jble .L75
.L68:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L76
	move.w %a2,player_invincible_time
	move.w %d5,player_explosion
	move.w upgrade_x,%d2
	move.w %d2,%d0
	add.w #11,%d0
	move.w player_x,%d1
	cmp.w %d0,%d1
	jbgt .L77
	add.w #9,%d1
	cmp.w %d2,%d1
	jble .L77
	move.w upgrade_y,%d1
	move.w %d1,%d0
	addq.w #7,%d0
	cmp.w #87,%d0
	jble .L77
	cmp.w #100,%d1
	jbgt .L77
	move.w #101,upgrade_y
	jbsr rand
	mulu.w #9,%d0
	clr.l %d1
	clr.l %d2
	jbmi .L244
.L78:
	moveq.l #15,%d2
	asr.l %d2,%d0
	addq.w #1,%d0
	move.w %d0,-(%sp)
	jbsr upgrade_get
	addq.l #2,%sp
.L77:
	clr.w %d3
.L84:
	move.w %d3,%a0
	lea enemy_alive,%a2
	lea (%a0,%a0.l),%a1
	cmp.w #1,(%a2,%a1.l)
	jbeq .L245
.L81:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L84
	clr.w %d3
.L90:
	move.w %d3,%a0
	lea enemy_alive,%a2
	lea (%a0,%a0.l),%a1
	tst.w (%a2,%a1.l)
	jbeq .L87
	move.w heat_y,%d2
	jblt .L87
	move.w heat_x,%d1
	lea enemy_x,%a0
	move.w (%a0,%a1.l),%d0
	cmp.w %d1,%d0
	jbgt .L87
	add.w #24,%d0
	cmp.w %d1,%d0
	jblt .L87
	lea enemy_y,%a0
	move.w (%a0,%a1.l),%d1
	move.w %d1,%d0
	add.w #17,%d0
	cmp.w %d2,%d0
	jblt .L87
	cmp.w %d2,%d1
	jbgt .L87
	lea enemy_explosion,%a0
	move.w #1,(%a0,%a1.l)
	lea enemy_Vx,%a0
	clr.w (%a0,%a1.l)
	lea enemy_Vy,%a0
	clr.w (%a0,%a1.l)
	clr.w (%a2,%a1.l)
	move.w #-1,heat_y
	move.w %d3,-(%sp)
	jbsr upgrade_test
	addq.l #2,%sp
.L87:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L90
	clr.w %d3
.L98:
	move.w %d3,%a1
	lea (%a1,%a1.l),%a2
	lea enemy_alive,%a0
	tst.w (%a0,%a2.l)
	jbeq .L93
	move.w laser_x,%d2
	move.w %d2,%d0
	add.w #-13,%d0
	lea enemy_x,%a0
	move.w (%a0,%a2.l),%d1
	cmp.w %d0,%d1
	jbgt .L93
	add.w #32,%d1
	cmp.w %d2,%d1
	jblt .L93
	lea enemy_y,%a0
	move.w (%a0,%a2.l),%d1
	move.w laser_y,%d2
	cmp.w %d1,%d2
	jbgt .L97
	move.w %d2,%d0
	add.w laser_length,%d0
	cmp.w %d1,%d0
	jbge .L96
.L97:
	move.w %d1,%d0
	add.w #17,%d0
	cmp.w %d0,%d2
	jbgt .L93
	add.w laser_length,%d2
	cmp.w %d0,%d2
	jblt .L93
.L96:
	add.l %a1,%a1
	lea enemy_explosion,%a0
	move.w #1,(%a0,%a1.l)
	lea enemy_Vx,%a0
	clr.w (%a0,%a1.l)
	lea enemy_Vy,%a0
	clr.w (%a0,%a1.l)
	lea enemy_alive,%a0
	clr.w (%a0,%a1.l)
	move.w %d3,-(%sp)
	jbsr upgrade_test
	addq.l #2,%sp
.L93:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L98
	clr.w %d3
	cmp.w game_fire.l,%d3
	jbge .L213
.L109:
	clr.w %d5
.L108:
	move.w %d3,%a0
	move.l #player_bullet_x,%d4
	lea (%a0,%a0.l),%a2
	move.w (%a2,%d4.l),%d1
	jbeq .L105
	lea player_bullet_y,%a4
	move.w (%a4,%a2.l),%d2
	jbeq .L105
	move.w %d5,%a0
	lea enemy_alive,%a3
	lea (%a0,%a0.l),%a1
	cmp.w #1,(%a3,%a1.l)
	jbeq .L246
.L105:
	addq.w #1,%d5
	cmp.w #4,%d5
	jble .L108
	addq.w #1,%d3
	cmp.w game_fire.l,%d3
	jblt .L109
.L213:
	clr.w %d5
	clr.w %d3
.L115:
	move.w %d3,%a0
	add.l %a0,%a0
	lea enemy_alive,%a1
	tst.w (%a1,%a0.l)
	jbeq .L112
	moveq.l #1,%d5
.L112:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L115
	tst.w %d5
	jbne .L116
	addq.w #1,-208(%a6)
	cmp.w #100,-208(%a6)
	jbeq .L247
.L117:
	cmp.w #101,-208(%a6)
	slt %d0
	ext.w %d0
	and.w %d0,-208(%a6)
.L116:
	move.w #254,-(%sp)
	jbsr _rowread
	and.w #16,%d0
	addq.l #2,%sp
	jbeq .L125
	tst.w player_shield_on
	jbne .L125
	tst.w -210(%a6)
	jbne .L125
	tst.w player_explosion
	jbne .L125
	move.w #1,-210(%a6)
	move.w game_fire,%d1
	cmp.w #1,%d1
	jbeq .L248
.L126:
	cmp.w #3,%d1
	jbeq .L249
.L127:
	cmp.w #5,%d1
	jbeq .L250
.L125:
	move.w #254,-(%sp)
	jbsr _rowread
	and.w #2,%d0
	addq.l #2,%sp
	jbeq .L129
	tst.w player_explosion
	jbne .L129
	move.w player_x,%d0
	subq.w #2,%d0
	move.w %d0,player_x
	jblt .L251
.L129:
	move.w #254,-(%sp)
	jbsr _rowread
	and.w #8,%d0
	addq.l #2,%sp
	jbeq .L131
	tst.w player_explosion
	jbne .L131
	move.w player_x,%d0
	addq.w #2,%d0
	move.w %d0,player_x
	cmp.w #144,%d0
	jble .L131
	move.w #144,player_x
.L131:
	clr.w player_shield_on
	move.w #254,-(%sp)
	jbsr _rowread
	and.w #1,%d0
	addq.l #2,%sp
	jbeq .L133
	tst.w player_explosion
	jbne .L133
	move.w player_shield,%d0
	jble .L133
	move.w #1,player_shield_on
	move.w %d0,%d2
	subq.w #1,%d2
	move.w %d2,player_shield
	jblt .L252
.L133:
	move.w #254,-(%sp)
	jbsr _rowread
	and.w #4,%d0
	addq.l #2,%sp
	jbeq .L135
	tst.w player_explosion
	jbne .L135
	cmp.w #150,player_invincible_time.l
	jbeq .L253
.L135:
	move.w #223,-(%sp)
	jbsr _rowread
	addq.l #2,%sp
	tst.b %d0
	jbge .L139
	tst.w player_explosion
	jbne .L139
	tst.w cluster_y
	jblt .L254
.L140:
	cmp.w #2,cluster_press.l
	jbeq .L255
.L139:
	move.w #223,-(%sp)
	jbsr _rowread
	addq.l #2,%sp
	tst.b %d0
	jblt .L143
	cmp.w #1,cluster_press.l
	jbeq .L256
.L144:
	cmp.w #3,cluster_press.l
	jbeq .L257
.L143:
	move.w #239,-(%sp)
	jbsr _rowread
	addq.l #2,%sp
	tst.b %d0
	jbge .L146
	tst.w heat_y
	jblt .L258
.L146:
	move.w #251,-(%sp)
	jbsr _rowread
	addq.l #2,%sp
	tst.b %d0
	jbge .L153
	tst.w player_explosion
	jbne .L153
	tst.w item_disruption
	jble .L153
	tst.w disruption_press
	jbne .L153
	move.w #1,disruption_press
	clr.w %d4
	clr.w %d5
	clr.w %d3
.L160:
	move.w %d3,%a1
	lea (%a1,%a1.l),%a2
	lea enemy_alive,%a0
	cmp.w #1,(%a0,%a2.l)
	jbeq .L259
.L158:
	add.l %a1,%a1
	lea enemy_alive,%a0
	cmp.w #1,(%a0,%a1.l)
	jbeq .L260
.L156:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L160
	tst.w %d4
	jbne .L153
	cmp.w #1,%d5
	jbeq .L261
.L153:
	move.w #251,-(%sp)
	lea _rowread,%a2
	jbsr (%a2)
	addq.l #2,%sp
	tst.b %d0
	jblt .L178
	clr.w disruption_press
.L178:
	move.w #253,-(%sp)
	jbsr (%a2)
	addq.l #2,%sp
	tst.b %d0
	jbge .L179
	clr.w -(%sp)
	move.w #58,-(%sp)
	move.w #56,-(%sp)
	move.w #20,-(%sp)
	move.w #52,-(%sp)
	jbsr menu
	lea Sprite8,%a2
	moveq.l #1,%d3
	move.l virtual,%a1
	lea cluster,%a0
	moveq.l #7,%d2
	moveq.l #28,%d1
	moveq.l #59,%d0
	jbsr (%a2)
	move.l virtual,%a1
	lea heat,%a0
	moveq.l #5,%d2
	moveq.l #41,%d1
	moveq.l #57,%d0
	jbsr (%a2)
	move.w #1,-(%sp)
	move.w #57,-(%sp)
	move.w #63,-(%sp)
	move.w #53,-(%sp)
	move.w #63,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.l virtual,%a1
	lea shield,%a0
	moveq.l #6,%d2
	moveq.l #64,%d1
	moveq.l #54,%d0
	jbsr Sprite32
	move.w #1,-(%sp)
	move.w #67,-(%sp)
	move.w player_shield,%d0
	moveq.l #13,%d2
	ext.l %d0
	divs.w %d2,%d0
	add.w #69,%d0
	move.w %d0,-(%sp)
	move.w #67,-(%sp)
	move.w #69,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (30,%sp),%sp
	move.w cluster_level,%d1
	move.w %d1,%d0
	muls.w #26215,%d0
	clr.w %d0
	swap %d0
	asr.w #1,%d0
	moveq.l #15,%d2
	asr.w %d2,%d1
	sub.w %d1,%d0
	subq.w #3,%d0
	move.w %d0,-(%sp)
	pea .LC0
	move.l %a6,%d3
	add.l #-200,%d3
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #29,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	cmp.w #1,heat_accuracy.l
	jbeq .L262
.L180:
	cmp.w #1,heat_level.l
	jbeq .L263
.L181:
	cmp.w #1,heat_level.l
	jbeq .L264
.L182:
	move.w laser_level,%d0
	add.w #-15,%d0
	moveq.l #10,%d2
	ext.l %d0
	divs.w %d2,%d0
	addq.w #1,%d0
	move.w %d0,-(%sp)
	pea .LC0
	move.l %a6,%d3
	add.l #-200,%d3
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #53,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	pea .LC3
	move.w #65,-(%sp)
	move.w #69,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w player_shield,-(%sp)
	pea .LC4
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #65,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	addq.w #6,%sp
	move.l #3840,(%sp)
	move.l virtual,-(%sp)
	pea 19456.w
	move.l 2472(%a5),%a0
	jbsr (%a0)
	jbsr pause
	jbsr init
	lea (12,%sp),%sp
.L179:
	move.w #223,-(%sp)
	lea _rowread,%a2
	jbsr (%a2)
	and.w #64,%d0
	addq.l #2,%sp
	jbeq .L183
	move.w #1,game_quick_exit
	jbsr stop
	jbsr end
.L183:
	move.w #247,-(%sp)
	jbsr (%a2)
	addq.l #2,%sp
	tst.b %d0
	jbge .L184
	tst.w player_explosion
	jbne .L184
	tst.w laser_press
	jbne .L184
	tst.w laser_y
	jblt .L265
.L184:
	clr.w laser_press
.L185:
	move.w #191,-(%sp)
	jbsr _rowread
	and.w #1,%d0
	addq.l #2,%sp
	jbeq .L186
	jbsr stop
	jbsr main_menu
	cmp.w #-1,%d0
	jbeq .L266
.L187:
	jbsr pause
	jbsr init
.L186:
	jbsr paint
	clr.w %d3
	move.w game_speed,%d0
.L233:
	cmp.w %d3,%d0
	jble .L4
	addq.w #1,%d3
	jbra .L233
	.even
.L266:
	jbsr high
	jbsr quit
	jbra .L187
	.even
.L265:
	move.w item_laser,%d0
	jble .L184
	subq.w #1,%d0
	move.w %d0,item_laser
	move.w #1,laser_press
	move.w #86,laser_y
	move.w player_x,%d0
	add.w #9,%d0
	move.w %d0,laser_x
	move.w laser_level,laser_length
	jbra .L185
	.even
.L264:
	cmp.w #3,heat_accuracy.l
	jbne .L182
	pea .LC2
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	addq.l #8,%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #41,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	jbra .L182
	.even
.L263:
	cmp.w #2,heat_accuracy.l
	jbne .L181
	pea .LC1
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	addq.l #8,%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #41,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	jbra .L181
	.even
.L262:
	moveq.l #4,%d0
	sub.w heat_level,%d0
	move.w %d0,-(%sp)
	pea .LC0
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #41,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	jbra .L180
	.even
.L261:
	clr.w %d3
.L171:
	clr.w %d4
.L170:
	move.w #2,-(%sp)
	move.w %d4,-(%sp)
	move.w #159,-(%sp)
	move.w %d4,-(%sp)
	clr.w -(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #1,%d4
	cmp.w #99,%d4
	jble .L170
	pea 3840.w
	move.l virtual,-(%sp)
	pea 19456.w
	move.l 2472(%a5),%a0
	jbsr (%a0)
	lea (12,%sp),%sp
	addq.w #1,%d3
	cmp.w #3,%d3
	jble .L171
	clr.w %d3
	move.w enemy_ship,%d0
.L177:
	cmp.w #5,%d0
	jbgt .L174
	move.w %d3,%a0
	add.l %a0,%a0
	lea enemy_Vx,%a1
	clr.w (%a1,%a0.l)
	lea enemy_Vy,%a1
	clr.w (%a1,%a0.l)
.L174:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L177
	subq.w #1,item_disruption
	jbra .L153
	.even
.L260:
	moveq.l #1,%d5
	jbra .L156
	.even
.L259:
	lea enemy_Vx,%a0
	tst.w (%a0,%a2.l)
	jbne .L158
	lea enemy_Vy,%a0
	tst.w (%a0,%a2.l)
	jbne .L158
	moveq.l #1,%d4
	jbra .L158
	.even
.L258:
	move.w item_heat,%d1
	jble .L146
	tst.w player_explosion
	jbne .L146
	tst.w player_shield_on
	jbne .L146
	move.w player_x,%d2
	addq.w #5,%d2
	move.w %d2,heat_x
	move.w #80,heat_y
	clr.w %d3
	move.w heat_target,%d0
.L152:
	move.w %d3,%a0
	add.l %a0,%a0
	lea enemy_alive,%a1
	cmp.w #1,(%a1,%a0.l)
	jbeq .L267
.L149:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L152
	move.w %d0,heat_target
	subq.w #1,%d1
	move.w %d1,item_heat
	jbra .L146
	.even
.L267:
	move.w %d3,%d0
	jbra .L149
	.even
.L257:
	clr.w cluster_press
	jbra .L143
	.even
.L256:
	move.w #2,cluster_press
	jbra .L144
	.even
.L255:
	move.w #3,cluster_press
	move.w #1,cluster_radius
	jbra .L139
	.even
.L254:
	tst.w cluster_press
	jbne .L140
	move.w item_cluster,%d0
	jble .L140
	tst.w cluster_radius
	jbne .L140
	tst.w player_shield_on
	jbne .L140
	move.w #1,cluster_press
	move.w player_x,%d1
	addq.w #5,%d1
	move.w %d1,cluster_x
	move.w #80,cluster_y
	subq.w #1,%d0
	move.w %d0,item_cluster
	jbra .L139
	.even
.L253:
	move.w #1,player_invincible
	move.w #15,player_invincible_time
	cmp.w #74,player_x.l
	jbgt .L137
	move.w #144,player_x
	jbra .L135
	.even
.L137:
	clr.w player_x
	jbra .L135
	.even
.L252:
	clr.w player_shield
	jbra .L133
	.even
.L251:
	clr.w player_x
	jbra .L129
	.even
.L250:
	move.w player_x,%d0
	move.w %d0,%d1
	subq.w #3,%d1
	move.w %d1,player_bullet_x
	move.w %d0,player_bullet_x+2
	move.w %d0,%d2
	addq.w #3,%d2
	move.w %d2,player_bullet_x+4
	add.w #9,%d1
	move.w %d1,player_bullet_x+6
	add.w #9,%d0
	move.w %d0,player_bullet_x+8
	move.w #87,player_bullet_y
	move.w #84,player_bullet_y+2
	move.w #81,player_bullet_y+4
	move.w #84,player_bullet_y+6
	move.w #87,player_bullet_y+8
	jbra .L125
	.even
.L249:
	lea player_bullet_x,%a0
	move.w player_x,(%a0)
	move.w (%a0),%d0
	move.w %d0,%d2
	addq.w #3,%d2
	move.w %d2,player_bullet_x+2
	addq.w #6,%d0
	move.w %d0,player_bullet_x+4
	move.w #87,player_bullet_y
	move.w #83,player_bullet_y+2
	move.w #87,player_bullet_y+4
	jbra .L127
	.even
.L248:
	move.w player_x,%d0
	addq.w #3,%d0
	move.w %d0,player_bullet_x
	move.w #87,player_bullet_y
	jbra .L126
	.even
.L247:
	clr.w %d3
.L122:
	move.w %d3,%a0
	add.l %a0,%a0
	lea enemy_alive,%a1
	move.w #1,(%a1,%a0.l)
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L122
	jbsr rand
	mulu.w #7,%d0
	clr.l %d1
	clr.l %d2
	jbmi .L268
.L123:
	moveq.l #15,%d2
	asr.l %d2,%d0
	addq.w #1,%d0
	move.w %d0,enemy_ship
	addq.w #1,game_level
	jbsr changeship
	jbra .L117
	.even
.L268:
	add.l #32767,%d0
	jbra .L123
	.even
.L246:
	lea enemy_x,%a0
	move.w (%a0,%a1.l),%d0
	cmp.w %d1,%d0
	jbgt .L105
	add.w #24,%d0
	cmp.w %d1,%d0
	jblt .L105
	lea enemy_y,%a0
	move.w (%a0,%a1.l),%d1
	move.w %d1,%d0
	add.w #17,%d0
	cmp.w %d2,%d0
	jblt .L105
	cmp.w %d2,%d1
	jbgt .L105
	lea enemy_explosion,%a0
	move.w #1,(%a0,%a1.l)
	lea enemy_Vx,%a0
	clr.w (%a0,%a1.l)
	lea enemy_Vy,%a0
	clr.w (%a0,%a1.l)
	clr.w (%a3,%a1.l)
	clr.w (%a2,%d4.l)
	clr.w (%a4,%a2.l)
	move.w %d5,-(%sp)
	jbsr upgrade_test
	addq.l #2,%sp
	jbra .L105
	.even
.L245:
	move.w cluster_radius,%d4
	jble .L81
	move.w cluster_x,%d2
	move.w %d2,%d0
	sub.w %d4,%d0
	lea enemy_x,%a0
	move.w (%a0,%a1.l),%a0
	move.w %a0,%d1
	add.w #17,%d1
	cmp.w %d0,%d1
	jblt .L81
	add.w %d4,%d2
	cmp.w %a0,%d2
	jblt .L81
	move.w cluster_at_y,%d2
	move.w %d2,%d0
	sub.w %d4,%d0
	lea enemy_y,%a0
	move.w (%a0,%a1.l),%a0
	move.w %a0,%d1
	add.w #17,%d1
	cmp.w %d0,%d1
	jblt .L81
	add.w %d4,%d2
	cmp.w %a0,%d2
	jblt .L81
	lea enemy_explosion,%a0
	move.w #1,(%a0,%a1.l)
	lea enemy_Vx,%a0
	clr.w (%a0,%a1.l)
	lea enemy_Vy,%a0
	clr.w (%a0,%a1.l)
	clr.w (%a2,%a1.l)
	move.w %d3,-(%sp)
	jbsr upgrade_test
	addq.l #2,%sp
	jbra .L81
	.even
.L244:
	add.l #32767,%d0
	jbra .L78
	.even
.L243:
	clr.w (%a1,%a0.l)
	tst.w %d3
	jbne .L62
	cmp.w #1,%d1
	sne %d0
	ext.w %d0
	and.w %d0,-210(%a6)
.L62:
	cmp.w #2,%d3
	jbeq .L269
.L63:
	cmp.w #4,%d3
	jbne .L58
	cmp.w #5,%d1
	sne %d0
	ext.w %d0
	and.w %d0,-210(%a6)
	jbra .L58
	.even
.L269:
	cmp.w #3,%d1
	sne %d0
	ext.w %d0
	and.w %d0,-210(%a6)
	jbra .L63
	.even
.L242:
	clr.w %d3
.L49:
	move.w %d3,%a0
	lea player_bullet_y,%a1
	add.l %a0,%a0
	subq.w #2,(%a1,%a0.l)
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L49
	addq.w #1,-202(%a6)
	addq.w #1,-204(%a6)
	cmp.w #1,-202(%a6)
	jbeq .L270
.L50:
	cmp.w #4,-204(%a6)
	jbeq .L271
.L51:
	cmp.w #4,-204(%a6)
	jbeq .L272
.L52:
	cmp.w #1,-202(%a6)
	jbeq .L273
.L53:
	cmp.w #2,-202(%a6)
	slt %d0
	ext.w %d0
	and.w %d0,-202(%a6)
	cmp.w #5,-204(%a6)
	slt %d0
	ext.w %d0
	and.w %d0,-204(%a6)
	jbra .L44
	.even
.L273:
	addq.w #1,player_bullet_x+8
	jbra .L53
	.even
.L272:
	addq.w #1,player_bullet_x+6
	jbra .L52
	.even
.L271:
	subq.w #1,player_bullet_x+2
	jbra .L51
	.even
.L270:
	subq.w #1,player_bullet_x
	jbra .L50
	.even
.L241:
	clr.w %d3
.L40:
	move.w %d3,%a0
	lea player_bullet_y,%a1
	add.l %a0,%a0
	subq.w #2,(%a1,%a0.l)
	addq.w #1,%d3
	cmp.w #2,%d3
	jble .L40
	addq.w #1,-204(%a6)
	cmp.w #5,-204(%a6)
	jbeq .L274
.L41:
	cmp.w #5,-204(%a6)
	jbeq .L275
.L42:
	cmp.w #6,-204(%a6)
	slt %d0
	ext.w %d0
	and.w %d0,-204(%a6)
	jbra .L35
	.even
.L275:
	addq.w #1,player_bullet_x+4
	jbra .L42
	.even
.L274:
	subq.w #1,player_bullet_x
	jbra .L41
	.even
.L240:
	subq.w #2,player_bullet_y
	jbra .L34
	.even
.L239:
	move.w #2,enemy_bullet_accuracy
	jbra .L25
	.even
.L238:
	move.w #2,enemy_bullet_speed
	jbra .L24
	.even
.L237:
	move.w #1,enemy_bullet_accuracy
	jbra .L23
	.even
.L236:
	lea enemy_Vy,%a0
	neg.w (%a0,%a1.l)
	clr.w (%a2,%a1.l)
	jbra .L9
	.even
.L235:
	neg.w (%a2,%a0.l)
	clr.w (%a1,%a0.l)
	jbra .L16
	.even
.L234:
	jbsr changeship
	jbra .L11
	nop
	.even
	.globl	pause
pause:
	link.w %a6,#0
	move.l %a2,-(%sp)
	move.l %d3,-(%sp)
	clr.w %d3
.L277:
	move.w #191,-(%sp)
	lea _rowread,%a2
	jbsr (%a2)
	and.w #1,%d0
	addq.l #2,%sp
	jbne .L278
	move.w #253,-(%sp)
	jbsr (%a2)
	and.w #1,%d0
	addq.l #2,%sp
	jbeq .L277
.L278:
	move.w #191,-(%sp)
	jbsr (%a2)
	and.w #1,%d0
	addq.l #2,%sp
	jbeq .L282
	moveq.l #1,%d3
.L282:
	move.w #191,-(%sp)
	lea _rowread,%a2
	jbsr (%a2)
	and.w #1,%d0
	addq.l #2,%sp
	jbne .L282
	move.w #253,-(%sp)
	jbsr (%a2)
	and.w #1,%d0
	addq.l #2,%sp
	jbne .L282
	move.w %d3,%d0
	move.l -8(%a6),%d3
	move.l -4(%a6),%a2
	unlk %a6
	rts
	.even
	.globl	init
init:
	link.w %a6,#0
	move.w #127,-(%sp)
	move.w #239,-(%sp)
	move.l virtual,-(%sp)
	move.l 1672(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1792,-(%sp)
	move.l 2672(%a5),%a0
	jbsr (%a0)
	unlk %a6
	rts
	.even
	.globl	stop
stop:
	link.w %a6,#0
	move.w #1,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 2672(%a5),%a0
	jbsr (%a0)
	move.l 1676(%a5),%a0
	jbsr (%a0)
	addq.l #4,%sp
	jbsr GrayOff
	unlk %a6
	rts
	.even
	.globl	menu
menu:
	link.w %a6,#0
	movm.l #0x1f38,-(%sp)
	move.w 8(%a6),%a4
	move.w 10(%a6),%a3
	move.w 12(%a6),%d4
	move.w 14(%a6),%d5
	cmp.w #1,16(%a6)
	jbeq .L296
.L288:
	clr.w %d3
	cmp.w %d3,%d5
	jble .L295
.L293:
	clr.w -(%sp)
	move.w %a3,%d0
	add.w %d3,%d0
	move.w %d0,-(%sp)
	move.w %a4,%d1
	add.w %d4,%d1
	move.w %d1,-(%sp)
	move.w %d0,-(%sp)
	move.w %a4,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #1,%d3
	cmp.w %d3,%d5
	jbgt .L293
	jbra .L295
	.even
.L296:
	move.w %d4,%d0
	moveq.l #15,%d1
	lsr.w %d1,%d0
	add.w %d4,%d0
	asr.w #1,%d0
	moveq.l #80,%d1
	sub.w %d0,%d1
	move.w %d1,%a4
	move.w %d5,%d0
	moveq.l #15,%d1
	lsr.w %d1,%d0
	add.w %d5,%d0
	asr.w #1,%d0
	moveq.l #50,%d1
	sub.w %d0,%d1
	move.w %d1,%a3
	jbra .L288
	.even
.L295:
	move.w #1,-(%sp)
	move.w %a3,-(%sp)
	add.w %a4,%d4
	move.w %d4,%d0
	subq.w #2,%d0
	move.w %d0,-(%sp)
	move.w %a3,-(%sp)
	move.w %a4,%d6
	addq.w #1,%d6
	move.w %d6,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %a3,%d3
	add.w %d5,%d3
	move.w %d3,%a2
	subq.w #1,%a2
	move.w %a2,-(%sp)
	move.w %d4,%d5
	subq.w #1,%d5
	move.w %d5,-(%sp)
	move.w %a2,-(%sp)
	move.w %d6,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %d3,%d7
	subq.w #2,%d7
	move.w %d7,-(%sp)
	move.w %a4,-(%sp)
	move.w %a3,%d6
	addq.w #1,%d6
	move.w %d6,-(%sp)
	move.w %a4,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %d7,-(%sp)
	move.w %d5,-(%sp)
	move.w %d6,-(%sp)
	move.w %d5,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (40,%sp),%sp
	move.w #1,-(%sp)
	move.w %d3,-(%sp)
	move.w %d5,-(%sp)
	move.w %d3,-(%sp)
	move.w %a4,%d5
	addq.w #2,%d5
	move.w %d5,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %a2,-(%sp)
	move.w %d4,-(%sp)
	addq.w #1,%d6
	move.w %d6,-(%sp)
	move.w %d4,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %d6,-(%sp)
	subq.w #3,%d4
	move.w %d4,-(%sp)
	move.w %d6,-(%sp)
	move.w %d5,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	subq.w #3,%d3
	move.w %d3,-(%sp)
	move.w %d4,-(%sp)
	move.w %d3,-(%sp)
	move.w %d5,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (40,%sp),%sp
	move.w #1,-(%sp)
	move.w %d3,-(%sp)
	move.w %d5,-(%sp)
	move.w %d6,-(%sp)
	move.w %d5,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %d3,-(%sp)
	move.w %d4,-(%sp)
	move.w %d6,-(%sp)
	move.w %d4,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %a3,%d0
	addq.w #3,%d0
	move.w %d0,-(%sp)
	move.w %d4,-(%sp)
	move.w %d0,-(%sp)
	move.w %d5,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.w %d3,-(%sp)
	move.w %a4,%d0
	addq.w #3,%d0
	move.w %d0,-(%sp)
	move.w %d6,-(%sp)
	move.w %d0,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	movm.l -32(%a6),#0x1cf8
	unlk %a6
	rts
	.even
	.globl	choose
choose:
	link.w %a6,#0
	movm.l #0x1f38,-(%sp)
	move.w 8(%a6),%d7
	move.w 10(%a6),%a3
	move.w 12(%a6),%a2
	move.w 14(%a6),%d6
	move.w 18(%a6),%a4
	move.w 20(%a6),%d5
	jbsr stop
.L298:
	clr.w -(%sp)
	move.l 320(%a5),%a0
	jbsr (%a0)
	clr.w %d4
	addq.l #2,%sp
	tst.w %d6
	jbgt .L305
.L310:
	move.l 324(%a5),%a0
	jbsr (%a0)
	move.w %d0,%d3
	cmp.w #264,%d0
	jbeq .L307
	cmp.w #13,%d0
	jbeq .L307
	cmp.w #337,%d0
	jbeq .L307
	cmp.w #340,%d0
	jbne .L310
.L307:
	clr.w %d0
	cmp.w #264,%d3
	jbeq .L297
	cmp.w #13,%d3
	jbeq .L334
	clr.w %d4
	cmp.w %d4,%d6
	jbgt .L322
.L333:
	cmp.w #337,%d3
	jbeq .L335
.L323:
	cmp.w #340,%d3
	jbne .L298
	addq.w #1,%d5
	cmp.w %a4,%d5
	jble .L298
	moveq.l #1,%d5
	jbra .L298
	.even
.L335:
	subq.w #1,%d5
	tst.w %d5
	jbgt .L323
	move.w %a4,%d5
	jbra .L323
	.even
.L322:
	move.w #2,-(%sp)
	move.w %a3,%d0
	add.w %d4,%d0
	move.w %d5,%d1
	subq.w #1,%d1
	muls.w 16(%a6),%d1
	add.w %d1,%d0
	move.w %d0,-(%sp)
	move.w %d7,%d1
	add.w %a2,%d1
	move.w %d1,-(%sp)
	move.w %d0,-(%sp)
	move.w %d7,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #1,%d4
	cmp.w %d4,%d6
	jbgt .L322
	jbra .L333
	.even
.L334:
	clr.w %d4
	cmp.w %d4,%d6
	jbgt .L317
.L331:
	move.w %d5,%d0
	jbra .L297
	.even
.L317:
	move.w #2,-(%sp)
	move.w %a3,%d0
	add.w %d4,%d0
	move.w %d5,%d1
	subq.w #1,%d1
	muls.w 16(%a6),%d1
	add.w %d1,%d0
	move.w %d0,-(%sp)
	move.w %d7,%d1
	add.w %a2,%d1
	move.w %d1,-(%sp)
	move.w %d0,-(%sp)
	move.w %d7,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #1,%d4
	cmp.w %d4,%d6
	jbgt .L317
	jbra .L331
	.even
.L305:
	move.w #2,-(%sp)
	move.w %a3,%d0
	add.w %d4,%d0
	move.w %d5,%d1
	subq.w #1,%d1
	muls.w 16(%a6),%d1
	add.w %d1,%d0
	move.w %d0,-(%sp)
	move.w %d7,%d1
	add.w %a2,%d1
	move.w %d1,-(%sp)
	move.w %d0,-(%sp)
	move.w %d7,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #1,%d4
	cmp.w %d4,%d6
	jbgt .L305
	jbra .L310
	.even
.L297:
	movm.l -32(%a6),#0x1cf8
	unlk %a6
	rts
.LC5:
	.ascii "_\0"
.LC7:
	.ascii "%c_ \0"
.LC6:
	.ascii "_ \0"
	.even
	.globl	InputStr
InputStr:
	link.w %a6,#-200
	movm.l #0x1f38,-(%sp)
	move.w 8(%a6),%a4
	move.w 10(%a6),%a3
	move.w 12(%a6),%a2
	clr.w %d6
	clr.w %d0
	cmp.w %a2,%d6
	jbge .L350
.L341:
	move.w %d0,%a0
	add.l %a0,%a0
	lea name1,%a1
	move.w #32,(%a1,%a0.l)
	addq.w #1,%d0
	cmp.w %a2,%d0
	jblt .L341
.L350:
	jbsr stop
	pea .LC5
	move.l %a6,%d3
	add.l #-200,%d3
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	addq.l #8,%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w %a3,-(%sp)
	move.w %a4,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w %d6,%d4
	lsl.w #2,%d4
	add.w %d6,%d4
	move.w %d4,%d7
.L342:
	move.l 324(%a5),%a0
	jbsr (%a0)
	move.w %d0,%d5
	cmp.w #264,%d0
	jbeq .L342
	cmp.w #13,%d0
	jbeq .L343
	cmp.w #257,%d0
	jbeq .L351
.L347:
	cmp.w #257,%d5
	jbeq .L342
	cmp.w %a2,%d6
	jbge .L342
	move.w %d6,%a0
	add.l %a0,%a0
	lea name1,%a1
	move.w %d5,(%a1,%a0.l)
	move.w %d5,-(%sp)
	pea .LC7
	move.l %a6,%d3
	add.l #-200,%d3
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w %a3,-(%sp)
	move.w %d4,%d0
	add.w %d6,%d0
	add.w %a4,%d0
	move.w %d0,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #5,%d4
	addq.w #5,%d7
	addq.w #1,%d6
	jbra .L342
	.even
.L351:
	tst.w %d6
	jble .L347
	subq.w #5,%d4
	subq.w #5,%d7
	subq.w #1,%d6
	move.w %d6,%a0
	add.l %a0,%a0
	lea name1,%a1
	move.w #32,(%a1,%a0.l)
	pea .LC6
	move.l %a6,%d3
	add.l #-200,%d3
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	addq.l #8,%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w %a3,-(%sp)
	move.w %d7,%d0
	add.w %d6,%d0
	add.w %a4,%d0
	move.w %d0,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	jbra .L347
	.even
.L343:
	jbsr init
	movm.l -232(%a6),#0x1cf8
	unlk %a6
	rts
.LC10:
	.ascii "By: Jason Ho\0"
.LC11:
	.ascii "%c\0"
.LC12:
	.ascii "%d\0"
.LC8:
	.ascii "Congratulations!\0"
.LC9:
	.ascii "You made it into the hall of fame!\0"
.LC13:
	.ascii "HALL OF FAME\0"
.LC14:
	.ascii "Pilot\0"
.LC15:
	.ascii "Level\0"
	.even
	.globl	high
high:
	link.w %a6,#-200
	movm.l #0x1f30,-(%sp)
	moveq.l #5,%d5
	jbsr stop
	clr.w %d3
	move.w game_level,%d0
.L358:
	move.w %d3,%a0
	add.l %a0,%a0
	lea high_score,%a1
	cmp.w (%a1,%a0.l),%d0
	jbge .L397
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L358
.L354:
	cmp.w #4,%d5
	jble .L398
.L359:
	jbsr init
	tst.w game_lives
	jblt .L375
	jbsr pause
.L375:
	jbsr stop
	jbsr clrscr
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC10
	move.w #91,-(%sp)
	move.w #60,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w %d4
	lea (12,%sp),%sp
	clr.w %d7
	clr.w %d6
	clr.w %d5
.L385:
	clr.w %d3
	sub.l %a3,%a3
.L384:
	move.w %d3,%a0
	add.w %d5,%a0
	move.w %a0,%a0
	add.l %a0,%a0
	lea high_name,%a1
	move.w (%a1,%a0.l),-(%sp)
	pea .LC11
	lea (-200,%a6),%a2
	move.l %a2,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %a2,-(%sp)
	move.w %d6,%d0
	add.w %d4,%d0
	add.w #36,%d0
	move.w %d0,-(%sp)
	move.w %a3,%d0
	add.w %d3,%d0
	add.w #15,%d0
	move.w %d0,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	addq.w #5,%a3
	addq.w #1,%d3
	cmp.w #11,%d3
	jble .L384
	move.w %d4,%a0
	add.l %a0,%a0
	lea high_score,%a1
	move.w (%a1,%a0.l),-(%sp)
	pea .LC12
	move.l %a2,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %a2,-(%sp)
	move.w %d7,%d0
	add.w %d4,%d0
	add.w #36,%d0
	move.w %d0,-(%sp)
	move.w #120,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	add.w #9,%d7
	add.w #9,%d6
	add.w #12,%d5
	addq.w #1,%d4
	cmp.w #4,%d4
	jble .L385
	jbra .L399
	.even
.L398:
	jbsr clrscr
	move.w #1,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC8
	move.w #10,-(%sp)
	move.w #33,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC9
	move.w #22,-(%sp)
	move.w #23,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w %d4
	lea (24,%sp),%sp
	moveq.l #4,%d1
	sub.w %d5,%d1
	cmp.w %d4,%d1
	jble .L388
	clr.w %d0
.L369:
	clr.w %d3
.L368:
	move.w %d3,%a0
	sub.w %d0,%a0
	move.w %a0,%a0
	add.l %a0,%a0
	add.l #high_name,%a0
	move.w 72(%a0),96(%a0)
	addq.w #1,%d3
	cmp.w #11,%d3
	jble .L368
	moveq.l #4,%d2
	sub.w %d4,%d2
	move.w %d2,%a0
	lea high_score,%a2
	add.l %a0,%a0
	moveq.l #3,%d2
	sub.w %d4,%d2
	move.w %d2,%a1
	add.l %a1,%a1
	move.w (%a2,%a1.l),(%a2,%a0.l)
	add.w #12,%d0
	addq.w #1,%d4
	cmp.w %d4,%d1
	jbgt .L369
.L388:
	move.w #12,-(%sp)
	move.w #45,-(%sp)
	move.w #40,-(%sp)
	jbsr InputStr
	clr.w %d3
	addq.l #6,%sp
.L374:
	move.w %d5,%d0
	muls.w #12,%d0
	add.w %d3,%d0
	move.w %d0,%a2
	add.l %a2,%a2
	move.w %d3,%a0
	add.l %a0,%a0
	lea name1,%a3
	lea high_name,%a1
	move.w (%a3,%a0.l),(%a1,%a2.l)
	addq.w #1,%d3
	cmp.w #11,%d3
	jble .L374
	move.w %d5,%a0
	add.l %a0,%a0
	lea high_score,%a1
	move.w game_level,(%a1,%a0.l)
	jbra .L359
	.even
.L397:
	move.w %d3,%d5
	jbra .L354
	.even
.L399:
	move.w #2,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC13
	move.w #3,-(%sp)
	move.w #32,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC14
	move.w #24,-(%sp)
	move.w #15,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC15
	move.w #24,-(%sp)
	move.w #109,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (34,%sp),%sp
	move.w #1,-(%sp)
	move.w #32,-(%sp)
	move.w #138,-(%sp)
	move.w #32,-(%sp)
	move.w #15,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	jbsr init
	jbsr pause
	movm.l -228(%a6),#0xcf8
	unlk %a6
	rts
.LC16:
	.ascii "GAME OVER\0"
	.even
	.globl	quit
quit:
	link.w %a6,#0
	move.l %a2,-(%sp)
	jbsr stop
	jbsr GrayOn
	move.l #15663231,-(%sp)
	move.l __L_plane,-(%sp)
	move.l 1672(%a5),%a0
	jbsr (%a0)
	lea clrscr,%a2
	jbsr (%a2)
	move.w #2,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC16
	move.w #38,-(%sp)
	move.w #48,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC10
	move.w #50,-(%sp)
	move.w #62,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (28,%sp),%sp
	move.l #15663231,(%sp)
	move.l __D_plane,-(%sp)
	move.l 1672(%a5),%a0
	jbsr (%a0)
	jbsr (%a2)
	move.w #2,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC16
	move.w #36,-(%sp)
	move.w #46,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC10
	move.w #50,-(%sp)
	move.w #62,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (32,%sp),%sp
	clr.w -(%sp)
	move.l 320(%a5),%a0
	jbsr (%a0)
	addq.l #2,%sp
.L405:
	move.l 324(%a5),%a0
	jbsr (%a0)
	cmp.w #13,%d0
	jbeq .L402
	cmp.w #264,%d0
	jbne .L405
.L402:
	jbsr stop
	jbsr end
	move.l -4(%a6),%a2
	unlk %a6
	rts
	.even
.LC28:
	.byte	0
	.byte	0
	.byte	-97
	.byte	99
.LC17:
	.ascii "PLAY GAME\0"
.LC18:
	.ascii "OPTIONS\0"
.LC19:
	.ascii "HIGH SCORE\0"
.LC20:
	.ascii "HELP\0"
.LC21:
	.ascii "QUIT GAME\0"
.LC22:
	.ascii "Speed\0"
.LC23:
	.ascii "Enemy Explosion\0"
.LC24:
	.ascii "Item/Shield Display\0"
.LC25:
	.ascii "Contrast\0"
.LC31:
	.ascii "Up\0"
.LC32:
	.ascii "Down\0"
.LC29:
	.ascii "On\0"
.LC30:
	.ascii "Off\0"
.LC26:
	.ascii "Faster\0"
.LC27:
	.ascii "Slower\0"
.LC33:
	.ascii "[2nd] - Fire Bullets\0"
.LC34:
	.ascii "[LEFT] - Move Left\0"
.LC35:
	.ascii "[RIGHT] - Move Right\0"
.LC36:
	.ascii "[UP] - Activate Shields\0"
.LC37:
	.ascii "[DOWN] - Teleport to the other side\0"
.LC38:
	.ascii "[F1] - Cluster Bomb\0"
.LC39:
	.ascii "[F2] - Heat Seeker\0"
.LC40:
	.ascii "[F3] - Lasers\0"
.LC41:
	.ascii "[F4] - Disruption Field\0"
.LC42:
	.ascii "[F5] - Check Item Levels\0"
.LC43:
	.ascii "[HOME] - Quick Exit\0"
.LC44:
	.ascii "[ESC] - Main Menu/Quit Game\0"
	.even
	.globl	main_menu
main_menu:
	link.w %a6,#-4040
	movm.l #0x1e30,-(%sp)
	moveq.l #1,%d5
.L408:
	move.w #1,-(%sp)
	move.w #60,-(%sp)
	move.w #62,-(%sp)
	clr.w -(%sp)
	clr.w -(%sp)
	jbsr menu
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC17
	move.w #28,-(%sp)
	move.w #62,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC18
	move.w #38,-(%sp)
	move.w #67,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (32,%sp),%sp
	move.w #1,-(%sp)
	pea .LC19
	move.w #48,-(%sp)
	move.w #62,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC20
	move.w #58,-(%sp)
	move.w #72,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC21
	move.w #68,-(%sp)
	move.w #63,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w %d5,-(%sp)
	move.w #5,-(%sp)
	move.w #10,-(%sp)
	move.w #9,-(%sp)
	move.w #48,-(%sp)
	move.w #26,-(%sp)
	move.w #56,-(%sp)
	jbsr choose
	move.w %d0,%d5
	lea (44,%sp),%sp
	cmp.w #5,%d0
	jbhi .L409
	and.l #0xFFFF,%d0
	add.l %d0,%d0
	.set .LI442,.+2
	move.w .L442-.LI442.b(%pc,%d0.l),%d0
	jmp %pc@(2,%d0:w)
	.even
	.even
.L442:
	.word .L410-.L442
	.word .L409-.L442
	.word .L412-.L442
	.word .L434-.L442
	.word .L435-.L442
	.word .L441-.L442
	.even
.L410:
	clr.w %d0
	jbra .L407
	.even
.L409:
	moveq.l #1,%d0
	jbra .L407
	.even
.L412:
	pea 3840.w
	pea 19456.w
	move.l %a6,%d6
	add.l #-3840,%d6
	move.l %d6,-(%sp)
	move.l 2472(%a5),%a0
	jbsr (%a0)
	moveq.l #1,%d5
	lea (12,%sp),%sp
.L413:
	move.w #1,-(%sp)
	move.w #47,-(%sp)
	move.w #82,-(%sp)
	clr.w -(%sp)
	clr.w -(%sp)
	lea menu,%a2
	jbsr (%a2)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC22
	move.w #35,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC23
	move.w #44,-(%sp)
	move.w #53,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (32,%sp),%sp
	move.w #1,-(%sp)
	pea .LC24
	move.w #53,-(%sp)
	move.w #47,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC25
	move.w #62,-(%sp)
	move.w #66,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w %d5,-(%sp)
	move.w #4,-(%sp)
	move.w #9,-(%sp)
	move.w #9,-(%sp)
	move.w #70,-(%sp)
	move.w #33,-(%sp)
	move.w #45,-(%sp)
	lea choose,%a3
	jbsr (%a3)
	move.w %d0,%d5
	lea (34,%sp),%sp
	cmp.w #1,%d0
	jbeq .L415
	cmp.w #2,%d0
	jbeq .L446
	cmp.w #3,%d0
	jbeq .L447
	cmp.w #4,%d0
	jbne .L427
	moveq.l #1,%d5
.L428:
	move.w #1,-(%sp)
	move.w #29,-(%sp)
	move.w #32,-(%sp)
	move.w #20,-(%sp)
	move.w #50,-(%sp)
	jbsr menu
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC31
	move.w #44,-(%sp)
	move.w #77,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC32
	move.w #53,-(%sp)
	move.w #71,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (32,%sp),%sp
	move.w %d5,-(%sp)
	move.w #2,-(%sp)
	move.w #9,-(%sp)
	move.w #7,-(%sp)
	move.w #20,-(%sp)
	move.w #43,-(%sp)
	move.w #70,-(%sp)
	jbsr choose
	move.w %d0,%d5
	lea (14,%sp),%sp
	jbeq .L429
	cmp.w #1,%d0
	jbeq .L448
.L432:
	cmp.w #2,%d5
	jbne .L428
	move.l 2652(%a5),%a0
	jbsr (%a0)
	jbra .L428
	.even
.L448:
	move.l 2648(%a5),%a0
	jbsr (%a0)
	jbra .L432
	.even
.L429:
	moveq.l #4,%d5
	jbra .L413
	.even
.L427:
	moveq.l #2,%d5
	pea 3840.w
	move.l %d6,-(%sp)
.L445:
	pea 19456.w
	move.l 2472(%a5),%a0
	jbsr (%a0)
	lea (12,%sp),%sp
	jbra .L408
	.even
.L447:
	move.w #1,-(%sp)
	move.w #29,-(%sp)
	move.w #32,-(%sp)
	move.w #20,-(%sp)
	move.w #50,-(%sp)
	jbsr (%a2)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC28
	move.w #3,-(%sp)
	move.w #3,-(%sp)
	move.w #46,-(%sp)
	move.w #74,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC28
	move.w #3,-(%sp)
	move.w #3,-(%sp)
	move.w #55,-(%sp)
	move.w #74,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	lea (40,%sp),%sp
	move.w #1,-(%sp)
	pea .LC29
	move.w #44,-(%sp)
	move.w #80,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC30
	move.w #53,-(%sp)
	move.w #80,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	moveq.l #2,%d0
	sub.w item_show,%d0
	move.w %d0,-(%sp)
	move.w #2,-(%sp)
	move.w #9,-(%sp)
	move.w #3,-(%sp)
	move.w #2,-(%sp)
	move.w #45,-(%sp)
	move.w #73,-(%sp)
	jbsr (%a3)
	lea (34,%sp),%sp
	tst.w %d0
	jbeq .L426
	moveq.l #2,%d1
	sub.w %d0,%d1
	move.w %d1,item_show
.L426:
	moveq.l #3,%d5
	jbra .L413
	.even
.L446:
	move.w #1,-(%sp)
	move.w #29,-(%sp)
	move.w #32,-(%sp)
	move.w #20,-(%sp)
	move.w #50,-(%sp)
	jbsr (%a2)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC28
	move.w #3,-(%sp)
	move.w #3,-(%sp)
	move.w #46,-(%sp)
	move.w #74,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC28
	move.w #3,-(%sp)
	move.w #3,-(%sp)
	move.w #55,-(%sp)
	move.w #74,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	lea (40,%sp),%sp
	move.w #1,-(%sp)
	pea .LC29
	move.w #44,-(%sp)
	move.w #80,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC30
	move.w #53,-(%sp)
	move.w #80,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	moveq.l #2,%d0
	sub.w enemy_explosion_show,%d0
	move.w %d0,-(%sp)
	move.w #2,-(%sp)
	move.w #9,-(%sp)
	move.w #3,-(%sp)
	move.w #2,-(%sp)
	move.w #45,-(%sp)
	move.w #73,-(%sp)
	jbsr (%a3)
	lea (34,%sp),%sp
	tst.w %d0
	jbeq .L424
	moveq.l #2,%d1
	sub.w %d0,%d1
	move.w %d1,enemy_explosion_show
.L424:
	moveq.l #2,%d5
	jbra .L413
	.even
.L415:
	move.w #1,-(%sp)
	move.w #35,-(%sp)
	move.w #39,-(%sp)
	move.w #20,-(%sp)
	move.w #50,-(%sp)
	jbsr menu
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	lea (12,%sp),%sp
	move.w game_speed,%d0
	ext.l %d0
	divs.w #200,%d0
	moveq.l #99,%d1
	sub.w %d0,%d1
	move.w %d1,-(%sp)
	pea .LC12
	move.l %a6,%d3
	add.l #-4040,%d3
	move.l %d3,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d3,-(%sp)
	move.w #39,-(%sp)
	move.w #77,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #1,-(%sp)
	pea .LC26
	move.w #48,-(%sp)
	move.w #70,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC27
	move.w #57,-(%sp)
	move.w #68,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w %d5,-(%sp)
	move.w #2,-(%sp)
	move.w #9,-(%sp)
	move.w #7,-(%sp)
	move.w #27,-(%sp)
	move.w #47,-(%sp)
	move.w #67,-(%sp)
	jbsr choose
	move.w %d0,%d5
	lea (34,%sp),%sp
	jbeq .L416
	cmp.w #1,%d0
	jbeq .L449
.L419:
	cmp.w #2,%d5
	jbne .L415
	move.w game_speed,%d0
	add.w #200,%d0
	move.w %d0,game_speed
	cmp.w #19800,%d0
	jble .L415
	clr.w game_speed
	jbra .L415
	.even
.L449:
	move.w game_speed,%d0
	add.w #-200,%d0
	move.w %d0,game_speed
	jbge .L419
	move.w #19800,game_speed
	jbra .L419
	.even
.L416:
	moveq.l #1,%d5
	jbra .L413
	.even
.L434:
	pea 3840.w
	pea 19456.w
	move.l %a6,%d3
	add.l #-3840,%d3
	move.l %d3,-(%sp)
	move.l 2472(%a5),%a0
	jbsr (%a0)
	jbsr init
	move.w game_level,%d5
	move.w #-1,game_level
	jbsr high
	move.w %d5,game_level
	moveq.l #3,%d5
	pea 3840.w
	move.l %d3,-(%sp)
	pea 19456.w
	move.l 2472(%a5),%a0
	jbsr (%a0)
	lea (24,%sp),%sp
	jbra .L408
	.even
.L435:
	pea 3840.w
	pea 19456.w
	move.l %a6,%d3
	add.l #-3840,%d3
	move.l %d3,-(%sp)
	move.l 2472(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.w #95,-(%sp)
	move.w #138,-(%sp)
	move.w #2,-(%sp)
	move.w #11,-(%sp)
	jbsr menu
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC33
	move.w #9,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (34,%sp),%sp
	move.w #1,-(%sp)
	pea .LC34
	move.w #16,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC35
	move.w #23,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC36
	move.w #30,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC37
	move.w #37,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (40,%sp),%sp
	move.w #1,-(%sp)
	pea .LC38
	move.w #44,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC39
	move.w #51,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC40
	move.w #58,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC41
	move.w #65,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (40,%sp),%sp
	move.w #1,-(%sp)
	pea .LC42
	move.w #72,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC43
	move.w #79,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC44
	move.w #86,-(%sp)
	move.w #21,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (30,%sp),%sp
	cmp.w #13,%d5
	jbeq .L437
	cmp.w #264,%d5
	jbeq .L437
.L440:
	move.l 324(%a5),%a0
	jbsr (%a0)
	cmp.w #13,%d0
	jbeq .L437
	cmp.w #264,%d0
	jbne .L440
.L437:
	moveq.l #4,%d5
	pea 3840.w
	move.l %d3,-(%sp)
	jbra .L445
	.even
.L441:
	moveq.l #-1,%d0
.L407:
	movm.l -4064(%a6),#0xc78
	unlk %a6
	rts
.LC45:
	.ascii "Alien Invasion\0"
.LC46:
	.ascii "By: Ben Cherry\0"
.LC47:
	.ascii "v1.5\0"
	.even
	.globl	intro
intro:
	link.w %a6,#0
	movm.l #0x1820,-(%sp)
	clr.w %d3
	clr.w %d4
	jbsr GrayOn
.L464:
	move.l #15663231,-(%sp)
	move.l __L_plane,-(%sp)
	move.l 1672(%a5),%a0
	jbsr (%a0)
	lea clrscr,%a2
	jbsr (%a2)
	move.w #2,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC45
	move.w %d3,-(%sp)
	move.w #24,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC46
	move.w %d3,%d0
	add.w #12,%d0
	move.w %d0,-(%sp)
	move.w #62,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (28,%sp),%sp
	move.l #15663231,(%sp)
	move.l __D_plane,-(%sp)
	move.l 1672(%a5),%a0
	jbsr (%a0)
	jbsr (%a2)
	move.w #2,-(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC45
	moveq.l #90,%d0
	sub.w %d3,%d0
	move.w %d0,-(%sp)
	move.w #22,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	clr.w -(%sp)
	move.l 1596(%a5),%a0
	jbsr (%a0)
	move.w #1,-(%sp)
	pea .LC46
	moveq.l #81,%d0
	sub.w %d3,%d0
	move.w %d0,-(%sp)
	move.w #62,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (32,%sp),%sp
	move.w #1,-(%sp)
	pea .LC47
	move.w #93,-(%sp)
	move.w #143,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	cmp.w #46,%d3
	jbeq .L468
.L453:
	addq.w #1,%d3
	cmp.w #99,%d3
	jble .L464
	jbra .L469
	.even
.L468:
	cmp.w #13,%d4
	jbeq .L457
	cmp.w #264,%d4
	jbeq .L457
.L460:
	move.l 324(%a5),%a0
	jbsr (%a0)
	move.w %d0,%d4
	cmp.w #13,%d0
	jbeq .L457
	cmp.w #264,%d0
	jbne .L460
.L457:
	cmp.w #264,%d4
	jbeq .L470
.L461:
	cmp.w #13,%d4
	jbeq .L471
.L462:
	jbsr GrayOn
	jbra .L453
	.even
.L471:
	jbsr main_menu
	tst.w %d0
	jbgt .L462
	jbsr end
	jbra .L462
	.even
.L470:
	jbsr stop
	jbsr end
	jbra .L461
	.even
.L469:
	jbsr GrayOff
	movm.l -12(%a6),#0x418
	unlk %a6
	rts
	.even
	.globl	changeship
changeship:
	link.w %a6,#0
	movm.l #0x1038,-(%sp)
	clr.w %d3
.L481:
	move.w %d3,%a0
	lea (%a0,%a0.l),%a2
	lea enemy_alive,%a0
	cmp.w #1,(%a0,%a2.l)
	jbne .L475
	lea enemy_Vx,%a3
	lea rand,%a4
	jbsr (%a4)
	and.l #0xFFFF,%d0
	clr.l %d1
	clr.l %d2
	jbmi .L484
.L478:
	moveq.l #13,%d2
	asr.l %d2,%d0
	subq.w #2,%d0
	move.w %d0,(%a3,%a2.l)
	jbsr (%a4)
	and.l #0xFFFF,%d0
	clr.l %d1
	clr.l %d2
	jbmi .L485
.L479:
	moveq.l #13,%d2
	asr.l %d2,%d0
	subq.w #2,%d0
	lea enemy_Vy,%a0
	move.w %d0,(%a0,%a2.l)
	tst.w (%a3,%a2.l)
	jbne .L475
	tst.w %d0
	jbne .L475
	move.w #2,(%a3,%a2.l)
	jbra .L475
	.even
.L485:
	add.l #8191,%d0
	jbra .L479
	.even
.L484:
	add.l #8191,%d0
	jbra .L478
	.even
.L475:
	addq.w #1,%d3
	cmp.w #4,%d3
	jble .L481
	movm.l -16(%a6),#0x1c08
	unlk %a6
	rts
	.even
	.globl	reset
reset:
	link.w %a6,#0
	move.w #1,game_fire
	move.w #200,player_shield
	move.w #5,item_disruption
	move.w #5,item_heat
	move.w #5,item_cluster
	move.w #5,item_laser
	move.w #15,laser_level
	move.w #20,cluster_level
	move.w #3,heat_level
	move.w #1,heat_accuracy
	unlk %a6
	rts
	.even
	.globl	start
start:
	link.w %a6,#0
	move.l %a2,-(%sp)
	move.w #74,player_x
	clr.w player_explosion
	clr.w game_level
	move.w #3,game_lives
	move.w #-1,cluster_y
	clr.w cluster_radius
	clr.w cluster_press
	move.w #-1,heat_y
	move.w #-1,laser_y
	clr.w laser_press
	move.w #-2,laser_length
	move.w #101,upgrade_y
	clr.w player_invincible
	clr.w repeat1
	clr.w repeat2
	clr.w enemy_bullet_accuracy
	move.w #1,enemy_bullet_speed
	move.w #150,player_invincible_time
	clr.w %d0
.L492:
	move.w %d0,%a0
	add.l %a0,%a0
	lea enemy_alive,%a1
	clr.w (%a1,%a0.l)
	lea player_bullet_y,%a1
	clr.w (%a1,%a0.l)
	lea enemy_bullet_y,%a1
	move.w #101,(%a1,%a0.l)
	lea enemy_explosion,%a1
	clr.w (%a1,%a0.l)
	lea enemy_y,%a1
	move.w #3,(%a1,%a0.l)
	addq.w #1,%d0
	cmp.w #4,%d0
	jble .L492
	clr.w %d0
	move.w __calculator,%d2
.L499:
	move.w %d0,%a1
	lea (%a1,%a1.l),%a2
	move.w #129,%d1
	tst.w %d2
	jbne .L498
	moveq.l #101,%d1
.L498:
	lea enemy1_particles_y,%a0
	move.w %d1,(%a0,%a2.l)
	add.l %a1,%a1
	lea enemy1_particles_x,%a0
	move.w #-1,(%a0,%a1.l)
	addq.w #1,%d0
	cmp.w #29,%d0
	jble .L499
	jbsr reset
	jbsr changeship
	move.l -4(%a6),%a2
	unlk %a6
	rts
	.even
	.globl	drawship
drawship:
	link.w %a6,#0
	movm.l #0x1e30,-(%sp)
	move.w 8(%a6),%d5
	move.w 10(%a6),%d4
	move.w 12(%a6),%d6
	move.w %d6,%a3
	lea (%a3,%a3.l),%a2
	lea enemy_alive,%a0
	cmp.w #1,(%a0,%a2.l)
	jbne .L504
	cmp.w #1,%d5
	jbeq .L521
.L506:
	cmp.w #2,%d5
	jbeq .L522
.L507:
	cmp.w #2,%d5
	jbeq .L523
.L509:
	cmp.w #3,%d5
	jbeq .L524
.L511:
	cmp.w #4,%d5
	jbeq .L525
.L512:
	cmp.w #5,%d5
	jbeq .L526
.L513:
	cmp.w #5,%d5
	jbeq .L527
.L514:
	cmp.w #5,%d5
	jbeq .L528
.L516:
	cmp.w #6,%d5
	jbeq .L529
.L517:
	cmp.w #6,%d5
	jbeq .L530
.L518:
	cmp.w #7,%d5
	jbeq .L531
.L519:
	cmp.w #7,%d5
	jbne .L504
	move.w repeat1,%d0
	subq.w #1,%d0
	and.w #1,%d0
	jbne .L504
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy7+68,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L504
	.even
.L531:
	move.w repeat1,%d0
	and.w #1,%d0
	jbne .L519
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy7,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L519
	.even
.L530:
	move.w repeat1,%d0
	subq.w #1,%d0
	and.w #1,%d0
	jbne .L518
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy6+56,%a0
	moveq.l #14,%d2
	jbsr Sprite32
	jbra .L518
	.even
.L529:
	move.w repeat1,%d0
	and.w #1,%d0
	jbne .L517
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy6,%a0
	moveq.l #14,%d2
	jbsr Sprite32
	jbra .L517
	.even
.L528:
	cmp.w #3,%d4
	jbne .L516
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy5+136,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L516
	.even
.L527:
	cmp.w #2,%d4
	jbeq .L515
	cmp.w #4,%d4
	jbne .L514
.L515:
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy5+68,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L514
	.even
.L526:
	cmp.w #1,%d4
	jbne .L513
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy5,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L513
	.even
.L525:
	move.w %d4,%d2
	muls.w #68,%d2
	add.l #enemy4-68,%d2
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	move.l %d2,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L512
	.even
.L524:
	move.w %d6,%a0
	add.l %a0,%a0
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%d1
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy3,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L511
	.even
.L523:
	cmp.w #2,%d4
	jbeq .L510
	cmp.w #4,%d4
	jbne .L509
.L510:
	lea (%a3,%a3.l),%a1
	lea enemy_y,%a0
	move.w (%a0,%a1.l),%d1
	lea enemy_x,%a0
	move.w (%a0,%a1.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy2+68,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L509
	.even
.L522:
	cmp.w #1,%d4
	jbeq .L508
	cmp.w #3,%d4
	jbne .L507
.L508:
	lea enemy_y,%a0
	move.w (%a0,%a2.l),%d1
	lea enemy_x,%a0
	move.w (%a0,%a2.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy2,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L507
	.even
.L521:
	move.w %d4,%d2
	muls.w #68,%d2
	add.l #enemy1-68,%d2
	lea enemy_y,%a0
	move.w (%a0,%a2.l),%d1
	lea enemy_x,%a0
	move.w (%a0,%a2.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	move.l %d2,%a0
	moveq.l #17,%d2
	jbsr Sprite32
	jbra .L506
	.even
.L504:
	movm.l -24(%a6),#0xc78
	unlk %a6
	rts
	.even
	.globl	enemyshoot
enemyshoot:
	link.w %a6,#0
	move.l %a3,-(%sp)
	move.l %a2,-(%sp)
	clr.w %d1
.L540:
	move.w %d1,%a0
	lea (%a0,%a0.l),%a1
	lea enemy_alive,%a0
	cmp.w #1,(%a0,%a1.l)
	jbne .L535
	lea enemy_bullet_x,%a3
	lea enemy_x,%a0
	move.w (%a0,%a1.l),%d0
	addq.w #8,%d0
	move.w %d0,(%a3,%a1.l)
	lea enemy_bullet_y,%a2
	lea enemy_y,%a0
	move.w (%a0,%a1.l),%a0
	lea (17,%a0),%a0
	move.w %a0,(%a2,%a1.l)
	cmp.w #157,%d0
	jble .L538
	move.w #157,(%a3,%a1.l)
.L538:
	cmp.w #97,(%a2,%a1.l)
	jble .L535
	move.w #97,(%a2,%a1.l)
.L535:
	addq.w #1,%d1
	cmp.w #4,%d1
	jble .L540
	move.l (%sp)+,%a2
	move.l (%sp)+,%a3
	unlk %a6
	rts
	.even
	.globl	upgrade_test
upgrade_test:
	link.w %a6,#0
	clr.w %d1
	clr.w %d0
.L549:
	move.w %d0,%a0
	add.l %a0,%a0
	lea enemy_alive,%a1
	tst.w (%a1,%a0.l)
	jbeq .L546
	moveq.l #1,%d1
.L546:
	addq.w #1,%d0
	cmp.w #4,%d0
	jble .L549
	tst.w %d1
	jbne .L543
	cmp.w #100,upgrade_y.l
	jble .L543
	move.w 8(%a6),%a0
	add.l %a0,%a0
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%a1
	addq.w #6,%a1
	move.w %a1,upgrade_x
	lea enemy_y,%a1
	move.w (%a1,%a0.l),%a0
	lea (13,%a0),%a0
	move.w %a0,upgrade_y
.L543:
	unlk %a6
	rts
	.even
	.globl	upgrade_get
upgrade_get:
	link.w %a6,#0
	move.l %d3,-(%sp)
	move.w 8(%a6),%d2
	cmp.w #1,%d2
	jbeq .L575
.L554:
	cmp.w #2,%d2
	jbeq .L576
.L562:
	cmp.w #3,%d2
	jbeq .L577
.L563:
	cmp.w #4,%d2
	jbeq .L578
.L564:
	cmp.w #5,%d2
	jbeq .L579
.L565:
	cmp.w #6,%d2
	jbeq .L580
.L566:
	cmp.w #7,%d2
	jbeq .L581
.L567:
	cmp.w #8,%d2
	jbeq .L582
.L568:
	cmp.w #115,cluster_level.l
	jble .L569
	move.w #115,cluster_level
	move.w #200,player_shield
.L569:
	cmp.w #205,laser_level.l
	jble .L570
	move.w #205,laser_level
	move.w #200,player_shield
.L570:
	cmp.w #9,%d2
	jbne .L553
	move.w heat_level,%d0
	move.w %d0,%d1
	subq.w #1,%d1
	move.w %d1,heat_level
	cmp.w #1,%d0
	jbne .L572
	cmp.w #1,heat_accuracy.l
	jbeq .L583
.L572:
	tst.w heat_level
	jbne .L573
	cmp.w #2,heat_accuracy.l
	jbeq .L584
.L573:
	tst.w heat_level
	jbne .L553
	cmp.w #3,heat_accuracy.l
	jbne .L553
	move.w #1,heat_level
	move.w #200,player_shield
	jbra .L553
	.even
.L584:
	move.w #1,heat_level
	move.w #3,heat_accuracy
	jbra .L573
	.even
.L583:
	move.w #1,heat_level
	move.w #2,heat_accuracy
	jbra .L572
	.even
.L582:
	move.w game_level,%d0
	moveq.l #50,%d3
	ext.l %d0
	divs.w %d3,%d0
	move.w %d0,%d1
	lsl.w #3,%d1
	add.w %d0,%d1
	add.w %d0,%d1
	move.w laser_level,%d0
	add.w %d1,%d0
	add.w #10,%d0
	move.w %d0,laser_level
	jbra .L568
	.even
.L581:
	move.w game_level,%d1
	moveq.l #50,%d3
	ext.l %d1
	divs.w %d3,%d1
	move.w %d1,%d0
	lsl.w #2,%d0
	add.w %d1,%d0
	add.w cluster_level,%d0
	addq.w #5,%d0
	move.w %d0,cluster_level
	jbra .L567
	.even
.L580:
	move.w game_level,%d1
	moveq.l #50,%d3
	ext.l %d1
	divs.w %d3,%d1
	move.w item_laser,%d0
	add.w %d1,%d0
	addq.w #2,%d0
	move.w %d0,item_laser
	jbra .L566
	.even
.L579:
	move.w game_level,%d1
	moveq.l #50,%d3
	ext.l %d1
	divs.w %d3,%d1
	move.w item_cluster,%d0
	add.w %d1,%d0
	addq.w #2,%d0
	move.w %d0,item_cluster
	jbra .L565
	.even
.L578:
	move.w game_level,%d1
	moveq.l #50,%d3
	ext.l %d1
	divs.w %d3,%d1
	move.w item_heat,%d0
	add.w %d1,%d0
	addq.w #2,%d0
	move.w %d0,item_heat
	jbra .L564
	.even
.L577:
	move.w game_level,%d1
	moveq.l #50,%d3
	ext.l %d1
	divs.w %d3,%d1
	move.w item_disruption,%d0
	add.w %d1,%d0
	addq.w #2,%d0
	move.w %d0,item_disruption
	jbra .L563
	.even
.L576:
	move.w #200,player_shield
	jbra .L562
	.even
.L575:
	cmp.w #5,game_fire.l
	jbeq .L585
.L555:
	cmp.w #3,game_fire.l
	jbeq .L586
.L557:
	cmp.w #1,game_fire.l
	jbne .L554
	move.w #3,game_fire
	jbra .L554
	.even
.L586:
	cmp.w #19,game_level.l
	jble .L558
	move.w #5,game_fire
	jbra .L557
	.even
.L558:
	jbsr rand
	and.l #0xFFFF,%d0
	clr.l %d1
	clr.l %d2
	jbmi .L587
.L560:
	moveq.l #12,%d2
	asr.l %d2,%d0
	move.w %d0,%d2
	addq.w #2,%d2
	jbra .L557
	.even
.L587:
	add.l #4095,%d0
	jbra .L560
	.even
.L585:
	jbsr rand
	and.l #0xFFFF,%d0
	clr.l %d1
	clr.l %d2
	jbmi .L588
.L556:
	moveq.l #12,%d2
	asr.l %d2,%d0
	move.w %d0,%d2
	addq.w #2,%d2
	jbra .L555
	.even
.L588:
	add.l #4095,%d0
	jbra .L556
	.even
.L553:
	move.l -4(%a6),%d3
	unlk %a6
	rts
.LC48:
	.ascii "Level: %d\0"
.LC49:
	.ascii "x%d\0"
	.even
	.globl	paint
paint:
	link.w %a6,#-204
	movm.l #0x1c38,-(%sp)
	jbsr clrscr
	move.w game_level,-(%sp)
	pea .LC48
	move.l %a6,%d4
	add.l #-200,%d4
	move.l %d4,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d4,-(%sp)
	move.w #1,-(%sp)
	clr.w -(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.l #heart,-204(%a6)
	moveq.l #1,%d3
	move.l virtual,%a1
	lea heart,%a0
	moveq.l #8,%d2
	clr.w %d1
	move.w #130,%d0
	jbsr Sprite16
	cmp.w #1,item_show.l
	jbeq .L682
.L590:
	cmp.w #1,player_shield_on.l
	jbeq .L683
.L591:
	move.w game_lives,-(%sp)
	pea .LC49
	move.l %d4,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d4,-(%sp)
	move.w #1,-(%sp)
	move.w #148,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w upgrade_y,%d1
	cmp.w #100,%d1
	jble .L684
.L592:
	move.w player_invincible,%d1
	jble .L593
	move.w repeat2,%d0
	move.w %d0,%d2
	addq.w #1,%d2
	move.w %d2,repeat2
	cmp.w #3,%d2
	jble .L594
	move.w #1,repeat2
.L594:
	move.w %d1,%d0
	addq.w #1,%d0
	move.w %d0,player_invincible
	cmp.w player_invincible_time.l,%d0
	jble .L593
	clr.w player_invincible
	move.w #150,player_invincible_time
	clr.w repeat2
.L593:
	tst.w player_explosion
	jbne .L596
	move.w repeat2,%d0
	and.w #1,%d0
	jbeq .L685
.L596:
	move.w repeat1,%d1
	move.w %d1,%d2
	addq.w #1,%d2
	move.w %d2,repeat1
	cmp.w #6,%d2
	jbgt .L597
	move.w #1,enemy_frame
.L597:
	move.w %d1,%d0
	subq.w #6,%d0
	cmp.w #6,%d0
	jbhi .L598
	move.w #2,enemy_frame
.L598:
	move.w %d1,%d0
	add.w #-13,%d0
	cmp.w #6,%d0
	jbhi .L599
	move.w #3,enemy_frame
.L599:
	add.w #-20,%d1
	cmp.w #7,%d1
	jbhi .L600
	move.w #4,enemy_frame
.L600:
	cmp.w #28,%d2
	jble .L601
	clr.w repeat1
.L601:
	clr.w %d4
	clr.w %d5
.L609:
	move.w %d5,%a0
	lea enemy1_particles_x,%a4
	lea (%a0,%a0.l),%a2
	move.w (%a4,%a2.l),%d0
	jblt .L606
	lea enemy1_particles_y,%a3
	move.w (%a3,%a2.l),%d1
	cmp.w #100,%d1
	jbgt .L606
	cmp.w #160,%d0
	jbgt .L606
	tst.w %d1
	jblt .L606
	cmp.w #1,enemy_explosion_show.l
	jbeq .L686
.L607:
	move.w %d4,%a0
	add.l %a0,%a0
	lea enemy_particles_Vx,%a1
	move.w (%a1,%a0.l),%d0
	add.w %d0,(%a4,%a2.l)
	lea enemy_particles_Vy,%a1
	move.w (%a1,%a0.l),%d1
	add.w %d1,(%a3,%a2.l)
.L606:
	addq.w #1,%d4
	cmp.w #6,%d4
	sne %d0
	ext.w %d0
	and.w %d0,%d4
	addq.w #1,%d5
	cmp.w #29,%d5
	jble .L609
	move.w player_explosion,%d0
	jble .L610
	cmp.w #34,%d0
	jble .L687
.L611:
	move.w player_explosion,%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,player_explosion
	cmp.w #100,%d1
	jble .L610
	move.w game_lives,%d0
	subq.w #1,%d0
	move.w %d0,game_lives
	jblt .L688
.L613:
	jbsr reset
	move.w #74,player_x
	move.w #1,player_invincible
	clr.w player_explosion
.L610:
	tst.w player_explosion
	jble .L614
	clr.w %d5
.L620:
	move.w %d5,%a0
	lea (%a0,%a0.l),%a1
	lea player_particles_y,%a0
	move.w (%a0,%a1.l),%d1
	jblt .L617
	lea player_particles_x,%a0
	move.w (%a0,%a1.l),%d0
	jblt .L617
	cmp.w #160,%d0
	jble .L689
.L617:
	addq.w #1,%d5
	cmp.w #5,%d5
	jble .L620
	subq.w #2,player_particles_x
	subq.w #1,player_particles_x+2
	addq.w #1,player_particles_x+6
	addq.w #2,player_particles_x+8
	subq.w #3,player_particles_x+10
	subq.w #1,player_particles_y
	subq.w #1,player_particles_y+2
	subq.w #2,player_particles_y+4
	subq.w #2,player_particles_y+6
	subq.w #1,player_particles_y+8
	subq.w #1,player_particles_y+10
.L614:
	clr.w %d5
	sub.l %a3,%a3
	moveq.l #0,%d3
.L633:
	lea enemy_explosion,%a0
	cmp.w #1,(%a0,%d3.l)
	jbeq .L690
.L625:
	lea enemy_explosion,%a2
	move.w (%a2,%d3.l),%d0
	jbne .L691
.L623:
	addq.w #5,%a3
	addq.l #2,%d3
	addq.w #1,%d5
	cmp.w #4,%d5
	jble .L633
	clr.w %d5
.L638:
	move.w %d5,-(%sp)
	move.w enemy_frame,-(%sp)
	move.w enemy_ship,-(%sp)
	jbsr drawship
	addq.l #6,%sp
	addq.w #1,%d5
	cmp.w #4,%d5
	jble .L638
	clr.w %d5
	cmp.w game_fire.l,%d5
	jbge .L678
.L644:
	move.w %d5,%a0
	lea (%a0,%a0.l),%a1
	lea player_bullet_x,%a0
	move.w (%a0,%a1.l),%d0
	jbeq .L641
	lea player_bullet_y,%a0
	move.w (%a0,%a1.l),%d1
	jbne .L692
.L641:
	addq.w #1,%d5
	cmp.w game_fire.l,%d5
	jblt .L644
.L678:
	clr.w %d5
.L650:
	move.w %d5,%a0
	lea (%a0,%a0.l),%a1
	lea enemy_bullet_y,%a0
	move.w (%a0,%a1.l),%d1
	cmp.w #99,%d1
	jble .L693
.L647:
	addq.w #1,%d5
	cmp.w #4,%d5
	jble .L650
	move.w cluster_y,%d1
	jblt .L651
	moveq.l #1,%d3
	move.l virtual,%a1
	lea cluster,%a0
	moveq.l #7,%d2
	move.w cluster_x,%d0
	jbsr Sprite8
	move.w cluster_y,%d0
	subq.w #2,%d0
	move.w %d0,cluster_y
	jblt .L694
.L651:
	move.w cluster_radius,%d0
	jble .L653
	cmp.w #1,%d0
	jbeq .L695
.L654:
	move.w #1,-(%sp)
	pea .LC28
	move.w %d0,-(%sp)
	move.w %d0,-(%sp)
	move.w cluster_at_y,-(%sp)
	move.w cluster_x,%d0
	addq.w #4,%d0
	move.w %d0,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	move.w cluster_radius,%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,cluster_radius
	lea (14,%sp),%sp
	cmp.w cluster_level.l,%d1
	jble .L653
	clr.w cluster_radius
	clr.w cluster_press
.L653:
	move.w heat_y,%d1
	jblt .L656
	moveq.l #1,%d3
	move.l virtual,%a1
	lea heat,%a0
	moveq.l #5,%d2
	move.w heat_x,%d0
	jbsr Sprite8
	subq.w #2,heat_y
	move.w heat_target,%d3
	move.w %d3,%a0
	add.l %a0,%a0
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	addq.w #8,%d0
	move.w heat_x,%d2
	cmp.w %d2,%d0
	jbge .L657
	move.w repeat1,%d1
	ext.l %d1
	divs.w heat_level,%d1
	move.l %d1,%d0
	swap %d0
	tst.w %d0
	jbne .L657
	sub.w heat_accuracy,%d2
	move.w %d2,heat_x
.L657:
	move.w %d3,%a0
	add.l %a0,%a0
	lea enemy_x,%a1
	move.w (%a1,%a0.l),%d0
	addq.w #8,%d0
	move.w heat_x,%d2
	cmp.w %d2,%d0
	jble .L656
	move.w repeat1,%d1
	ext.l %d1
	divs.w heat_level,%d1
	move.l %d1,%d0
	swap %d0
	tst.w %d0
	jbne .L656
	add.w heat_accuracy,%d2
	move.w %d2,heat_x
.L656:
	move.w laser_y,%d0
	jblt .L659
	jbne .L660
	move.w laser_length,%d0
	jblt .L661
	cmp.w #87,%d0
	jbgt .L662
	move.w #1,-(%sp)
	clr.w -(%sp)
	move.w laser_x,-(%sp)
	move.w %d0,-(%sp)
.L681:
	move.w laser_x,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
.L664:
	subq.w #2,laser_length
.L660:
	move.w laser_y,%d1
	moveq.l #87,%d0
	sub.w %d1,%d0
	cmp.w laser_length.l,%d0
	jble .L696
.L665:
	move.w laser_y,%d1
	moveq.l #87,%d0
	sub.w %d1,%d0
	move.w laser_length,%d2
	cmp.w %d0,%d2
	jbge .L666
	tst.w %d1
	jble .L666
	move.w #1,-(%sp)
	move.w %d1,-(%sp)
	move.w laser_x,-(%sp)
	add.w %d2,%d1
	move.w %d1,-(%sp)
	move.w laser_x,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
.L666:
	move.w laser_y,%d0
	jbeq .L659
	subq.w #2,%d0
	move.w %d0,laser_y
	jbra .L659
	.even
.L696:
	move.w #1,-(%sp)
	move.w %d1,-(%sp)
	move.w laser_x,-(%sp)
	move.w #87,-(%sp)
	move.w laser_x,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	jbra .L665
	.even
.L662:
	move.w #1,-(%sp)
	clr.w -(%sp)
	move.w laser_x,-(%sp)
	move.w #87,-(%sp)
	jbra .L681
	.even
.L661:
	move.w #-1,laser_y
	jbra .L664
	.even
.L695:
	move.w cluster_y,cluster_at_y
	move.w #-1,cluster_y
	jbra .L654
	.even
.L694:
	clr.w cluster_press
	jbra .L651
	.even
.L693:
	lea enemy_bullet_x,%a0
	move.w (%a0,%a1.l),%d0
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy_bullet,%a0
	moveq.l #3,%d2
	jbsr Sprite8
	jbra .L647
	.even
.L692:
	moveq.l #1,%d3
	move.l virtual,%a1
	lea player_bullet,%a0
	moveq.l #4,%d2
	jbsr Sprite8
	jbra .L641
	.even
.L691:
	move.w #1,-(%sp)
	pea .LC28
	addq.w #5,%d0
	move.w %d0,-(%sp)
	move.w %d0,-(%sp)
	lea enemy_y,%a0
	move.w (%a0,%d3.l),%a0
	addq.w #8,%a0
	move.w %a0,-(%sp)
	lea enemy_x,%a0
	move.w (%a0,%d3.l),%a0
	lea (20,%a0),%a0
	move.w %a0,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	move.w (%a2,%d3.l),%d0
	move.w %d0,%d2
	addq.w #1,%d2
	move.w %d2,(%a2,%d3.l)
	lea (14,%sp),%sp
	cmp.w #15,%d2
	jble .L623
	clr.w (%a2,%d3.l)
	jbra .L623
	.even
.L690:
	move.w %a3,%d4
	add.w %d5,%d4
	move.w %d4,%d1
.L630:
	move.w %d4,%a1
	add.l %a1,%a1
	lea enemy_x,%a2
	lea enemy1_particles_x,%a0
	move.w (%a2,%d3.l),%a2
	addq.w #8,%a2
	move.w %a2,(%a0,%a1.l)
	lea enemy_y,%a2
	lea enemy1_particles_y,%a0
	move.w (%a2,%d3.l),%a2
	addq.w #8,%a2
	move.w %a2,(%a0,%a1.l)
	addq.w #1,%d4
	move.w %d1,%d0
	addq.w #6,%d0
	cmp.w %d4,%d0
	jbgt .L630
	jbra .L625
	.even
.L689:
	moveq.l #1,%d3
	move.l virtual,%a1
	lea particle,%a0
	moveq.l #5,%d2
	jbsr Sprite8
	jbra .L617
	.even
.L688:
	jbsr high
	jbsr quit
	jbra .L613
	.even
.L687:
	move.w #1,-(%sp)
	pea .LC28
	addq.w #5,%d0
	move.w %d0,-(%sp)
	move.w %d0,-(%sp)
	move.w #95,-(%sp)
	move.w player_x,%d2
	add.w #13,%d2
	move.w %d2,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	lea (14,%sp),%sp
	jbra .L611
	.even
.L686:
	moveq.l #1,%d3
	move.l virtual,%a1
	lea enemy_particle,%a0
	moveq.l #1,%d2
	jbsr Sprite8
	jbra .L607
	.even
.L685:
	move.l #player_ship,-204(%a6)
	moveq.l #1,%d3
	move.l virtual,%a1
	lea player_ship,%a0
	moveq.l #13,%d2
	moveq.l #87,%d1
	move.w player_x,%d0
	jbsr Sprite16
	jbra .L596
	.even
.L684:
	move.l #upgrade,-204(%a6)
	moveq.l #1,%d3
	move.l virtual,%a1
	lea upgrade,%a0
	moveq.l #7,%d2
	move.w upgrade_x,%d0
	jbsr Sprite16
	addq.w #1,upgrade_y
	jbra .L592
	.even
.L683:
	move.w repeat1,%d1
	moveq.l #3,%d2
	ext.l %d1
	divs.w %d2,%d1
	move.l %d1,%d0
	swap %d0
	tst.w %d0
	jbne .L591
	move.w #1,-(%sp)
	pea .LC28
	move.w #10,-(%sp)
	move.w #10,-(%sp)
	move.w #95,-(%sp)
	move.w player_x,%d0
	add.w #9,%d0
	move.w %d0,-(%sp)
	move.l 1608(%a5),%a0
	jbsr (%a0)
	lea (14,%sp),%sp
	jbra .L591
	.even
.L682:
	lea Sprite8,%a2
	move.l virtual,%a1
	lea cluster,%a0
	moveq.l #7,%d2
	clr.w %d1
	moveq.l #43,%d0
	jbsr (%a2)
	move.w item_cluster,-(%sp)
	pea .LC12
	move.l %d4,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d4,-(%sp)
	move.w #1,-(%sp)
	move.w #52,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.l virtual,%a1
	lea heat,%a0
	moveq.l #5,%d2
	moveq.l #1,%d1
	moveq.l #64,%d0
	jbsr (%a2)
	move.w item_heat,-(%sp)
	pea .LC12
	move.l %d4,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d4,-(%sp)
	move.w #1,-(%sp)
	move.w #73,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #1,-(%sp)
	move.w #5,-(%sp)
	move.w #88,-(%sp)
	move.w #1,-(%sp)
	move.w #88,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w item_laser,-(%sp)
	pea .LC12
	move.l %d4,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d4,-(%sp)
	move.w #1,-(%sp)
	move.w #93,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.l virtual,%a1
	lea disruption,%a0
	moveq.l #3,%d2
	moveq.l #2,%d1
	moveq.l #109,%d0
	jbsr (%a2)
	move.w item_disruption,-(%sp)
	pea .LC12
	move.l %d4,-(%sp)
	move.l 332(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.w #4,-(%sp)
	move.l %d4,-(%sp)
	move.w #1,-(%sp)
	move.w #117,-(%sp)
	move.l 1700(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	move.l virtual,%a1
	lea shield,%a0
	moveq.l #6,%d2
	moveq.l #94,%d1
	moveq.l #125,%d0
	jbsr Sprite32
	move.w #1,-(%sp)
	move.w #97,-(%sp)
	move.w player_shield,%d0
	moveq.l #13,%d2
	ext.l %d0
	divs.w %d2,%d0
	add.w #140,%d0
	move.w %d0,-(%sp)
	move.w #97,-(%sp)
	move.w #140,-(%sp)
	move.l 1692(%a5),%a0
	jbsr (%a0)
	lea (10,%sp),%sp
	jbra .L590
	.even
.L659:
	pea 3840.w
	move.l virtual,-(%sp)
	pea 19456.w
	move.l 2472(%a5),%a0
	jbsr (%a0)
	movm.l -228(%a6),#0x1c38
	unlk %a6
	rts
	.even
	.globl	end
end:
	link.w %a6,#0
	move.l virtual,-(%sp)
	move.l 652(%a5),%a0
	jbsr (%a0)
	addq.l #4,%sp
	jbsr __exit
	nop
.comm _nostub,2
.comm _ti89,2
.comm _ti92plus,2
.globl player_x
	.even
player_x:
	.space 2
	.even
.globl enemy1_particles_x
	.even
enemy1_particles_x:
	.space 60
	.even
.globl enemy1_particles_y
	.even
enemy1_particles_y:
	.space 60
	.even
.globl enemy_frame
	.even
enemy_frame:
	.space 2
	.even
.globl game_fire
	.even
game_fire:
	.space 2
	.even
.globl name1
	.even
name1:
	.space 24
	.even
