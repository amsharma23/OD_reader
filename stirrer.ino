//Initializing LED Pin
int led_pin = 3;
int ipp1 = 9;
int ipp2 = 10;
int ipp3 = 11;
void setup() {
  //Declaring LED pin as output
  pinMode(led_pin, OUTPUT);
  pinMode(ipp1, OUTPUT);
  pinMode(ipp2, OUTPUT);
  pinMode(ipp3, OUTPUT);
}
void loop() {
  analogWrite(led_pin,60); //from 0 to 255
  //digitalWrite(ipp1,LOW);
  //analogWrite(led_pin,100);
}
