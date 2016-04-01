
/*******************NATIVE C THREADS IMPLEMENTATION ******************************************/
/****************** IMPLEMENTATION SUITABLE FOR LINUX UBUNTU *********************************/
/************************* Parallel Merge Sort Benchmark *******************************/
//FILES NEEDED: uncompress data.zip that contains the array to sort.
//Find files in: Sample Data/data.zip

#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define N 15000000

typedef struct STmer{
    int num;
    int iniA,tamA,iniB,tamB;
}thestructmer;

typedef struct ST{
    int num;
    int ini,fin;
}thestruct;

void ini_mat(double **a,double **b,double **c);

void Merge(thestructmer s );
void Tm_sort(thestruct s );

void showElapsed(int id, char *m);
void showVector(int *v, int n, int id);
int * merge(int *A, int asize, int *B, int bsize);
void swap(int *v, int i, int j);
void m_sort(int *A, int min, int max);
void read_data(int *data);

int *data;

int main()
{
     int tid, nthreads, i, j, k, id, p=4,sw =0, nt = 4, amount, ini=0, fin;
     struct timespec start, finish;
     double elapsed;
     std::thread princ[20];
     
     printf("Reading...");
     data = (int *)malloc((N)*sizeof(int));
     
     read_data(data);
     
     clock_gettime(CLOCK_MONOTONIC, &start);

     thestruct st[4];
     thestructmer stmer[4];

     for(i=0;i<4;i++){
		
	   amount = N / p;
       int dif = N - (amount * p);
       ini = i * amount + MIN(i,dif);

       fin = ini + amount;
       if(i < dif)
          fin++;
       
       st[i].num = i;
       st[i].ini = ini;
       st[i].fin = fin;

       princ[i]= std::thread(Tm_sort,st[i]); 
     }
      
    for(i=0;i<4;i++)
      princ[i].join();
    
    p = 2;
    for(i=0;i<2;i++){
        stmer[i].num = 0;
	stmer[i].iniA = st[i*2].ini;
	stmer[i].iniB = st[i*2+1].ini;
	stmer[i].tamA = st[i*2].fin-st[i*2].ini;
	stmer[i].tamB = st[i*2+1].fin-st[i*2+1].ini;
	
        princ[i]= std::thread(Merge,stmer[i]);  
    }

    for(i=0;i<2;i++)
      princ[i].join();
    
    merge(&data[stmer[0].iniA], stmer[0].tamA+stmer[0].tamA ,&data[stmer[1].iniA], stmer[1].tamA+stmer[1].tamA);    
    
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("%d processors; %f secs\n",nt,elapsed);

    return 0;
}

void Merge(thestructmer s)
{
    int iniA,tamA,iniB,tamB,num,i;

    iniA = s.iniA;
    tamA = s.tamA;
    iniB = s.iniB;
    tamB = s.tamB;
    num  = s.num;

    merge(&data[iniA], tamA ,&data[iniB], tamB);

    printf(".");
}


void Tm_sort(thestruct s)
{
    int ini,fin,num;

    ini = s.ini;
    fin = s.fin;
    num = s.num;
    m_sort(data,ini,fin);
    printf("\nProc %d  ini: %d fin %d",num,ini,fin);

    printf(".");

}

void showVector(int *v, int n, int id)
{
	int i;
	printf("%d: ",id);
	for(i=0;i<n;i++)
		printf("%d ",v[i]);
	putchar('\n');
}

int * merge(int *A, int asize, int *B, int bsize) {
	int ai, bi, ci, i;
	int *C=NULL;
	int csize = asize+bsize;

	ai = 0;
	bi = 0;
	ci = 0;

    if(C==NULL)
	   C = (int *)malloc(csize*sizeof(int));
	while ((ai < asize) && (bi < bsize)) {
		if (A[ai] <= B[bi]) {
			C[ci] = A[ai];
			ci++; ai++;
		} else {
			C[ci] = B[bi];
			ci++; bi++;
		}
	}

	if (ai >= asize)
		for (i = ci; i < csize; i++, bi++)
			C[i] = B[bi];
	else if (bi >= bsize)
		for (i = ci; i < csize; i++, ai++)
			C[i] = A[ai];

	for (i = 0; i < asize; i++)
		A[i] = C[i];
	for (i = 0; i < bsize; i++)
		B[i] = C[asize+i];

	/* showVector(C, csize, 0); */
	return C;
}

void swap(int *v, int i, int j)
{
	int t;
	t = v[i];
	v[i] = v[j];
	v[j] = t;
}

void m_sort(int *A, int min, int max)
{
	int *C;		/* dummy, just to fit the function */
	int mid = (min+max)/2;
	int lowerCount = mid - min + 1;
	int upperCount = max - mid;

	/* If the range consists of a single element, it's already sorted */
	if (max == min) {
		return;
	} else {
		/* Otherwise, sort the first half */
		m_sort(A, min, mid);
		/* Now sort the second half */
		m_sort(A, mid+1, max);
		/* Now merge the two halves */
		C = merge(A + min, lowerCount, A + mid + 1, upperCount);
	}
}

void read_data(int *data){
   int i=0;
   FILE *fp;
   fp = fopen("data.txt","r");

	while(fscanf(fp,"%d",&data[i])!=EOF && i<N){
        i++;
	}

	fclose(fp);
}

