#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  const char *devicePath = "/dev/ttyUSB0"; // Ajusta el nombre del dispositivo según tu configuración
  int fd = open(devicePath, O_WRONLY); // Abre el dispositivo en modo escritura

  if (fd == -1) {
    perror("Error al abrir el puerto serie");
    return 1;
  }

  const char *mensaje = "rot(-35)";
  write(fd, mensaje, strlen(mensaje)); // Escribe el mensaje en el puerto serie
  close(fd); // Cierra el puerto serie

  return 0;
}