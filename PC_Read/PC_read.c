#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define SERIAL_PORT "/dev/ttyUSB0" 

int main() {
    int serial_port = open(SERIAL_PORT, O_RDWR);

    if (serial_port == -1) {
        perror("Error abriendo el puerto serie");
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(serial_port, &tty) != 0) {
        perror("Error obteniendo la configuración del puerto serie");
        close(serial_port);
        return -1;
    }

    // Configuración del puerto serie
    tty.c_cflag &= ~PARENB;  // Sin paridad
    tty.c_cflag &= ~CSTOPB;  // 1 bit de parada
    tty.c_cflag |= CS8;      // 8 bits de datos
    tty.c_cflag &= ~CRTSCTS; // Deshabilita control de flujo de hardware

    cfsetospeed(&tty, B9600); // Velocidad de transmisión (en baudios)
    cfsetispeed(&tty, B9600);

    // Configura el puerto serie con la nueva configuración
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        perror("Error configurando el puerto serie");
        close(serial_port);
        return -1;
    }

    // Lectura del puerto serie
    char read_buf[256];
    memset(&read_buf, '\0', sizeof(read_buf));

    while (1) {
        ssize_t num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
        
        if (num_bytes > 1) {
            printf("Mensaje recibido: %s", read_buf);
            memset(&read_buf, '\0', sizeof(read_buf));
        } else {
            printf("Leyendo del puerto serie...\n");
            //break;
        }

        // Puedes agregar un pequeño retardo aquí para evitar consumir demasiados recursos
        usleep(10000);  // 10 milisegundos
    }

    // Cierra el puerto serie
    close(serial_port);

    return 0;
}
