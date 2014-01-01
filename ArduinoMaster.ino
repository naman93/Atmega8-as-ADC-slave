int statePin = 2; // used by slave to track if master is in initialization mode or data receive mode 
int masterClock = 4; // used to synchronize data transfer from slave to master
int dataPin = 5; // data is transferred on this line, one bit at a time

int data[40]; // used to hold data received from slave

// tells slave that master is ready to receive data
void init_data_receive()
{
  digitalWrite(statePin,HIGH); // master tells slave that it is in initialization state by drivind state pin high
  digitalWrite(masterClock,HIGH);
  delayMicroseconds(200);
  digitalWrite(masterClock,LOW);
  delay(2);
}

// gives a single pulse on master clock line
void pulse()
{
  digitalWrite(masterClock,HIGH);
  delayMicroseconds(500);
  digitalWrite(masterClock,LOW);
  delay(2);
}
  
// function used to receive data from slave
void getData(void)
{
  float decimalData1 = 0.0;
  float decimalData2 = 0.0;
  
  init_data_receive();
  
  digitalWrite(statePin,LOW); // master tells slave that it is in data receive mode
  
  for (int i=0; i<=39; i++) // 40 bits of data. data from 4 adc pins on salve are read and transferred. 10 bits used to represent data from each pin 
  {
    pulse();
    
    if(digitalRead(5) == HIGH) // read the state of data pin and store it
    {
      data[i] = 1;
    }
    else
    {
      data[i] = 0;
    }
  }
  
  for (float i=20.0; i<30.0; i=i+1.0) // conversion from binary to decimal
  {
    if ((data[(int)i]) == 1)
    {
      decimalData1 = decimalData1 + pow(2.0,(i-20.0));
    }
  }
  
  for (float i=30.0; i<40.0; i=i+1.0)
  {
    if ((data[(int)i]) == 1)
    {
      decimalData2 = decimalData2 + pow(2.0,(i-30.0));
    }
  }
  
  Serial.print(decimalData1);
  Serial.print("      ");
  Serial.print(decimalData2);
  Serial.println();
}
    

void setup()
{
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,INPUT);
  
  Serial.begin(9600);
  while(!Serial) // wait until serial communication is initialized
  {
    ;
  }
  
  delay(300); // time given for slave to initialize
}

void loop()
{
    getData();
}


