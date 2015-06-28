/* Universidad Nacional de La Matanza */
/* Sistemas Operativos Avanzados */
/* TP 2 - Computacion de Alto Rendimiento */
/* Algoritmo de Multiplicacion de Matrices */
/* Basado en codigo provisto por el Prof. Fernando Tinetti (UNLP) */

#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>

void initvalmat(float *mat, int n, float val); 
void verify_result(float *c, int n);
double dwalltime();


main(int argc, char *argv[])
{
  float *A, *B, *C;
  int n, i, j,k;
  int ii, jj, kk, tamBloque;
  int posicion, posII, posJJ;  
  double timetick;

  /* Chequeando parametros */
  if (argc != 3 || (n = atoi(argv[1])) <= 0 || (tamBloque = atoi(argv[2])) <= 0 )
  {
    printf("Uso: %s n tamBloque\n  donde n: dimension de la matriz cuadrada (nxn X nxn)\ntamBloque: dimension por bloque\n", argv[0]);
    exit(1);
  }

  // Inicializando Matrices
  A = (float *) malloc(n*n*sizeof(float));
  B = (float *) malloc(n*n*sizeof(float));
  C = (float *) malloc(n*n*sizeof(float));
  initvalmat(A, n, 1.0); 
  initvalmat(B, n, 1.0); 
  initvalmat(C, n, 0.0); 


  printf("Multiplicando matrices de %d x %d \n", n, n);  
  timetick = dwalltime();
  
  /*************************************************************/
  /* Programar aqui el algoritmo de multiplicacion de matrices */
  /*************************************************************/
  # pragma omp parallel for private(i,j,k, ii,jj, posicion, posII, posJJ), shared (A,B,C)
  for (ii = 0; ii < n; ii += tamBloque){

    posII = ii + tamBloque;
    if(posII >= n) posII = n;
    
    for (jj = 0; jj < n; jj += tamBloque){

      posJJ = jj + tamBloque;
      if(posJJ >= n) posJJ = n;

      for (k = 0; k < n; k++) {
        posicion=k*n;
        
        for (i = ii; i < posII; i++) { 
          float *celdaC = &C[posicion + i];
          for (j = jj; j < posJJ; j++) 
            *celdaC += A[ posicion + j ] * B[j*n + i];
        }
      }
    }
  }    
  /*************************************************************/
  
  timetick = dwalltime() - timetick;
  verify_result(C, n);

  printf("Resultado correcto. Tiempo de ejecucion: %f segundos\n", timetick);
}

// Funciones Auxiliares

/* Init square matrix with a specific value */
void initvalmat(float *mat, int n, float val)
{
  int i, j;      /* Indexes */

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      mat[i*n + j] = val;
    }
  }
}

double dwalltime()
{
  double sec;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

void verify_result(float *c, int n)
{
  int i, j;
  
  /* Check results (just in case...) */
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      if (c[i*n + j] != n)
      {
        printf("Error en %d, %d, valor: %f\n", i, j, c[i*n + j]);
    exit(-1);
      }
    }
  }
}

