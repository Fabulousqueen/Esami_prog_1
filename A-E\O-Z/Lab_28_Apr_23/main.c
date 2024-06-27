#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 30
typedef struct{
    char *nome_file;
    char a;
    char b;
}parameters;

parameters readInput(int argc,char **argv){

    if(argc!=4){
        fprintf(stderr,"Usa i parametri:%s <file> <a> <b>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.nome_file=argv[1];
    p.a=*argv[2];
    p.b=*argv[3];

    if(p.a>p.b){
        fprintf(stderr,"a<=b\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

typedef struct node{
    struct node *next;
    char word[SIZE];
}node;

typedef struct{
    node *head;
    node *tail;
}queue;

void initQueue(queue *q){

    q->head=q->tail=NULL;

}

void enQueue(queue *q,char *word){

    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Errore nell'allocazione di memori per il nuovo nodo\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->word,word,SIZE);

    if(q->head==NULL && q->tail==NULL){
        q->head=q->tail=n;
    }
    else{
        q->tail->next=n;
        q->tail=n;
    }

}

char *deQueue(queue *q){

    node *ptr=q->head;
    q->head=q->head->next;

    char *word=malloc(sizeof(char)*SIZE);
    if(word==NULL){
        fprintf(stderr,"Errore nell'allocazione di memoria per la stringa\n");
        exit(EXIT_FAILURE);
    }

    strncpy(word,ptr->word,SIZE);
    free(ptr);
    return word;
}

queue buildQueue(char *nome_file){

    FILE *f=fopen(nome_file,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in lettura\n");
        exit(EXIT_FAILURE);
    }

    queue q;
    initQueue(&q);

    char word[SIZE];
    while(fscanf(f,"%s\n",word)!=EOF){
        enQueue(&q,word);
    }

    fclose(f);
    return q;
}

void printQueue(queue *q){
    
    while(q->head!=NULL){
        char *word=deQueue(q);
        printf("%s\n",word);
        free(word);
        //q.head=q.head->next;
    }

}

void elab(queue q, char a, char b){

    while(q.head!=NULL){
        for(int i=0; i<SIZE; i++){
            if(q.head->word[i]>=a && q.head->word[i]<=b){
                q.head->word[i]='0';
            }
        }
        q.head=q.head->next;
    }

}

int main(int argc,char **argv){

    parameters p=readInput(argc,argv);
    queue q=buildQueue(p.nome_file);
    elab(q,p.a,p.b);
    printQueue(&q);

    return 0;
}