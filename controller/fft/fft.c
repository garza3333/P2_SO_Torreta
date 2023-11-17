#include <stdio.h>
#include <fftw3.h>
#include <math.h>

#define TAMANO_ARREGLO 60

// Función para encontrar la frecuencia predominante
double encontrar_frecuencia_predominante(double* frecuencias) {
    fftw_complex* in, * out;
    fftw_plan p;
    double frecuencia_predominante;
    int i;

    // Crear arreglos de entrada y salida para la FFT
    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * TAMANO_ARREGLO);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * TAMANO_ARREGLO);

    // Crear el plan para la FFT
    p = fftw_plan_dft_1d(TAMANO_ARREGLO, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // Llenar el arreglo de entrada con los valores de frecuencia
    for (i = 0; i < TAMANO_ARREGLO; ++i) {
        in[i][0] = frecuencias[i];
        in[i][1] = 0.0;
    }

    // Ejecutar la transformada de Fourier
    fftw_execute(p);

    // Encontrar la frecuencia predominante (índice del máximo en el espectro)
    int indice_max = 0;
    double magnitud_max = 0.0;
    for (i = 0; i < TAMANO_ARREGLO / 2; ++i) {
        double magnitud = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        if (magnitud > magnitud_max) {
            magnitud_max = magnitud;
            indice_max = i;
        }
    }

    // Calcular la frecuencia predominante
    frecuencia_predominante = (double)indice_max;

    // Liberar memoria y destruir el plan de la FFT
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return frecuencia_predominante;
}