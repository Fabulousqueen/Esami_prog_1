#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct{
    int L;
    char C;
    int A;
    int B;
}parameters;

parameters readInput(int argc,char **argv){

    if(argc!=5){
        fprintf(stderr,"Usa i parametri: %s <L> <C> <A> <B>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.L=atoi(argv[1]);
    p.C=*argv[2];
    p.A=atoi(argv[3]);
    p.B=atoi(argv[4]);

    if(p.L<10 || p.L>15){
        fprintf(stderr,"L deve essere un intero compreso tra [10,15]\n");
        exit(EXIT_FAILURE);
    }

    if(p.C<'a' || p.C>'z'){
        fprintf(stderr,"C deve essere una lettera tra a-z\n");
        exit(EXIT_FAILURE);
    }

    
    if(p.A<4 || p.A>8){
        fprintf(stderr,"A deve essere un intero compreso tra [4,8]\n");
        exit(EXIT_FAILURE);
    }
    
    if(p.B<4 || p.B>8){
        fprintf(stderr,"B deve essere un intero compreso tra [4,8]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

char ***allocateS(int A, int B){

    char ***S=malloc(sizeof(char**)*A);
    if(S==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<A; i++){
        S[i]=(char**)malloc(sizeof(char*)*B);
        if(S[i]==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
        }
    }

    return S;
}

char *genString(int L){

    char *string=malloc(sizeof(char)*L);
    if(string==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<L; i++){
        string[i]=get_random()%('z'-'a'+1)+'a';
    }

    return string;
}

char ***fillS(int A, int B, int L){

    char ***S=allocateS(A,B);

    for(int i=0; i<A; i++){
        for(int j=0; j<B; j++){
            char *string=genString(L);
            S[i][j]=string;
        }
    }

    return S;
}

void printMatrix(char ***S, int A, int B){

    for(int i=0; i<A; i++){
        for(int j=0; j<B; j++){
            printf("%s ", S[i][j]);        
        }
        printf("\n");
    }

}

//=========== INSERTION SORT ===========
void sortS(char ***S, int A, int B){
    
    for(int j=0; j<B; j++){
        for(int i=1; i<A; i++){
            int index=i-1;
            char *string=S[i][j];
            while(index>=0 && strcmp(S[index][j],string)>0){
                S[index+1][j]=S[index][j];
                index--;
            }
            S[index+1][j]=string;
        }
    }    
}

/*
//=========== BUBBLE SORT ===========
void sortS(char ***S, int A, int B){
    
    int swap=1; 
    while(swap){
        swap=0;
        for(int j=0; j<B; j++){
            for(int i=0; i<A-1; i++){
                if(strcmp(S[i][j],S[i+1][j])>0){
                    char *temp=S[i+1][j];
                    S[i+1][j]=S[i][j];
                    S[i][j]=temp;
                    swap=1;
                }
            }
        }
    }

}
*/
/*
//=========== SELECTION SORT ===========
void sortS(char ***S, int A, int B){

    for(int j=0; j<B; j++){
        for(int i=0; i<A-1; i++){
            int min=i;
            for(int k=i+1; k<A; k++){
                if(strcmp(S[min][j],S[k][j])>0){
                    min=k;
                }
            }
            char *temp=S[min][j];
            S[min][j]=S[i][j];
            S[i][j]=temp;
        }
    }    
}
*/

void printMax(char ***S, parameters *p){

    int max=0;
    int row;
    int col;

    for(int i=0; i<p->A; i++){
        for(int j=0; j<p->B; j++){ 
            int num_c=0;//numero massimo di 'c' 
            for(int k=0; k<p->L; k++){
                if(S[i][j][k]==p->C){
                    num_c++;
                }
            }
            if(max<num_c){
                max=num_c;
                row=i;
                col=j;
            }
        }
    }

    printf("Stringa con %d occorrenze di '%c' trovata agli indici %d, %d:\n", max, p->C, row, col);
    
    for(int i=0; i<p->L; i++){
        if(S[row][col][i]==p->C){
            printf("*");
        }
        else{
            printf("%c",S[row][col][i]);
        }
    }
    printf("\n");

}

void destroyS(char ***S, int A, int B){

    for(int i=0; i<A; i++){
        for(int j=0; j<B; j++){
           free(S[i][j]);
           S[i][j]=NULL;        
        }
        free(S[i]);
        S[i]=NULL;
    }
    free(S);
    S=NULL;

}

int main(int argc,char **argv){

    parameters p=readInput(argc,argv);
    //printf("A=%d B=%d L=%d C=%c\n", p.A, p.B, p.L, p.C);
    char ***S=fillS(p.A,p.B,p.L);
    /*printf("S:\n");
    printMatrix(S,p.A,p.B);
    printf("\nS-sorted:\n");*/
    sortS(S,p.A,p.B);
    printf("\n");
    printMatrix(S,p.A,p.B);
    printf("\n");
    printMax(S,&p);

    destroyS(S,p.A,p.B);
    return 0;
}