
/************************** OpenMP IMPLEMENTATION ******************************************/
/****************** IMPLEMENTATION SUITABLE FOR MICROSOFT WINDOWS ***********************/
/**********************Dense Matrix-Matrix Multiplication Benchmark *******************************/
//FILES NEEDED: Uncompress float.zip for a sample of matrix 
//Find files in: Sample Data/float.zip

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 1000
#define COL 1000

#define NRA ROW   /* number of rows in matrix A */
#define NCA COL   /* number of columns in matrix A */
#define NCB COL   /* number of columns in matrix B */

void ini_mat(double **a,double **b,double **c);

int main (int argc, char *argv[])
{
    int	tid, nthreads, i, j, k, chunk;
    double startT,stopT;
    double **a,**b,**c;
    int nt=4,p;

    if(isdigit(argv[1])){
       p = atoi(argv[1]);
    if(p<1 || p>20)
        printf("# of threads should be between 1 and 20 - it runs 4 threads -> default");
    else
        nt = p;
    }
    omp_set_num_threads(nt);

    a = (double **)malloc(ROW*sizeof(double*));
    b = (double **)malloc(ROW*sizeof(double*));
    c = (double **)malloc(ROW*sizeof(double*));

	for (i=0;i<ROW;i++){
		a[i] = (double*)malloc(COL*sizeof(double));
		b[i] = (double*)malloc(COL*sizeof(double));
		c[i] = (double*)malloc(COL*sizeof(double));
    }

    chunk = 10;                    /* set loop iteration chunk size */
    printf("Initializing matrix\n");
    ini_mat(a,b,c);

    startT = clock();

/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Starting matrix multiple with %d threads\n",nthreads);
    }

  /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  printf("Thread %d starting matrix multiply...\n",tid);

  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    {
    for(j=0; j<NCB; j++)
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   /*** End of parallel region ***/

   stopT = clock();
   printf("%d processors; %f secs\n",nthreads,(stopT-startT)/CLOCKS_PER_SEC);
/*
   for(i=0;i<ROW;i++){
            for(j=0;j<COL;j++){
               printf("%.2lf ",c[i][j]);
            }
            printf("\n");
        }
*/
return 0;
}

void ini_mat(double **a,double **b,double **c){
   int i,j;

   FILE *fp;
   fp = fopen("float.txt","r");

   for (i=0; i<ROW; i++)
    for (j=0; j<COL; j++)
      fscanf(fp,"%lf",&a[i][j]);

  for (i=0; i<ROW; i++)
    for (j=0; j<COL; j++)
      fscanf(fp,"%lf",&b[i][j]);

  fclose(fp);

  for (i=0; i<ROW; i++)
    for (j=0; j<COL; j++)
      c[i][j]= 0.0;
}
