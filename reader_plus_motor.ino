//Initializing Pin
int motor = 9; //motor pin 9
int analogPin1 = A0; 
int analogPin2 = A1;
int analogPin3 = A2;                    
int Ledp1 = 23;
int Ledp2 = 25;
int Ledp3 = 27;

int val[3] = {0,0,0}; // variable to store the current output
float cval[3] = {0,0,0};//corrected values
float amb[3] = {0,0,0}; //ambient light values
float led[3] = {0,0,0}; //when led is ON
float pVolt[3] = {0,0,0};
float pertrans[3] = {0,0,0};
float OD[3] = {0,0,0};
float ODMin[3][3] ={{0,0.0},{0,0.0},{0,0.0}}; 

int nrd = 5;
int nrds = 30;

int rowCount = 30000;
int rowNumber = 0;

void setup() {
  Serial.begin(9600);  //  setup serial
  pinMode(Ledp1,OUTPUT);
  pinMode(Ledp2,OUTPUT);
  pinMode(Ledp3,OUTPUT);
  pinMode(motor, OUTPUT);
}

void loop() {


 analogWrite(motor, 0); //turn OFF motors
 delay(5000);
  
//reading sequence

//we average across 5 reading; each of which is itself composed of 30 readings
for (int j=0;j<nrd;j++){ 
  
  //get ambient readings
  for(int i=0;i<3;i++){amb[i] = 0;}
 
  digitalWrite(Ledp1, LOW); //turns OFF LED
  digitalWrite(Ledp2, LOW);
  digitalWrite(Ledp3, LOW);
  delay(10); //time to settle
 
  
  for(int i=0; i<nrds ; i++){
      val[0] = analogRead(analogPin1);  // read the input pin
      val[1] = analogRead(analogPin2);
      val[2] = analogRead(analogPin3);
      amb[0] = amb[0] + val[0];
      amb[1] = amb[1] + val[1];
      amb[2] = amb[2] + val[2];
      delay(5);
    }
   amb[0] = amb[0]/nrds;
   amb[1] = amb[1]/nrds;
   amb[2] = amb[2]/nrds;

  //get LED readings
  for(int i=0;i<3;i++){led[i] = 0;}

  digitalWrite(Ledp1, HIGH); //truns on LED
  digitalWrite(Ledp2, HIGH);
  digitalWrite(Ledp3, HIGH);
  delay(10);//time to settle
  
  for(int i=0; i<nrds ; i++){
    val[0] = analogRead(analogPin1);
    val[1] = analogRead(analogPin2);
    val[2] = analogRead(analogPin3);
    led[0] = led[0] + val[0]/nrds;
    led[1] = led[1] + val[1]/nrds;
    led[2] = led[2] + val[2]/nrds;
    delay(5);
  }
  led[0] = led[0]/nrds;
  led[1] = led[1]/nrds;
  led[2] = led[2]/nrds;

 
  digitalWrite(Ledp1, LOW); //turns off LED
  digitalWrite(Ledp2, LOW);
  digitalWrite(Ledp3, LOW);
  
  cval[0] += (led[0] - amb[0]); //corrected voltage values
  cval[1] += (led[1] - amb[1]);
  cval[2] += (led[2] - amb[2]);
}

  cval[0] /= 5; //corrected voltage values
  cval[1] /= 5;
  cval[2] /= 5;

  
  pVolt[0] = cval[0]/1024.0 * 5; //converts analog Read to units of Volts 1024units = 5V
  pertrans[0] = pVolt[0]/3.60 * 100; //percent MAX transmission(blank)=> 100% trans corresponds to 4V
  OD[0] = 2 - log10(pertrans[0]); // def of OD
  
  pVolt[1] = cval[1]/1024.0 * 5; //converts analog Read to units of Volts 1024units = 5V
  pertrans[1] = pVolt[1]/3.78 * 100; //percent MAX transmission(blank)=> 100% trans corresponds to 4V
  OD[1] = 2 - log10(pertrans[1]); // def of OD
  
  
  pVolt[2] = cval[2]/1024.0 * 5; //converts analog Read to units of Volts 1024units = 5V
  pertrans[2] = pVolt[2]/3.28 * 100; //percent MAX transmission(blank)=> 100% trans corresponds to 4V
  OD[2] = 2 - log10(pertrans[2]); // def of OD

  
  for (int i = 0;i<=2;i++) {                   // move data down for 9 min avg
    for (int j=2;j>=1;j--){
      ODMin[i][j] = ODMin[i][j-1];
      }
   }

  for (int i = 0;i<=2;i++) {
    ODMin[i][0] = (OD[i] + ODMin[i][1] + ODMin[i][2]); 
  }
 

  


  if (rowNumber < rowCount) {
    Serial.print(++rowNumber);
    Serial.print("\t 1:");
    Serial.print(OD[0]);
    Serial.print("\t 2:");    
    Serial.print(OD[1]);
    Serial.print("\t 3:");
    Serial.println(OD[2]);
    Serial.print(++rowNumber);
    Serial.print("\t 1(6min avg):");
    Serial.print(ODMin[0][0]);
    Serial.print("\t 2(6min avg):");    
    Serial.print(ODMin[1][0]);
    Serial.print("\t 3(6min avg):");
    Serial.println(ODMin[2][0]);
    }

  analogWrite(motor,85); //"kick-start" motors
  delay(3000);
  analogWrite(motor,55); //turns motor ON to normal speed 
  
  delay(10000);// measure every value every 3mins
}
