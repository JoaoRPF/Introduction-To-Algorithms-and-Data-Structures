#include <stdio.h>
#include <string.h>
#define ATLETA 1
#define ADEPTO 0

typedef struct {
  char nomeProprio[50];
  char apelido [50];
  int estatuto;
  int numsocio;
  int credito;
  int meses;
  int modalidades[26];
} associado;

typedef struct {
  int lucro;
  int balanco;
  associado listaAssociados[1000];
  int numAssociados;
  int modalidadesclube[26];
} clube;

/*-------------------------------------------------FUNCOES AUXILIARES-------------------------------------------------------*/
/*numeroModalidades int -> int
Esta funcao serve para calcular o numero de modalidades em que determinado socio esta inscrito*/
int numeroModalidades(int numsocio,clube*XPTO){
	int i,nModalidades=0;
	for(i=0;i<26;i++){
	  if((*XPTO).listaAssociados[numsocio-101].modalidades[i]==1)
	    nModalidades++;
	}
	return nModalidades;
}


/*incializamodalidades clube -> 
Esta funcao serve para inicializar o vetor modalidades do clube com todas as modalidades a 0*/
void inicializamodalidades(clube*XPTO){
  int i,j;
  for(i=0;i<26;i++){
    (*XPTO).modalidadesclube[i]=0;
    for(j=0;j<1000;j++){
      (*XPTO).listaAssociados[j].modalidades[i]=0;
    }
  }
}
/*--------------------------------------------------------------------------------------------------------------------------*/


void regista(clube*XPTO){
  scanf("%s %s",(*XPTO).listaAssociados[(*XPTO).numAssociados].nomeProprio, (*XPTO).listaAssociados[(*XPTO).numAssociados].apelido);
  (*XPTO).listaAssociados[(*XPTO).numAssociados].estatuto = ADEPTO;
  (*XPTO).listaAssociados[(*XPTO).numAssociados].numsocio = 101+(*XPTO).numAssociados;
  (*XPTO).listaAssociados[(*XPTO).numAssociados].credito = 0;
  (*XPTO).listaAssociados[(*XPTO).numAssociados].meses = 0;
  (*XPTO).numAssociados++;
  (*XPTO).lucro += 200; 
}

void inscreve(clube*XPTO){
  int modalidade,numSocio;
  scanf("%d",&numSocio);
  getchar(); /*le o espaço entre o numero de socio e a modalidade*/
  modalidade = getchar();	/* vai tomar o valor em decimal da tabela ASCII */
  modalidade=modalidade-65;	/*Subtrai-se 65 pois é o valor de "A" e assim garante-se que a modalidade A fica no índice 0*/
  numSocio=numSocio-101;	/*Subtrai-se 101 pois o primeiro socio (indice 0) tem o numero 101*/ 
  if ((*XPTO).listaAssociados[numSocio].credito>=0){
    (*XPTO).listaAssociados[numSocio].modalidades[modalidade]=1;
    (*XPTO).listaAssociados[numSocio].credito+=10;
    (*XPTO).lucro += 10;
    (*XPTO).listaAssociados[numSocio].estatuto=ATLETA;
    (*XPTO).modalidadesclube[modalidade]++;
  }
}

