#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct{
    int a;
    int b;
    double alpha;
    double beta;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=5){
        fprintf(stderr,"Usa parametri:%s <a> <b> <alpha> <beta>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.a=atoi(argv[1]);
    p.b=atoi(argv[2]);
    p.alpha=atoi(argv[3]);
    p.beta=atoi(argv[4]);

    if(p.a<5 || p.a>20){
        fprintf(stderr,"a deve essere compreso nell'intervallo [5,20]\n");
        exit(EXIT_FAILURE);
    }
    
    if(p.b<5 || p.b>20){
        fprintf(stderr,"b deve essere compreso nell'intervallo [5,20]\n");
        exit(EXIT_FAILURE);
    }
    
    if(p.alpha<10 || p.alpha>20){
        fprintf(stderr,"alpha deve essere compreso nell'intervallo [5,20]\n");
        exit(EXIT_FAILURE);
    }

    if(p.beta<1 || p.beta>5){
        fprintf(stderr,"alpha deve essere compreso nell'intervallo [5,20]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

unsigned int get_random() {
static unsigned int m_w = 123456;
static unsigned int m_z = 789123;
m_z = 36969 * (m_z & 65535) + (m_z >> 16);
m_w = 18000 * (m_w & 65535) + (m_w >> 16);
return (m_z << 16) + m_w;
}

double ***allocMatrix(int a, int b){

    double ***M=malloc(sizeof(double **)*a);
    if(M==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<a; i++){
        M[i]=(double **)malloc(sizeof(double*)*b);
        if(M[i]==NULL){
            fprintf(stderr,"Errore nell'allocazione dinamica\n");
            exit(EXIT_FAILURE);
        }
        for(int j=0; j<b; j++){
            M[i][j]=(double *)malloc(sizeof(double));
            if(M[i][j]==NULL){
                fprintf(stderr,"Errore nell'allocazione dinamica\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return M;
}

void fillMatrix(double ***M, parameters *par){

    double I=(par->alpha + par->beta) - (par->alpha - par->beta);
    for(int i=0; i<par->a; i++){
        for(int j=0; j<par->b; j++){
            double p=get_random()/(RAND_MAX*2.0);
            //printf("p=%lf\n",p);
            if(p>0.2){
                *M[i][j]=get_random()/(RAND_MAX/I + par->alpha);
            }
            else if(p<=0.2){
                free(M[i][j]);
                M[i][j]=NULL;
            }
        }
    }

}

void printMatrix(double ***M, int a, int b){

    for(int i=0; i<a; i++){
        for(int j=0; j<b; j++){
            if(M[i][j]) printf("%5.2lf ",*M[i][j]);
            else printf("%5c ", 'N');
        }
        printf("\n");
    }

}

double ***trasposeMatrix(double ***M, int a, int b){

    double ***Mt=allocMatrix(b,a);
    
    for(int i=0; i<b; i++){
        for(int j=0; j<a; j++){
            if(M[j][i]){
                *Mt[i][j]=*M[j][i];
            }
            else{
                free(Mt[i][j]);
                Mt[i][j]=NULL;
            }
        }
    }

    return Mt; 
}

void destroyMatrix(double ***M,int a, int b){
    
    for(int i=0; i<a; i++){
        for(int j=0; j<b; j++){
            free(M[i][j]);
        }
        free(M[i]);
    }
    free(M);

}

int main(int argc, char **argv){
    
    parameters par=readInput(argc,argv);
    double ***m=allocMatrix(par.a,par.b);
    fillMatrix(m,&par);
    printf("*** Matrice m (%d x %d) ***\n", par.a, par.b);
    printMatrix(m,par.a,par.b);
    
    double ***m1=trasposeMatrix(m,par.a,par.b);
    printf("*** Matrice m1 (%d x %d) ***\n", par.b, par.a);
    printMatrix(m1,par.b,par.a);

    destroyMatrix(m,par.a,par.b);
    destroyMatrix(m1,par.b,par.a);

    return 0;
}