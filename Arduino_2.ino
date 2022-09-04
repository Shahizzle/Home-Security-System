int buzzer = 8;
int motionled = 7;
int nomotionled = 13;
int pin = 2;

int value = 0;
int pirState = LOW;
int cmd = 0;

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(motionled,OUTPUT);
  pinMode(nomotionled,OUTPUT);
  pinMode(pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  value = digitalRead(pin);
  delay(5000);

  if (value == 1)
  {
    Serial.println("1");
    digitalWrite(motionled,HIGH);
    digitalWrite(nomotionled,LOW);
    while ( !cmd )
    {
      tone(buzzer, 1000);
      delay(100);
      tone(buzzer,500);
      delay(100);
  String sentInfo = Serial.readStringUntil('\r\n');
  cmd = sentInfo.toInt();
    }
    noTone(buzzer);

  }
  else if (value == 0)
  {
    Serial.println("0");
    digitalWrite(motionled,LOW);
    digitalWrite(nomotionled,HIGH);
    noTone(buzzer);
    delay(1000);
  }

}
