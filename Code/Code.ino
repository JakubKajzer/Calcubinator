 #include <Adafruit_MCP23017.h>
#include <Wire.h>
#include <TimerOne.h>

#define DEBUG false


#define SDI 12
#define CLK A1
#define LA 13
#define OE 11

#define DIGIT1 5
#define DIGIT2 6
#define DIGIT3 7
#define DIGIT4 8
#define DIGIT5 9
#define DIGIT6 10

#define BUTTON 2
#define MCP_INT 3

 Adafruit_MCP23017 mcp;

volatile int stateMachine=0;
volatile bool prevBtnState=true;
volatile int btnState=true;

volatile short digits[6]={1,2,3,4,5,6};
volatile uint32_t value=123456;

volatile short digitPosition =0;

volatile bool sdiData1[8];
volatile bool sdiData2[8];
volatile bool updateSwitchesFlag = true;
void setup() 
{
  
  if(DEBUG) Serial.begin(115200);
  ////INTERRUPT INIT/////////////////////////////////////////////////
  Timer1.initialize(2000);
  Timer1.attachInterrupt(updateDisplay);
  attachInterrupt(digitalPinToInterrupt(MCP_INT), mcpInt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON), btnInt, RISING);
  
  if(DEBUG) Serial.println("PRZERWANIA");
//  MCP23017 INIT////////////////////////////////////////////////////
  mcp.begin(7);
  mcp.setupInterrupts(true,false,LOW);
  for(int i = 0; i <16;i++)
  {
    mcp.pinMode(i, INPUT);
    mcp.setupInterruptPin(i,CHANGE);
  }

  if(DEBUG) Serial.println("MCP23017 INIT");
  
  //SCT2024 INIT////////////////////////////////////////////////////
  pinMode(SDI,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(LA,OUTPUT);
  pinMode(OE,OUTPUT);
  
  pinMode(DIGIT1,OUTPUT);
  pinMode(DIGIT2,OUTPUT);
  pinMode(DIGIT3,OUTPUT);
  pinMode(DIGIT4,OUTPUT);
  pinMode(DIGIT5,OUTPUT);
  pinMode(DIGIT6,OUTPUT);

  pinMode(BUTTON, INPUT);
  pinMode(MCP_INT, INPUT);

  pinMode(A0, INPUT);
  pinMode(A3, INPUT);

  if(DEBUG) Serial.println("SCT2024 INIT");
///INIT VALUE FOR LED DRIVER//////////////////////////////////////////
  digitalWrite(LA,LOW);
  digitalWrite(CLK,LOW);
  digitalWrite(OE,HIGH);

  if(DEBUG) Serial.println("SCT2024 INIT VALUES");

}

void loop() 
{
  if(DEBUG) Serial.println(stateMachine);
  if(DEBUG) Serial.println(mcp.readGPIOAB());
  
  updateStateMachine();
  if(updateSwitchesFlag)
  	{
  		updateSwitches();
  		updateSwitchesFlag = false;
	}
  if(stateMachine)
  	calculateHex();
  else
  	calculateBin();
}

void mcpInt()
{
  if(DEBUG) Serial.println("mcpInt");
  updateSwitchesFlag = true;
}

void updateStateMachine()
{
  if(!btnState && prevBtnState)
  {
    prevBtnState = false;
    stateMachine++;
  }
  if(btnState)
  {
      prevBtnState = true;
  }
   btnState=true;
   if (stateMachine == 2) stateMachine=0;
   
}
void btnInt()
{
	btnState=false;
}
void calculateBin()
{
	if(DEBUG) Serial.println("Calculate function");
   noInterrupts();
   	  digits[0]=(value%1000000)/100000;
      digits[1]=(value%100000)/10000;
  	  digits[2]=(value%10000)/1000;
  	  digits[3]=(value%1000)/100;
  	  digits[4]=(value%100)/10;
  	  digits[5]=value%10;

 	 removeLeadingZeros();
     interrupts();
}
void calculateHex()
{
	if(DEBUG) Serial.println("Calculate function");
   noInterrupts();
   	  digits[0]=(int)(value/pow(16,5))%16;
  	  digits[1]=(int)(value/pow(16,4))%16;
  	  digits[2]=(int)(value/pow(16,3))%16;
  	  digits[3]=(int)(value/pow(16,2))%16;
  	  digits[4]=(value/16)%16;
  	  digits[5]=value%16;

 	 removeLeadingZeros();
     interrupts();
}
void removeLeadingZeros()
{
	if(digits[0]+digits[1]+digits[2]+digits[3]+digits[4]==0) digits[4]=16;
    if(digits[0]+digits[1]+digits[2]+digits[3]==0) digits[3]=16;
    if(digits[0]+digits[1]+digits[2]==0) digits[2]=16;
  	if(digits[0]+digits[1]==0) digits[1]=16;
  	if(digits[0]==0) digits[0]=16;
}

