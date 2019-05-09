#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NOME 1023 /* maximo de caracteres para o nome do contacto */
#define MAX_EMAIL 511 /* maximo de caracteres para o email do contacto */
#define MAX_NUMERO_TELEFONE 63 /* maximo de caracteres para o numero de telefone */
#define MODULO 31 /* modulo usado na funcao hash para calcular o indice da hashtable */

/* estrutura node que integra a hashtable e a lista e que contem 
a estrutura contacto, um ponteiro para o proximo node de uma 
lista e outro para o proximo node da hashtable */
typedef struct ListNode { 
    struct contacto *c;
    struct ListNode *nextNodeList;
    struct ListNode *nextNodeHashtable;
} ListNode;

/* estrutura contacto que contem tres strings, o nome, o email
e o numero de telefone */
typedef struct contacto {
    char *nome;
    char *email;
    char *numeroTelefone;
} contacto;

/* estrutura que consiste num ponteiro para um contacto */
typedef contacto* Contacto;

/* estrutura que consiste num ponteiro para um node */
typedef ListNode* Link;

Link criaContacto(char *nome, char *email, char *numeroTelefone);

Link insereContactoList(Link head, Link node);

void listaContactos(Link head);

Link removeContacto(Link head, char *nome);

void freeNode(Link node);

void alteraEmail(Link node, char *novoEmail);

void freeList(Link head);

void printContacto(Link node);

void numOcorrenciasDominio(Link head, char *dominio);

char *getDominio(Link node);

int comprimentoDominio(Link node);

Link procuraContactoHashtable(Link *hashtableHeads, char *nome);

void insereContactoHashtable(Link *hashtableHeads, Link node);

void removeContactoHashtable(Link *hashtableHeads, char *nome);

int hash(char *nome);

int main(){
    /* inicializo o ponteiro para cabeca da lista e o ponteiro 
    para a hashtable a NULL */
    Link head = NULL, *hashtableHeads = NULL;
    /* inicializo o caracter que guarda o comando e as strings
    que funcionam como buffers para a informacao do contacto que
    se recebe do input (nome, email, numero de telefone e dominio) */
    char cmd, bufferNome[MAX_NOME], bufferEmail[MAX_EMAIL], bufferNumTelefone[MAX_NUMERO_TELEFONE], bufferDominio[MAX_EMAIL];
    int i;

    /* inicializo todos os buffers com '\0' */
    memset(bufferNome, '\0', sizeof(char) * MAX_NOME);
    memset(bufferEmail, '\0', sizeof(char) * MAX_EMAIL);
    memset(bufferDominio, '\0', sizeof(char) * MAX_EMAIL);
    memset(bufferNumTelefone, '\0', sizeof(char) * MAX_NUMERO_TELEFONE);

    /* inicializo a hashtable (um vetor de ponteiros) e os ponteiros que 
    apontam para as cabecas das listas da hashtable todos a NULL */
    hashtableHeads = (Link*) malloc(MODULO*sizeof(Link));

    for (i = 0; i < MODULO; i++){
        hashtableHeads[i] = NULL;
    }    

    while(1){

        /* obtenho o comando */
        scanf("%c", &cmd);

        /* caso o comando seja terminar, dou free dos nodes
        e da hashtable, mais precisamente o freeList da free
        dos nodes e depois dou free da hashtable */
        if (cmd == 'x'){
            freeList(head);
            free(hashtableHeads);
            break;
        }

        switch (cmd)
            {
            case 'a':
                {
                Link nodeAux = NULL;
                /* faco getchar para me livrar do espaco a seguir ao comando
                e depois dou scanf das tres strings correspondentes ao nome,
                email e numero de telefone */
                getchar();
                scanf("%s %s %s", bufferNome, bufferEmail, bufferNumTelefone);
                /* vou verificar se o contacto ja existe a partir da funcao
                procuraContactoHashtable e so crio o contacto caso esse nome 
                nao exista */
                nodeAux = procuraContactoHashtable(hashtableHeads, bufferNome);
                if (nodeAux == NULL ){
                    /* crio o contacto e insiro-o na lista e na hashtable,
                    basicamente uso os mesmos nodes para as duas estruturas
                    e arranjo os ponteiros de modo a poder percorrer as duas */
                    Link node = criaContacto(bufferNome, bufferEmail, bufferNumTelefone);
                    head = insereContactoList(head, node);
                    insereContactoHashtable(hashtableHeads, node);
                }
                /* caso ja exista dou print da mensagem de erro */
                else{
                    printf("Nome existente.\n");
                }
                break;
                }
            case 'l':
                {
                listaContactos(head);
                break;
                }    
            case 'p':
                {
                Link node = NULL;
                /* faco getchar para me livrar do espaco a seguir ao comando
                e depois dou scanf da string que contem o nome do contacto que
                pretendo procurar */
                getchar();
                scanf("%s", bufferNome);
                node = procuraContactoHashtable(hashtableHeads, bufferNome);
                /* se o contacto existir dou print do contacto, caso contrario
                dou print da mensagem de erro */
                if (node == NULL){
                    printf("Nome inexistente.\n");
                    break;
                }
                printContacto(node);
                break;
                }
            case 'r':
                {
                Link node = NULL;
                /* faco getchar para me livrar do espaco a seguir ao comando
                e depois dou scanf da string que contem o nome do contacto que
                pretendo remover */
                getchar();
                scanf("%s", bufferNome);
                node = procuraContactoHashtable(hashtableHeads, bufferNome);
                /* se o contacto existir removo-o da lista e de seguida da
                hashtable, caso contrario dou print da mensagem de erro */
                if (node == NULL){
                    printf("Nome inexistente.\n");
                    break;
                }
                head = removeContacto(head, bufferNome);
                removeContactoHashtable(hashtableHeads, bufferNome);
                break;
                }    
            case 'e':
                {
                Link node = NULL;
                /* faco getchar para me livrar do espaco a seguir ao comando
                e depois dou scanf das strings que contem o nome do contacto, 
                cujo email pretendo alterar, e o novo email a por no contacto */
                getchar();
                scanf("%s %s", bufferNome, bufferEmail);
                node = procuraContactoHashtable(hashtableHeads, bufferNome);
                /* se o contacto existir altero o email, caso contrario dou 
                print da mensagem de erro */
                if (node == NULL){
                    printf("Nome inexistente.\n");
                    break;
                }
                alteraEmail(node, bufferEmail);
                break;
                }
            case 'c':
                {
                /* faco getchar para me livrar do espaco a seguir ao comando
                e depois dou scanf da string que contem o dominio cujo numero
                de ocorrencias pretendo calcular */
                getchar();
                scanf("%s", bufferDominio);
                numOcorrenciasDominio(head, bufferDominio);
                break;
                }    
            }    


        /* reinicio o comando a '\0' */
        cmd = '\0';
    }    

    return 0;
    
}

