	.file	"right-reg.c"
#NO_APP
/* Include Patch: __not_reg_relative */
	.macro __save_the_stack_pointer
		move.l %sp,__save__sp__
	.endm
	.data
tigcc_compiled.:
	.set _A_LINE,0xA000
	.globl	Image_C0
	.data
	.even
Image_C0:
	.word	1024
	.word	3008
	.word	1984
	.word	3232
	.word	3232
	.word	2880
	.word	960
	.word	1824
	.word	1213
	.word	1471
	.word	972
	.word	1279
	.word	208
	.word	576
	.word	64
	.word	328
	.word	1240
	.word	2256
	.word	2720
	.word	6144
	.word	9216
	.word	6464
	.word	-28320
	.word	-22528
	.word	2224
	.word	15608
	.globl	Image_C1
	.even
Image_C1:
	.word	1728
	.word	4064
	.word	3264
	.word	2144
	.word	2144
	.word	3296
	.word	3312
	.word	3832
	.word	1655
	.word	1620
	.word	1279
	.word	511
	.word	1120
	.word	1264
	.word	760
	.word	760
	.word	1904
	.word	3944
	.word	464
	.word	5856
	.word	15552
	.word	31200
	.word	-5984
	.word	-9760
	.word	13512
	.word	15608
