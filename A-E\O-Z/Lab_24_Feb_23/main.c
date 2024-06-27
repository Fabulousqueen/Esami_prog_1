#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct{
    int N;
    char a;
    char b;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=4){
        fprintf(stderr,"Usa i parametri: %s <N> <a> <b>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.N=atoi(argv[1]);
    p.a=*argv[2];
    p.b=*argv[3];

    if(p.N<5 || p.N>10){
        fprintf(stderr,"N deve essere compreso nell'intervallo [5,10]\n");
        exit(EXIT_FAILURE);
    }

    if(p.a<'a' || p.a>'z'){
        fprintf(stderr,"a deve essere una lettera tra a-z");
        exit(EXIT_FAILURE);
    }

    if(p.b<'a' || p.b>'z'){
        fprintf(stderr,"a deve essere una lettera tra a-z");
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

typedef struct node{
    char c;
    struct node *next;
}node;

typedef node *stack;

char genVowel(){

    char vocali[]="aeiou";
    
    return vocali[get_random()%5];
}

char genConsonant(){

    char consonanti[]="qwrtypsdfghjklzxcvbnm";
    
    return consonanti[get_random()%21];
}

void push(stack *top, char c){

    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita\n");
        exit(EXIT_FAILURE);
    }
    n->c=c;

    n->next=*top;
    *top=n;
}

stack fillStack(int N, char a, char b){

    stack top=NULL;
    for(int i=0; i<N; i++){
        int x=get_random()%9+1;
        char c;
        for(int j=0; j<x; j++){
            if(x>=1 && x<=4){
                c=genVowel();
            }
            else if(x>=5 && x<=9){
                c=genConsonant();
            }

            if(c==a){
                c='*';
            }
            else if(c==b){
                c='?';
            }
            push(&top,c);
        }
        c=x+'0';//converto x in un carattere e lo assegno a c
        push(&top,c);
    }

    return top;
}

void printStack(stack *top){

    printf("** TOP->  ");
    while(*top!=NULL){
        node *ptr=*top;
        printf("%c ",ptr->c);
        *top=(*top)->next;
        free(ptr);
    }
    printf("<- BOTTOM **\n\n");
}

//Nel frattempo che stampo la pila, dealloco la memoria che essa occupa.
//Avrei potuto fare una funzione a parte per distruggere la pila.

int main(int argc, char **argv){
    
    parameters p=readInput(argc,argv);
    stack top=fillStack(p.N,p.a,p.b);
    printStack(&top);

    return 0;
}