// C Source File
// Created 1/26/2005; 5:10:21 PM

#include <tigcclib.h>
#include "stacks.h"
#include "extgraph.h"

struct b;
typedef struct b {
	int x, y;
	int xo, yo;
	int w,h;
	void* sprite;
	void (*behavior)(struct b*);
} BlockT, *BlockTP;

typedef BlockT DataType;

typedef struct elem {
	DataType value;
	struct elem *next;
} ElemT, *ElemTP;

typedef struct {
	ElemTP top;
} StackT, *StackTP;

StackTP construct(void);
void destruct(StackTP*);
int push(StackTP, DataType);
DataType pop(StackTP);
void clear(StackTP);

StackTP construct(void) {
	StackTP s;
	if((s = malloc(sizeof(StackT))) == NULL)
		return NULL;
	s->top = NULL;
	return s;
}

void destruct(StackTP *this) {
	clear(*this);
	free(*this);
	*this = NULL;
}

int push(StackTP this, DataType d) {
	ElemTP aux;
	if((aux = malloc(sizeof(ElemT))) == NULL)
		return 0;
	aux->next = this->top;
	aux->value = d;
	this->top = aux;
	return 1;
}

DataType pop(StackTP this){
	ElemTP aux = this->top;
	this->top = aux->next;
	DataType n = aux->value;
	free(aux);
	return n;
}

void clear(StackTP this) {
	while(this->top != NULL)pop(this);
}

void upAndDown(BlockTP this) {
	if(--this->yo < 0) {
		this->yo = 8;
		this->y--;
	}
}

void leftAndRight(BlockTP this) {
	if(++this->xo > 8) {
		this->xo = 0;
		this->x++;
	}
}

// Main Function
void _main(void)
{
	clrscr();
	
	INT_HANDLER int_1 = GetIntVec(AUTO_INT_1);
	INT_HANDLER int_5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5, DUMMY_HANDLER);

//	GrayOn():

	StackTP s = construct();
	int x = 0, y = 0, xo = 0, yo = 0;
	BlockT blockTemplate[3];
	blockTemplate[1].x = blockTemplate[1].y = blockTemplate[1].xo = blockTemplate[1].yo = 0;
	blockTemplate[1].w = blockTemplate[1].h = 8;
	blockTemplate[1].sprite = block;
	blockTemplate[1].behavior = upAndDown;
	blockTemplate[2].x = blockTemplate[2].y = blockTemplate[2].xo = blockTemplate[2].yo = 0;
	blockTemplate[2].w = blockTemplate[2].h = 8;
	blockTemplate[2].sprite = block;
	blockTemplate[2].behavior = leftAndRight;


	while(!_keytest(RR_ESC)) {
		clrscr();
		int row, col;
		for(row = y; row < y + (LCD_HEIGHT / 8); row++) {
			for(col = x; col < x + (LCD_WIDTH / 8); col++) {
				if(m1[row][col] < 100)
					Sprite8_XOR((col-x)*8 + xo,(row-y)*8 + yo, 8, block, LCD_MEM);
				else if(m1[row][col] > 100) {
					BlockTP temp = malloc(sizeof(BlockT));
					*temp = blockTemplate[m1[row][col]-100];
					temp->x = col;
					temp->y = row;
					push(s,*temp);
					m1[row][col] = 0;
				}
			}
		}
		ElemTP aux;
		for(aux = s->top; aux != NULL; aux = aux->next) {
			Sprite8_XOR((aux->value.x - x)*8 + xo + aux->value.xo,(aux->value.y - y)*8 + yo + aux->value.yo,
									aux->value.h,(char*)aux->value.sprite,LCD_MEM);
			aux->value.behavior(&(aux->value));
		}
	}


//	GrayOff();

	SetIntVec(AUTO_INT_1, int_1);
	SetIntVec(AUTO_INT_5, int_5);
}
