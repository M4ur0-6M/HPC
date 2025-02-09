#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

int ** allocate_matrix( int size )
{
  int * vals = (int *) malloc( size * size * sizeof(int) );

  int ** ptrs = (int **) malloc( size * sizeof(int*) );

  int i;
  for (i = 0; i < size; ++i) {
    ptrs[ i ] = &vals[ i * size ];
  }

  return ptrs;
}

void init_matrix( int **matrix, int size )
{
  int i, j;

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[ i ][ j ] = rand() % 10;
    }
  }
}


int main( int argc, char *argv[] )
{
  int **matrix1, **matrix2, **matrix3;
  int size, i, j, k, chunksize, numthreads;
  int sum = 0;
  struct timeval tstart, tend;
  double exectime;

  if (argc != 3) {
    fprintf( stderr, "%s <matrix size> <number of thread>\n", argv[0] );
    return -1;
  }

  size = atoi( argv[1] );
  numthreads = atoi( argv[2] );

  if (size % numthreads != 0) {
    fprintf( stderr, "tamanno %d debe de ser un multiplo del numero de hilos %d\n",
	     size, numthreads );
    return -1;
  }
  omp_set_num_threads( numthreads );
  chunksize = size / numthreads;

  matrix1 = allocate_matrix( size );
  matrix2 = allocate_matrix( size );
  matrix3 = allocate_matrix( size );
  
  init_matrix( matrix1, size );
  init_matrix( matrix2, size );


  gettimeofday( &tstart, NULL );
  
  #pragma omp parallel for shared(matrix1, matrix2, matrix3, chunksize) \
  private(i,j,k,sum) schedule(static, chunksize)
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      sum = 0; 
      for (k = 0; k < size; ++k) { 
	sum += matrix1[ i ][ k ] * matrix2[ k ][ j ];
      }
      matrix3[ i ][ j ] = sum;
    }
  }
  gettimeofday( &tend, NULL );

  exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0;
  exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0;  

  printf( "%i,%f\n",size, exectime/1000.0);

  return 0;
}
