#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 30
typedef struct{
    int k;
    char *file_name;
}parameters;

parameters readInput(int argc,char**argv){

    if(argc!=3){
        fprintf(stderr,"Usa i parametri: %s <file_name> <k>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.file_name=argv[1];
    p.k=atoi(argv[2]);

    if(p.k<5 || p.k>15){
        fprintf(stderr,"k deve essere comrpreso tra [5,15]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

typedef struct node{
    char word[SIZE];
    struct node *next;
}node;

typedef node *stack;

void pushWord(stack *top, char *word){

    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->word,word,SIZE);

    n->next=*top;
    *top=n;
}

char *popWord(stack *top){

    node *ptr=*top;
    char *word=malloc(sizeof(char)*SIZE);
    strncpy(word,(*top)->word,SIZE);
    *top=(*top)->next;
    free(ptr);
    return word;
    
}

stack buildStack(char *file_name){

    FILE *f=fopen(file_name,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    stack top=NULL;
    char word[SIZE];
    while(fscanf(f,"%s\n",word)!=EOF){
        pushWord(&top,word);
    }

    fclose(f);
    return top;
}

/*
//=============BUBBLE SORT=============
void sortWord(char *word, int L){

    int swap=1;
    while(swap){
        swap=0;
        for(int i=0; i<L-1; i++){
            if(word[i+1]<word[i]){
                char temp=word[i+1];
                word[i+1]=word[i];
                word[i]=temp;
                swap=1;
            }
        }
    }

}
*/

/*
//=============INSERTION SORT=============
void sortWord(char *word, int L){

    for(int i=1; i<L; i++){
        char temp=word[i];
        int index=i-1;
        while(index>=0 && word[index]>temp){
            word[index+1]=word[index];
            index--;
        }
        word[index+1]=temp;
    }

}
*/

//=============SELECTION SORT=============
void sortWord(char *word, int L){

    for(int i=0; i<L-1; i++){
        int min=i;
        for(int j=i+1; j<L; j++){
            if(word[min]>word[j]){
                min=j;
            }
        }
        char temp=word[min];
        word[min]=word[i];
        word[i]=temp;
    }

}

void printStack(stack *top, int k){

    while(*top!=NULL) {
        char *word=popWord(top);
        int L=strlen(word);
        if (L>=k) {
            sortWord(word,L);
        }
        printf("%s\n",word);
        free(word);
    }
    
}

int main(int argc,char**argv){

    parameters p=readInput(argc,argv);
    stack top=buildStack(p.file_name);
    printStack(&top,p.k);

    return 0;
}