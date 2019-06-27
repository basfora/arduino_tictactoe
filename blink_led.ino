int my_led = 12;

void setup() {
  // put your setup code here, to run once:
 pinMode(my_led, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  Serial.println("Blinking LED");
  // put your main code here, to run repeatedly:
    digitalWrite(my_led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(my_led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
