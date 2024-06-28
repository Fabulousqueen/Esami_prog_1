#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 31//deve essere compreso il carattere di terminazione (30+1)
typedef struct{
    char *input;
}parameters;

parameters readInput(int argc,char **argv){

    if(argc!=2){
        fprintf(stderr,"Usa i parametri: %s <input>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.input=argv[1];

    return p;
}

typedef struct node{
    char word[SIZE];
    struct node *next;
}node;

typedef struct{
    node *head;
    node *tail;
}queue;

void initQueue(queue *q){

    q->head=q->tail=NULL;

}

void enqueueWord(queue *q, char *word){

    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Errore nell'allocazione dimnamica\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->word,word,SIZE);

    if(q->head==NULL && q->tail==NULL){
        q->head=q->tail=n;
        n->next=NULL;//Non è necessario
    }
    else{
        q->tail->next=n;
        q->tail=n;
    }

}

char *dequeueWord(queue *q){

    node *tmp=q->head;

    char *word=malloc(sizeof(char)*SIZE);
    if(word==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }
    strncpy(word,tmp->word,SIZE);
    q->head=q->head->next;
    free(tmp);

    return word;
}

queue buildQueue(char *input, int *n){

    FILE *f=fopen(input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    queue q;
    initQueue(&q);
    char word[SIZE];
    while(fscanf(f,"%s\n",word)!=EOF){
        enqueueWord(&q,word);
        (*n)++;
    }

    fclose(f);
    return q;
}

/*
//--countStrings() definita per contare gli elementi della coda. Non è necessaria, 
//dato che buildQueue() conta quanti elementi sono stati letti da input.txt-- 
 
int countStrings(queue q){

    int n=0;
    while(q.head!=NULL){
        n++;
        q.head=q.head->next;
    }

    //printf("%d ",n);
    return n;
}
*/

void printQueue(queue q){

    while(q.head!=NULL){
        printf("%s\n",q.head->word);
        q.head=q.head->next;
    }

}

char **buildStringArray(queue q, int n){

    char **W=malloc(sizeof(char*)*n);

    for(int i=0; i<n; i++){
        char *word=dequeueWord(&q);
        //printf("%s\n",word);
        W[i]=strdup(word);
        free(word);
    }

    return W;
}


void printArray(char **words, int n){

    for(int i=0; i<n; i++){
        printf("%s\n",words[i]);
    }

}

//==========INSERTION SORT==========//
void sortArray(char **W, int n){

    for(int i=1; i<n; i++){
        int index=i-1;
        char *word_tmp=W[i];
        while(index>=0 && strcmp(W[index],word_tmp)>0){
            W[index+1]=W[index];
            index--;
        }
        W[index+1]=word_tmp;
    }
    
} 

/*
//--ESERCIZI:algoritmi di ordinamento--

//==========BUBBLE SORT==========//
void sortArray(char **W, int n){

    int swap=1;

    while(swap){
        swap=0;
        for(int i=0; i<n-1; i++){
            if(strcmp(W[i+1],W[i])<0){
                char *tmp=W[i];
                W[i]=W[i+1];
                W[i+1]=tmp;
                swap=1; 
            }
        }
    }

}

//==========SELECTION SORT==========//
void sortArray(char **W, int n){

    for(int i=0; i<n-1; i++){
        int min=i;
        for(int j=i+1; j<n; j++){
            if(strcmp(W[min],W[j])>0){
                min=j;
            }
        }
        char *tmp=W[i];
        W[i]=W[min];
        W[min]=tmp;
    }

}
*/

void destroyArray(char **W, int n){

    for(int i=0; i<n; i++){
        free(W[i]);
        W[i]=NULL;
    }
    free(W);
    W=NULL;

}

int main(int argc,char **argv){

    parameters p=readInput(argc,argv);
    int n=0;
    queue q=buildQueue(p.input,&n);
    //printf("n=%d",n);
    //printQueue(q);
    char **W=buildStringArray(q,n);
    sortArray(W,n);
    printArray(W,n);

    destroyArray(W,n);

    return 0;
}