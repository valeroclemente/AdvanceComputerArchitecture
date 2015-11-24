#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define ROW 1000
#define COL 1000

#define NRA ROW   /* number of rows in matrix A */
#define NCA COL   /* number of columns in matrix A */
#define NCB COL   /* number of columns in matrix B */

void ini_mat(double **a,double **b,double **c);

int main (int argc, char *argv[])
{
    int	tid, nthreads, i, j, k, id, p,sw =0, nt = 4, amount, ini=0, fin;
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

    for (i=0; i<ROW; i++)
       for (j=0; j<COL; j++)
           c[i][j]= 0.0;

    MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

    if(id==0){
        printf("Reading...");
        ini_mat(a,b,c);
        startT = clock();
        printf("\nSending...");

        for(i=0;i<ROW;i++)
            MPI_Bcast(a[i],COL,MPI_DOUBLE,0,MPI_COMM_WORLD);
        for(i=0;i<ROW;i++)
            MPI_Bcast(b[i],COL,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }
    else{
        double *chunk;
        int s;
        for(i=0;i<ROW;i++)
            MPI_Bcast(a[i],COL,MPI_DOUBLE,0,MPI_COMM_WORLD);
        for(i=0;i<ROW;i++)
            MPI_Bcast(b[i],COL,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }

    // int inia=0;
     amount = ROW / p;
     int dif = ROW - (amount * p);
     i = id;
     ini = i * amount + MIN(id,dif);

     fin = ini + amount;
     if(id < dif)
         fin++;

    printf("Proc %d  ini: %d fin %d\n",id,ini,fin);

    for (i=ini; i<fin; i++)
    {
    for(j=0; j<COL; j++)
      for (k=0; k<COL; k++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }

   if(id==0){

        for(k=1;k<p;k++){
         amount = ROW / p;
         ini = k * amount + MIN(k,dif);
         fin = ini + amount;
         if(k < dif)
           fin++;

            for(i=ini;i<fin;i++){
                MPI_Recv(c[i],COL,MPI_DOUBLE,k,i,MPI_COMM_WORLD,&status);

            }
        }

        stopT = clock();
        printf("%d processors; %f secs\n",id,(stopT-startT)/CLOCKS_PER_SEC);
   /*
        for(i=0;i<ROW;i++){
            for(j=0;j<COL;j++){
               printf("%.2lf ",c[i][j]);
            }
            printf("\n");
        }
    */
   }
   else{
       for (i=ini; i<fin; i++)
       {
          MPI_Send(c[i],COL,MPI_DOUBLE,0,i,MPI_COMM_WORLD);
       }
   }

   MPI_Finalize();

return 0;
}

void ini_mat(double **a,double **b,double **c){
   int i,j;

   FILE *fp;
   fp = fopen("float.txt","r");

 //  printf("Initializing matrix\n");
   for (i=0; i<ROW; i++)
    for (j=0; j<COL; j++)
       fscanf(fp,"%lf",&a[i][j]);

  for (i=0; i<ROW; i++)
    for (j=0; j<COL; j++)
      fscanf(fp,"%lf",&b[i][j]);

  fclose(fp);
}

