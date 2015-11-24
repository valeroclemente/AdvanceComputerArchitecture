#include <iostream>
#include <thread>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define ROW 1000
#define COL 1000

typedef struct ST{
    int num;
    int ini,fin;
}thestruct;

void ini_mat(double **a,double **b,double **c);

void Mult(thestruct s );

double **a,**b,**c,co=0;


int main()
{
    struct timespec start, finish;
    double elapsed;
    int	tid, nthreads, i, j, k, id, p=4,sw =0, nt = 4, amount, ini=0, fin;
    std::thread princ[4];

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

     printf("Reading...\n");
     ini_mat(a,b,c);

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    thestruct st[20];
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
       princ[i]= std::thread(Mult,st[i]);  
    }

    for(i=0;i<4;i++)
      princ[i].join();

    
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("%d processors; %f secs\n",nt,elapsed);


    return 0;
}

void Mult(thestruct s)
{
    int i,j,k,ini,fin;

    ini = s.ini;
    fin = s.fin;

    for (i=ini; i<fin; i++)
    {
    for(j=0; j<COL; j++)
      for (k=0; k<COL; k++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }

    printf(".");
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
