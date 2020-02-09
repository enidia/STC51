#include"stc15.h"
#include<stdlib.h>
#include <intrins.h>
#define uchar unsigned char;
#define uint unsigned int;

sbit outsideBlue = P1^3;//这些都是LED灯的引脚
sbit outsideYellow = P1^4;
sbit outsideGreen = P1^5;
sbit outsidewhite = P1^6;
sbit insideGreen = P1^7;
sbit insideBlue = P5^4;
sbit center = P5^5;
sbit left = P4^4;//数码管的位选引脚
sbit right = P4^5;//数码管的位选引脚
sbit S = P3^3;	//单片机左下的开关
sbit trig = P1^1;//发射超声波的引脚
sbit echo = P1^0;//接受回响的引脚
sbit speaker = P1^2;//扬声器
sbit S2 = P3^7;	//给电子琴做琴键的几个按键
sbit S3 = P3^6;
sbit S4 = P3^5;
sbit S5 = P4^1;
sbit S6 = P4^2;
sbit S7 = P3^4;


uchar code numberTable[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};//共阳数码管的字段表（无小数点）
uchar code numberTablePoint[16]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};//共阳数码管的字段表（有小数点）
unsigned int code soundTable[] = {64361,64426,64489,64547,64603,64655,64704,64751,64795,64837,64876,64913,64948,64981,65012,65042,65060,65095,65120,65144,65166,65186,65206};//音符表,这里有毒，数据类型一改成uint就会走音


uchar mode;		//模式
uchar modecpy;	//模式的副本 ，保存下来与模式比较 ，若当前模式与副本不同则说明模式已经改变
uchar a;
int count=0;	//计数器的数字
uchar leftNumber;	//数码管左边显示的数字
uchar rightNumber;	//数码管右边显示的数字
bit Sctrl;	//控制是否接受按键的一个变量
uint reflectCtrl;//消抖
uint longCtrl;	//长按
uchar second ;	//用来储存时间的一个变量
uchar minute ;	//用来储存时间的一个变量
uchar unit;		//用来储存时间的一个变量
float xdata machineCycle;	//单片机计数周期
bit b = 1;	//用来决定是否接受超声波的一个变量
uint distance = 0;	//距离值
uint STH0;	//储存定时器初值高八位
uint STL0;	//储存定时器初值低八位
uchar key;	//储存音符
bit timer0Mode;	//定时器0的模式
uchar timeTemp;	//给定时器分频用的
uint temp;
uint centimeter;	//储存距离，单位是厘米
uint decimeter;		//储存距离，单位是分米
uint centimeterTemp[10];	//储存距离的临时变量，单位是厘米
uchar interruptTemp;	//用来给换档按键消抖的一个变量
uint i,j,k,l,m,n,y,x,z;	//给灯效、循环等等使用的几个变量
uint bright;
bit musicList;	//用来储存歌曲序号，切割的时候用
bit musicListCopy;
uchar effectMode;	//灯效模式

void init();//初始化函数
void lightEffect();//灯效模块
void lightEffectAndDigitron();//灯效和数码管
void stopwatch();//秒表模块
void distanceMeasurement();//测距模块
void piano();//电子琴模块
void musicPlayer();//音乐播放模块
void Delay(unsigned int x);//自己写的延时函数
void init2();
void Delay20us();
void makeSound(unsigned char key);
void animation1(unsigned int x);//数码管动画1
void animation0(unsigned int x);//数码管动画0
void Delay100ms();
void Delay20ms();
void Delay154639us();//拍速为97bpm的情况下，延时一个16分音符的长度
void playMusic97(unsigned char key,unsigned char length);//拍速为97bpm的情况下的音乐播放函数
void music0();//第0首音乐
void music1();//第1首音乐
void effect0(unsigned int m);
void effect1(unsigned int m);
void effect2(unsigned int m);
void effect3(unsigned int m);
void effect4(unsigned int m);
void effect5();
void effect6();
void effect7();
void effect8();
void effect9();
void effect10();
void effect11();
void effect12(unsigned int m);
void effect13(unsigned int m);
void effect14(unsigned int m);
void effect15(unsigned int m);




