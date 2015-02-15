[code]
#define VCC  12
#define OUT  10
#define S2   8
#define S3   9
#define S0   6
#define S1   7

#define  Filters_R  0
#define  Filters_G  1
#define  Filters_B  2

long count = 0;
int counter = 0;
int G_flag = 1;
int RGB = 0;
float scaleFactor[3];

/******************************************
* IMPORTANT - Make sure you calibrate
* your sensor for White Balance initially
* e.g. place your TCS3200 in front
* of a piece of white paper
*******************************************/

void setup()
{
  pinMode(OUT, INPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
 
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH);


  Serial.begin(9600);
  Serial.println("Starting...");
  
  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++)
  {
    Select_Filters(i);
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
    scaleFactor[i] = 255.0/count;

    Serial.print("count: ");
    Serial.println(count);
    Serial.print("counter: ");
    Serial.println(counter);
    Serial.print("scaleFactor: ");
    Serial.println(scaleFactor[i],9);    
    counter=0;
    count=0;
  }
  
  digitalWrite(VCC, LOW);

}

void loop()
{
  Serial.println("loop");
  
  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++)
  {
    RGB=i;
    Select_Filters(i);
    count=0;
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
    Serial.print("value: ");
    Serial.println(scaleFactor[i]*count);
    Serial.print("counter ");
    Serial.println(counter);
    counter=0;
    count=0;
  }
  
  digitalWrite(VCC, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);  
  
  delay(6000);
  digitalWrite(S1, HIGH);
}

void Select_Filters(int RGB)
{
  switch(RGB)
  {
    case Filters_R:          //Red
    digitalWrite(S2, LOW);    
    digitalWrite(S3, LOW);
    Serial.println("-----select Red color");
    break;
    
    case Filters_G:          //Green
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, HIGH);
    Serial.println("-----select Green color");
    break;
    
    case Filters_B:          //Blue
    digitalWrite(S2, LOW);    
    digitalWrite(S3, HIGH);
    Serial.println("-----select Blue color");
    break;
    
    default:                  //Clear(no filter)
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, LOW);
    Serial.println("-----no filter");
    break;
  }
}

[/code]
