#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct{
    int N;
    int M;
    char *nome_file;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=4){
        fprintf(stderr,"Usa i parametri: %s <nome_file> <N> <M>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.nome_file=argv[1];
    p.N=atoi(argv[2]);
    p.M=atoi(argv[3]);

    if(p.N<5 || p.N>10){
        fprintf(stderr,"N deve essere compreso nell'intervallo [5,10]\n");
        exit(EXIT_FAILURE);
    }

    if(p.M<5 || p.M>10){
        fprintf(stderr,"M deve essere compreso nell'intervallo [5,10]\n");
        exit(EXIT_FAILURE); 
    }

    return p;
}

double ***allocateMatrix(int n, int m){

    double ***M=malloc(sizeof(double**)*n);
    if(M==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<n; i++){
        M[i]=(double **)malloc(sizeof(double*)*m);
        if(M[i]==NULL){
            fprintf(stderr,"Errore nell'allocazione dinamica\n");
            exit(EXIT_FAILURE);
        }
        for(int j=0; j<m; j++){
            M[i][j]=(double *)malloc(sizeof(double));
            if(M[i][j]==NULL){
                fprintf(stderr,"Errore nell'allocazione dinamica\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return M;
}

double ***fillMatrix(parameters *p){

    FILE *f=fopen(p->nome_file,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    double ***A=allocateMatrix(p->N,p->M);

    for(int i=0; i<p->N; i++){
        for(int j=0; j<p->M; j++){
            if(fscanf(f,"%lf ", A[i][j])==EOF){
                fprintf(stderr,"Errore nel parsing del file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(f);
    return A;
}

void printMatrix(double ***M, int n, int m){

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            printf("%lf ",*M[i][j]);
        }
        printf("\n");
    }

}


double ***normalize(double ***A, int n, int m){

    double ***B=allocateMatrix(n,m);

    for(int j=0; j<m; j++){
        double MAX_A_j=0;
        for(int i=0; i<n; i++){
            if(MAX_A_j < *A[i][j]){
                MAX_A_j=*A[i][j];
            }
        }

        for(int i=0; i<n; i++){
            *B[i][j]=*A[i][j]/MAX_A_j;
        }
    }

    return B;
}

//In realtà la funzione per l'ordinamento doveva essere una sola: sortMatrixCols()
//La consegna chiede di utilizzare un ordinamento tra: bubblesort / insertionsort / selectionsort

void insertionSortCols(double ***M, int n, int m){

    for(int j=0; j<m; j++){
        for(int i=1; i<n; i++){
            int index=i-1;
            double tmp=*M[i][j];
            while(index>=0 && *M[index][j]>tmp){
                *M[index+1][j]=*M[index][j];
                index--;
            }
            *M[index+1][j]=tmp;
        }
    }

}

void BubbleSortCols(double ***M, int n, int m){

    int swap=1;

    while(swap){
        swap=0;
        for(int j=0; j<m; j++){
            for(int i=0; i<n-1; i++){
                if(*M[i+1][j]<*M[i][j]){
                    double tmp=*M[i][j];
                    *M[i][j]=*M[i+1][j];
                    *M[i+1][j]=tmp;
                    swap=1;
                }
            }
        }
    }

}

void selectionSort(double ***M, int n, int m){

    for(int j=0; j<m; j++){
        for(int i=1; i<n-1; i++){
            int min=i;
            for(int k=i+1; k<n; k++){
                if(*M[min][j]>*M[k][j]){
                    min=k;
                }
            }
            double tmp=*M[i][j];
            *M[i][j]=*M[min][j];
            *M[min][j]=tmp;
        }
    }

}

void destroyMatrix(double ***M, int n, int m){

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            free(M[i][j]);
            M[i][j]=NULL;
        }
        free(M[i]);
        M[i]=NULL;
    }
    free(M);
    M=NULL;

}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);
    
    double ***A=fillMatrix(&p);
    printf("** A **\n");
    printMatrix(A,p.N,p.M);

    double ***B=normalize(A,p.N,p.M);
    printf("\n** B **\n");
    printMatrix(B,p.N,p.M);

    //insertionSortCols(B,p.N,p.M);
    //BubbleSortCols(B,p.N,p.M);
    selectionSort(B,p.N,p.M);
    printf("\n** B (sorted columns) **\n");
    printMatrix(B,p.N,p.M);

    destroyMatrix(A,p.N,p.M);
    destroyMatrix(B,p.N,p.M);

    return 0;
}