#define E1 10
#define I1 8      //CODE OF THE BLUETOOH CAR//
#define E2 5      //IT ALSO AVOIDES OBSTACLES THANKS TO ITS ULTRASONIC SENSOR//
#define I3 7
#define I4 6

const int Trigger=12;
const int Echo=13;
int valor;

void setup() {
  // put your setup code here, to run once:

for(int i=8;i<11;i++)   //WE SETUP THE PINES//
pinMode(i,OUTPUT);

pinMode(Echo,INPUT);
pinMode(Trigger,OUTPUT);

Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
valor=Serial.read();


int cm=ping(Trigger,Echo);  //WE CALL THE FUNCTION TO OBTAIN THE DISTANCE OF THE SENSOR//

if(cm<200 && cm>0){                  //IF THE SENSOR DETECTS SOMETHING WITHIN 10cm, THE CAR MUST TURN LEFT TO AVOID THE OBSTACLE//
  
  digitalWrite(E2,HIGH);
  digitalWrite(I3,HIGH);              //TURN LEFT//
  digitalWrite(I4,LOW);

  digitalWrite(E1,HIGH);
  digitalWrite(I1,LOW);    
  digitalWrite(I2,HIGH);

  delay(500);

  
  digitalWrite(E1,LOW);   

  digitalWrite(E2,LOW);   //STOP MOVING//
  digitalWrite(I3,LOW);   
  digitalWrite(I4,LOW);
}  


if(valor=='F' && cm>=200){

digitalWrite(E1,HIGH);
digitalWrite(I1,HIGH);    //WHEEL 1 GO AHEAD//
digitalWrite(I2,LOW);

digitalWrite(E2,HIGH);
digitalWrite(I3,HIGH);    //WHEEL 2 GO AHEAD//
digitalWrite(I4,LOW);

}

if(valor=='B' && cm>=200){

digitalWrite(E1,HIGH);
digitalWrite(I1,LOW);    
digitalWrite(I2,HIGH);

digitalWrite(E2,HIGH);
digitalWrite(I3,LOW);    
digitalWrite(I4,HIGH);

}

if(valor=='L' && cm>=200){
  digitalWrite(E2,HIGH);
  digitalWrite(I3,HIGH);    
  digitalWrite(I4,LOW);

  digitalWrite(E1,HIGH);
  digitalWrite(I1,LOW);    
  digitalWrite(I2,HIGH);
}

if(valor=='R' && cm>=200){
  digitalWrite(E1,HIGH);
  digitalWrite(I1,HIGH);    
  digitalWrite(I2,LOW);

  digitalWrite(E2,HIGH);
  digitalWrite(I3,LOW);    
  digitalWrite(I4,HIGH);
}


if(valor=='S' && cm>=200){

  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);
  digitalWrite(I3,LOW);   
  digitalWrite(I4,LOW);
}
}




int ping(int Trigger, int Echo){  //THIS FUNCTION RETURNS THE VALUES FROM THE ULTRASONIC SENSOR//
  long duration, distancia;
  
  digitalWrite(Trigger,LOW);

  delayMicroseconds(4);
  digitalWrite(Trigger,HIGH);

  delayMicroseconds(10);
  digitalWrite(Trigger,LOW);

  duration=pulseIn(Echo,HIGH);

  distancia=duration * 10 / 29,2 / 2;

  return distancia;
}


