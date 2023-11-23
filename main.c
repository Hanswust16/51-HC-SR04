#include<reg51.h>
#include<intrins.h>
sbit RS = P2^6;   
sbit RW = P2^5;
sbit EN = P2^7;			  
sbit key1=P3^1;    //按键设置
sbit key2=P3^0;	   //操作键
sbit key3=P3^2;
sbit key4=P3^3;
sbit RX=P2^1;
sbit TX=P2^0;
sbit Buzzer=P2^6;
bit flag=0;
unsigned int tempH=100,tempL=1;
unsigned char unit,tenn,hun;
unsigned char string1,string2,string3,string_1,string_2,string_3;
unsigned int distancecm=0;
unsigned char code str[]="0123456789";
unsigned char code str1[]="distance:" ;
#define RS_CLR RS=0 
#define RS_SET RS=1
 
#define RW_CLR RW=0 
#define RW_SET RW=1 
 
#define EN_CLR EN=0
#define EN_SET EN=1
 
#define DataPort P0
 
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
 bit LCD_Check_Busy(void) 
 { 
 DataPort= 0xFF; 
 RS_CLR; 
 RW_SET; 
 EN_CLR; 
 _nop_(); 
 EN_SET;
 return (bit)(DataPort & 0x80);
 }
//写入命令
 void LCD_Write_Com(unsigned char com) 
 {  
 while(LCD_Check_Busy());
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 DataPort= com; 
 _nop_(); 
 EN_CLR;
 }
//写入数据
 void LCD_Write_Data(unsigned char Data) 
 { 
 while(LCD_Check_Busy()); 
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 DataPort= Data; 
 _nop_();
 EN_CLR;
 }
 
//清屏
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }
//字符串函数
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //第一行
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //第二行
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
 
//初始化
 void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);   
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);    //关闭显示 
   LCD_Write_Com(0x01);    //清屏
   LCD_Write_Com(0x06);    //显示光标移动设置
   DelayMs(5); 
   LCD_Write_Com(0x0C);    
   }
 //	 声波起始信号
 void ultrasonicstart()
 {
   RX=1; 
   DelayUs2x(20);
   RX=0;
 }
 
//测距初始化
void ultrasonic_count()
{
  unsigned int time=0;
 
  time=TH0*256+TL0;
  TH0=0;
  TL0=0;
  distancecm=(time*1.7)/100;
  tenn=distancecm/100;
  unit=distancecm/10%10;
  hun=distancecm%10;
  if((distancecm>=700)||(flag==1))	 //超出量程
  {
   flag=0;
   LCD_Write_String(9,0,"ERRCM");
  }
 else
 {
  LCD_Write_Com(0x80+9);
  LCD_Write_Data(str[tenn]);
  LCD_Write_Com(0x80+10);
  LCD_Write_Data(str[unit]);
  LCD_Write_Com(0x80+11);
  LCD_Write_Data(str[hun]);
  LCD_Write_Com(0x80+12);
  LCD_Write_Data('C');
  LCD_Write_Com(0x80+13);
  LCD_Write_Data('M');
   
 }
}
 
 
//报警值处理
void FBI()
{
if((distancecm>tempH)||(distancecm<tempL))
  
 Buzzer=~Buzzer;
 else
  Buzzer=1;				 
}
//按键处理
void keyscan()
{
								 
 if(!key1)
 {
  DelayMs(20);
  if(!key1)
  {
  	while(!key1);
	tempH++;
	if(tempH>=200)
	tempH=0;
  
  }
 
 }
 
 if(!key2)
 {
  DelayMs(20);
  if(!key2)
  {
  	while(!key2);
	tempH--;
	if(tempH<=0)
	 tempH=0;
 
  }
 
 }
 
 if(!key3)
 {
  DelayMs(20);
  if(!key3)
  {
  	while(!key3);
	tempL++;
	if(tempL>=100)
	tempL=0;
  }
 
 }
if(!key4)
 {
  DelayMs(20);
  if(!key4)
  {
  	while(!key4);
	tempL--;
	if(tempL<=0)
	tempL=0;
  }
 
 }
 
}
//主函数
 void main()		 
{
 
TMOD=0x01;
TH0=0;
TL0=0;
ET0=1;
EA=1;
Buzzer=1;
LCD_Init(); 	  //初始化
LCD_Clear();	//清屏
LCD_Write_String(0,0,str1);
LCD_Write_String(0,1,"TH:");
LCD_Write_String(9,1,"TL:");
while(1)
{
  keyscan();
  string1=tempH/100;
  string2=tempH/10%10;
  string3=tempH%10;
  string_1=tempL/100;
  string_2=tempL/10%10;
  string_3=tempL%10;
  LCD_Write_Com(0xC0+3);	   //上界显示
  LCD_Write_Data('0'+string1);
  LCD_Write_Com(0xC0+4);
  LCD_Write_Data('0'+string2);
  LCD_Write_Com(0xC0+5);
  LCD_Write_Data('0'+string3);
 
  LCD_Write_Com(0xC0+12);	 //下界显示
  LCD_Write_Data('0'+string_1);
  LCD_Write_Com(0xC0+13);	 
  LCD_Write_Data('0'+string_2);
  LCD_Write_Com(0xC0+14);	
 LCD_Write_Data('0'+string_3);
 ultrasonicstart();
 while(!TX);
 TR0=1;
 while(TX);
 TR0=0;
 ultrasonic_count();
 FBI();
 DelayMs(100);
} 
 }
 
void timer0() interrupt 1//计时器中断
 
{
 
 flag=1;  //判断：等于1继续循环，否，则跳出
 
}