#include "STC15Fxxxx.H"
#include "intrins.h"
#include <stdio.h>
#include "ADC.h"
#include "delay.h"
#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
  sbit col_1=P3^4;
  sbit col_2=P3^5;
  sbit col_3=P3^6;
  sbit col_4=P3^7;	//矩阵键盘地址分配
  ////////////////
 sbit lcdrs=P0^7;
 sbit lcdrw=P0^6;
 sbit lcde=P0^5;	  //液晶显示地址分配
 sbit led_green=P0^1;
 sbit feng=P0^1;
 sbit PWM=P0^4;
uint time=0;
struct 
{
    float set_angle;//设定速度 
    float actual_angle;//实际速度
    float error;//偏差  
    float error_next;//上一个偏差  
    float error_last;//上上一个偏差 
	float error_add; 
    float kp,ki,kd;//定义比例，积分，微分参数  
    float umax,umin; 
}pid;
 ////////////////
 sfr ADC_LOW2=0xBE;   //ADC	low 2-bit result register;
#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟
#define uchar unsigned char 
#define  uint unsigned int

uint num=0;
uint count=0;
uchar pwm_count=0;
uchar num_py;
uchar py_count=0;
uint pwm_count1; 
bit z_bit1,z_bit2;z_bit3;
bit py_bit; 
bit pz_bit;
uint ad_ture=0; 
uchar keynum,flag=0;
uchar num1,num2,flag1,flag2,flag3,flag4;
uint jiao=0; 
uchar set_jiao=0;
uint t0_count=0;
bit display_bit=1,flag_bit=0,set_jiao_bit=1; 
uchar s1[3]={0x01,0x22,0x00}; 