int main()
{
	init();//初始化
	while (1)
	{
		init2();//初始化
		animation0(400);//显示动画
		switch(mode)
		{
			case 0: lightEffect();break;
			case 1: lightEffectAndDigitron();break;
			case 2: stopwatch();break;
			case 3: distanceMeasurement();break;
			case 4: piano();break;
			case 5: musicPlayer();break;
		}
		
	}
}


void init()
{
	mode = 0;
	IT0 = 1;//外部中断0下降沿触发
	EX0 = 1;//开启外部中断0
	EA = 1;//开启总中断
	P4M1 = 0x00;//数码管推挽输出
	P4M0 = 0x30;	
	interruptTemp = 0;
	
}

void init2()
{
	outsideBlue = 1;
	outsideYellow = 1;
	outsideGreen = 1;
	outsidewhite = 1;
	insideGreen = 1;
	insideBlue = 1;
	center = 1; 
	P2 = 0xff;
	speaker = 0;
	leftNumber = 0;
	rightNumber = 0;
	count = 0;
	TR0 = 0;
}

void lightEffect()//灯效
{
	modecpy = mode;
	effectMode = 0;
	
	while (1)
	{
		if (modecpy != mode) break;
		
		outsideBlue = 1;
		outsideYellow = 1;
		outsideGreen = 1;
		outsidewhite = 1;
		insideGreen = 1;
		insideBlue = 1;
		
		effectMode = rand()%16;
		
		switch(effectMode)
		{
			case 0:       effect0(600);break;
			case 1:       effect1(20001);break;
			case 2:       effect2(600);break;
			case 3:       effect3(600);break;
			case 4:       effect4(600);break;
			case 5:       effect5();break;
			case 6:       effect6();break;
			case 7:       effect7();break;
			case 8:       effect8();break;
			case 9:       effect9();break;
			case 10:      effect10();break;
			case 11:      effect11();break;
			case 12:      effect12(600);break;
			case 13:      effect13(600);break;
			case 14:      effect14(600);break;
			case 15:      effect15(600);break;
		}
		
	}
	
}

void lightEffectAndDigitron()//计数器
{

    bright = 0;
	a= 121;
	reflectCtrl=0;
	longCtrl=0;	
	P2 = 0xff;	
	modecpy = mode;	

	while (1)
	{
		a--;
		if (a==0) a = 121;
			
		if (a<=120&&a>=90) {outsideYellow = 1;outsideBlue = 0;insideGreen = 0;}
		if (a<90&&a>=60) {outsideBlue = 1; insideGreen = 1;outsidewhite = 0;}			
		if (a<60&&a>=30) {outsidewhite = 1;outsideGreen = 0;}			
		if (a<30){outsideGreen = 1;outsideYellow = 0;}			
		bright = (a%121)*2;		
		
		for(i = 0; i < 240; i++)
		{	
			
			if (reflectCtrl != 0)reflectCtrl--;
			if (longCtrl != 0)longCtrl--;
			
				
			
			if ((S == 0)&&(reflectCtrl == 0)&&(longCtrl != 0))	//检测长按
			{
				count = -1;
			}
			
			if ((S == 0)&&(reflectCtrl == 0)&&(longCtrl == 0))	//检测暂停键
			{
				count++;
				count %= 100;
				leftNumber = count/10;
				rightNumber = count%10;
				
				reflectCtrl = 4000;
				longCtrl = 8000;
			}
				
				
				
			if(i < bright)
			{
				center = 0;				
				insideBlue = 1;
				right = 0;
				left = 1;
				P2 = numberTable[rightNumber];
				Delay(40);
			}	
			else
			{
				center = 1;				
				insideBlue = 0;
				right = 1;
				left = 0;
				P2 = numberTable[leftNumber];
				Delay(30);	
			}
			
			if (modecpy != mode) return;
			
		}				
		
		
	}
}

