int analogPin1 = A1; 
int analogPin2 = A2;
int analogPin3 = A3;                    
// outside leads to ground and +5V
int val[3] = {0,0,0};  // variable to store the value read
int rowCount =30000;
int rowNumber = 0;
float cval[3] = {0,0,0};
int Ledp1 = 3;
int Ledp2 = 5;
int Ledp3 = 6;
float amb[3] = {0,0,0};
float led[3] = {0,0,0};
float pVolt[3] = {0,0,0};
float pertrans[3] = {0,0,0};
float OD[3] = {0,0,0};
int nrds = 30;

void setup() {
  Serial.begin(9600);  //  setup serial
  pinMode(Ledp1,OUTPUT);
  pinMode(Ledp2,OUTPUT);
  pinMode(Ledp3,OUTPUT);
}

void loop() {
 digitalWrite(Ledp1, LOW); //turns off LED
 digitalWrite(Ledp2, LOW);
 digitalWrite(Ledp3, LOW);
 delay(5000); //time to settle
  
  //reading sequence

  //get ambient readings
  for(int i=0;i<3;i++){
    amb[i] = 0;}
  
  for(int i=0; i<nrds ; i++){
    val[0] = analogRead(analogPin1);  // read the input pin
    val[1] = analogRead(analogPin2);
    val[2] = analogRead(analogPin3);
    amb[0] = amb[0] + val[0]/nrds;
    amb[1] = amb[1] + val[1]/nrds;
    amb[2] = amb[2] + val[2]/nrds;
    delay(5);
  }

  //get LED readings
  for(int i=0;i<3;i++){
    led[i] = 0;}

  digitalWrite(Ledp1, HIGH); //truns on LED
  digitalWrite(Ledp2, HIGH);
  digitalWrite(Ledp3, HIGH);
  delay(5000);//time to settle
  
  for(int i=0; i<nrds ; i++){
    val[0] = analogRead(analogPin1);
    val[1] = analogRead(analogPin2);
    val[2] = analogRead(analogPin3);
    led[0] = led[0] + val[0]/nrds;
    led[1] = led[1] + val[1]/nrds;
    led[2] = led[2] + val[2]/nrds;
    delay(5);
  }
  digitalWrite(Ledp1, LOW); //turns off LED
  digitalWrite(Ledp2, LOW);
  digitalWrite(Ledp3, LOW);
  
  cval[0] = led[0] - amb[0]; //corrected voltage values
  cval[1] = led[1] - amb[1];
  cval[2] = led[2] - amb[2];
  
  pVolt[0] = cval[0]/1024.0 * 5; //converts analog Read to units of Volts 1024units = 5V
  pertrans[0] = pVolt[0]/3.60 * 100; //percent MAX transmission(blank)=> 100% trans corresponds to 4V
  OD[0] = 2 - log10(pertrans[0]); // def of OD
  
  pVolt[1] = cval[1]/1024.0 * 5; //converts analog Read to units of Volts 1024units = 5V
  pertrans[1] = pVolt[1]/3.78 * 100; //percent MAX transmission(blank)=> 100% trans corresponds to 4V
  OD[1] = 2 - log10(pertrans[1]); // def of OD
  
  
  pVolt[2] = cval[2]/1024.0 * 5; //converts analog Read to units of Volts 1024units = 5V
  pertrans[2] = pVolt[2]/3.28 * 100; //percent MAX transmission(blank)=> 100% trans corresponds to 4V
  OD[2] = 2 - log10(pertrans[2]); // def of OD
  
  if (rowNumber < rowCount) {
    Serial.print(++rowNumber);
    Serial.print("\t Gluc:");
    Serial.print(OD[0]);
    Serial.print("\t 1x:");    
    Serial.print(OD[1]);
    Serial.print("\t 2x:");
    Serial.println(OD[2]);
    }
  delay(120000);// debug value
}