/* funcao que recebe o nome do contacto e que calcula o indice
da hashtable para saber em que lista vou inserir o contacto */
int hash(char *nome)
{
    int h = 0, a = 127;

    /* soma os numeros ascii dos caracteres do nome, 
    multiplica por um fator e calcula o resto da divisao
    pelo modulo */
    for (; *nome != '\0'; nome++)
        h = (a*h + *nome) % MODULO;

    return h;
}

/* funcao que, dado um ponteiro para a hashtable e um nome, procura 
na hashtable um contacto que tenha aquele nome e devolve o node que 
contem esse contacto, caso nao exista devolve NULL */
Link procuraContactoHashtable(Link *hashtableHeads, char *nome){
    int i = hash(nome);
    Link nodeAux = NULL;

    /* calculo o indice da hashtable na declaracao acima atraves do nome
    e percorro a lista correspondente a esse indice ate encontrar o contacto
    com esse nome e devolvo-o caso este exista */
    for (nodeAux = hashtableHeads[i]; nodeAux != NULL; nodeAux = nodeAux->nextNodeHashtable){
        if (strcmp(nodeAux->c->nome, nome) == 0){
            return nodeAux;
        }
    }

    /* caso contrario devolvo NULL */
    return NULL;
}

/* funcao que, dado um ponteiro para a hashtable e um ponteiro para um node, 
insere o node numa lista da hashtable cujo indice e calculado atraves da 
funcao hash */
void insereContactoHashtable(Link *hashtableHeads, Link node){
    int i = hash(node->c->nome);
    Link nodeAux = NULL;

    /* se o ponteiro para a cabeca da lista da hashtable
    apontar para NULL a cabeca da lista passa a ser o node 
    passado como argumento */
    if (hashtableHeads[i] == NULL){
        hashtableHeads[i] = node;
        return;
    }
    /* caso contrario, percorro a lista ate ao ultimo node
    e ponho o ponteiro que aponta para o proximo node da lista
    da hashtable a apontar para o node passado como argumento */
    for (nodeAux = hashtableHeads[i]; nodeAux->nextNodeHashtable != NULL; nodeAux = nodeAux->nextNodeHashtable);
    nodeAux->nextNodeHashtable = node;

}

