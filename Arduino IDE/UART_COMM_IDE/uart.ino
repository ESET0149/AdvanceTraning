#define LED_PIN 2
 
String inputString = "";
bool stringComplete = false;
 
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);
  Serial.println("Please Send on or off command in the Serial Monitor!");
}
 
void loop() {
  // put your main code here, to run repeatedly:
  if(stringComplete){
    inputString.trim();
    if(inputString.equalsIgnoreCase("off")){
      digitalWrite(LED_PIN,LOW);
      Serial.println("LED_OFF");
    }
    else if(inputString.equalsIgnoreCase("on")){
      digitalWrite(LED_PIN,HIGH);
      Serial.println("LED_ON");
    }
    else{
      Serial.println("No Such Command Exists, try \"on\" or \"off\"");
    }
    inputString = "";
    stringComplete = false;
  }
 
  while(Serial.available()){
    char inChar = (char)Serial.read();
    if(inChar== '\n' || inChar=='\r'){
      stringComplete = true;
    }
    else{
      inputString += inChar;
    }
  }
}