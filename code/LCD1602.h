#include "stc15fxxxx.h"

#ifndef _LCD1602__H
#define	_LCD1602__H



void LCD1602Init(void);
void LcdWriteDate(unsigned char date);
void LcdWriteCom(unsigned char com);
bit LcdBusy();
void LCD(void);
#endif

