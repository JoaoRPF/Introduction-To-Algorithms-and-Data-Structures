#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINHAS 1025
#define MAXNOME 1024
#define NUMSEP 9
#define SLOTSIZE 3000

#define key(a) (a != NULL ? a->word : "")
#define less(a,b) (strcmp(a,b)<0)
#define eq(a,b) (strcmp(a,b)==0)
#define NULLitem NULL

const char separators[] = {' ','\t',',',';','.','?','!','"','\n'};

/*-------------------------------------------------------------------------------------*/
/*--------------------------------------TYPEDEF----------------------------------------*/
/*-------------------------------------------------------------------------------------*/

typedef struct{
char*conteudo; 
int id;
}linha;

typedef struct palavra{
  char*word;
  int numOcorrencias;
  int*lista;
  int tamanho_lista;
}*Item;

typedef char* Key;

typedef struct STnode* link;
struct STnode { Item item; link l, r;};

/*-------------------------------------------------------------------------------------*/
/*--------------------------------------FUNCOES----------------------------------------*/
/*-------------------------------------------------------------------------------------*/

void split(char *lista, int id);
void IndexWord(char* word, int nlinha, int z);
void IncrementaItem(Item x, int line_number);
Item newItem(char*nome, int linha);
void deleteItem(Item a);
void STinit(link*head);
int count(link h);
int STcount(link head);
link insertR(link h, Item item);
void STinsert(link*head, Item item);
link NEW(Item item, link l, link r);
Item searchR(link h, Key v);
Item STsearch(link head, Key v);
link deleteR(link h, Key k);
link delete(link h, Item item);
link max(link h);
void STdelete(link*head, Key k);
void sortR(link h, void (*visit)(Item));
void STsort(link head, void (*visit)(Item));
void visitItem(Item a);
void strtolower(char*string);
int checkconsec(char*linha_verificar,char*Pa,char*Pb);
int checkexistenceline(Item resultado2,int linha_verificar);


link texto;
linha*lista;

