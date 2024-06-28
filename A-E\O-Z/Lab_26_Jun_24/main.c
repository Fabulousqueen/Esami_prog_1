#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE_STRING 21//bisogna comprendere anche il carattere di terminazione (20+1)
#define SIZE_ARRAY 200
typedef struct{
    char *input;
    int k;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=3){
        fprintf(stderr,"Usage: %s <input> <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.input=argv[1];
    p.k=atoi(argv[2]);

    if(p.k<5 || p.k>10){
        fprintf(stderr,"k deve essere compreso tra [5,10]\n");
        exit(EXIT_FAILURE);
    }

    return p;    
}

typedef struct{
    char key[SIZE_STRING];
    int lenght;
}record;

record *buildArray(char *input){

    FILE *f=fopen(input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in lettura\n");
        exit(EXIT_FAILURE);
    }

    record *Matrix_r=malloc(sizeof(record)*SIZE_ARRAY);

    for(int i=0; i<SIZE_ARRAY; i++){
        if(fscanf(f,"%s ",Matrix_r[i].key)!=EOF){
            Matrix_r[i].lenght=strlen(Matrix_r[i].key);
        }
        else{
            Matrix_r[i].lenght=-1;
        }   
    }

    fclose(f);
    return Matrix_r;
}

void filter(record *Matrix_r){

    for(int i=0; i<SIZE_ARRAY; i++){
        for(int j=0; j<Matrix_r[i].lenght; j++){
            
            switch(Matrix_r[i].key[j]){
                case ',':
                case ';':
                case ':':
                case '.':
                    Matrix_r[i].key[j]='\0';
                    Matrix_r[i].lenght--;
                    break;
                default:
                    break;
            }
            
        }
    }

}

void createSet(record *Matrix_r){
    
    for(int i=0; i<SIZE_ARRAY; i++){
        for(int j=i+1; j<SIZE_ARRAY; j++){
            if(strcmp(Matrix_r[i].key,Matrix_r[j].key)==0){
                Matrix_r[j].lenght=-1; 
            }
        }
    }

}

void printArray(record *Matrix_r, int k){
    
    //--PENULTIMO PUNTO e ULTIMO PUNTO utilizzano la stessa funzione, che viene chiamata due volte 
    //con due parametri differenti--
    printf("Lunghezza della stringa >=%d\n",k);
    int index=1;
    for(int i=0; i<SIZE_ARRAY; i++){
        if(Matrix_r[i].lenght!=-1){
            printf("printArray() [%d] [word: %s] [lenght: %d]\n",index,Matrix_r[i].key,Matrix_r[i].lenght);
            index++;
        }
    }

}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);
    record *Matrix_r=buildArray(p.input);
    filter(Matrix_r);

    createSet(Matrix_r);
    printf("createSet()..\n");
    printArray(Matrix_r,1);
    printArray(Matrix_r,p.k);

    free(Matrix_r);
    
    return 0;   
}