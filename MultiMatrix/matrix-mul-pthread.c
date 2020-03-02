
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int size, num_threads;
int **matrix1, **matrix2, **matrix3;

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

void * worker( void *arg )
{
  int i, j, k, tid, portion_size, row_start, row_end;
  int sum;
  
  tid = *(int *)(arg); 
  portion_size = size / num_threads;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;

  for (i = row_start; i < row_end; ++i) {
    for (j = 0; j < size; ++j) { 
      sum = 0;
      for (k = 0; k < size; ++k) { 
	sum += matrix1[ i ][ k ] * matrix2[ k ][ j ];
      }
      matrix3[ i ][ j ] = sum;
    }
  }
}

int main( int argc, char *argv[] )
{
  int i;
  int sum = 0;
  struct timeval tstart, tend;
  double exectime;
  pthread_t * threads;

  if (argc != 3) {
    fprintf( stderr, "%s <matrix size> <number of threads>\n", argv[0], argv[1] );
    return -1;
  }

  size = atoi( argv[1] );
  num_threads = atoi( argv[2] );

  if ( size % num_threads != 0 ) {
    fprintf( stderr, "tamanno %d debe de ser un multiplo del numero de hilos %d\n",
	     size, num_threads );
    return -1;
  }

  threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );

  matrix1 = allocate_matrix( size );
  matrix2 = allocate_matrix( size );
  matrix3 = allocate_matrix( size );
  
  init_matrix( matrix1, size );
  init_matrix( matrix2, size );


  gettimeofday( &tstart, NULL );
  for ( i = 0; i < num_threads; ++i ) {
    int *tid;
    tid = (int *) malloc( sizeof(int) );
    *tid = i;
    pthread_create( &threads[i], NULL, worker, (void *)tid );
  }

  for ( i = 0; i < num_threads; ++i ) {
    pthread_join( threads[i], NULL );
  }
  gettimeofday( &tend, NULL );

  exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0; 
  exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0;   

  printf( "%i,%f\n",size, exectime/1000.0);

  return 0;
}
