#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 30
typedef struct{
    char *input;
    int x;
    int y;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=4){
        fprintf(stderr,"Usage: %s <input>\n", argv[0]);
        exit(1);
    }

    parameters p;
    p.input=argv[1];
    p.x=atoi(argv[2]);
    p.y=atoi(argv[3]);

    if(p.x<10 || p.x>60){
        fprintf(stderr,"x deve essere compreso nell'intervallo [10,60]\n");
        exit(1);
    }

    if(p.y<10 || p.y>60){
        fprintf(stderr,"y deve essere compreso nell'intervallo [10,60]\n");
        exit(1);
    }

    if(p.x>p.y){
        fprintf(stderr,"x deve essere minore di y\n");
        exit(1);
    }

    return p;
}

typedef struct record{
    char nome[SIZE];
    char cognome[SIZE];
    int età;
}record;

typedef struct node{
    record r;
    struct node *next;
}node;

typedef node *list;

void insertNode(list *l, record r){

    node *n=malloc(sizeof(node));
    n->next=NULL;
    n->r=r;
    
    //1 CASO: Testa è vuota
    if(*l==NULL){
        *l=n;
    }
    //2 CASO: Inserimento in testa
    else if(strcmp(n->r.cognome,(*l)->r.cognome)<0){
        n->next=*l;//Modifico il next del nuovo nodo, così punta alla vecchia testa
        *l=n;//Modifico la testa così punta al nuovo nodo
    }
    //3 CASO: Ricerca della posizione in cui devo mettere il nuovo nodo
    else{
        node *ptr=*l;
        while(ptr->next!=NULL && strcmp(n->r.cognome, ptr->next->r.cognome)>0){
            /*La strcmp nel while deve valutare se il cognome in n è maggiore del cognome in ptr->next.
            Se la condizione di strcmp fosse stata < 0, allora ptr si sarebbe fermato quando il cognome in 
            ptr->next era minore del cognome in n.*/
            ptr=ptr->next;                                                              
        }                                                                       

        if(ptr->next!=NULL){
            n->next=ptr->next;
        }
        ptr->next=n;
    }
}

list readFile(char *input){

    FILE *f=fopen(input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in lettura\n");
        exit(1);
    }

    list head=NULL;
    record r;

    while(fscanf(f,"%s %s %d\n", r.nome, r.cognome, &r.età)!=EOF){
        insertNode(&head,r);
    }

    fclose(f);
    return head;
}

void printList(list l){

    while(l!=NULL){
        printf("%s %s %d\n", l->r.nome, l->r.cognome, l->r.età);
        l=l->next;
    }
}

node *removeNode(list *l, node *to_rmv){

    if(*l==to_rmv){
        node *ptr=*l;
        *l=(*l)->next;
        free(ptr);
        return *l;
    }
    
    node *ptr=*l;//La versione ottimizzata è ptr=(*l)->next
    node *prev=ptr;

    while(ptr!=NULL && ptr!=to_rmv){
        prev=ptr;
        ptr=ptr->next;
    }

    prev->next=ptr->next;
    free(ptr);

    return prev;
    
}

void removeByAge(list *l, parameters p){

    node *ptr=*l;

    while(ptr!=NULL){
        if(ptr->r.età>=p.x && ptr->r.età<=p.y){
            node *to_rmv=ptr;
            ptr=removeNode(l,to_rmv);
        }  
        ptr=ptr->next;
    }

}

/*

//--Questa funzione è equivalente alle due sopra, solo che la rimozione 
//del nodo dalla lista è diversa da come la si implementa di solito
//(VEDI removeNode())--

void removeByAgeAllInOne(list *l, parameters p){

    while((*l)->r.età>=p.x && (*l)->r.età<=p.y){
        node *ptr=*l;
        *l=(*l)->next;
        free(ptr);
    }

    node *ptr=(*l)->next;
    node *prev=*l;

    while(ptr!=NULL){
        if(ptr->r.età>=p.x && ptr->r.età<=p.y){
            prev->next=ptr->next;
            free(ptr);
            ptr=prev->next;
        }
        prev=prev->next;
        ptr=ptr->next;
    }

}
*/

void destroyList(list *l){
    
    node *ptr=*l;

    while(*l!=NULL){
        *l=(*l)->next;
        free(ptr);
        ptr=*l;
    }

}


int main(int argc, char** argv){

    parameters p=readInput(argc,argv);

    printf("\n(lista ordinata)\n");
    list A=readFile(p.input);
    printList(A);

    printf("\n(lista ordinata dopo la rimozione dei record con eta’ in [30, 45])\n");
    removeByAge(&A,p);
    printList(A);
    //removeByAgeAllInOne(&A,p);
    //printList(A);
    destroyList(&A);
}