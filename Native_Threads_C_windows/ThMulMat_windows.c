
/******************NATIVE C THREADS IMPLEMENTATION ******************************************/
/****************** IMPLEMENTATION SUITABLE FOR MICROSOFT WINDOWS ***********************/
/************ Dense Matrix-Matrix Multiplication Benchmark *******************************/
//File Needed: uncompress float.zip to get a matrix for multiplying. 
//Find files in: Sample Data/float.zip


#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define ROW 1000
#define COL 1000

typedef struct ST{
    int num;
    int ini,fin;
}thestruct;

void ini_mat(double **a,double **b,double **c);

void Mult(thestruct* );

double **a,**b,**c,co=0;

int main()
{
    int	tid, nthreads, i, j, k, id, p=4,sw =0, nt = 4, amount, ini=0, fin;
    double startT,stopT;
    int myhandle[20];

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

     printf("Reading...");
     ini_mat(a,b,c);
     startT = clock();

    thestruct st[4];
    amount = ROW / p;
    int dif = ROW - (amount * p);
    for(i=0;i<4;i++){
       ini = i * amount + MIN(i,dif);
       fin = ini + amount;
         if(i < dif)
           fin++;
       
       printf("\nProc %d  ini: %d fin %d",i,ini,fin);
	   st[i].num = i;
       st[i].ini = ini;
       st[i].fin = fin;
       myhandle[i] = (int)_beginthread((void(*)(void*))Mult,0,(void*)&st[i]);   
    }

    WaitForMultipleObjects(4, myhandle, 1, 1000000000);
    
    while(co<4)
        i=i;
    stopT = clock();
    printf("%d processors; %f secs\n",id,(stopT-startT)/CLOCKS_PER_SEC);
}

void Mult(thestruct* s)
{
    int i,j,k,ini,fin;

    ini = (thestruct*)(s)->ini;
    fin = (thestruct*)(s)->fin;

    for (i=ini; i<fin; i++)
    {
    for(j=0; j<COL; j++)
      for (k=0; k<COL; k++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }

    co=co+1;
    printf(".");
    _endthread();
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
}
