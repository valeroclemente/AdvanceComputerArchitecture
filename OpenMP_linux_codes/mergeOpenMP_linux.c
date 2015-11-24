#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <ctype.h>
#include <string.h>

#define N 15000000
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void showElapsed(int id, char *m);
void showVector(int *v, int n, int id);
int * merge(int *A, int asize, int *B, int bsize, int *C);
void swap(int *v, int i, int j);
void m_sort(int *A, int min, int max);
void read_data(int *data);

double startT,stopT;
double startTime;

int main(int argc, char** argv) {
	int * data;
	int * other;
	int m,n=N;
	int id,p;
	int s = 0;
	int i;
	int chunk;
	int nt = 4;
	int cini[30]={0},cfin[30]={0};
	int part = nt;

    if(isdigit(argv[1])){
       p = atoi(argv[1]);
    if(p<1 || p>20)
        printf("# of threads should be between 1 and 20 - it runs 4 threads -> default");
    else
        nt = p;
    }

    data = (int *)malloc((n)*sizeof(int));
	other = (int *)malloc((n)*sizeof(int));

    read_data(data);

    omp_set_num_threads(nt);
    startT = clock();
    double ss = omp_get_wtime();


    chunk = n / nt;

    #pragma omp parallel for
    for(i=0;i<nt;i++){
        cini[i] = i * chunk;
        cfin[i] = cini[i] + chunk - 1;
        if(i==nt-1 && chunk*nt!= n )
            cfin[i] = cfin[i] + (n - chunk*nt) ;

        m_sort(data, cini[i], cfin[i]);
    }


     int j,t=nt,x;
     do{
      j = t;
     #pragma omp parallel for
     for(i=0;i<t-1;i=i+2){
           merge(&data[cini[i]], cfin[i]-cini[i]+1, &data[cini[i+1]], cfin[i+1]-cini[i+1]+1, &other[MIN(cini[i],cini[i+1])]);
           cini[j]=MIN(cini[i],cini[i+1]);
           cfin[j]=MAX(cfin[i],cfin[i+1]);
           j++;
      }
      if(t%2!=0){
         cini[j]=cini[t-1];
         cfin[j]=cfin[t-1];
         j++;
      }
      if(j-t>1){
         for(i=t,x=0;i<j;i++,x++){
            cini[x]=cini[i];
            cfin[x]=cfin[i];
         }
      }
      t=j-t;
     }while(t>1);

	    stopT = clock();
            double ee = omp_get_wtime();
		
	    FILE * fout;

		printf("%d; %d processors; %f secs\n",s,nt,ee-ss);

		fout = fopen("result.txt","w");
		for(i=0;i<n;i++){
			fprintf(fout,"%d\n",other[i]);
			//printf("%d \n",other[i]);
		}
		fclose(fout);

   return 0;
}

void showElapsed(int id, char *m)
{
	printf("%d: %s %f secs\n",id,m,(clock()-startTime)/CLOCKS_PER_SEC);
}

void showVector(int *v, int n, int id)
{
	int i;
	printf("%d: ",id);
	for(i=0;i<n;i++)
		printf("%d ",v[i]);
	putchar('\n');
}

int * merge(int *A, int asize, int *B, int bsize, int *C) {
	int ai, bi, ci, i;
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
		  m_sort(A, mid+1, max);

		C = merge(A + min, lowerCount, A + mid + 1, upperCount,NULL);
	}
}

void read_data(int *data){
   int i=0;
   FILE *fp;
   fp = fopen("data.txt","r");

	while(fscanf(fp,"%d",&data[i])!=EOF && i<N){
        i++;
	}

    printf("-%d-",i);
	fclose(fp);

}
