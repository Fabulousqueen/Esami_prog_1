#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 255
typedef struct{
    char *IN;
    char *OUT;
}parameters;

parameters readParameters(int argc, char **argv){

    if(argc!=3){
        fprintf(stderr,"Usa parametri: %s <IN> <OUT>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.IN=argv[1];
    p.OUT=argv[2];

    return p;
}

unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

typedef struct{
    int n;
    int m;
}dimensions;

dimensions getDimensions(char *IN /*, int *n, int *m*/){
    
    FILE *f=fopen(IN,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    dimensions d;
    d.n=0;
    d.m=0;

    char buffer[SIZE];
    while(fgets(buffer,SIZE,f)){
        //(*n)++;
        d.n++;
    }

    for(int i=0; i<SIZE; i++){
        if(buffer[i]==' '){
            //(*m)++;
            d.m++;
        }
    }
    d.m++;
    //(*m)++;
    //printf("rows=%d cols=%d\n",d.n, d.m);
    
    fclose(f);
    return d;
}

double ***createMatrix(char *input, int n, int m){

    FILE *f=fopen(input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    double ***A=malloc(sizeof(double**)*n);
    if(A==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<n; i++){
        A[i]=malloc(sizeof(double*)*m);
        if(A[i]==NULL){
            fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
            exit(EXIT_FAILURE);
        }
        for(int j=0; j<m; j++){
            A[i][j]=malloc(sizeof(double));
            if(A[i][j]==NULL){
                fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
                exit(EXIT_FAILURE);
            }
            if(fscanf(f,"%lf ", A[i][j])==EOF){
                fprintf(stderr,"Errore nel parsing del file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(f);
    return A;
}

//--printMatrix() scritta sola per debuggare, non è richiesta
//dalla consegna--
void printMatrix(double ***A, int n, int m){

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(A[i][j]){
                printf("%lf ",*A[i][j]);
            }
        }
        printf("\n");
    }

}

int sparsify(double ***A, int n, int m){

    int elem=0;

    for(int i=0; i<n; i++){
        double mean=0;
        for(int j=0; j<m; j++){
            mean+=*A[i][j];
        }
        mean=mean/m;
        for(int j=0; j<m; j++){
            if(mean>*A[i][j]){
                free(A[i][j]);
                A[i][j]=NULL;
            }
            if(A[i][j]){
                elem++;
            }
        }
    }

    return elem;

}

double *collect(double ***A, int n, int m, int elem){

    double *B=malloc(sizeof(double)*elem);
    if(B==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }

    int k=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(A[i][j]){
                B[k]=*A[i][j];
                k++;
            }
        }
    }

    return B;
}

void writeToFile(char *output, double *B, int elem){

    FILE *f=fopen(output,"w");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<elem; i++){
        fprintf(f,"%lf ",B[i]);
    }

    fclose(f);
}

void destroyMatrix(double ***A, int n, int m){

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            free(A[i][j]);
            A[i][j]=NULL;
        }
        free(A[i]);
        A[i]=NULL;
    }
    free(A);

}

int main(int argc, char **argv){

    parameters p=readParameters(argc,argv);
    //printf("IN=%s OUT=%s\n", p.IN, p.OUT);

    //int n=0;
    //int m=0;
    dimensions d=getDimensions(p.IN);//,&n,&m);
    //printf("n=%d m=%d\n",d.n, d.m);
    
    double ***A=createMatrix(p.IN,d.n,d.m);
    //printMatrix(A,d.n,d.m);
    
    int elem=sparsify(A,d.n,d.m);
    //printMatrix(A,d.n,d.m);
    //printf("\nelem=%d\n",elem);
    
    double *B=collect(A,d.n,d.m,elem);
    /*for(int k=0; k<elem; k++){
        printf("%lf ",B[k]);
    }*/
    
    writeToFile(p.OUT,B,elem);

    destroyMatrix(A,d.n,d.m);
    free(B);
    return 0;
}