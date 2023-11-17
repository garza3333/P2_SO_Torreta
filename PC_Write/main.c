#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <mensaje>\n", argv[0]);
    return 1;
  }

  const char *devicePath = "/dev/ttyUSB0"; // Ajusta el nombre del dispositivo según tu configuración
  int fd = open(devicePath, O_WRONLY); // Abre el dispositivo en modo escritura

  if (fd == -1) {
    perror("Error al abrir el puerto serie");
    return 1;
  }

  const char *mensaje = argv[1];
  size_t mensaje_len = strlen(mensaje);
  
  // Crear un nuevo mensaje que incluya '\n' al final
  char mensaje_con_salto_de_linea[mensaje_len + 2]; 
  snprintf(mensaje_con_salto_de_linea, sizeof(mensaje_con_salto_de_linea), "%s\n", mensaje);

  write(fd, mensaje_con_salto_de_linea, strlen(mensaje_con_salto_de_linea)); // Escribe el mensaje en el puerto serie
  close(fd); // Cierra el puerto serie

  return 0;
}
