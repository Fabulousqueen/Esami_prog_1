#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 30
typedef struct{
    char *input;
    int k;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=3){
        fprintf(stderr,"Usa i parametri:%s <input> <k>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.input=argv[1];
    p.k=atoi(argv[2]);

    if(p.k<7 || p.k>13){
        fprintf(stderr,"k deve essere un numero compreso tra [7,13]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

typedef struct node{
    char word[30];
    struct node *next;
    struct node *prev;
}node;

typedef node *list;

void insertWord(list *l, char *word){

    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Errore nell'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->word,word,SIZE);

    if(*l==NULL){
        *l=n;
        n->next=n->prev=NULL;
    }
    else if(strcmp((*l)->word,n->word)>0){
        n->prev=NULL;
        n->next=*l;
        (*l)->prev=n;
        *l=n;
    }
    else{
        node *ptr=*l;
        while(ptr->next!=NULL && strcmp(ptr->next->word,n->word)<0){
            ptr=ptr->next;
        }

        if(ptr->next!=NULL){
            n->next=ptr->next;
            ptr->next->prev=n;
        }
        ptr->next=n;
        n->prev=ptr;

    }

}

list buildList(char *input){

    FILE *f=fopen(input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

    list l=NULL;
    char word[SIZE];

    while(fscanf(f,"%s\n",word)!=EOF){
        insertWord(&l, word);
    }

    fclose(f);
    return l;
}

void printList(list l){

    while(l!=NULL){
        printf("%s\n",l->word);
        l=l->next;
    }

}

int elab(list l, int k){

    int n=0;

    while(l!=NULL){
        int lenght=strlen(l->word);
        if(lenght>=k){
            n++;
        }
        l=l->next;
    } 

    return n;

}

void destroyList(list l){

    while(l!=NULL){
        node *ptr=l;
        l=l->next;
        free(ptr);
    }

}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);
    list l=buildList(p.input);
    int n=elab(l,p.k);
    printf("Numero di stringhe aventi lunghezza pari o maggiore di %d: %d\n",p.k,n);
    printf("\nOutput:\n");
    printList(l);

    destroyList(l);
    return 0;
}