void stopwatch()//秒表
{
	STH0=0x4c;STL0=0x00;//设定定时器初值
	TMOD = 0x00;//定时器为模式0（16位自动重载）
	left = 1;
	right = 1;
	ET0=1;//允许定时器0中断 
	modecpy = mode;
	second = 0;
	minute = 0;
	timer0Mode = 0;//使定时器0进入模式0
	TR0 = 0;	
	
	while(1)
	{
		if (reflectCtrl != 0)reflectCtrl--;
		if (longCtrl != 0)longCtrl--;
		
		if (modecpy != mode) break;//检测换挡键
		
		
		
		if ((S == 0)&&(reflectCtrl == 0)&&(longCtrl != 0))	//检测长按
		{
			TR0 = 0;
			second = 0;
			minute = 0;
		}
		
		if ((S == 0)&&(reflectCtrl == 0)&&(longCtrl == 0))	//检测暂停键
		{
			TR0 = !TR0;
			reflectCtrl = 2000;
			longCtrl = 4000;
		}
		
		if (minute > 0) unit = minute;	//判断应该使用的时间单位
		else unit = second;
		
		
		leftNumber = unit/10;
		rightNumber = unit%10;

		P2 = 0xff;	//消影	
		left = 0;
		right = 1;
		P2 = numberTable[leftNumber];
		Delay (40);	
		
		
		P2 = 0xff;	//消影	
		left = 1;
		right = 0;
		P2 = numberTable[rightNumber];
		Delay (40);	
	
	}
}

void distanceMeasurement()//测距
{
	modecpy = mode;	
	//TMOD = 0x00;//定时器为模式0（16位自动重载）
	machineCycle=12/11.0592;//计算单片机机器周期   晶振=11.0592M	单位us
	left = 1;
	right = 1;
	centimeter = 0;
	reflectCtrl = 0;

	while(1)
	{		
		if (modecpy != mode) return;
		
		if (S == 0)	//当按键按下时进行一次完整的测距和计算
		{
			
			animation1(400);//显示一下过场动画
			
			for (i=9;i>=0;i--)	//计算距离值，单位是厘米centimeter,这里要算出10组临时的距离值
			{
				TH1=0;//重置计时器
				TL1=0;
				
				trig=1;	//生成20us的脉冲宽度的触发信号			
				Delay20us();
				trig=0;
					
				while(!echo);	//等待回响信号变高电平
				TR1=1;		//启动定时器1
				
				while(echo);	//等待回响信号变低电平	
				TR1=0;		//关闭定时器1
					
				centimeterTemp[i] = 34*(machineCycle*(TH1*256+TL1)/1000)/2;
				
				P2 = 0xff;		//将次数显示出来作为一个彩蛋
				left = 1;
				right = 0;
				P2 = numberTable[i];
				
				Delay(30000);	//加点延迟防止受到上一次回声的影响
			}
			
			for (i=0;i<9;i++)	//给这10组数据排序一下
			{
				for (j=i+1;j<10;j++)
				{
					if (centimeterTemp[i]>centimeterTemp[j]) 
					{
						temp = centimeterTemp[i];
						centimeterTemp[i] = centimeterTemp[j];
						centimeterTemp[j] = temp;
					}
				}
			}
			
			temp = 0;
			for (i=1;i<9;i++)	//掐头去尾取平均值
			{
				temp += centimeterTemp[i];
			}
			centimeter = temp/8;
			
			if (centimeter >= 100)
			{
				decimeter = centimeter/10;
				unit = decimeter;
			}
			else unit = centimeter;
			
			leftNumber = unit/10;
			rightNumber = unit%10;
			
		}
		
		if (centimeter > 400)//如果大于4米则给出超距提示
		{
			P2 = 0xff;	
			left = 0;
			right = 1;
			P2 = 0xbf;
			Delay(40);	
			
			P2 = 0xff;	
			left = 1;
			right = 0;
			P2 = 0xbf;
			Delay (40);	
		}
		else	//如果不大于4米则正常显示
		{
			P2 = 0xff;	
			left = 0;
			right = 1;
			if (centimeter < 100) P2 = numberTable[leftNumber];
			else P2 = numberTablePoint[leftNumber];
			Delay(40);	
			
			P2 = 0xff;	
			left = 1;
			right = 0;
			P2 = numberTable[rightNumber];
			Delay (40);	
		}
		
	}
}