void desinscreve(clube*XPTO){
  int i,numSocio,modalidade;
  scanf("%d",&numSocio);
  getchar();
  modalidade = getchar();
  modalidade=modalidade-65; 
  numSocio=numSocio-101;
  if ((*XPTO).listaAssociados[numSocio].modalidades[modalidade]==1){
    (*XPTO).listaAssociados[numSocio].modalidades[modalidade]=0;
    (*XPTO).modalidadesclube[modalidade]--;
  }
  for(i=0;i<26;i++){
    if((*XPTO).listaAssociados[numSocio].modalidades[i]==1)	/*Teste para ver se o sócio ainda está inscrito noutras modalidades*/
      break;
  } 
  if(i==26)	/*Se o ciclo anterior tiver chegado ao fim, o i vai estar em 26, logo o sócio nao está inscrito em mais nenhuma modalidade*/
    (*XPTO).listaAssociados[numSocio].estatuto=ADEPTO;
}
void lista(int tipo,clube*XPTO){
  int i;
  
  if (tipo==0){		
    for(i=0;i<XPTO->numAssociados;i++){
      printf("%d %s %s %d %d %d\n", (*XPTO).listaAssociados[i].numsocio, (*XPTO).listaAssociados[i].nomeProprio, (*XPTO).listaAssociados[i].apelido, (*XPTO).listaAssociados[i].meses, (*XPTO).listaAssociados[i].credito, numeroModalidades((*XPTO).listaAssociados[i].numsocio,XPTO));
    }
  }
  
  if (tipo==1){
    for(i=0;i<1000;i++){
      if((*XPTO).listaAssociados[i].estatuto==ATLETA)
	printf("%d %s %s %d %d %d\n", (*XPTO).listaAssociados[i].numsocio, (*XPTO).listaAssociados[i].nomeProprio, (*XPTO).listaAssociados[i].apelido, (*XPTO).listaAssociados[i].meses, (*XPTO).listaAssociados[i].credito, numeroModalidades((*XPTO).listaAssociados[i].numsocio,XPTO));
    }
  }
  
  if (tipo==2){
    int i,u=0;
    associado listaDivida[1000],aux;	/*Cria a lista de endividados e um associado auxiliar*/
    for (i=0;i<(*XPTO).numAssociados;i++){	/*Adiciona as pessoas endividadas a uma lista*/
      if((*XPTO).listaAssociados[i].credito<0){
	listaDivida[u]=(*XPTO).listaAssociados[i];
	u++;
      }
    }
    i=0; /*Reinicializa o indice*/
    while (i!=u){ /* u representa o numero de pessoas endividadas*/
      if (listaDivida[i].credito>listaDivida[i+1].credito){ /*organiza mediante a divida*/
	aux=listaDivida[i];
	listaDivida[i]=listaDivida[i+1];
	listaDivida[i+1]=aux;
	i=0;
      }
      if (listaDivida[i].credito==listaDivida[i+1].credito){
	if(listaDivida[i].numsocio>listaDivida[i+1].numsocio){ /*se a divida for igual, organiza mediante o numero de socio*/
	  aux=listaDivida[i];
	  listaDivida[i]=listaDivida[i+1];
	  listaDivida[i+1]=aux;
	  i=0;
	}
      } 
      else
	i++;
    }
    for(i=0;i<u;i++){
      printf("%d %s %s %d %d %d\n", listaDivida[i].numsocio, listaDivida[i].nomeProprio, listaDivida[i].apelido, listaDivida[i].meses, listaDivida[i].credito, numeroModalidades(listaDivida[i].numsocio,XPTO));
    }
  }
  if (tipo==3){
    int i,maior, imaior=0, copia[26];
    for (i=0;i<26;i++){ /* cria uma copia do vector XPTO.modalidadesclube*/
      copia[i]=(*XPTO).modalidadesclube[i];
    }	       
    maior=copia[0];                     
    for(i=0;i<26;i++){
      if (maior<copia[i]){ /*percorre todo o vector copia a procura do maior valor*/			
	maior=copia[i];
	imaior=i;
	i=-1; /*quer-se reinicializar o indice, para isso da-se o valor "-1" pois o ciclo for irá adicionar 1 após fazer uma accao(comando i++)*/
      }
      if (i==25 && maior!=0){ /* quando ja se chegou ao fim do vector (i==25) ja se tem a certeza que "maior" e o maior valor*/
	putchar(imaior+65);
	printf(" %d\n",maior);
	copia[imaior]=0; /* muda-se o antigo maior valor para 0 pois ja foi imprimido */
	i=-1;
	imaior=0;
	maior=0;
      }
      if (i==25 && maior==0) /* quando se chega ao final do vector e "maior" tem o valor 0, nao ha mais modalidades com atletas*/
	break; 
    }
  }			
  
}
void pagamento(clube*XPTO){
  int montante,numSocio;
  scanf("%d %d",&numSocio, &montante);
  numSocio-=101;
  (*XPTO).listaAssociados[numSocio].credito+=montante;
  (*XPTO).lucro += montante;
}
void incrementa(clube*XPTO){
  int i,mod,lucroMensal;
  for(i=0;i<(*XPTO).numAssociados;i++){
    (*XPTO).listaAssociados[i].meses++;
    for (mod=0;mod<26;mod++){
      if((*XPTO).listaAssociados[i].modalidades[mod]==1){
	(*XPTO).listaAssociados[i].credito= (*XPTO).listaAssociados[i].credito-10;
      }
    }
    if ((*XPTO).listaAssociados[i].credito<(-100)){
      (*XPTO).listaAssociados[i].estatuto=ADEPTO;
      for (mod=0;mod<26;mod++){
	(*XPTO).listaAssociados[i].modalidades[mod]=0;
      }
    }
  }
  lucroMensal = (*XPTO).lucro;
  (*XPTO).lucro = 0;
  printf("%d\n", lucroMensal);
}

void sair(clube*XPTO){
  int i;
  for(i=0;i<(*XPTO).numAssociados;i++){
    (*XPTO).balanco = (*XPTO).balanco + (*XPTO).listaAssociados[i].credito;
  }
  printf("%d %d\n",(*XPTO).numAssociados,(*XPTO).balanco);
}
int main(){
  clube XPTO;
  char comando;
  inicializamodalidades(&XPTO);
  XPTO.lucro = 0;
  XPTO.balanco = 0;
  XPTO.numAssociados = 0;
  comando=getchar();
  while(comando!='x'){
    switch(comando){
    case 'r':
      regista(&XPTO); 
      getchar();
      break;
    case 'i':{
      inscreve(&XPTO);
      getchar();
      break;
    }
    case 'k':{
      desinscreve(&XPTO);
      getchar();
      break;
    }
    case 'l':{
      int tipo;
      scanf("%d",&tipo);      
      lista(tipo,&XPTO);
      getchar();
      break;
    }
    case '+':{
      incrementa(&XPTO);
      getchar();
      break;
    }
    case 'p':{
      pagamento(&XPTO);
      getchar();
      break;
    }
    default:
      printf("ERRO: Comando inserido desconhecido\n");
    }
 comando=getchar(); 
 }
    sair(&XPTO);
    return 0;			
}
  
  
	
