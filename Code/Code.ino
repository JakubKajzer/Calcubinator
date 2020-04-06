#include <Adafruit_MCP23017.h>
#include <Wire.h>


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

#define BUTTON A3

bool bytes[16];
Adafruit_MCP23017 mcp;

String temp;
int stateMachine=0;
bool prevBtnState;
int btnState;

long long timer=0;
short digits[6];
unsigned int value;

void setup() 
{
  
  Serial.begin(115200);

  //MCP23017 INIT////////////////////////////////////////////////////
  mcp.begin(8);
  
  for(int i = 0; i <16;i++)
  {
    mcp.pinMode(i, INPUT);
  }
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

///INIT VALUE FOR LED DRIVER//////////////////////////////////////////
  digitalWrite(LA,LOW);
  digitalWrite(CLK,LOW);
  digitalWrite(OE,HIGH);

  
}

void loop() 
{

  
  if(millis()-timer >200)
  {
      btnState = digitalRead(BUTTON);
      if(!btnState && prevBtnState)
      {
        prevBtnState = false;
        stateMachine++;
      }
      if(btnState)
      {
        prevBtnState = true;
      }
      timer=millis();
  }

  value=obtainBinValue();
  digits[0]=(value)/100000;
  digits[1]=(value%100000)/10000;
  digits[2]=(value%10000)/1000;
  digits[3]=(value%1000)/100;
  digits[4]=(value%100)/10;
  digits[5]=value%10;

  for(int i=0;i<6;i++)
  {
    Serial.print(digits[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  printNumbers(digits[0],digits[3],1,0);
  delay(5);
  printNumbers(digits[1],digits[4],2,0);
  delay(5);
  printNumbers(digits[2],digits[5],3,0);
  updateSwitches();



  
 /* Serial.println(obtainBinValue());
  for(int i=15;i>=0;i--)
  {
    Serial.print(bytes[i]);
  }
  Serial.println();
  */
  
  
}

unsigned int obtainBinValue()
{
  unsigned int wynik = 0;
  unsigned int factor = 1;
  for(int i=0;i<16;i++)
  {
    wynik+=bytes[i]*factor;
    factor*=2;
  }
  return wynik;
}

void printNumbers(int left, int right,int pos, bool dot)
{
  // E,D,DP,C,G,A,F,B

  bool sdiData1[8];
  bool sdiData2[8];
  switch(left)
  {
    case 0:
    {
      bool temp[8]={1,1,0,1,0,1,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 1:
    {
      bool temp[8]={0,0,0,1,0,0,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 2:
    {
      bool temp[8]={1,1,0,0,1,1,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 3:
    {
      bool temp[8]={0,1,0,1,1,1,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 4:
    {
      bool temp[8]={0,0,0,1,1,0,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 5:
    {
      bool temp[8]={0,1,0,1,1,1,1,0};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 6:
    {
      bool temp[8]={1,1,0,1,1,1,1,0};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 7:
    {
      bool temp[8]={0,0,0,1,0,1,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 8:
    {
      bool temp[8]={1,1,0,1,1,1,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
    case 9:
    {
      bool temp[8]={0,1,0,1,1,1,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData1[i]=temp[i];
      }
      if(dot)
        sdiData1[2]=true;
      break;
    }
  }
  switch(right)
  {
    case 0:
    {
      bool temp[8]={1,1,0,1,0,1,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 1:
    {
      bool temp[8]={0,0,0,1,0,0,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 2:
    {
      bool temp[8]={1,1,0,0,1,1,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 3:
    {
      bool temp[8]={0,1,0,1,1,1,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 4:
    {
      bool temp[8]={0,0,0,1,1,0,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 5:
    {
      bool temp[8]={0,1,0,1,1,1,1,0};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 6:
    {
      bool temp[8]={1,1,0,1,1,1,1,0};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 7:
    {
      bool temp[8]={0,0,0,1,0,1,0,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 8:
    {
      bool temp[8]={1,1,0,1,1,1,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
    case 9:
    {
      bool temp[8]={0,1,0,1,1,1,1,1};
      for(int i = 0; i<8; i++)
      {
        sdiData2[i]=temp[i];
      }
      if(dot)
        sdiData2[2]=true;
      break;
    }
  }

  switch(pos)
  {
    case 1:
    {
        digitalWrite(DIGIT1,HIGH); 
        digitalWrite(DIGIT2,LOW);
        digitalWrite(DIGIT3,LOW);
        digitalWrite(DIGIT4,HIGH); 
        digitalWrite(DIGIT5,LOW);
        digitalWrite(DIGIT6,LOW);
        break;
    }
    case 2:
    {
        digitalWrite(DIGIT1,LOW); 
        digitalWrite(DIGIT2,HIGH);
        digitalWrite(DIGIT3,LOW);
        digitalWrite(DIGIT4,LOW); 
        digitalWrite(DIGIT5,HIGH);
        digitalWrite(DIGIT6,LOW);
        break;
    }
    case 3:
    {
        digitalWrite(DIGIT1,LOW); 
        digitalWrite(DIGIT2,LOW);
        digitalWrite(DIGIT3,HIGH);
        digitalWrite(DIGIT4,LOW); 
        digitalWrite(DIGIT5,LOW);
        digitalWrite(DIGIT6,HIGH);
        break;
    }
  }

  
  digitalWrite(OE,HIGH);
  for(int i=0;i<8;i++)
  {
    digitalWrite(SDI,sdiData2[i]);
    digitalWrite(CLK,HIGH);
    digitalWrite(CLK,LOW);
  }
  for(int i=0;i<8;i++)
  {
    digitalWrite(SDI,sdiData1[i]);
    digitalWrite(CLK,HIGH);
    digitalWrite(CLK,LOW);
  }

  digitalWrite(LA,HIGH);
  digitalWrite(LA,LOW);
  digitalWrite(OE,LOW);
}
void updateSwitches()
{
  bytes[0]  = mcp.digitalRead(15);
  bytes[1]  = mcp.digitalRead(14);
  bytes[2]  = mcp.digitalRead(13);
  bytes[3]  = mcp.digitalRead(12);
  bytes[4]  = mcp.digitalRead(11);
  bytes[5]  = mcp.digitalRead(10);
  bytes[6]  = mcp.digitalRead(9);
  bytes[7]  = mcp.digitalRead(8);
  bytes[8]  = mcp.digitalRead(7);
  bytes[9]  = mcp.digitalRead(6);
  bytes[10] = mcp.digitalRead(5);
  bytes[11] = mcp.digitalRead(4);
  bytes[12] = mcp.digitalRead(3);
  bytes[13] = mcp.digitalRead(2);
  bytes[14] = mcp.digitalRead(1);
  bytes[15] = mcp.digitalRead(0);  
}
