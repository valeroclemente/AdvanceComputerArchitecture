
/************************** SERIAL CODE IMPLEMENTATION ******************************************/
/***********************Dense Matrix-Matrix Multiplication Benchmark *******************************/
//FILES NEEDED: Uncompress float.zip for a sample of matrix 
//Find files in: Sample Data/float.zip

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

  printf("Starting matrix multiply...\n");

  for (i=0; i<NRA; i++)
    {
    for(j=0; j<NCB; j++)
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }

   stopT = clock();
   printf("Serial; %f secs\n",(stopT-startT)/CLOCKS_PER_SEC);
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
