#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct{
    int L;
    char *Z;
    char *W;
    char K; 
}parameters;

unsigned int get_random() {
    static unsigned int m_w = 123456;
   static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

parameters readInput(int argc, char **argv){

    if(argc!=5){
        fprintf(stderr,"Usa parametri:%s <L> <Z> <W> <K>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.L=atoi(argv[1]);
    p.Z=argv[2];
    p.W=argv[3];
    p.K=*argv[4];

    int lenght=strlen(p.Z);
    if(lenght!=p.L){
        fprintf(stderr,"Z deve avere lunghezza L\n");
        exit(EXIT_FAILURE);
    }

    lenght=strlen(p.W);
    if(lenght!=p.L){
        fprintf(stderr,"W deve avere lunghezza L\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

char *replaceChar(char *s1, char *s2, char c){

    int L=strlen(s1);
    char *s3=malloc(sizeof(char)*L);
    if(s3==NULL){
        fprintf(stderr,"Errore nell'allocazionde dinamica della memoria\n");
        exit(EXIT_FAILURE);
    }

    strncpy(s3,s1,L);

    for(int i=0; i<L; i++){
        if(s3[i]==c){
            s3[i]=s2[i];
        }
    }

    return s3;
}

char *inverString(char *s2){

    int L=strlen(s2);
    char *s4=malloc(sizeof(char)*L);
    if(s4==NULL){
        fprintf(stderr,"Errore nell'allocazionde dinamica della memoria\n");
        exit(EXIT_FAILURE);
    }

    int n=L;
    for(int i=0; i!=L; i++){
        s4[i]=s2[n-1];
        n--;
    }
    s4[L]='\0';

    return s4;
}

char *concatString(char *s3, char *s4, int *L){

    *L=strlen(s3)*2;
    char *s5=malloc(sizeof(char)*(*L));
    if(s5==NULL){
        fprintf(stderr,"Errore nell'allocazionde dinamica della memoria\n");
        exit(EXIT_FAILURE);
    }

    strncat(s5,s3,*L);
    strncat(s5,s4,*L);

    return s5;
}

void sort(char *s5, int L){
    
    /*
    //===BUBBLE SORT===
    int swap=1;
    while(swap){
        swap=0;
        for(int i=0; i<L-1; i++){
            if(s5[i]>s5[i+1]){
                char tmp=s5[i+1];
                s5[i+1]=s5[i];
                s5[i]=tmp;
                swap=1;
            }
        }
    }
    */
    
    //===SELECTION SORT===
    for(int i=0; i<L; i++){
        int min=i;
        for(int j=i+1; j<L; j++){
            if (s5[min]>s5[j]){
                min=j;
            }
        }
        char tmp=s5[min];
        s5[min]=s5[i];
        s5[i]=tmp;
    }
    
    /*
    //===INSERTION SORT===
    for(int i=1; i<L; i++){
        int index=i-1;
        char c=s5[i];
        while(index>=0 && s5[index]>c){
            s5[index+1]=s5[index];
            index--;
        }
        s5[index+1]=c;
    }
    */
}

void printResult(char *s5, int L){

   for(int i=0; i<L; i++){
        int c=(int)s5[i];
        if(c%2!=0){
            s5[i]='*';
        }
    } 

    printf("\n%s\n",s5);
}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);

    char *s3=replaceChar(p.Z,p.W,p.K);
    //printf("\n%s\n",s3);
    char *s4=inverString(p.W);
    //printf("\n%s\n",s4);
    int L_s5=0;//lunghezza di s5
    char *s5=concatString(s3,s4,&L_s5);
    //printf("\ns5 (before bubble sort):%s\n",s5);
    sort(s5,L_s5);
    //printf("\ns5 (after bubble sort):%s\n",s5);
    printResult(s5,L_s5);

    free(s3);
    free(s4);
    free(s5);

    return 0;
}