void piano()//电子琴
{
	modecpy = mode;
	TMOD = 0x01;
	ET0 = 1;
	EA = 1;
	speaker = 0;
	timer0Mode = 1;
	temp = 0;
	while (1)
	{
		
		if (modecpy != mode) return;//检测换挡键
	
		P0 = 0xfe;//扫描第1行
		if (P0 != 0xfe)
		{
			temp = P0;
			switch(temp)
			{
				case 0xee:makeSound(22);break;
				case 0xde:makeSound(21);break;
				case 0xbe:makeSound(19);break;
				case 0x7e:makeSound(17);break;
			}
			continue;
		}
		
		P0 = 0xfd;//扫描第2行
		if (P0 != 0xfd)
		{
			temp = P0;
			switch(temp)
			{
				case 0xed:makeSound(16);break;
				case 0xdd:makeSound(14);break;
				case 0xbd:makeSound(12);break;
				case 0x7d:makeSound(10);break;
			}
			continue;
		}
		
		P0 = 0xfb;//扫描第3行
		if (P0 != 0xfb)
		{
			temp = P0;
			switch(temp)
			{
				case 0xeb:makeSound(9);break;
				case 0xdb:makeSound(7);break;
				case 0xbb:makeSound(5);break;
				case 0x7b:makeSound(4);break;
			}
			continue;
		}
		
		P0 = 0xf7;//扫描第4行
		if (P0 != 0xf7)
		{
			temp = P0;
			switch(temp)
			{
				case 0xe7:makeSound(2);break;
				case 0xd7:makeSound(0);break;
				case 0xb7:makeSound(1);break;
				case 0x77:makeSound(3);break;
			}
			continue;
		}
		
		//扫描零散的按键
		if (S == 0) 	{makeSound(6) ;continue;}
		if (S2 == 0)	{makeSound(8) ;continue;}
		if (S3 == 0)	{makeSound(11);continue;}
		if (S4 == 0)	{makeSound(13);continue;}
		if (S5 == 0)	{makeSound(15);continue;}
		if (S6 == 0)	{makeSound(18);continue;}
		if (S7 == 0)	{makeSound(20);continue;}
		
		TR0 = 0;

	}
}

void musicPlayer()//音乐播放器
{
	modecpy = mode;
	musicList = 0;
	
	while(1)
	{
		speaker = 0;
		TR0 = 0;
		
		if(modecpy != mode) return;
		
		Delay100ms();//消抖
		
		if (musicList == 0) music0();
		else music1();
		

	}
	
}

void changeMode() interrupt 0  //INT0中断入口
{
	if (interruptTemp == 0)
	{
		mode++;
		mode %= 6;
	}
	
	interruptTemp++;	//换档函数生效后就不能马上再次生效，必须要经过过场动画重置后才能生效，起到消抖功能
	
}

void timer0() interrupt 1	 //	定时器0的中断程序
{
	TH0 = STH0;// 赋初值
	TL0 = STL0;
	
	if (timer0Mode == 0)//若定时器0处于模式0，则进行计时
	{
		timeTemp++;
		if(timeTemp == 20)
		{
			timeTemp=0;
			second++;
		}
		if(second == 60)
		{
			second = 0;
			minute++;
		}
	}
	else speaker = !speaker;//若定时器处于模式1，则产生方波
}

void makeSound(unsigned char key)//根据键位给定时器赋初值
{
	STH0 = soundTable[key]/256;
	STL0 = soundTable[key]%256;
	TR0 = 1;
}

void animation1(unsigned int x)//超声波开始测距前的过场动画
{
	i = x;
	while (i--)
	{
		for (j=0;j<x;j++)
		{
			if (j<i)
			{
				P2 = 0xff;
				left = 0;
				right = 1;
				P2 = 0xc2;
				Delay(5);
				
				P2 = 0xff;
				left = 1;
				right = 0;
				P2 = 0xa3;
				Delay(5);
				P2 = 0xff;
			}
			else
			{
				Delay(10);
			}
		}
	}
}

