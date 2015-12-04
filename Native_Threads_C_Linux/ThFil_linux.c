
/************************** NATIVE C THREADS IMPLEMENTATION ********************************/
/****************** IMPLEMENTATION SUITABLE FOR LINUX UBUNTU ******************************/
/*********************** Image Averaging Filter Benchmark ********************************/
//FILES NEEDED: bmp file of 8 bits (The repository has a sample file)
//Find files in: Sample Data/lady1000.bmp


#include <iostream>
#include <thread>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef struct ST{
    int num;
    int ini,fin;
}thestruct;

void fil(thestruct s);

unsigned char* readBMP(char* filename, unsigned char info[54], unsigned char end[1024], int *size)
{
    int i;
    unsigned char* data;

    FILE* f = fopen(filename, "rb");

    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    *size = width * height;

    data = (unsigned char*) malloc(*size);
    fread(data, sizeof(unsigned char), *size, f); // read the rest of the data at once
    fread(end, sizeof(unsigned char), 1024, f);
    fclose(f);

    return data;
}

void writeBMP(char* filename, unsigned char* data, unsigned char info[54], unsigned char end[1024], int size){

     //Put everything again in a bmp file
    int i;
    FILE* newf = fopen(filename,"wb");
    fwrite(info,sizeof(unsigned char),54,newf);
    fwrite(data,sizeof(unsigned char),size,newf);
    fwrite(end,sizeof(unsigned char),1024,newf);
    fclose(newf);

}

void copy_image(unsigned char* original, unsigned char *copia, int size){
    int i;
    for(i=0;i<size;i++)
        copia[i] = original[i];
}

unsigned char* image;
unsigned char header[54];
unsigned char end[1024];
unsigned char* final_image;
float DesE = 20.0;
int window = 30, co = 0;
int TX,TY;

int main()
{
    struct timespec start, finish;
    double elapsed;
    int size, p=4, amount, ini ,fin,i, nt=4;
    double startT,stopT;
    thestruct st[4];
    std::thread princ[4];
	
    printf("Reading...");

    clock_gettime(CLOCK_MONOTONIC, &start);

    image = readBMP("lady1000.bmp",header,end,&size);
     
    TX = *(int*)&header[18];
    TY = *(int*)&header[22];

     //extracting size
       final_image = (unsigned char*) malloc(size*sizeof(unsigned char));

     //copy image
       copy_image(image,final_image,size);

       for(i=0;i<4;i++){
       amount = TX / p;
       int dif = TX - (amount * p);
       ini = i * amount + MIN(i,dif);

       fin = ini + amount;
       if(i < dif)
          fin++;

        if(ini!=0)
            ini = ini - window/2;
        if(fin!=TX)
            fin = fin + window/2;
        //applying filter
       
       st[i].num = i;
       st[i].ini = ini;
       st[i].fin = fin;
       princ[i] = std::thread(fil,st[i]);  
       }

      for(i=0;i<4;i++)
        princ[i].join();
    
    writeBMP("exit.bmp",final_image,header,end,size);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("%d processors; %f secs\n",nt,elapsed);

    return 0;
}

void fil(thestruct s)
{
    int i,j,k,ini,fin,mi,mj,tam,num;

    tam = window;
    ini = s.ini;
    fin = s.fin;
    num = s.num;

	float acum=0;
	acum=0;
	
	printf("Proc %d: %d - %d\n",num,ini,fin);

for(i=0;i<=TY-tam;i++){
   for(j=ini;j<=fin-tam;j++){
      for(mi=i;mi<i+tam;mi++)
         for(mj=j;mj<j+tam;mj++){
             acum+=image[(mi*TX)+mj];
         }
		 acum=(acum/((tam*tam)-1));
		 if(acum>DesE)
            final_image[((i+tam/2)*TX)+(j+tam/2)]=(unsigned char)(acum);
         acum=0;
   }
}
    printf(".");
}

