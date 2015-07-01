/* Universidad Nacional de La Matanza */
/* Sistemas Operativos Avanzados */
/* TP 2 - Computacion de Alto Rendimiento */
/* Algoritmo de Multiplicacion de Matrices */
/* Basado en codigo provisto por el Prof. Fernando Tinetti (UNLP) */

#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <mpi.h>

void initvalmat(float *mat, int n, float val); 
void verify_result(float *c, int n);
double dwalltime();


main(int argc, char *argv[])
{
	float *A, *B, *C;
	int n, i, j,k,posicion,posC;
	double timetick;

	int rank;
	MPI_Status Stat;
	int count;

	int comTask;
	int finTask;
	int cantPorTask;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Chequeando parametros */
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) )
	{
		printf("Uso: %s n\n  donde n: dimension de la matriz cuadrada (nxn X nxn)\n", argv[0]);
		MPI_Finalize();
		exit(1);
	}

	if (n%count!=0) {
		if (rank==0) printf("El tamaño de la matriz no es divisible por la cantidad de procesadores que hay\n");
		MPI_Finalize();
		exit(-1);
	}

	// Inicializando Matrices
	A = (float *) malloc(n*n*sizeof(float));
	B = (float *) malloc(n*n*sizeof(float));
	C = (float *) malloc(n*n*sizeof(float));

	if (rank == 0)
	{
		initvalmat(A, n, 1.0); 
		initvalmat(B, n, 1.0); 
		initvalmat(C, n, 0.0);
	}

	cantPorTask = n / count;
	comTask = rank * cantPorTask;
	finTask = (rank + 1) * cantPorTask;

	//printf("Multiplicando matrices de %d x %d \n", n, n);  
	timetick = MPI_Wtime();

	/*************************************************************/
	/* Programar aqui el algoritmo de multiplicacion de matrices */
	/*************************************************************/
	
	MPI_Bcast(C, n*n, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Bcast(B, n*n, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Scatter(A, n*cantPorTask, MPI_FLOAT, &A[n*comTask], n*cantPorTask,MPI_FLOAT, 0, MPI_COMM_WORLD);
    
	for ( i = comTask; i < finTask; i++)
	{
		posicion= i*n;
		for ( j = 0; j < n; j++)
		{
				posC=posicion+j;
				for ( k = 0; k < n; k++ )
					C[posC] += A[posicion+k] * B[k*n+j];
		}
	}
	
	MPI_Gather(&C[n*comTask], n*cantPorTask, MPI_FLOAT,C, n*cantPorTask, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	/*************************************************************/

	timetick = MPI_Wtime() - timetick;

	if (rank == 0)
	{	
		verify_result(C, n);
		//printf("Resultado correcto. Tiempo de ejecucion: %f segundos\n", timetick);
		printf("%f ; ", timetick);
	}
	
	MPI_Finalize();
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
		MPI_Finalize();
		exit(-1);
      }
    }
  }
}

