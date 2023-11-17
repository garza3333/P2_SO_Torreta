#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <fftw3.h>
#include <mpi.h>

#define SERIAL_PORT "/dev/ttyUSB0"

#define AUDIO_SAMPLES 120
#define TARGET_DISTANCE 8
#define WASP_FREQ 440
#define SHOOT_ERROR_TOLERANCE 0.5

void protectBees();
void warden();
void shoot();
char* getSensorData();
double getFrequency(float audio[AUDIO_SAMPLES]);
void rotateX(int degrees);
void rotateY(int degrees);
void sendMessage(char* message, int length);

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int sampleRate = 60;
    while(1){
        protectBees();
        usleep(round(1000000/sampleRate));
    }

    MPI_Finalize();
    return 0;
}

void protectBees(){
    char* data = getSensorData();
    int distance = atoi(strstr(data,"dist:") + strlen("dist:"));
    char* frequency = strstr(data,"micropData:") + strlen("micropData:");
    if(distance <= TARGET_DISTANCE){
        if (abs(getFrequency(frequency) - WASP_FREQ) <= SHOOT_ERROR_TOLERANCE){
            shoot();
        } else {
            warden();
        }
    } else {
        warden();
    }
}

void warden (){
    int movesY = 5;
    int movesX = 6;
    for (size_t i = 0; i < movesY; i++){
        rotateY(20);
        for (size_t j = 0; j <= movesX; j++){
            if(j < (movesX/2) || j > (movesX/2)){
                rotateX(-30);
            } else {
                rotateX(30*(movesX));
            }
        }
    }
}

void shoot(){
    char* message = "shooter:0";
    sendMessage(message, strlen(message));
}

char* getSensorData(){
    char message[] = "getSenD:0";
    sendMessage(message, sizeof(message));

    int serial_port = open(SERIAL_PORT, O_RDONLY);

    if (serial_port == -1) {
        perror("Error abriendo el puerto serie");
        return NULL;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(serial_port, &tty) != 0) {
        perror("Error obteniendo la configuración del puerto serie");
        close(serial_port);
        return NULL;
    }

    // Configuración del puerto serie
    tty.c_cflag &= ~PARENB;  // Sin paridad
    tty.c_cflag &= ~CSTOPB;  // 1 bit de parada
    tty.c_cflag |= CS8;      // 8 bits de datos

    cfsetospeed(&tty, B9600); // Velocidad de transmisión (en baudios)
    cfsetispeed(&tty, B9600);

    // Configura el puerto serie con la nueva configuración
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        perror("Error configurando el puerto serie");
        close(serial_port);
        return NULL;
    }

    // Lectura del puerto serie
    char* read_buf = (char*)malloc(256);
    memset(read_buf, '\0', 256);

    while (1) {
        ssize_t num_bytes = read(serial_port, read_buf, 256);

        if (num_bytes > 1) {
            printf("Mensaje recibido: %s", read_buf);
            memset(read_buf, '\0', 256);
            break;
        } else {
            printf("Leyendo del puerto serie...\n");
        }
        usleep(200000);  // 200 milisegundos
    }

    // Cierra el puerto serie
    close(serial_port);

    return read_buf;
}

double getFrequency(float audio[AUDIO_SAMPLES]){
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Crea un vector de entrada
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * AUDIO_SAMPLES);
    for (int i = 0; i < AUDIO_SAMPLES; ++i) {
        in[i][0] = audio[i];
        in[i][1] = 0.0;
    }

    // Divide el vector entre los procesos
    int local_elements = AUDIO_SAMPLES / size;
    double* local_in = (double*) malloc(local_elements * sizeof(double));
    // Trocea un mensaje en partes iguales y los envía individualmente al resto de procesos y a sí mismo
    MPI_Scatter(in, local_elements, MPI_DOUBLE, local_in, local_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Configura la transformada de Fourier
    fftw_plan plan;
    fftw_complex* out = (fftw_complex*) fftw_malloc(AUDIO_SAMPLES * sizeof(fftw_complex));
    plan = fftw_mpi_plan_dft_1d(AUDIO_SAMPLES, local_in, out, MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE);

    // Ejecuta la transformada de Fourier
    fftw_execute(plan);

    // Imprime el resultado desde el proceso 0
    if (rank == 0) {
        printf("Transformada de Fourier:\n");
        for (int i = 0; i < AUDIO_SAMPLES; ++i) {
            printf("Re(%d): %f, Im(%d): %f\n", i, out[i][0], i, out[i][1]);
        }
    }

    // Limpia y finaliza
    fftw_destroy_plan(plan);
    fftw_free(out);
    free(in);
    free(local_in);

    MPI_Finalize();

    return 0.0;  // Ajusta el tipo de retorno según sea necesario
}

void rotateX(int degrees){
    char message[20];
    snprintf(message, sizeof(message), "rotateX:%d", degrees);
    sendMessage(message, strlen(message));
}

void rotateY(int degrees){
    char message[20];
    snprintf(message, sizeof(message), "rotateY:%d", degrees);
    sendMessage(message, strlen(message));
}

void sendMessage(char* message, int length){
    int fd = open(SERIAL_PORT, O_WRONLY); // Abre el dispositivo en modo escritura

    if (fd == -1) {
        perror("Error al abrir el puerto serie");
        exit(1);
    }

    write(fd, message, length); // Escribe el mensaje en el puerto serie
    close(fd); // Cierra el puerto serie
}
