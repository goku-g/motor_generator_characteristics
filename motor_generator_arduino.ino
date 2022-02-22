
const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference
unsigned long refresh; // To store time for refresh of reading

int rpm; // RPM value

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan
int ln1 = 8;
int ln2 = 9;
int enA = 10;

float v_out = 0.0;
float v_in = 0.0;
float r1 = 30000.0;
float r2 = 7500.0;
int value = 0;

const int voltage = A0;
const int back_emf = A1;

int i = 50;

float measure_voltage(int sensor_pin)
{
  value = analogRead(sensor_pin);
  // Serial.println(value);
  v_out = (value * 5.0) / 1024.0;
  v_in = v_out / (r2/(r1+r2));
  // Serial.print("Input = ");
  // Serial.println(v_in);
  return v_in;
}

float measure_rpm(int dataIN)
{
  // RPM Measurement
  currentstate = digitalRead(dataIN); // Read IR sensor state
  if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         duration = ( micros() - prevmillis ); // Time difference between revolution in microsecond
         rpm = (60000000/duration); // rpm = (1/ time millis)*1000*1000*60;
         prevmillis = micros(); // store time for nect revolution calculation
       }
   }
  prevstate = currentstate; // store this scan (prev scan) data for next scan
  
  // LCD Display
  if( ( millis()-refresh ) >= 100 )
    {
       return rpm;
       //Serial.println(rpm);  
    }
}

void setup()
{
  
  pinMode(dataIN,INPUT);  
  pinMode(ln1,OUTPUT);
  pinMode(ln2,OUTPUT);
  pinMode(enA,OUTPUT);    
  prevmillis = 0;
  prevstate = LOW;  
  Serial.begin(115200);
}
void loop()
{
  
  if(i<=255)
  {
    digitalWrite(ln1,HIGH);
    digitalWrite(ln2,LOW);
    analogWrite(enA,i);

    double prevtime = micros();
    while(micros() - prevtime <= 2000000)
    {
      // Serial.println(micros() - prevtime);
      float rpm  = measure_rpm(dataIN);
      
      float in_voltage = measure_voltage(voltage);
      float out_voltage = measure_voltage(back_emf);
  
      Serial.print("RPM ");
      Serial.println(rpm);
      Serial.print("Voltage_IN ");
      Serial.println(in_voltage);
      Serial.print(" Voltage_OUT ");
      Serial.println(out_voltage);
         
    } 
  }
  else
  {
    i = 45;
  }
  i = i + 5;
}
