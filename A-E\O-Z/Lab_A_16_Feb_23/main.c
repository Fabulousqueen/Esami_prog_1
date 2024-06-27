#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct{
    char *input;
    char *output;
    int n;
    int m;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=5){
        fprintf(stderr,"Usa i parametri: %s <input> <output> <n> <m>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.input=argv[1];
    p.output=argv[4];
    p.n=atoi(argv[2]);
    p.m=atoi(argv[3]);

    if(p.n<0){
        if(stderr,"n deve essere maggiore di 0\n");
        exit(EXIT_FAILURE);
    }
    
    if(p.m<0){
        if(stderr,"n deve essere maggiore di 0\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

double ***allocMatrix(int n, int m){

    double ***M=malloc(sizeof(double**)*n);
    if(M==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<n; i++){
        M[i]=(double **)malloc(sizeof(double *)*m);
        if(M[i]==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
        }   
        for(int j=0; j<m; j++){
            M[i][j]=(double *)malloc(sizeof(double));
            if(M[i][j]==NULL){
            fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
            exit(EXIT_FAILURE);
            }
        }
    }

    return M;
}

double ***readMatrix(parameters p){

    FILE *f=fopen(p.input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    double ***M=allocMatrix(p.n, p.m);
    
    for(int i=0; i<p.n; i++){
        for(int j=0; j<p.m; j++){
            if(fscanf(f,"%lf ", M[i][j])==EOF){
                fprintf(stderr,"Errore nel parsing del file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(f);
    return M;
}

double ***trasposeMAtrix(double ***M, int n, int m){

    double ***Mt=allocMatrix(m,n);

    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            *Mt[i][j]=*M[j][i];
        }
    }

    return Mt;
}

void printMatrix(double ***M, int n, int m){
    
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            printf("%6.2lf ",*M[i][j]);
        }
        puts("");
    }

}

void writeMatrix(double ***Mt, parameters p){

    FILE *f=fopen(p.output, "w");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<p.m; i++){
        for(int j=0; j<p.n; j++){
            fprintf(f,"%6.2lf ",*Mt[i][j]);
        }
        fprintf(f,"\n");
    }

    fclose(f);
}

void sortColMatrix(double ***Mt, int n, int m){
    
    for(int j=0; j<n; j++){
        for(int i=1; i<m; i++){  
            int index=i-1;
            double element=*Mt[i][j];
            while(index>=0 && *Mt[index][j]>element){
                *Mt[index+1][j]=*Mt[index][j];
                index--;
            }
            *Mt[index+1][j]=element;
        }
    }

}

/*
//--Per esercizio faccio anche bubbleSort e selectioSort,
//ma non sono richiesti dal compito--

//=============== BUBBLE SORT ==============

void sortColMatrix(double ***Mt, int n, int m){

    int swap=1;

    while(swap){
        swap=0;
        for(int j=0; j<n; j++){
            for(int i=0; i<m-1; i++){  
                if(*Mt[i][j]>*Mt[i+1][j]){
                    double tmp=*Mt[i][j];
                    *Mt[i][j]=*Mt[i+1][j];
                    *Mt[i+1][j]=tmp;
                    swap=1;
                }
            }
        }
    }
}

//============== SELECTION SORT ==============
void sortColMatrix(double ***Mt, int n, int m){

    for(int j=0; j<n; j++){
        for(int i=0; i<m; i++){
            int min=i;  
            for(int k=i+1; k<m; k++){
                if(*Mt[min][j]>*Mt[k][j]){
                    min=k;
                }
            }
            double tmp=*Mt[i][j];
            *Mt[i][j]=*Mt[min][j];
            *Mt[min][j]=tmp;
        }
    }

}
*/

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
    //printf("input=%s output=%s n=%d m=%d\n", p.input, p.output, p.n, p.m);
    
    double ***M=readMatrix(p);
    printf("\n** M **\n");
    printMatrix(M,p.n,p.m);

    double ***Mt=trasposeMAtrix(M,p.n,p.m);
    printf("\n** Mt **\n");
    printMatrix(Mt,p.m,p.n);

    sortColMatrix(Mt, p.n, p.m);
    printf("\n** Mt-sorted**\n");
    printMatrix(Mt,p.m,p.n);

    writeMatrix(Mt,p);

    destroyMatrix(M,p.n,p.m);
    destroyMatrix(Mt,p.m,p.n);

    return 0;
}