/* funcao que, dado um ponteiro para a hashtable e um nome, remove o node 
da hashtable que tem um nome igual ao dado como argumento, neste caso da-se 
free do node porque anteriormente ja se removeu o node da lista mas sem dar 
free do node */
void removeContactoHashtable(Link *hashtableHeads, char *nome){
    int i = hash(nome);
    Link nodeAux = NULL, prevNode = NULL;

    /* depois de calcular o indice da hashtable, atraves do nome, percorre-se
    a lista com dois ponteiros desfasados um do outro pela distancia de um node
    e depois alteram-se os ponteiros de modo a que o ponteiro next do anterior 
    ao que vai ser removido aponte para a cabeca do que se segue a ele e depois,
    atraves de um ponteiro auxiliar, dou free do node */
    for (nodeAux = hashtableHeads[i], prevNode = NULL; nodeAux != NULL; prevNode = nodeAux, nodeAux = nodeAux->nextNodeHashtable){
        if (strcmp(nodeAux->c->nome, nome) == 0){
            /* caso o node a eliminar seja a cabeca, simplesmente
            poe-se o ponteiro da cabeca da lista a apontar para
            o proximo node da lista */
            if (nodeAux == hashtableHeads[i]){
                hashtableHeads[i] = nodeAux->nextNodeHashtable;
            }
            else{
                prevNode->nextNodeHashtable = nodeAux->nextNodeHashtable;
            }
            freeNode(nodeAux);
            break;
        }    
    }
}

/* funcao que dado tres strings, correspondentes ao nome, email e
numero de telefone, aloca memoria dinamicamente de modo a armazena-las,
sendo que primeiramente aloca memoria para o node e para a estrutura 
contacto que vai conter essas tres strings, pondo os ponteiros que
apontam para os proximos nodes da lista principal e das listas da
hashtable ambos a apontar para NULL. No fim retorna o node criado. */
Link criaContacto(char *nome, char *email, char *numeroTelefone){
    Link node = NULL;

    /* aloca memoria para o node */
    node = (Link) malloc(sizeof(ListNode));

    /* aloca memoria para a estrutura contacto e para as tres
    strings (nome, email e numero de telefone) */
    node->c = (Contacto) malloc(sizeof(contacto));
    node->c->nome = (char*) malloc(sizeof(char)*(strlen(nome) + 1));
    node->c->email = (char*) malloc(sizeof(char)*(strlen(email) + 1));
    node->c->numeroTelefone = (char*) malloc(sizeof(char)*(strlen(numeroTelefone) + 1));
    /* copia as strings para o endereco que lhes foi alocado */
    strcpy(node->c->nome, nome);
    strcpy(node->c->email, email);
    strcpy(node->c->numeroTelefone, numeroTelefone);

    /* coloca os ponteiros que apontam para os proximos nodes
    ambos a NULL */
    node->nextNodeList = NULL;
    node->nextNodeHashtable = NULL;

    return node;
}

/* funcao que, dado o ponteiro para a cabeca da lista principal 
e um node, insere o node na lista e retorna a cabeca */
Link insereContactoList(Link head, Link node){
    Link nodeAux = NULL;

    /* se a cabeca apontar para NULL entao
    a cabeca fica igual ao node criado */
    if (head == NULL){
        return node;
    }
    /* caso contrario, percorro a lista ate ao ultimo node,
    ponho o ponteiro que aponta para o proximo node da lista
    a apontar para o node passado como argumento e devolvo a
    cabeca da lista */
    for (nodeAux = head; nodeAux->nextNodeList != NULL; nodeAux = nodeAux->nextNodeList);
    nodeAux->nextNodeList = node;

    return head;
}

/* funcao que, dado o ponteiro para cabeca da lista principal, percorre
a lista dando print de todos os contactos */
void listaContactos(Link head){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->nextNodeList){
        printContacto(nodeAux);
    }
}

/* funcao que, dado o ponteiro para cabeca da lista principal e um nome,
percorre a lista ate encontrar o nome e depois remove o node sem dar free
uma vez que o free ocorreu quando a funcao removeu o node da hashtable,
retornando o ponteiro para cabeca da lista */
Link removeContacto(Link head, char *nome){
    Link nodeAux = NULL, prevNode = NULL;

    /*percorre-se a lista com dois ponteiros desfasados um do outro pela distancia
    de um node e depois alteram-se os ponteiros de modo a que o ponteiro next do 
    anterior ao que vai ser removido aponte para a cabeca do que se segue a ele */
    for (nodeAux = head, prevNode = NULL; nodeAux != NULL; prevNode = nodeAux, nodeAux = nodeAux->nextNodeList){
        if (strcmp(nodeAux->c->nome, nome) == 0){
            /* caso o node a eliminar seja a cabeca, simplesmente
            poe-se o ponteiro da cabeca da lista a apontar para
            o proximo node da lista */
            if (nodeAux == head){
                head = nodeAux->nextNodeList;
            }
            else{
                prevNode->nextNodeList = nodeAux->nextNodeList;
            }
            break;
        }    
    }

    return head;        
}