void updateDisplay()
{

		switch (digitPosition) 
		{
		    case 1:
		      printNumbers(digits[0],digits[3],1,0);
		      break;
		    case 2:
		      printNumbers(digits[1],digits[4],2,0);
		      break;
		    case 3:
		      printNumbers(digits[2],digits[5],3,0);
		      break;
		}
		
		if(++digitPosition == 4) digitPosition = 1;
}

void printNumbers(int left, int right,int pos, bool dot)
{
  // E,D,DP,C,G,A,F,B
	bool lookup[17][8] =
	   {{1,1,0,1,0,1,1,1}, //0
		{0,0,0,1,0,0,0,1}, //1
		{1,1,0,0,1,1,0,1}, //2
		{0,1,0,1,1,1,0,1}, //3
		{0,0,0,1,1,0,1,1}, //4
		{0,1,0,1,1,1,1,0}, //5
		{1,1,0,1,1,1,1,0}, //6
		{0,0,0,1,0,1,0,1}, //7
		{1,1,0,1,1,1,1,1}, //8
		{0,1,0,1,1,1,1,1}, //9
		{1,0,0,1,1,1,1,1}, //A
		{1,1,0,1,1,0,1,0}, //B
		{1,1,0,0,0,1,1,0}, //C
		{1,1,0,1,1,0,0,1}, //D
		{1,1,0,0,1,1,1,0}, //E
		{1,0,0,0,1,1,1,0}, //F
		{0,0,0,0,0,0,0,0}}; //NULL


		digitalWrite(DIGIT1,LOW); 
        digitalWrite(DIGIT2,LOW);
        digitalWrite(DIGIT3,LOW);
        digitalWrite(DIGIT4,LOW);
        digitalWrite(DIGIT5,LOW);
        digitalWrite(DIGIT6,LOW);

  

  
  digitalWrite(OE,HIGH);
  for(int i=0;i<8;i++)
  {
    digitalWrite(SDI,lookup[right][i]);
    digitalWrite(CLK,HIGH);
    digitalWrite(CLK,LOW);
  }
  for(int i=0;i<8;i++)
  {
    digitalWrite(SDI,lookup[left][i]);
    digitalWrite(CLK,HIGH);
    digitalWrite(CLK,LOW);
  }

  digitalWrite(LA,HIGH);
  digitalWrite(LA,LOW);
  digitalWrite(OE,LOW);

  switch(pos)
  {
    case 1:
    {
        digitalWrite(DIGIT1,HIGH); 
        digitalWrite(DIGIT4,HIGH); 
        break;
    }
    case 2:
    {
        digitalWrite(DIGIT2,HIGH);
        digitalWrite(DIGIT5,HIGH);
        break;
    }
    case 3:
    {
        digitalWrite(DIGIT3,HIGH);
        digitalWrite(DIGIT6,HIGH);
        break;
    }
  }
}
void updateSwitches()
{
  if(DEBUG) Serial.println("updateSwitches function");
  value=mcp.readGPIOAB();
  int temp=value;

  value>>=1;
  int i=15;
  while(value)
  {
    temp <<=1;
    temp |= value & 1;
    value >>=1;
    i--;
  }
  temp <<=i;
  value=temp;
  if(DEBUG) Serial.print("po odrwoceniu: ");
  if(DEBUG) Serial.println(value);

}
