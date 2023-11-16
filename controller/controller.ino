#include <Servo.h>
#include <NewPing.h>
#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define LASER_PIN 13

NewPing sonar(TRIGGER_PIN, ECHO_PIN);
Servo xServo;  // create servo object to control a servo
Servo yServo;  // create servo object to control a servo

int xAxisPot = A0; // servo 1

int yAxisPot = A1; // servo 2

int valX; 
int valY; 

int front_limit = 11;
int back_limit = 8;
int left_right_limit = 10;

float speedX=5;
float speedX2=10;

void setup() {
    Serial.begin(9600);
    xServo.attach(9);  
    yServo.attach(10);  
    resetServo();

}

void rotateX(){
  xServo.write(valX);                 
}
void rotateY(){
  yServo.write(valY);                 
}

void shoot(){ 

  Serial.println("shoot");
  digitalWrite(LASER_PIN,HIGH);
  delay(1000);
  digitalWrite(LASER_PIN,LOW);
  delay(1000);
}

void handleCommand(char command) {
  switch (command) {
    case 'D':
      valX += speedX;
      break;
    case 'A': 
      valX -= speedX;
      break;
    case 'W':
      valY += speedX2;
      break;
    case 'S': 
      valY -= speedX2;
      break;
    case 'R': 
      resetServo();
      break;
    case 'X': 
      shoot();
      break;

  }
}
void resetServo(){
  valX=100;
  rotateX();
  delay(500);
  valY=100;
  rotateY();
}

void loop() {

  if (Serial.available() > 0) {
    char command = Serial.read();
    handleCommand(command);
  }


  //rutine1_normal();


  rotateX();
  rotateY();

  shoot();

  printDistance();

  // Serial.println("x:");        
  // Serial.print(valX);

  // delay(100);
}


void printDistance() {
  long duration;
  int distance;

  // Generar un pulso ultrasónico para medir la distancia
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Medir el tiempo de ida y vuelta del eco
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calcular la distancia en centímetros
  distance = duration * 0.034 / 2; // La velocidad del sonido en el aire es de aproximadamente 0.034 cm/microsegundo

  // Mostrar la distancia en el puerto serie
  Serial.print("Distancia:");
  Serial.print(distance);
  Serial.println("");

  //delay(20); // Espera 500 milisegundos antes de realizar la siguiente medición
}

//int front_limit = 11;
//int back_limit = 8;
//int left_right_limit = 10;
// <>
void rutine1_normal(){

    // mitad delantera
    for(int i = 0; i<front_limit; i++){
      valX += speedX;
      rotateX();
      delay(100);
    }

    // derecha
    for(int i = 0; i<left_right_limit; i++){
      valY += speedX2;
      rotateY();
      delay(100);
    }

    // centro
    for(int i = 0; i<left_right_limit; i++){
      valY -= speedX2;
      rotateY();
      delay(100);
    }

    // izquierda
    for(int i = 0; i<left_right_limit; i++){
      valY -= speedX2;
      rotateY();
      delay(100);
    }

    // centro
    for(int i = 0; i<left_right_limit; i++){
      valY += speedX2;
      rotateY();
      delay(100);
    }

    // atras centro
    for(int i = 0; i<front_limit; i++){
      valX -= speedX;
      rotateX();
      delay(100);
    }

    // atras atras
    for(int i = 0; i<front_limit; i++){
      valX -= speedX;
      rotateX();
      delay(100);
    }


// derecha
    for(int i = 0; i<left_right_limit; i++){
      valY += speedX2;
      rotateY();
      delay(100);
    }

    // centro
    for(int i = 0; i<left_right_limit; i++){
      valY -= speedX2;
      rotateY();
      delay(100);
    }

    // izquierda
    for(int i = 0; i<left_right_limit; i++){
      valY -= speedX2;
      rotateY();
      delay(100);
    }

    // centro
    for(int i = 0; i<left_right_limit; i++){
      valY += speedX2;
      rotateY();
      delay(100);
    }


    // segunda mitad delantera
    for(int i = 0; i<front_limit; i++){
      valX += speedX;
      rotateX();
      delay(100);
    }


}

/*

valX = analogRead(xAxisPot);           
valX = map(valX, 0, 1023, 0, 180);    
valY = analogRead(yAxisPot);           
valY = map(valY, 0, 1023, 0, 180);    
xServo.write(valX);                 
yServo.write(valY);

Serial.print("x:");        
Serial.print(valX);
Serial.print(",");      
Serial.print("y:");        
Serial.print(valY);
Serial.println();
*/