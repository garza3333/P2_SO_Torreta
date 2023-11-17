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

#define MAX_COMMAND_LENGTH 50 // Define la longitud máxima del comando

char receivedChars[MAX_COMMAND_LENGTH]; // Almacena el string recibido
boolean newData = false;

arduinoFFT FFT;

Servo xServo;  // create servo object to control a servo
Servo yServo;  // create servo object to control a servo
int xAxisPot = A0; // servo 1
int yAxisPot = A1; // servo 2

int valX; // posición servo 1
int valY; // posición servo 2

int front_limit = 11; // maxima cantidad de giros de servomotor 1 hacia adelante
int back_limit = 8; // maxima cantidad de giros de servomotor 1 hacia atras
int left_right_limit = 10; // maxima cantidad de giros del servomotor 2 hacia izquierda y derecha

float speedX=5; // Velocidad servomotor 1: Up Ddown
float speedX2=10; // Velocidad servo motor 2: Left Rright
float moveDelay=100;

int audioRecordIndex; // moverse en el buffer de audio

const int b_av_size = 20; // maximo de 20 acciones 
int distance;

char action_buffer[b_av_size][7]; // buffer de acciones
int b_a_index = 0;

int value_buffer[b_av_size]; // buffer to store instructions values
int b_v_index = 0; // current value buffer position

char current_inst[7]; // Tamaño de la instruccion
int current_val;

bool inaction = false;  // flag de control de accion de la torreta


void setup() {

    Serial.begin(9600);
    xServo.attach(SERVO_X);  
    yServo.attach(SERVO_Y);  
    resetServo();
    clean_buffers();

}

void loop() {

  double vReal[SAMPLES]; // Variables para el uso del micro
  double vImag[SAMPLES];

  recvWithEndMarker();
  showNewData();

  printDistance();

  delay(1000);

}



void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n'; // Carácter de final de línea ('\n')
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= MAX_COMMAND_LENGTH) {
        ndx = MAX_COMMAND_LENGTH - 1;
      }
    } else {
      receivedChars[ndx] = '\0'; // Agrega el carácter nulo al final del string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    
    splitCommand(receivedChars, current_inst, current_val);

    Serial.println(receivedChars);
    Serial.print("Instruction: ");
    Serial.println(current_inst);
    Serial.print("Value: ");
    Serial.println(current_val);

    add_action(current_inst, current_val);
    do_action(current_inst, current_val);

    newData = false;
  }
}

void update_values(int x, int y, bool a){
    inaction = a;
    valX = x;
    valY = y;

}

void add_action(char act[7], int val){

  Serial.print("addaction\n");
  if (b_a_index < b_av_size) {
    Serial.print(b_a_index);
    Serial.print(b_av_size);
    strncpy(action_buffer[b_a_index], act, 7); // Copiar el contenido de 'act' a 'action_buffer'
    value_buffer[b_v_index] = val;

    b_a_index++;
    b_v_index++;
  }
  

}

void do_action(const char inst[7], int val) {

  Serial.print("doaction\n");
  if (b_a_index > 0) {

      b_a_index--;
      b_v_index--;

    }


  if (b_a_index < b_av_size) {
    
    Serial.print("excecute\n");
    handleCommand(current_inst, current_val);

    strcpy(action_buffer[b_a_index], "nonins");
    value_buffer[b_v_index] = 0;

    // Actualiza el comando actual y el valor
    strcpy(current_inst, action_buffer[b_a_index]);
    current_val = value_buffer[b_v_index];
  }

}

void clean_buffers() {
  // Reseteando el buffer de acciones
  for (int i = 0; i < b_av_size; i++) {
    strcpy(action_buffer[i], "nonins");
  }

  // Reseteando el buffer de valores
  for (int i = 0; i < b_av_size; i++) {
    value_buffer[i] = 0;
  }
}
void getSensorsData(){
  Serial.print("rotX:"+ String(valX) + "rotY:" + String(valY) + "distance:" +String(distance)  );
  
}

void handleCommand(char command[7], int val) {

  // Ejemplo de comparación con un string literal
    if (strcmp(command, "rotateX") == 0) {

      rotateX(current_val);
        
    } else if (strcmp(command,"rotateY") == 0){

      rotateY(current_val);

    } else if (strcmp(command,"shooter") == 0){

      shoot();
        
    } else if (strcmp(command,"reseter") == 0){

      resetServo();
        
    } else if (strcmp(command,"getSenD") == 0){

      getSensorsData();
        
    } else{
      Serial.println("Comando no reconocido");
    }
    
}

void resetServo(){
  valX=100;
  rotateX(0);
  delay(500);
  valY=100;
  rotateY(0);
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



void rotateX(float angle){
  Serial.println("Rotate");
  valX+=angle;
  xServo.write(valX);  
  moveSoundEffect();               
}
void rotateY(float angle){
  valY+=angle;
  yServo.write(valY);                 
  moveSoundEffect();
}

void splitCommand(const char *command, char *instruction, int &value) {
  // Copiar la cadena original a una variable temporal
  char commandCopy[strlen(command) + 1];
  strcpy(commandCopy, command);

  // Usar strtok para dividir la cadena usando ':'
  char *token = strtok(commandCopy, ":");

  // Verificar si se encontró el token
  if (token != NULL) {
    // Copiar la primera parte (instrucción) a la variable correspondiente
    strcpy(instruction, token);

    // Obtener el siguiente token (valor)
    token = strtok(NULL, ":");

    // Verificar si se encontró el segundo token y convertirlo a un número entero
    if (token != NULL) {
      value = atoi(token);
    }
  }
}




