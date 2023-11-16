#include <arduinoFFT.h>
#include <Servo.h>
#define SERVO_Y 10
#define SERVO_X 9
#define LASER_PIN 8
#define ECHO_PIN 7
#define TRIGGER_PIN 6
#define SOUND_EFFECT_PIN 4
#define SAMPLES 1024  // Número de muestras para la FFT
#define SAMPLING_FREQUENCY 10000  // Frecuencia de muestreo en Hz
arduinoFFT FFT;
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
float moveDelay=100;

int audioRecordIndex;

void setup() {
    Serial.begin(9600);
    xServo.attach(SERVO_X);  
    yServo.attach(SERVO_Y);  
    resetServo();
}

void loop() {

  double vReal[SAMPLES]; // Variables para el uso del micro
  double vImag[SAMPLES];

  //recordAudio(vReal);

  
  if (Serial.available() > 0) {
    char command = Serial.read();
    handleCommand(command);
  }

  

  //rutine1_normal();
  //shoot();

  printDistance();

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

void shoot(){ 

  Serial.println("shoot");
  digitalWrite(LASER_PIN,HIGH);
  shootSoundEffect();
  delay(1000);
  digitalWrite(LASER_PIN,LOW);
  delay(1000);
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


void rutine1_normal(){

    // mitad delantera
    for(int i = 0; i<front_limit; i++){
      valX += speedX;
      rotateX();
      delay(moveDelay);
    }

    // derecha
    for(int i = 0; i<left_right_limit; i++){
      valY += speedX2;
      rotateY();
      delay(moveDelay);
    }

    // centro
    for(int i = 0; i<left_right_limit; i++){
      valY -= speedX2;
      rotateY();
      delay(moveDelay);
    }

    // izquierda
    for(int i = 0; i<left_right_limit; i++){
      valY -= speedX2;
      rotateY();
      delay(moveDelay);
    }

    // centro
    for(int i = 0; i<left_right_limit; i++){
      valY += speedX2;
      rotateY();
      delay(moveDelay);
    }

    // atras centro
    for(int i = 0; i<front_limit; i++){
      valX -= speedX;
      rotateX();
      delay(moveDelay);
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


void recordAudio(double* vReal) {
  
    vReal[audioRecordIndex] = analogRead(A0);
    audioRecordIndex+=1;
    if(audioRecordIndex>= SAMPLES)
    {
      audioRecordIndex=0;
    }
  
}



void shootSoundEffect(){
  playTone(1000, 30);  // Frecuencia de 1000 Hz durante 30 ms
  playTone(1500, 20);  // Frecuencia de 1500 Hz durante 20 ms
  playTone(2000, 10);  // Frecuencia de 2000 Hz durante 10 ms
}
void moveSoundEffect(){

  // playTone(2000, 1);  // Frecuencia de 2000 Hz durante 10 ms
}
void alertSoundEffect(){
  playTone(1000, 30);  // Frecuencia de 1000 Hz durante 30 ms
  playTone(1500, 20);  // Frecuencia de 1500 Hz durante 20 ms
  playTone(2000, 10);  // Frecuencia de 2000 Hz durante 10 ms
}

void playTone(int frequency, int duration) {
  tone(SOUND_EFFECT_PIN, frequency, duration);
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // Esperar hasta que la duración del tono haya transcurrido
  }
  noTone(SOUND_EFFECT_PIN);
}



void rotateX(){
  xServo.write(valX);  
  moveSoundEffect();               
}
void rotateY(){
  yServo.write(valY);                 
  moveSoundEffect();
}



