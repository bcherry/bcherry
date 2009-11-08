/* ######################################################################## */
/* #                               Orage GL                               # */
/* #                              Version 1.1                             # */
/* #                              Prototypes                              # */
/* #                          par Vincent Corfdir                         # */
/* ######################################################################## */

/* Oppérations Pixels */
void PxlOn(int x, int y, void *plan);
void PxlOff(int x, int y, void *plan);
void PxlChange(int x, int y, void *plan);
/* Oppérations Lignes */
void Horizontal(int y, void *plan);
void Vertical(int x, void *plan);
/* Touches */
void GL_INIT_ROWREAD(int par);
int GL_ESCpressed(void);
int GL_LEFTpressed(void);
int GL_RIGHTpressed(void);
int GL_UPpressed(void);
int GL_DOWNpressed(void);
/* Sprites */

//8 bits
void GL_Sprite8_XOR(int x, int y, unsigned char sprite[], void *plan);
void GL_Sprite8_OR(int x, int y, unsigned char sprite[], void *plan);
void GL_Sprite8_AND(int x, int y, unsigned char sprite[], void *plan);
void GL_xSprite8_XOR(int x, int y, unsigned char sprite[], int vtaille, void *plan);
void GL_xSprite8_OR(int x, int y, unsigned char sprite[], int vtaille, void *plan);
void GL_xSprite8_AND(int x, int y, unsigned char sprite[], int vtaille, void *plan);
void GL_SpriteH8_XOR(int x, int y, unsigned char sprite[], void *plan);
void GL_SpriteH8_OR(int x, int y, unsigned char sprite[], void *plan);
void GL_SpriteH8_AND(int x, int y, unsigned char sprite[], void *plan);
void GL_xSpriteH8_XOR(int x, int y, unsigned char sprite[], int vtaille, void *plan);
void GL_xSpriteH8_OR(int x, int y, unsigned char sprite[], int vtaille, void *plan);
void GL_xSpriteH8_AND(int x, int y, unsigned char sprite[], int vtaille, void *plan);

//16 bits
void GL_Sprite16_XOR(int x, int y, unsigned int sprite[], void *plan);
void GL_Sprite16_OR(int x, int y, unsigned int sprite[], void *plan);
void GL_Sprite16_AND(int x, int y, unsigned int sprite[], void *plan);
void GL_xSprite16_XOR(int x, int y, unsigned int sprite[], int vtaille, void *plan);
void GL_xSprite16_OR(int x, int y, unsigned int sprite[], int vtaille, void *plan);
void GL_xSprite16_AND(int x, int y, unsigned int sprite[], int vtaille, void *plan);
void GL_SpriteH16_XOR(int x, int y, unsigned int sprite[], void *plan);
void GL_SpriteH16_OR(int x, int y, unsigned int sprite[], void *plan);
void GL_SpriteH16_AND(int x, int y, unsigned int sprite[], void *plan);
void GL_xSpriteH16_XOR(int x, int y, unsigned int sprite[], int vtaille, void *plan);
void GL_xSpriteH16_OR(int x, int y, unsigned int sprite[], int vtaille, void *plan);
void GL_xSpriteH16_AND(int x, int y, unsigned int sprite[], int vtaille, void *plan);

//32 bits
void GL_Sprite32_XOR(int x, int y, unsigned long sprite[], void *plan);
void GL_Sprite32_OR(int x, int y, unsigned long sprite[], void *plan);
void GL_Sprite32_AND(int x, int y, unsigned long sprite[], void *plan);
void GL_xSprite32_XOR(int x, int y, unsigned long sprite[], int vtaille, void *plan);
void GL_xSprite32_OR(int x, int y, unsigned long sprite[], int vtaille, void *plan);
void GL_xSprite32_AND(int x, int y, unsigned long sprite[], int vtaille, void *plan);
void GL_SpriteH32_XOR(int x, int y, unsigned long sprite[], void *plan);
void GL_SpriteH32_OR(int x, int y, unsigned long sprite[], void *plan);
void GL_SpriteH32_AND(int x, int y, unsigned long sprite[], void *plan);
void GL_xSpriteH32_XOR(int x, int y, unsigned long sprite[], int vtaille, void *plan);
void GL_xSpriteH32_OR(int x, int y, unsigned long sprite[], int vtaille, void *plan);
void GL_xSpriteH32_AND(int x, int y, unsigned long sprite[], int vtaille, void *plan);

/* Ralentissement */
void sleep(unsigned int i);