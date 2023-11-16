// Definición de pines del sensor ultrasónico
const int trigPin = 5;  // Pin TRIG conectado al pin 9 de Arduino
const int echoPin = 6; // Pin ECHO conectado al pin 10 de Arduino

// Variables para almacenar la duración y la distancia
long duracion;
int distancia;

void setup() {
  Serial.begin(9600); // Iniciar comunicación serial a 9600 baudios
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Envía un pulso ultrasónico para activar el sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lee el tiempo que tarda en llegar el eco
  duracion = pulseIn(echoPin, HIGH);

  // Calcula la distancia en centímetros (la velocidad del sonido es 343m/s -> 1cm = 29.1us)
  distancia = duracion * 0.034 / 2;

  // Muestra la distancia medida por el sensor en el Monitor Serie
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(1000); // Espera 1 segundo antes de tomar la próxima lectura
}
