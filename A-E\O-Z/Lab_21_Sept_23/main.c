#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 31//Si deve includere il carattere di terminazione(30+1)
typedef struct{
    char *file_name;
    int k;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=3){
        fprintf(stderr,"Usa parametri: %s <file_name> <k>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.file_name=argv[1];
    p.k=atoi(argv[2]);

    if(p.k<5 || p.k>15){
        fprintf(stderr,"k deve essere compreso nell'intervallo [5,15]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

typedef struct node{
    struct node *next;
    char word[SIZE];
}node;

typedef node *stack;

void pushWord(stack *top, char *word){

    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita(come te)\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->word,word,SIZE);

    n->next=*top;
    *top=n;
}

char *popWord(stack *top){

    node *ptr=*top;
    *top=(*top)->next;
    
    char *word=malloc(sizeof(char)*SIZE);
    if(word==NULL){
        fprintf(stderr,"Allocazione dinamica della memoria fallita(come te)\n");
        exit(EXIT_FAILURE);
    }
    strncpy(word,ptr->word,SIZE);
    
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

int countStrings(stack top){

    int n=0;
    while(top!=NULL){
        n++;
        top=top->next;
    }
    return n;
}

/*
//==============INSERTION SORT==============//
void sortWord(stack top, int k){

    while(top!=NULL){
        int lenght=strlen(top->word);
        if(lenght>=k){
            for(int j=1; j<lenght; j++){
                int index=j-1;
                char temp=top->word[j];
                while(index>=0 && top->word[index]>temp){
                    top->word[index+1]=top->word[index];
                    index--;
                }
                top->word[index+1]=temp;
            }
        }
        top=top->next;
    }

}
//==============BUBBLE SORT==============//
void sortWord(stack top, int k){
    
    int swap=1;

    while(swap){
        swap=0;
        node *current=top;
        while(current!=NULL){
        int lenght=strlen(current->word);
        if(lenght>=k){
            for(int j=0; j<lenght-1; j++){
                if(current->word[j+1]<current->word[j]){
                    char temp=current->word[j+1];
                    current->word[j+1]=current->word[j];
                    current->word[j]=temp;
                    swap=1;
                }
            }
        }
        current=current->next;
        }
    }

}
*/
//==============SELECTION SORT==============//
void sortWord(stack top, int k){

    node *current=top;
    while(current!=NULL){
        int lenght=strlen(current->word);
        if(lenght>=k){
            for(int i=0; i<lenght-1; i++){
                int min=i;
                for(int j=i+1; j<lenght; j++){
                    if(current->word[min]>current->word[j]){
                        min=j;
                    }
                }
                char temp=current->word[min];
                current->word[min]=current->word[i];
                current->word[i]=temp;
            }
        }
        current=current->next;
    }

}

void printStack(stack *top){

    while(*top!=NULL){
        char *word=popWord(top);
        printf("%s\n",word);
        free(word);
    }

}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);
    stack top=buildStack(p.file_name);
    //int n=countStrings(top);
    sortWord(top,p.k);
    printStack(&top);

    return 0;
}