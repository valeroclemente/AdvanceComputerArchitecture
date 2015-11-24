
/*************************SERIAL CODE IMPLEMENTATION ******************************************/
/*********************Merge Sort Benchmark *******************************/
//FILES NEEDED: Uncompress data.zip for a sample of array 
//Find files in: Sample Data/data.zip

#include <stdio.h>
#include <time.h>
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
	int m,n=N;
	int s = 0;
	int i;
	int nt = 4;
	int cini[30]={0},cfin[30]={0};

    data = (int *)malloc((n)*sizeof(int));

    printf("Reading\n");
    read_data(data);

	startT = clock();

    printf("Sorting\n");
    m_sort(data, 0, n);

	stopT = clock();
	printf("Saving\n");
	FILE * fout;

		printf("; Serial; %f secs\n",(stopT-startT)/CLOCKS_PER_SEC);

		fout = fopen("result.txt","w");
		for(i=0;i<n;i++){
			fprintf(fout,"%d\n",data[i]);
			//printf("%d \n",data[i]);
		}
		fclose(fout);

   return 0;
}

void showVector(int *v, int n, int id)
{
	int i;
	printf("%d: ",id);
	for(i=0;i<n;i++)
		printf("**%d ",v[i]);
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
