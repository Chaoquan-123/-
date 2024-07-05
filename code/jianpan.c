#include "Keyboard.h"
#include "delay.h"

u8 key_board(void)
{		
	unsigned char temp,num;

	num = 0xff;
	KEY_PORT=0xef;
	temp=KEY_PORT;
	temp=temp&0x0f;
	while(temp!=0x0f)
	{
		Delay_ms(10); //延时消抖
		temp=KEY_PORT;
		temp=temp&0x0f;
		while(temp!=0x0f)
		{
			temp=KEY_PORT;
			switch(temp)
			{
				case 0xee : num='1';
				break;
				case 0xed : num='2';
				break;
				case 0xeb : num='3';
				break;
				case 0xe7 : num='+';
				break;
			}
			while(temp!=0x0f)
			{
				temp=KEY_PORT;
				temp=temp&0x0f;
			}
			return num;
		}
	}

	KEY_PORT=0xdf;
	temp=KEY_PORT;
	temp=temp&0x0f;
			while(temp!=0x0f)
	{
		Delay_ms(10); //延时消抖
		temp=KEY_PORT;
		temp=temp&0x0f;
			while(temp!=0x0f)
		{
			temp=KEY_PORT;
		switch(temp)
			{
					case 0xde : num='4';
					break;
					case 0xdd : num='5';
					break;
					case 0xdb : num='6';
					break;
					case 0xd7 : num='-';
					break;
			}
			while(temp!=0x0f)
			{
				temp=KEY_PORT;
				temp=temp&0x0f;
			}
			return num;
		}
	}

	KEY_PORT=0xbf;
	temp=KEY_PORT;
	temp=temp&0x0f;
	while(temp!=0x0f)
	{
		Delay_ms(10); //延时消抖
		temp=KEY_PORT;
		temp=temp&0x0f;
			while(temp!=0x0f)
		{
			temp=KEY_PORT;
		switch(temp)
			{
				case 0xbe : num='7';
				break;
				case 0xbd : num='8';
				break;
				case 0xbb : num='9';
				break;
				case 0xb7 : num='*';
				break;
			}
			while(temp!=0x0f)
			{
				temp=KEY_PORT;
				temp=temp&0x0f;
			}
			return num;
		}
	}

	KEY_PORT=0x7f;
	temp=KEY_PORT;
	temp=temp&0x0f;
	while(temp!=0x0f)
	{
		Delay_ms(10); //延时消抖
		temp=KEY_PORT;
		temp=temp&0x0f;
			while(temp!=0x0f)
		{
			temp=KEY_PORT;
		switch(temp)
			{
				case 0x7e : num='.';
				break;
				case 0x7d : num='0';
				break;
				case 0x7b : num='=';
				break;
				case 0x77 : num='/';
				break;
			}
			while(temp!=0x0f)
			{
				temp=KEY_PORT;
				temp=temp&0x0f;
			}
			return num;
		}
	}
	return 0;//没有按下 
}