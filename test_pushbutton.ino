int ledPin = 13; 
int buttonPin = 2;
int prevButtonValue = 0;
int buttonValue = 0;

void setup( ) {

pinMode(buttonPin, INPUT);
pinMode(ledPin, OUTPUT);
Serial.begin(9600);
}

void loop( )
{
buttonValue = digitalRead(buttonPin);

  if (buttonValue != prevButtonValue && buttonValue == 1) {
     Serial.println("Button Pressed");
     digitalWrite(ledPin, HIGH);
     delay(1000);
     digitalWrite(ledPin, LOW);    
  } 
}