void animation0(unsigned int x)//切换模式的过场动画
{
	i = x;
	while (i--)
	{
		for (j=0;j<x;j++)
		{
			if (j<i)
			{	P2 = 0xff;
				left = 0;
				right = 1;
				P2 = 0x7f;
				Delay(2);
				
				P2 = 0xff;
				
				left = 1;
				right = 0;
				P2 = numberTablePoint[mode];
				Delay(5);
				P2 = 0xff;
			}
			else
			{
				Delay(10);
			}
		}
	}
	
	interruptTemp = 0;//将换挡函数重置为可生效状态
}

void playMusic97(unsigned char key,unsigned char length)//拍速为97bpm的情况下的音乐播放函数
{
	if(key == 0) TR0 = 0;//判断休止符
	else makeSound(key);//若不是休止符则发声
	for(i=0;i<length;i++) Delay154639us();//一个循环决定延时多少个16分音符，控制音符长度
}

void Delay20ms()		
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms()	
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay154639us()	//拍速为97bpm的情况下，一个16分音符的长度
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 7;
	j = 128;
	k = 143;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay20us()	//给超声波测距使用的一个延时函数	
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 52;
	while (--i);
}

void Delay(unsigned int x)
{
	
    while( x-- )
    {	
		
    }
}

void music0()//第0首音乐
{
	musicListCopy = musicList;
	
	playMusic97(0,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(4,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,7);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(4,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,7);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(4,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,3);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,3);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,3);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,8);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(16,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(16,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,6);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,5);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(16,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(18,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(16,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(18,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(19,6);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(6,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(11,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
}

void music1()//第1首音乐
{
	musicListCopy = musicList;
	
	playMusic97(9	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(5	,8);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7	,8);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(2	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(2	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(4	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(5	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	
	makeSound(0);
	for(i=0;i<2;i++) Delay154639us();//一个循环决定延时多少个16分音符，控制音符长度
	//playMusic97(0	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	//音符与标志符冲突，只好把函数写到外面来
	
	playMusic97(5	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(7	,8);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(5	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(5	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14	,2);     if(S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,8);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(10	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,3);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(14	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(14	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(16	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(17	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,4);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,1);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(14	,4);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(7	,8);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(0	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(12	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,1);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	TR0 = 0;Delay20ms();
	playMusic97(12	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(10	,2);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(9	,2);    if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
	playMusic97(5	,6);     if (S == 0) {musicList = !musicList;return;}  if(modecpy != mode) return;
}

void effect0(unsigned int m)//灯效0，最佳参数600
{   
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) center = 0; else center = 1;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideBlue = 0; else insideBlue = 1;	
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}		
			k++;	
			if (k>z) outsideBlue = 0; else outsideBlue = 1;
		}
		
		if (z == 0) break;
		
	}
	
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) outsideBlue  = 1; else outsideBlue  = 0;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideBlue = 1; else insideBlue = 0;
			
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}	
			k++;
			if (k>z) center  = 1; else center = 0;
			
		}	
		
		if (z == 0) break;	
	}
	
}

void effect1(unsigned int m)//灯效1，最佳参数20001
{   
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while(m--)
	{
		if(modecpy != mode) return;
		if (m<=20000&&m>=15000) {outsideYellow = 1;outsideBlue = 0;}
		if (m<15000&&m>=10000) {outsideBlue = 1; outsidewhite = 0;}			
		if (m<10000&&m>=5000) {outsidewhite = 1;outsideGreen = 0;}			
		if (m<5000){outsideGreen = 1;outsideYellow = 0;}				
	}
	
}

void effect2(unsigned int m)//灯效2，最佳参数600
{   
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) center = 0; else center = 1;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideGreen = 0; else insideGreen = 1;	
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}		
			k++;	
			if (k>z) outsideGreen = 0; else outsideGreen = 1;
		}
		
		if (z == 0) break;
		
	}
	
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) outsideGreen  = 1; else outsideGreen  = 0;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideGreen = 1; else insideGreen = 0;
			
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}	
			k++;
			if (k>z) center  = 1; else center = 0;
			
		}	
		
		if (z == 0) break;	
	}
	
}