/* funcao que, dado um ponteiro para um node, da free de toda a memoria
alocada para esse node */
void freeNode(Link node){
    /*printf("Vai eliminar o node :\n");
    printf("%s\n", node->nome);*/
    free(node->c->nome);
    /*printf("%s\n", node->email);*/
    free(node->c->email);
    /*printf("%s\n", node->numeroTelefone);*/
    free(node->c->numeroTelefone);
    /*printf("Deu free das strings\n");*/
    free(node->c);
    free(node);
}

/* funcao que, dado um ponteiro para um node e um email, altera o email do node
pelo novo email */
void alteraEmail(Link node, char *novoEmail){

    /* verifica se e necessario realocar memoria caso
    o novo email ocupe mais ou menos memoria que o anterior,
    caso contrario apenas copia o novo email */
    if (strlen(novoEmail) != strlen(node->c->email)){
        node->c->email = (char*) realloc(node->c->email, sizeof(char)*(strlen(novoEmail) + 1));
        strcpy(node->c->email, novoEmail);
        return;
    }

    strcpy(node->c->email, novoEmail);
}

/* funcao que, dada um ponteiro para a cabeca da lista principal, da free
de todos os nodes da lista */
void freeList(Link head){
    Link nodeAux = NULL, nextNodeAux = NULL;

    /* caso em que a lista esta vazia */
    if (head == NULL){
        return;
    }

    /* percorre a lista dando free de todos os nodes */
    nodeAux = head;
    nextNodeAux = head->nextNodeList;
    while (nextNodeAux != NULL){
        freeNode(nodeAux);
        nodeAux = nextNodeAux;
        nextNodeAux = nextNodeAux->nextNodeList;
    }
    freeNode(nodeAux);    
}

/* funcao que, dada um ponteiro para um node, da print do contacto desse node */
void printContacto(Link node){
    printf("%s %s %s\n", node->c->nome, node->c->email, node->c->numeroTelefone);
}

/* funcao que, dada um ponteiro para a cabeca da lista principal, percorre a lista
toda contando o numero de ocorrencias de um dado dominio, passado como argumento,
na mesma lista */
void numOcorrenciasDominio(Link head, char *dominio){
    Link nodeAux = NULL;
    int contadorDominios = 0;
    char *dominioAux;

    /* percorro a lista obtendo os dominios de cada node e comparo 
    com o node dado como argumento e aumento o contador cada vez que
    os dominios sao iguais */
    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->nextNodeList){
        dominioAux = getDominio(nodeAux);
        if(strcmp(dominio, dominioAux) == 0){
            contadorDominios++;
        }
        /* dou free da string alocada na funcao getDominio */
        free(dominioAux);
    }

    printf("%s:%d\n", dominio, contadorDominios);

}

/* funcao que, dada um ponteiro para um node, calcula o comprimento do email
do contacto desse node e depois percorre a string que contem o email e copia
o dominio para uma nova string alocada que depois devolve */
char *getDominio(Link node){
    int i = 0, e = 0, compDominio = 0, compLocal = 0, cont = 0;
    char *dominio;

    /* calcula o comprimento do dominio, calcula o comprimento
    da parte local do email e depois aloca memoria para o dominio */ 
    compDominio = comprimentoDominio(node);
    compLocal = strlen(node->c->email) - compDominio;
    dominio = (char *) malloc(sizeof(char)*(compDominio + 1));

    /* percorro a string que contem o email e copio o dominio para a
    nova string alocada */
    for (i = compLocal, e = 0; node->c->email[i] != '\0'; i += 1, e += 1){
        dominio[e] = node->c->email[i];
        cont++;
    }
    dominio[e] = '\0';

    return dominio;

}

/* funcao que, dada um ponteiro para um node, calcula o comprimento
do dominio do contacto desse node e o devolve */
int comprimentoDominio(Link node){
    int i = 0, compLocal = 0;

    /* percorro o email ate ao arroba e devolvo
    o comprimento da parte local */
    for (i = 0; node->c->email[i] != '@'; i++){
        compLocal++;
    }
    compLocal++;

    /* obtenho o comprimento do dominio subtraindo
    o comprimento da string do email ao comprimento
    da parte local */
    return (strlen(node->c->email) - compLocal);

}


