
/************************** MPI IMPLEMENTATION ******************************************/
/****************** IMPLEMENTATION SUITABLE FOR MICROSOFT WINDOWS ***********************/
/*********************** Image Averaging Filter Benchmark *******************************/
//FILES NEEDED: bmp file of 8 bits (The repository has a sample file)


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

//Funtion for reading a image 
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

//Funtion for writing the output image (filtered image)
void writeBMP(char* filename, unsigned char* data, unsigned char info[54], unsigned char end[1024], int size){

     //Put everything again in a bmp file
    int i;
    FILE* newf = fopen(filename,"wb");
    fwrite(info,sizeof(unsigned char),54,newf);
    fwrite(data,sizeof(unsigned char),size,newf);
    fwrite(end,sizeof(unsigned char),1024,newf);
    fclose(newf);

}

/*****************************   AVERAGING  FILTER  ***********************************************************/
void promediador(unsigned char *imagen,long TX,long TY, int tam,unsigned char *copia,float DesE,int ini,int fin){

    int i,j,mi,mj;
	float acum=0;
	acum=0;

for(i=0;i<=TY-tam;i++){
   for(j=ini;j<=fin-tam;j++){
      for(mi=i;mi<i+tam;mi++)
         for(mj=j;mj<j+tam;mj++){
             acum+=imagen[(mi*TX)+mj];
         }
		 acum=(acum/((tam*tam)-1));
		 if(acum>DesE)
            copia[((i+tam/2)*TX)+(j+tam/2)]=(unsigned char)(acum);
         acum=0;
   }
}

}

//Funtion for coping original image
void copy_image(unsigned char* original, unsigned char *copia, int size){
    int i;
    for(i=0;i<size;i++)
        copia[i] = original[i];
}

//Main function
int main(int argc, char *argv[])
{
    unsigned char* image;
    unsigned char header[54];
    unsigned char end[1024];
    unsigned char* final_image;
    float DesE = 20.0;
    int size, id, p, amount, ini ,fin, inie, fine;
    int window = 30;
    int TX,TY;
    double startT,stopT;

    //MPI Inicialization
    MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	
    //Call clock for taking time execution
    startT = clock();
    
    //Parallelism on MPI
    if(id==0){
       image = readBMP("lady1000.bmp",header,end,&size);
       MPI_Bcast(&size,1,MPI_INT,0,MPI_COMM_WORLD);
       MPI_Bcast(header,54,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
       MPI_Bcast(end,1024,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
       MPI_Bcast(image,size,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
    }
    else{
       MPI_Bcast(&size,1,MPI_INT,0,MPI_COMM_WORLD);
       image = (unsigned char*) malloc(size);
       MPI_Bcast(header,54,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
       MPI_Bcast(end,1024,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
       MPI_Bcast(image,size,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
    }
       TX = *(int*)&header[18];
       TY = *(int*)&header[22];

       amount = TX / p;
       int dif = TX - (amount * p);
       ini = id * amount + MIN(id,dif);

       fin = ini + amount;
       if(id < dif)
          fin++;

        inie = ini;
        fine = fin;

        if(ini!=0)
            ini = ini - window;
        if(fin!=TX)
            fin = fin + window;

        printf("Proc %d: %d - %d\n",id,inie,fine);

       //extracting size
       final_image = (unsigned char*) malloc(size*sizeof(unsigned char));

        //copy image
        copy_image(image,final_image,size);

        //applying filter
        promediador(image,TX,TY,window,final_image,DesE,ini,fin);

    if(id==0){
        int k,i,j,mi,mj;
        for(k=1;k<p;k++){
             amount = TX / p;
             int dif = TX - (amount * p);
             ini = k * amount + MIN(id,dif);

             fin = ini + amount;
             if(k < dif)
                fin++;

             unsigned char* imagell = (unsigned char*) malloc(size);
             MPI_Recv(imagell,size,MPI_UNSIGNED_CHAR,k,1234,MPI_COMM_WORLD,&status);

            for(i=ini;i<fin;i++)
                for(j=0;j<TY;j++)
                 final_image[TX*j+i] = imagell[TX*j+i];
        }
            //write final result
             writeBMP("exit.bmp",final_image,header,end,size);

             stopT = clock();
             printf("%d processors; %f secs\n",p,(stopT-startT)/CLOCKS_PER_SEC);
    }
    else{
        MPI_Send(&final_image[0],size,MPI_UNSIGNED_CHAR,0,1234,MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