void effect3(unsigned int m)//灯效3，最佳参数600
{   
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) center = 0; else center = 1;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideGreen = 0; else insideGreen = 1;	
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}		
			k++;	
			if (k>z) {outsidewhite = 0;outsideYellow = 0; }else {outsidewhite = 1;outsideYellow = 1;}
		}
		
		if (z == 0) break;
		
	}
	
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x){outsidewhite = 1;outsideYellow = 1;}else {outsidewhite = 0;outsideYellow = 0; }
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideGreen = 1; else insideGreen = 0;
			
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}	
			k++;
			if (k>z) center  = 1; else center = 0;
			
		}	
		
		if (z == 0) break;	
	}
	
}

void effect4(unsigned int m)//灯效4，最佳参数600
{   
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) center = 0; else center = 1;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideGreen = 0; else insideGreen = 1;	
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}		
			k++;	
			if (k>z) outsideBlue = 0; else outsideBlue = 1;
		}
		
		if (z == 0) break;
		
	}
	
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if (x<=m)
		{
			if ((i == m)&&(x != 0)) {i = 0;x--;}			
			i++;		
			if (i>x) outsideBlue  = 1; else outsideBlue  = 0;
		}
		
		if (x<400)
		{
			if ((j == m)&&(y != 0)) {j = 0;y--;}		
			j++;		
			if (j>y) insideGreen = 1; else insideGreen = 0;
			
		}
		
		if (x<200)
		{
			if ((k == m)&&(z != 0)) {k = 0;z--;}	
			k++;
			if (k>z) center  = 1; else center = 0;
			
		}	
		
		if (z == 0) break;	
	}
	
}

void effect5()//灯效5
{   
	center = 0;
	Delay100ms();
	center = 1;
	Delay100ms();
	center = 0;
	Delay100ms();
	center = 1;
	Delay100ms();
	
	insideBlue = 0;
	Delay100ms();
	insideBlue = 1;
	Delay100ms();
	insideBlue = 0;
	Delay100ms();
	insideBlue = 1;
	Delay100ms();
	
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
}

void effect6()//灯效6
{   
	center = 0;
	insideBlue = 0;
	Delay100ms();
	center = 1;
	insideBlue = 1;
	Delay100ms();
	center = 0;
	insideBlue = 0;
	Delay100ms();
	center = 1;
	insideBlue = 1;
	Delay100ms();
	
	outsideGreen = 0;
	insideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	insideGreen = 1;
	Delay100ms();
	outsideGreen = 0;
	insideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	insideGreen = 1;
	Delay100ms();
}

void effect7()//灯效7
{   
	outsidewhite = 0;
	outsideYellow = 0;
	insideGreen = 0;
	Delay100ms();
	outsidewhite = 1;
	outsideYellow = 1;
	insideGreen = 1;
	Delay100ms();
	outsidewhite = 0;
	outsideYellow = 0;
	insideGreen = 0;
	Delay100ms();
	outsidewhite = 1;
	outsideYellow = 1;
	insideGreen = 1;
	Delay100ms();
	
	outsideBlue = 0;
	outsideGreen = 0;
	Delay100ms();
	outsideBlue = 1;
	outsideGreen = 1;
	Delay100ms();
	outsideBlue = 0;
	outsideGreen = 0;
	Delay100ms();
	outsideBlue = 1;
	outsideGreen = 1;
	Delay100ms();
	
}

void effect8()//灯效8
{   
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	
	outsidewhite = 0;
	Delay100ms();
	outsidewhite = 1;
	Delay100ms();
	outsidewhite = 0;
	Delay100ms();
	outsidewhite = 1;
	Delay100ms();
	
	outsideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	Delay100ms();
	outsideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	Delay100ms();
	
	outsideYellow = 0;
	Delay100ms();
	outsideYellow = 1;
	Delay100ms();
	outsideYellow = 0;
	Delay100ms();
	outsideYellow = 1;
	Delay100ms();
}

