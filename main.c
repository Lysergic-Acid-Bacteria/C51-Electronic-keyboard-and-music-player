#include<reg52.h>

sbit BEEP = P2^5;
sbit led = P2^2;
sbit play=P3^2;//K3为开始播放按钮
sbit K4=P3^3;
unsigned char exit = 0;
unsigned char a,b,n,ge,shi;

#define P	0	//p表休止符
#define L1	1	//L为低音
#define L1_	2	//_表示升半音，即#
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13	//M为中音
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25	//H为高音
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36
#define STOP 37
#define END 38

unsigned int FreqTable[] = {
	0,
	63777,63872,63969,64054,64140,64216,64291,64360,64426,64489,64547,64603,
	64655,64704,64751,64795,64837,64876,64913,64948,64981,65012,65042,65070,
	65095,65120,65144,65166,65186,65206,65225,65242,65259,65274,65289,65303,0
};//音符频率表

unsigned char code tone[]={L1,L2,L3,L4,L5,L6,L7,M1,M2,M3,M4,M5,M6,M7,H1,H2,H3,H4,H5,H6,H7};

unsigned char code Music[][2] = {	//曲子本体
	{P,1}, {L1,8},{P,4},{L1,8},{P,4},{L1,8},{P,4},{L1,8},{P,4},
	{L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L4,1},{L5_,1},{L5,2}, {L4,1},{L4,1},{L4,1},{L4,1}, {L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L4,1},{L5_,1},{L5,2}, {L4,1},{L4,1},{L4,1},{L4,1},//7-8节
	{L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L4,1},{L5_,1},{L5,2}, {L4,1},{L4,1},{L4,1},{L4,1}, {L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L7,1},{L5_,1},{L5,2}, {L4,1},{L4,1},{L4,1},{L4,1},//9-10节
	{L5,1},{L5,1},{L5,1},{L5_,1}, {L5,1},{L5,1},{L5,1},{L5_,1}, {L5,1},{L5,1},{L5,2}, {M1,2},{L5,2}, {L5_,8},{L5_,2},{L4,2},{M1,2},{L5,2}, //11-12节
	{L5_,8}, {L5_,2},{L5_,2},{L5_,2},{L6_,2}, {M1,8}, {M1_,2},{L2_,2}, {M1,2},{L6_,1},{L5_,1}, //13-14节
	{L6_,8},  {L6_,2},{L6_,2}, {L6_,2},{M1,2}, {M1_,8}, {M1_,2},{L4,2},{M2_,2},{M1,2}, //15-16节
	{M1,2},{M1_,1},{M2_,1}, {M1_,2},{M1_,2},{M1_,2},{M2_,2}, {M4,3},{M5,1}, {M4,4}, {M5,2}, {M4,2},{M5,2}, {M5_,2},{M5,1},{M4,1}, //17-18节
	{M5,8}, {M5,8}, {L5_,1},{L5,1},{L5_,1},{L6_,1}, {L5_,1},{L5,1},{L5_,1},{L6_,1},{L5_,1},{L5,1},{L5_,1},{L6_,1}, {M1,2},{L6_,2}, //19-20节
	{L5_,1},{L5,1},{L5_,1},{L6_,1}, {L5_,1},{L5,1},{L5_,1},{L6_,1}, {L5_,1},{L5,1},{P,2}, {P,4}, {M1_,12},{P,4}, //21-22节
	{P,4}, {M2_,4}, {M4,4}, {M2_,4}, {M5,2},{M5_,2}, {M6_,8}, {P,4},//23-24节
	{M5_,2},{M5,2}, {M4,8}, {P,4}, {M4_,1},{M5_,1},{M6_,1},{H1,1}, {H1_,12}, {H1,1}, {H3,12},//25-27节
	{L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L4,1},{L5_,1},{L5,2}, {P,4}, {L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L4,1},{L5_,1},{L5,2}, {P,4},//28-29节
	{L4,1},{L4,1},{L4,1},{L5,1}, {L5_,2},{L4,1},{L4,1}, {L4,1},{L5_,1},{L5,2}, {P,4}, //30节
	{STOP,1},{END,1}//终止
};

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code clef[]={0x38,0x37,0x76};

unsigned char FreqSelect,MusicSelect;

void Delayms(unsigned int xms)	//@11.0592MHz
{
	unsigned char data i, j;
	while(xms--){
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Timer0_Init(){
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = 0xDC;
	TL0 = 0x00; 
	TF0 = 0;
	TR0 = 1;
	ET0 = 1;	
	EA = 1; 	
	PT0 = 1;	//定时器初始化
}

void back() interrupt 2
{
	Delayms(10);
	
	exit = 1;
}

void Display()
{
	MusicSelect=0;	
	while(exit == 0 && FreqSelect != END)
	{
			FreqSelect = Music[MusicSelect][0];
			Delayms(140 * Music[MusicSelect][1]); // 194BPM (60/194)/2=0.155s 155????????????????140??????????? 1Ϊ�ķ�֮һ�ģ���16��������
			MusicSelect++;
			
			TR0 = 0;	//启动定时器
			Delayms(5);
			TR0 = 1;
	}
	exit = 0;           //停止标志
    TR0 = 0;
    BEEP = 1;
	led=0;
	FreqSelect=0;
}

void judge()
{
	P1=0x0f;//P1=0000 1111
	if(P1!=0x0f)
	{
		Delayms(5);
		if(P1!=0X0F)
		{
			switch(P1)//行扫描
			{
				case 0x07: n=0; break;
				case 0x0b: n=1; break;
				case 0x0d: n=2; break;
				case 0x0e: n=3; break;
			}
			P1=0xf0;
			switch(P1)//列扫描
			{
				case 0x70: n=n; break;
				case 0xb0: n=n+4; break;
				case 0xd0: n=n+8; break;
				case 0xe0: n=n+12; break;
			}
		}P1=0x0f;a=1;
	}
}//判断键盘输入的数值

void first()
{
	if(n>11 && n!=15)
	{
		shi=n-12;
	}
	if(n<8)
	{
		ge=n;
	}
}//判断显示的数

void xianshi()
{
	P2=0x1c;
	P0=clef[shi];
	Delayms(10);
	P2=0x18;
	P0=table[ge];
	Delayms(10);
}//显示功能
	
void main()
{
	EA=1;
	EX0=1;
	EX1=1;
	IT0=1;
	IT1=1;
	PX0=1;
	PX1=0;
	Timer0_Init();
	while(1)
	{
		judge();
		first();
		xianshi();
		if(a==1)
		{
			while(a==1)
			{
				FreqSelect=tone[ge+shi*7-1];
				TR0=1;
				xianshi();
				if(P1==0x0f)
				{
					a=0;
				}
			}TR0=0;
		}
		if(play==0)
		{
			Delayms(10);					
			Display();
		}//按下	K3后播放曲子
	}
}



void Timer0_Routine() interrupt 1
{
	if(FreqTable[FreqSelect])
	{
		TH0 = FreqTable[FreqSelect] / 256;
		TL0 = FreqTable[FreqSelect] % 256;
		BEEP = !BEEP;
	}
}//蜂鸣器发声功能，根据选好的频率启停蜂鸣器以改变音调