int main (){
  int numeroLinhas,i;
  char buffer[MAXNOME+1], Pa[MAXNOME+1],Pb[MAXNOME+1], c;
  /* const char auxw[MAXNOME+1]; */
  Item resultado,resultado2;
  scanf("%d",&numeroLinhas);
  getchar();
  lista=(linha*)malloc(sizeof(linha)*numeroLinhas);                /*   ALOCAÇÃO DE MEMORIA   */
  for(i=0;i<numeroLinhas;i++){
    fgets(buffer,MAXNOME,stdin);
    lista[i].conteudo=(char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(lista[i].conteudo,buffer);
    lista[i].id=i;
  }
  STinit(&texto);
  for(i=0;i<numeroLinhas;i++)
    split(lista[i].conteudo,lista[i].id);
  while(scanf("%c",&c)>0){
    switch(c){
    case 'f':
      scanf("%s",buffer);
      strtolower(buffer);
      STdelete(&texto,buffer);
      break;
    case 's':
      for(i=0;i<numeroLinhas;i++)
  	printf("%s", lista[i].conteudo);
      break;
    case 'l':
      scanf("%s",buffer);
      strtolower(buffer);
      resultado=STsearch(texto,buffer);
      if (resultado!=NULLitem){
	for(i=0;i<(resultado->tamanho_lista);i++){
	  printf("%d %s",(lista[resultado->lista[i]].id)+1,lista[resultado->lista[i]].conteudo);
	}
      }
      break;
    case 'w':
      scanf("%s %s",Pa,Pb);
      strtolower(Pa);
      strtolower(Pb);
      resultado=STsearch(texto,Pa);
      resultado2=STsearch(texto,Pb);
      printf("%d\n",resultado->lista[0]);
      printf("%d\n",resultado->lista[1]);
      if (resultado != NULLitem && resultado2 != NULLitem){
    	for (i=0;i<(resultado->tamanho_lista);i++){
    	  if (checkexistenceline(resultado2,resultado->lista[i])){
    	    if (checkconsec(lista[resultado->lista[i]].conteudo,Pa,Pb)){
	      printf("%d %s",(lista[resultado->lista[i]].id)+1,lista[resultado->lista[i]].conteudo);
	    }
	  }
    	}
      }
      break;
    case 'h':
      STsort(texto,visitItem);
      break;
    /* case '-1': */
    /*   return 0; */
    }
  }
  for(i=0;i<numeroLinhas;i++)
    free(lista[i].conteudo);
  free(lista);
  return 0;
}

void strtolower(char*string){
  int i=0;
  while(string[i]!='\0'){
    string[i]=tolower(string[i]);
    i++;
  }
}
void split(char *lista, int id){
  int i, j, k;
  static int z=0;
  char buffer[MAXLINHAS];
  for(i = 0, k = 0; lista[i] != '\0'; i++, k++) {
    buffer[k] = tolower(lista[i]);
    for(j = 0; j < NUMSEP; j++) {
      if(lista[i] == separators[j]){
	if(k != 0) {
	  buffer[k] = '\0';
	  z++;
	  IndexWord(buffer,id,z);
	}
	k = -1;
      }
    }
  }
}

void IndexWord(char* word, int nlinha, int z){
  Item x = STsearch(texto, word);
  if(x==NULLitem)
    STinsert(&texto,newItem(word,nlinha));
  else
    IncrementaItem(x, nlinha);
}

void IncrementaItem(Item x, int line_number){
 x->numOcorrencias++;  
 if (x->lista[(x->tamanho_lista)-1]!=line_number){ /*ver se mudou de linha*/
    if (x->tamanho_lista % SLOTSIZE == 0)
      x->lista=realloc(x->lista,(SLOTSIZE + x->tamanho_lista)*sizeof (int));
    x->tamanho_lista++;
    x->lista[(x->tamanho_lista)-1]=line_number;
  }
}

Item newItem(char*word, int linha){ /*linha é a linha onde a palavra aparece*/
  Item x = (Item)malloc(sizeof(struct palavra));
  x->word = (char*)malloc(sizeof(char)*(strlen(word)+1));
  strcpy(x->word,word);
  x->numOcorrencias=1;
  x->lista = (int*)malloc(sizeof(int)*SLOTSIZE);
  x->lista[0]=linha;
  x->tamanho_lista=1;
  return x;
}

/* ------------------------------------- */
void STinit(link*head){
  *head = NULL;
}
/* ------------------------------------- */
int count(link h){
  if (h==NULL) return 0;
  else return count(h->r)+count(h->l)+1;
}

int STcount(link head){
  return count(head);
}
/*--------------------------------------*/
link insertR(link h, Item item) {
  if (h == NULL)
    return NEW(item, NULL, NULL);
  if (less(key(item), key(h->item)))
    h->l = insertR(h->l, item);
  else
    h->r = insertR(h->r, item);
  return h;
}

void STinsert(link*head, Item item){
  *head = insertR(*head, item);
}
link NEW(Item item, link l, link r){
  link x = (link)malloc(sizeof(struct STnode));
  x->item = item;
  x->l = l;
  x->r = r;
  return x;
}
/* ------------------------------------- */
Item searchR(link h, Key v){
  if (h == NULL)
    return NULLitem;
  if (eq(v, key(h->item)))
    return h->item;
  if (less(v, key(h->item)))
    return searchR(h->l, v);
  else
    return searchR(h->r, v);
}

Item STsearch(link head, Key v){
  return searchR(head, v);
}
/* ------------------------------------- */
link deleteR(link h, Key k){
  if (h==NULL) return h;
  else if (less(k, key(h->item))) h->l=deleteR(h->l,k);
  else if (less(key(h->item), k)) h->r=deleteR(h->r,k) ;
  else {
    if (h->l !=NULL && h->r !=NULL){ /*caso 3*/
      link aux=max(h->l);
      {Item x; x=h->item; h->item=aux->item; aux->item=x; }
      h->l= deleteR(h->l, key(aux->item));
    }
    else { /*casos 1 e 2*/
      link aux=h;
      if ( h->l == NULL && h->r == NULL ) h=NULL;
      else if (h->l==NULL) h=h->r;
      else h=h->l;
      deleteItem(aux->item);
      free(aux);
    }
  }
  return h;
}
void STdelete(link*head, Key k){
  *head = deleteR(*head, k);
}

void deleteItem(Item a){
  free(a->word);
  free(a);
}

link max(link h){
  if (h==NULL || h->r==NULL) return h;
  else
    return max(h->r);
}
/*-------------------------------------*/
void sortR(link h, void (*visit)(Item)){
  if (h == NULL)
    return;
  sortR(h->l, visit);
  visitItem(h->item);
  sortR(h->r, visit);
}

void STsort(link head, void (*visit)(Item)){
  sortR(head, visit);
}

void visitItem(Item a){
  printf("%s %d\n",a->word, a->numOcorrencias);
}
/* ------------------------------------- */
int checkconsec(char*lista,char*Pa,char*Pb){
  int i, j, k;
  char buffer[MAXLINHAS],Prev[MAXLINHAS];
  Prev[0]='\0';
  for(i = 0, k = 0; lista[i] != '\0'; i++, k++) {
    buffer[k] = tolower(lista[i]);
    for(j = 0; j < NUMSEP; j++) {
      if(lista[i] == separators[j]){
	if(k != 0) {
	  buffer[k] = '\0';
	  if((strcmp(Pa,buffer)==0 && strcmp(Pb,Prev))==0 || (strcmp(Pb,buffer)==0 && strcmp(Pa,Prev)==0))
	    return 1;
	  strcpy(Prev, buffer);                            
	}
	k = -1;
      }
    }
  }
  return 0;
}
int checkexistenceline(Item resultado2,int linha_verificar){
  int i;
  for(i=0;i<resultado2->tamanho_lista; i++){
    if(resultado2->lista[i]==linha_verificar)
      return 1;
  }
  return 0;
}

/* ------------------------------------- */