void effect9()//灯效9
{   
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	
	insideBlue = 0;
	Delay100ms();
	insideBlue = 1;
	Delay100ms();
	insideBlue = 0;
	Delay100ms();
	insideBlue = 1;
	Delay100ms();
	
	center = 0;
	Delay100ms();
	center = 1;
	Delay100ms();
	center = 0;
	Delay100ms();
	center = 1;
	Delay100ms();
	
	insideBlue = 0;
	Delay100ms();
	insideBlue = 1;
	Delay100ms();
	insideBlue = 0;
	Delay100ms();
	insideBlue = 1;
	Delay100ms();
	
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	outsideBlue = 0;
	Delay100ms();
	outsideBlue = 1;
	Delay100ms();
	
}

void effect10()//灯效10
{   
	outsideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	Delay100ms();
	outsideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	Delay100ms();
	
	insideGreen = 0;
	Delay100ms();
	insideGreen = 1;
	Delay100ms();
	insideGreen = 0;
	Delay100ms();
	insideGreen = 1;
	Delay100ms();
	
	center = 0;
	Delay100ms();
	center = 1;
	Delay100ms();
	center = 0;
	Delay100ms();
	center = 1;
	Delay100ms();
	
	insideGreen = 0;
	Delay100ms();
	insideGreen = 1;
	Delay100ms();
	insideGreen = 0;
	Delay100ms();
	insideGreen = 1;
	Delay100ms();
	
	outsideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	Delay100ms();
	outsideGreen = 0;
	Delay100ms();
	outsideGreen = 1;
	Delay100ms();
	
}

void effect11()//灯效11
{   
	bright = 240;
	while (bright--)
	{
		
		for(i = 0; i < 240; i++)
		{	
	
			if(i < bright)
			{
				center = 0;	
				outsideBlue = 0;
				insideBlue = 1;
				Delay(50);
			}	
			else
			{
				center = 1;	
				outsideBlue = 1;
				insideBlue = 0;
				Delay(50);
			}

		}				
			
	}
}

void effect12(unsigned int m)
{
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if ((i == m)&&(x != 0)) {i = 0;x--;}			
		i++;		
		if (i>x) 
		{
			outsideBlue = 0;
			outsideYellow = 0;
			outsideGreen = 0;
			outsidewhite = 0;
			insideGreen = 0;
			insideBlue = 0;
			center = 0; 	
		}
		else 
		{
			outsideBlue = 1;
			outsideYellow = 1;
			outsideGreen = 1;
			outsidewhite = 1;
			insideGreen = 1;
			insideBlue = 1;
			center = 1; 
		}

		if (x == 0) break;
	}

}

void effect13(unsigned int m)
{
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if ((i == m)&&(x != 0)) {i = 0;x--;}			
		i++;		
		if (i>x) 
		{
			outsideBlue = 0;
			outsideYellow = 0;
			outsideGreen = 0;
			outsidewhite = 0;
			insideGreen = 0;
			insideBlue = 0;
			center = 0; 	
		}
		else 
		{
			outsideBlue = 1;
			outsideYellow = 1;
			outsideGreen = 1;
			outsidewhite = 1;
			insideGreen = 1;
			insideBlue = 1;
			center = 1; 
		}

		if (x == 0) break;
	}
}
void effect14(unsigned int m)
{
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if ((i == m)&&(x != 0)) {i = 0;x--;}			
		i++;		
		if (i>x) 
		{
			outsideGreen = 0;
		}
		else 
		{
			outsideGreen = 1;
		}

		if (x == 0) break;
	}
}
void effect15(unsigned int m)
{
	x = y = z = m;
	i = 0;j = 0;k = 0;
	
	while (1)
	{
		if(modecpy != mode) return;
		if ((i == m)&&(x != 0)) {i = 0;x--;}			
		i++;		
		if (i>x) 
		{
			outsidewhite = 0;
		}
		else 
		{
			outsidewhite = 1;
		}

		if (x == 0) break;
	}
}