uint s2[2]={0,0}; 
uint s3[5]={0,0,0,0,0}; 
uint kk=0; 
uint a,c;
char increment_angle;
char middle_value;
//////////////////
void InitADC();	//AD转换初始化
uchar GetADCResult(uchar ch);    //获取转换初值
uint  get_adc(uchar ch);
void keyscan(void);
void keyfind(unsigned char);
void keyprocess(void);
void delay_ad(uint t); 
void jiao_du();
//延时调用AD转换 ///////////////////////////////////////////////////液晶显示驱动程序
void delay(unsigned int z)
  //延时1ms
  {
     unsigned int x,y;
	 for(x = 110 ; x > 0 ; x--)
	   for(y=z;y>0;y--);
   }
   void write_com(uchar com)
   {
     lcdrs=0;
	 lcdrw=0;
   	  P2=com;
	  delay(1);
	  lcde=1;
	  delay(1);
   	lcde=0;
   }
   void write_dat(uchar dat)//写数据
   {
   lcdrs=1;
   lcdrw=0;
   P2=dat;
   delay(1);
   lcde=1;
   delay(1);
   lcde=0;
   }
   void init()
   {
   	  lcde=0;
	  write_com(0x38);
	  write_com(0x0c);
   	  write_com(0x06);
   	  write_com(0x01);
   	  write_com(0x80);
   }
	void display_char(uchar y,uchar x,uchar *p,uchar count)
	//显示字符串//
{
	uchar i=0;
	for(i=0;i<count;i++)
	{
	if(y==0)
	x|=0x80;
	else
	x|=0xc0;
	write_com(x);
	write_dat(*p);
	x++;p++;
	}
}
   void display_xy_su(uchar y,uchar x,uint dat)
 {
   uint shi,ge;
   if(y==0) x|=0x80;
   else x|=0xc0;
   write_com(x);
   shi=dat/10;
   ge=dat%10;
   write_dat(0x30+shi);
   write_dat(0x30+ge);
   write_dat(0xdf);
 }
   void display_xy_recive(uchar y,uchar x,uint dat)
 {
   uint shi,ge;
   if(y==0) x|=0x80;
   else x|=0xc0;
   write_com(x);
   shi=dat/10;
   ge=dat%10;
   write_dat(0x30+shi);
   write_dat(0x30+ge);
 }
  void display_xy_bai(uchar y,uchar x,uint dat)
  {
    uint bai,shi,ge;
   if(y==0) x|=0x80;
   else x|=0xc0;
   write_com(x);
   bai=dat/100;
   shi=dat%100/10;
   ge=dat%10;
   write_dat(0x30+bai);
   write_dat(0x2e);
   write_dat(0x30+shi);
   write_dat(0x30+ge);

  }
	//////////////////////////////////////////////////////AD转换为实际角度值
 void delay_ad(unsigned int t)
  {
  uint k=0;
  for(k=0;k<t;k++)
  {
  ad_ture=get_adc(0);
  jiao_du();
  display_xy_su(0,6,jiao);
  delay(100);
  }
  }
  uint get_adc(uchar ch)
  {
  uint adc_r;
  adc_r=GetADCResult(ch)*4+ADC_LOW2;
  adc_r=adc_r*33/100;
  delay(1);
  return(adc_r);

  }
 uchar GetADCResult(uchar ch)
{
	u16	adc=0;
	ADC_RES = 0;
	ADC_RESL = 0;
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	adc=ADC_RES;
	adc=(adc<<2)+ADC_RESL;
    return adc;                 //返回ADC结果
}
  void Init_ADC()
  {
  //P1=P1M1=P1M0=0xff;
 P1M0=0x01;
 P1M1=0x01;
  ADC_RES=0;
  ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
 delay(1) ;
  }


	 void jiao_du()
 {
 switch(ad_ture)
 {
 case 76:
 case 77:
 case 78:
 case 79:
 case 80:jiao=0;led_green=0;feng=0;break;
 case 81:jiao=0;led_green=0;feng=0;break; 
 case 82:jiao=0;break;
 case 83:
 case 84:jiao=1;break;
 case 85:
 case 86:jiao=1;break;
 case 87:
 case 88:jiao=2;break;
 case 89:
 case 90:jiao=2;break;
 case 91:
 case 92:jiao=3;break;
 case 93:
 case 94:jiao=3;break;
 case 95:
 case 96:jiao=4;break;
 case 97:
 case 98:
 case 99:jiao=5;break;
 case 100:
 case 101:jiao=5;break;
 case 102:
 case 103:jiao=6;break;
 case 104:
 case 105:
 case 106:jiao=7;break;
 case 107:
 case 108:
 case 109:jiao=8;break;
 case 110:
 case 111:jiao=8;break;
 case 112:
 case 113:jiao=9;break;
 case 114:
 case 115:
 case 116:jiao=10;break;
 case 117:
 case 118:jiao=10;break;
 case 119:
 case 120:
 case 121:jiao=11;break;
 case 122:
 case 123:
 case 124:jiao=12;break;
 case 125:
 case 126:
 case 127:jiao=12;break;
 case 128:
 case 129:
 case 130:jiao=13;break;
 case 131:
 case 132:
 case 133:jiao=13;break;
 case 134:
 case 135:
 case 136:jiao=14;break;
 case 137:
 case 138:
 case 139:jiao=14;break;
 case 140:
 case 141:jiao=15;break;
 case 142:
 case 143:
 case 144:
 case 145:jiao=16;break;
 case 146:
 case 147:
 case 148:jiao=17;break;
 case 149:
 case 150:
 case 151:jiao=18;break;
 case 152:
 case 153:
 case 154:jiao=19;break;
 case 155:
 case 156:
 case 157:jiao=20;break;
 case 158:
 case 159:
 case 160:
 case 161:jiao=21;break;
 case 162:
 case 163:
 case 164:jiao=21;break;
 case 165:
 case 166:
 case 167:jiao=22;break;
 case 168:
 case 169:jiao=22;break;
 case 170:
 case 171:
 case 172:jiao=23;break;
 case 173:
 case 174:
 case 175:jiao=24;break;
 case 176:
 case 177:
 case 178:jiao=24;break;
 case 179:
 case 180:
 case 181:jiao=25;break;
 case 182:
 case 183:
 case 184:
 case 185:jiao=26;break;
 case 186:
 case 187:
 case 188:
 case 189:jiao=27;break;
 case 190:
 case 191:
 case 192:
 case 193:jiao=27;break;
 case 194:
 case 195:
 case 196:
 case 197:jiao=28;break;
 case 198:
 case 199:
 case 200:
 case 201:jiao=29;break;
 case 202:
 case 203:
 case 204:jiao=30;break;
 case 205:
 case 206:
 case 207:jiao=31;break;
 case 208:
 case 209:
 case 210:jiao=31;break;
 case 211:
 case 212:
 case 213:
 case 214:jiao=32;break;
 case 215:
 case 216:
 case 217:jiao=32;break;
 case 218:
 case 219:
 case 220:
 case 221:jiao=33;break;
 case 222:
 case 223:
 case 224:
 case 225:jiao=34;break;
 case 226:
 case 227:
 case 228:jiao=35;break;
 case 229:
 case 230:
 case 231:
 case 232:jiao=36;break;
 case 233:
 case 234:
 case 235:
 case 236:
 case 237:jiao=37;break;
 case 238:
 case 239:jiao=37;break;
 case 240:
 case 241:
 case 242:
 case 243:jiao=38;break;
 case 244:
 case 245:
 case 246:jiao=38;break;
 case 247:
 case 248:
 case 249:
 case 250:jiao=39;break;
 case 251:
 case 252:
 case 253:
 case 254:jiao=39;break;
 case 255:
 case 256:
 case 257:jiao=40;break;
 case 258:
 case 259:jiao=40;break;
 case 260:
 case 261:
 case 262:
 case 263:jiao=41;led_green=0;feng=0;break;
 case 264:
 case 265:
 case 266:
 case 267:jiao=42;led_green=0;feng=0;break;
 case 268:
 case 269:
 case 270:
 case 271:
 case 272:jiao=43;led_green=0;feng=0;break;
 case 273:
 case 274:
 case 275:
 case 276:jiao=44;led_green=0;feng=0;break;
 case 277:
 case 278:
 case 279:jiao=45;led_green=0;feng=0;break;
 case 280:
 case 281:
 case 282:
 case 283:
 case 284:jiao=46;break;
 case 285:
 case 286:
 case 287:
 case 288:jiao=47;break;
 case 289:
 case 290:
 case 291:
 case 292:
 case 293:jiao=48;break;
 case 294:
 case 295:
 case 296:
 case 297:
 case 298:jiao=49;break;
 case 299:
 case 300:
 case 301:
 case 302:
 case 303:jiao=50;break;
 case 304:
 case 305:
 case 306:
 case 307:
 case 308:jiao=51;break;
 case 309:
 case 310:
 case 311:
 case 312:
 case 313:jiao=52;break;
 case 314:
 case 315:
 case 316:
 case 317:
 case 318:jiao=53;break;
 case 319:
 case 320:
 case 321:
 case 322:
 case 323:jiao=54;break;
 case 324:
 case 325:
 case 326:
 case 327:
 case 328:jiao=55;break;
 case 329:
 case 330:
 case 331:jiao=56;break;
 case 332:
 case 333:
 case 334:jiao=57;break;
 case 335:
 case 336:
 case 337:jiao=58;break;
 case 338:
 case 339:
 case 340:jiao=59;break;
 case 341:
 case 342:jiao=60;break;
 case 343:
 case 344:
 case 345:
 case 346:
 case 347:jiao=60;break;

}
 }
