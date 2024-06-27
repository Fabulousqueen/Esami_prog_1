#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct{
    char *file_name;
    int N; 
    int M;
}parameters;

parameters readInput(int argc, char **argv){
    
    if(argc!=4){
        fprintf(stderr,"Usa parametri: %s <file_name> <N> <M>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.file_name=argv[1];
    p.N=atoi(argv[2]);
    p.M=atoi(argv[3]);

    if(p.N<5|| p.N>10){
        fprintf(stderr,"N deve essere compreso nell'intervallo [5,10]\n");
        exit(EXIT_FAILURE);
    }

    if(p.M<5|| p.M>10){
        fprintf(stderr,"M deve essere compreso nell'intervallo [5,10]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

double ***allocateMatrix(int N, int M){

    double ***A=(double ***)malloc(sizeof(double **)*N);
    if(A==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<N; i++){
        A[i]=(double **)malloc(sizeof(double *)*M);
        if(A[i]==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica\n");
        exit(EXIT_FAILURE);
        }
        for(int j=0; j<M; j++){
            A[i][j]=(double *)malloc(sizeof(double));
            if(A[i][j]==NULL){
            fprintf(stderr,"Errore nell'allocazione dinamica\n");
            exit(EXIT_FAILURE);
            }
        }
    }

    return A;
}

double ***fillMatrix(parameters *p){

    FILE *f=fopen(p->file_name,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in lettura\n");
        exit(EXIT_FAILURE);
    }

    double ***A=allocateMatrix(p->N,p->M);

    for(int i=0; i<p->N; i++){
        for(int j=0; j<p->M; j++){
            if(fscanf(f,"%lf ",A[i][j])==EOF){
                fprintf(stderr,"Errore nel parsing del file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(f);
    return A;
}

void printMatrix(double ***A, int N, int M){

    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            printf("%lf ", *A[i][j]);
        }
        puts("");
    }

}

double ***normalize(double ***A, int N, int M){
    
    double ***B=allocateMatrix(N,M);

    for(int j=0; j<M; j++){
        double MAX_A_j=0;
        for(int i=0; i<N; i++){
            if(MAX_A_j<*A[i][j]){
                MAX_A_j=*A[i][j];
            }
        }
        
        for(int i=0; i<N; i++){
            *B[i][j]=(*A[i][j])/MAX_A_j;
        }

    }

    return B;
}

void sortMatrixCols(double ***B, int N, int M){
    
    /*
    //BUBBLE SORT
    int swap=1;

    while(swap){
        swap=0;
        for(int j=0; j<M; j++){
            for(int i=0; i<N-1; i++){
                if(*B[i][j]>*B[i+1][j]){
                    double tmp=*B[i+1][j];
                    *B[i+1][j]=*B[i][j];
                    *B[i][j]=tmp;
                    swap=1;
                }
            }
        }
    }
    
    //INSERTION SORT
    for(int j=0; j<M; j++){
        for(int i=1; i<N; i++){
            int index=i-1;
            double element=*B[i][j];
            while(index>=0 && *B[index][j]>element){
                *B[index+1][j]=*B[index][j];
                index--;
            }
            *B[index+1][j]=element;
        }
    }
    */
    
    //SELECTION SORT
    for(int j=0; j<M; j++){
        for(int i=0; i<N-1; i++){
            int min=i;
            for(int k=i+1; k<N; k++){
                if(*B[min][j]>*B[k][j]){
                    min=k;
                }
            }
            double tmp=*B[i][j];
            *B[i][j]=*B[min][j];
            *B[min][j]=tmp;
        }
    }

}

void destroyMatrix(double ***Matrix, int N, int M){

    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            free(Matrix[i][j]);
            Matrix[i][j]=NULL;
        }
        free(Matrix[i]);
        Matrix[i]=NULL;
    }
    free(Matrix);
    Matrix=NULL;

}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);
    
    double ***A=fillMatrix(&p);
    printf("** A **\n");
    printMatrix(A,p.N,p.M);

    double ***B=normalize(A,p.N,p.M);
    printf("\n** B **\n");
    printMatrix(B,p.N,p.M);

    sortMatrixCols(B,p.N,p.M);
    printf("\n** B (sorted columns) **\n");
    printMatrix(B,p.N,p.M);

    destroyMatrix(A,p.N,p.M);
    destroyMatrix(B,p.N,p.M);

    return 0;
}