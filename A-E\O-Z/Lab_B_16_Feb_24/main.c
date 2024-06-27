#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define STRING_LENGHT 15
typedef struct{
    char *input_file;
    int n;
    int m;
    char *output_file;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=5){
        fprintf(stderr,"Usage: %s <input_file> <n> <m> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.input_file=argv[1];
    p.n=atoi(argv[2]);
    p.m=atoi(argv[3]);
    p.output_file=argv[4];

    if(p.n<0){
        fprintf(stderr,"n deve essere maggiore di 0\n");
        exit(EXIT_FAILURE);
    }
    
    if(p.m<0){
        fprintf(stderr,"m deve essere maggiore di 0\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

char ***allocMatrix(int n, int m){

    char ***M=malloc(sizeof(char **)*n);
    
    if(M==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica della memoria\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<n; i++){
        M[i]=(char **)malloc(sizeof(char *)*m);
        if(M[i]==NULL){
            fprintf(stderr,"Errore nell'allocazione dinamica della memoria\n");
            exit(EXIT_FAILURE);
        }
        for(int j=0; j<m; j++){
            M[i][j]=(char *)malloc(sizeof(char)*STRING_LENGHT);
            if(M[i][j]==NULL){
                fprintf(stderr,"Errore nell'allocazione dinamica della memoria\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return M;
}

void printMatrix(char ***M, int n, int m){

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            printf("%-15s ", M[i][j]);
        }
        puts("");
    }

}

char ***readMatrix(parameters *p){

    FILE *f=fopen(p->input_file,"r");

    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    char ***M=allocMatrix(p->n,p->m);

    for(int i=0; i<p->n; i++){
        for(int j=0; j<p->m; j++){
            if(fscanf(f,"%s ",M[i][j])==EOF){
                fprintf(stderr,"Errore nel parsing del file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(f);

    return M;
}

char ***trasposeMatrix(char ***M, int n, int m){

    char ***Mt=allocMatrix(m,n);

    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
           strncpy(Mt[i][j],M[j][i],STRING_LENGHT);
        }
    }

    return Mt;

}

void sortColMatrix(char ***Mt, int n, int m){

    for(int j=0; j<n ;j++){
        for(int i=1; i<m; i++){
            char *string=Mt[i][j];
            int index=i-1;
            while(index>=0 && strcmp(Mt[index][j],string)>0){
                Mt[index+1][j]=Mt[index][j];
                index--;
            }
            Mt[index+1][j]=string;
        }
    }

}

void writeMatrix(parameters *p, char ***Mt){
    
    FILE *f=fopen(p->output_file, "w");

    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in scrittura\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i=0; i<p->m; i++){
        for(int j=0; j<p->n; j++){
            fprintf(f, "%-15s ", Mt[i][j]);
        }
        fprintf(f,"\n");
    }

    fclose(f); 
}

void destroyMatrix(char ***M, int n, int m){
    
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
    //printf("%s %d %d %s\n", p.input_file, p.n, p.m, p.output_file);
    
    char ***M=readMatrix(&p);
    printf("\n** M **\n");
    printMatrix(M,p.n,p.m);
    
    char ***Mt=trasposeMatrix(M,p.n,p.m);
    printf("\n** Mt **\n");
    printMatrix(Mt,p.m,p.n);

    sortColMatrix(Mt,p.n,p.m);
    printf("\n** M-sorted **\n");
    printMatrix(Mt,p.m,p.n);

    writeMatrix(&p, Mt);

    destroyMatrix(Mt,p.m,p.n);
    destroyMatrix(M,p.n,p.m);
    return 0;
}