////////////////////////////////////////////////////////////给定值
  void keyscan()
  {

	
  col_1=0;col_2=0;col_3=0;col_4=0;//高位置0检测低位（列）有没有按下
  if((P3&0x0f)!=0x0f)
  {
  col_1=0;col_2=1;col_3=1;col_4=1;keyfind(0);
  col_1=1;col_2=0;col_3=1;col_4=1;keyfind(4);
  col_1=1;col_2=1;col_3=0;col_4=1;keyfind(8);
  col_1=1;col_2=1;col_3=1;col_4=0;keyfind(12);
  col_1=0;col_2=0;col_3=0;col_4=0;flag++;
  while(((P3&0x0f)!=0x0f));//等待按键弹回跳出这个if语句，按一次flag=1依次递增
  }
  }

	
  void keyfind(unsigned char col_dat)
  {
  unsigned char temp;
  temp=(~P3)&0x0f;
  switch(temp)
  {
  case 1:keynum=1+col_dat;break;
   case 2:keynum=2+col_dat;break;
    case 4:keynum=3+col_dat;break;
	 case 8:keynum=4+col_dat;break;
  }
  }
  void keyprocess(void)
  {
  switch(keynum)
  {
  case 1:if(flag==1)num1=7;
  if(flag==2)num2=7;break;
   case 2:if(flag==1)num1=4;
  if(flag==2)num2=4;break;
   case 3:if(flag==1)num1=1;
  if(flag==2)num2=1;break;
   case 5:if(flag==1)num1=8;
  if(flag==2)num2=8;break;
   case 6:if(flag==1)num1=5;
  if(flag==2)num2=5;break;
   case 7:if(flag==1)num1=2;
  if(flag==2)num2=2;break;
   case 8:if(flag==1)num1=0;
  if(flag==2)num2=0;break;
   case 9:if(flag==1)num1=9;
  if(flag==2)num2=9;break;
   case 10:if(flag==1)num1=6;
  if(flag==2)num2=6;break;
   case 11:if(flag==1)num1=3;
  if(flag==2)num2=3;break;
  case 4:flag1=1;break;//清除键
  case 12:flag=3;break;//确认键
  case 13:flag=4;num=1;break;
  case 14:flag=5;num=1;break;
  case 15:flag=6;num=1;break;
  case 16:flag=7;num=1;break;
  }

  keynum=0;
  if(flag1==1)//取消键
  {
  z_bit1=1;z_bit2=0;z_bit3=0;//pwm_count=10;pwm_count1=10;
  num1=0;num2=0;num=0;
  feng=0;led_green=0;num_py=0;
  flag=0;flag1=0;flag2=0;flag3=0;flag4=0;
  }
	
	
  if(flag==1||flag3==1)//取消键按下，flag3=1
  {
  flag3=1;
  display_xy_recive(1,4,num);
  delay(1);
  }
  if(flag==2||flag4==1)//输入两位数，flag4=1
  {
  flag4=1;
  display_xy_recive(1,4,num);
  delay(1);
  }
  	num=num1*10+num2;////////////////////////给定值
  if(num>60)py_bit=1;
  else py_bit=0;
  if(flag==3) //按下确认键
  {
  display_xy_recive(1,4,num);
	} }	
	///////////////////////////////////////////////////////PID运算
	void pid_init()
{
    pid.set_angle = 0.0;
    pid.actual_angle = 0.0;
    pid.error = 0.0;
    pid.error_next = 0.0;
    pid.error_last = 0.0;
	pid.error_add=0.0;
	pid.umax=60;
    pid.kp = 0.15;
    pid.Ti = 1.37;
    pid.Td = 1.97;
}
void pid_realise()//实现pid  
{
	int gex;
    pid.set_angle = num;//设置目标角度  
    pid.error = pid.set_angle - jiao;	  //设定-实际 
	if(pid.actual_angle>pid.umax)
	{
		if(pid.error>30)
		{
			gex=0;	
		}
		else
		{
			gex=1;
			if(pid.error<0)
			{
				pid.error_add+=pid.error;
			}
		}
	} 
   middle_value = pid.kp*pid.error+gex*pid.ki*pid.error_add+pid.kd*(pid.error-pid.error_next);//增量计算公式  
		increment_angle += middle_value;
    pid.error_next = pid.error;
  
}
////////////////////////////////////////////////PWM输出
void tim0() interrupt 1
{ 
TR0=0;//赋初值时，关闭定时器
 
TH0=0xff;//(65536-10)/256;//赋初值定时
 
TL0=0xf7;//(65536-10)%256;//0.01ms
 
TR0=1;//打开定时器
time++;
if(time>=100) //1khz
time=0;
if(time<=middle_value) 
PWM=1; 
else PWM=0; 
}
//////////////////////////////////////////////////////////////主函数
 void main(void)
  {
		
		uchar t;
		 PWM=0;
		TMOD=0x01;//定时器0工作方式1
    TH0=0xff;//(65536-10)/256;//赋初值定时
    TL0=0xf7;//(65536-10)%256;//0.01ms
    ET0=1;//开定时器0中断
    TR0=1;//启动定时器0
   P0M1=0x00;
   P0M0=0x10;    //PWM输出
   P1M0=0xe1;
   P1M1=0x01;	//AD转换
 
  init();
   TMOD=0x21;
   TH1=0xfd;
   TL1=0xfd;
   TR1=1;
   SM0=0;
   SM1=1;
   REN=1;
   EA=1;
   ES=1;
  TH0=0xff;TL0=0Xa3;EA=1;ET0=1;TR0=1;P1=0xff;
  display_bit=1;pz_bit=0;
  delay(10000);
  while(1)
  {
  //AD部分//
  for(t=0;t<5;t++)
  {
  s3[t]=get_adc(0);
  }
  ad_ture=(s3[0]+s3[1]+s3[2]+s3[3]+s3[4])/5;
  //角度//
  display_char(0,0,"Angle:",6);jiao_du();
  if(((jiao-num)<5)||((num-jiao)<5)){led_green=1;feng=1;} else {led_green=0;feng=0;}
 display_xy_su(0,6,jiao);
  //显示角度值
	 display_char(1,0,"Set:",4);
  keyscan();
  keyprocess();
 display_xy_su(1,4,num);//设定的角度值
  pid_realise();
	 
  }
  }
 