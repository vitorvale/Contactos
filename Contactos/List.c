#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NOME 1023
#define MAX_EMAIL 511
#define MAX_NUMERO_TELEFONE 63
#define MODULO 31

typedef struct ListNode {
    struct contacto *c;
    struct ListNode *nextNodeList;
    struct ListNode *nextNodeHashtable;
} ListNode;

typedef struct contacto {
    char *nome;
    char *email;
    char *numeroTelefone;
} contacto;

typedef contacto* Contacto;

typedef ListNode* Link;

Link criaContacto(char *nome, char *email, char *numeroTelefone);

Link insereContactoList(Link head, Link node);

void listaContactos(Link head);

/*int verificaNomeExistente(Link head, char *nome);*/

/*Link procuraContacto(Link head, char *nome);*/

Link removeContacto(Link head, char *nome);

void freeNode(Link node);

void alteraEmail(Link node, char *novoEmail);

void freeList(Link head);

void printNode(Link node);

void numOcorrenciasDominio(Link head, char *dominio);

char *getDominio(Link node);

int comprimentoDominio(Link node);

void inicializaHashtableContactos(Link *hashtableHeads);

Link procuraContactoHashtable(Link *hashtableHeads, char *nome);

void insereContactoHashtable(Link *hashtableHeads, Link node);

void removeContactoHashtable(Link *hashtableHeads, char *nome);

int hash(char *nome);

int main(){
    Link head = NULL, *hashtableHeads = NULL;
    char cmd, bufferNome[MAX_NOME], bufferEmail[MAX_EMAIL], bufferNumTelefone[MAX_NUMERO_TELEFONE], bufferDominio[MAX_EMAIL];
    int i;

    memset(bufferNome, '\0', sizeof(char) * MAX_NOME);
    memset(bufferEmail, '\0', sizeof(char) * MAX_EMAIL);
    memset(bufferDominio, '\0', sizeof(char) * MAX_EMAIL);
    memset(bufferNumTelefone, '\0', sizeof(char) * MAX_NUMERO_TELEFONE);

    /*inicializaHashtableContactos(hashtableHeads);*/

    hashtableHeads = (Link*) malloc(MODULO*sizeof(Link));

    for (i = 0; i < MODULO; i++){
        hashtableHeads[i] = NULL;
    }    

    while(1){

        scanf("%c", &cmd);

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
                getchar();
                scanf("%s %s %s", bufferNome, bufferEmail, bufferNumTelefone);
                /*printf("Nome - [%s]; Email - [%s]; Numero - [%s]\n", bufferNome, bufferEmail, bufferNumTelefone);*/
                nodeAux = procuraContactoHashtable(hashtableHeads, bufferNome);
                if (nodeAux == NULL ){
                    /*printf("Nome - [%s]; Email - [%p]; Numero - [%p]\n", bufferNome, bufferEmail, bufferNumTelefone);*/
                    Link node = criaContacto(bufferNome, bufferEmail, bufferNumTelefone);
                    /*printNode(node);*/
                    head = insereContactoList(head, node);
                    insereContactoHashtable(hashtableHeads, node);
                    nodeAux = procuraContactoHashtable(hashtableHeads, bufferNome);
                    /*printNode(nodeAux);*/
                }
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
                getchar();
                scanf("%s", bufferNome);
                node = procuraContactoHashtable(hashtableHeads, bufferNome);
                if (node == NULL){
                    printf("Nome inexistente.\n");
                    break;
                }
                printNode(node);
                break;
                }
            case 'r':
                {
                Link node = NULL;
                getchar();
                scanf("%s", bufferNome);
                node = procuraContactoHashtable(hashtableHeads, bufferNome);
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
                getchar();
                scanf("%s %s", bufferNome, bufferEmail);
                node = procuraContactoHashtable(hashtableHeads, bufferNome);
                if (node == NULL){
                    printf("Nome inexistente.\n");
                    break;
                }
                alteraEmail(node, bufferEmail);
                break;
                }
            case 'c':
                {
                getchar();
                scanf("%s", bufferDominio);
                numOcorrenciasDominio(head, bufferDominio);
                break;
                }    
            }    



        cmd = '\0';
    }    

    return 0;
    
}

int hash(char *nome)
{
    int h = 0, a = 127;

    for (; *nome != '\0'; nome++)
        h = (a*h + *nome) % MODULO;

    return h;
}

void inicializaHashtableContactos(Link *hashtableHeads){
    int i;

    hashtableHeads = (Link*) malloc(MODULO*sizeof(Link));

    for (i = 0; i < MODULO; i++){
        hashtableHeads[i] = NULL;
    }    
}

Link procuraContactoHashtable(Link *hashtableHeads, char *nome){
    int i = hash(nome);
    Link nodeAux = NULL;

    /*printf("Indice da hashtable : [%d]\n", i);
    printf("Ponteiro da cabeca do indice %d : [%p]\n", i, hashtableHeads[i]);*/
    for (nodeAux = hashtableHeads[i]; nodeAux != NULL; nodeAux = nodeAux->nextNodeHashtable){
        if (strcmp(nodeAux->c->nome, nome) == 0){
            return nodeAux;
        }
    }

    return NULL;
}

void insereContactoHashtable(Link *hashtableHeads, Link node){
    int i = hash(node->c->nome);
    Link nodeAux = NULL;

    if (hashtableHeads[i] == NULL){
        /*printf("Vai inserir o node na cabeca\n");*/
        /*printNode(node);*/
        hashtableHeads[i] = node;
        return;
    }
    for (nodeAux = hashtableHeads[i]; nodeAux->nextNodeHashtable != NULL; nodeAux = nodeAux->nextNodeHashtable);
    /*printf("Vai inserir o node no fim\n");
    printNode(node);
    printf("O nodeAux next e [%p]\n", nodeAux->nextNodeHashtable);*/
    nodeAux->nextNodeHashtable = node;

}

void removeContactoHashtable(Link *hashtableHeads, char *nome){
    int i = hash(nome);
    Link nodeAux = NULL, prevNode = NULL;

    for (nodeAux = hashtableHeads[i], prevNode = NULL; nodeAux != NULL; prevNode = nodeAux, nodeAux = nodeAux->nextNodeHashtable){
        /*printf("O nodeAux e [%p]\n", nodeAux);*/
        if (strcmp(nodeAux->c->nome, nome) == 0){
            if (nodeAux == hashtableHeads[i]){
                /*printf("Vai dar o endereco [%p] do next a [%p]\n", nodeAux->nextNodeHashtable, hashtableHeads[i]);*/
                hashtableHeads[i] = nodeAux->nextNodeHashtable;
            }
            else{
                prevNode->nextNodeHashtable = nodeAux->nextNodeHashtable;
            }
            /*printf("Vai dar free do endereco : [%p]\n", nodeAux);*/
            freeNode(nodeAux);
            break;
        }    
    }
}

Link criaContacto(char *nome, char *email, char *numeroTelefone){
    Link node = NULL;

    node = (Link) malloc(sizeof(ListNode));

    node->c = (Contacto) malloc(sizeof(contacto));
    node->c->nome = (char*) malloc(sizeof(char)*(strlen(nome) + 1));
    node->c->email = (char*) malloc(sizeof(char)*(strlen(email) + 1));
    node->c->numeroTelefone = (char*) malloc(sizeof(char)*(strlen(numeroTelefone) + 1));
    strcpy(node->c->nome, nome);
    strcpy(node->c->email, email);
    strcpy(node->c->numeroTelefone, numeroTelefone);

    node->nextNodeList = NULL;
    node->nextNodeHashtable = NULL;

    return node;
}

Link insereContactoList(Link head, Link node){
    Link nodeAux = NULL;

    if (head == NULL){
        return node;
    }
    for (nodeAux = head; nodeAux->nextNodeList != NULL; nodeAux = nodeAux->nextNodeList);
    nodeAux->nextNodeList = node;

    return head;
}

void listaContactos(Link head){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->nextNodeList){
        printNode(nodeAux);
    }
}

/*int verificaNomeExistente(Link head, char *nome){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->nextNodeList){
        if (strcmp(nodeAux->nome, nome) == 0){
            printf("Nome existente.\n");
            return 1;
        }
    }

    return 0;
}*/

/*Link procuraContacto(Link head, char *nome){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->nextNodeHashtable){
        if (strcmp(nodeAux->nome, nome) == 0){
            return nodeAux;
        }
    }

    return NULL;
}*/

Link removeContacto(Link head, char *nome){
    Link nodeAux = NULL, prevNode = NULL;

    for (nodeAux = head, prevNode = NULL; nodeAux != NULL; prevNode = nodeAux, nodeAux = nodeAux->nextNodeList){
        if (strcmp(nodeAux->c->nome, nome) == 0){
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

void alteraEmail(Link node, char *novoEmail){

    if (strlen(novoEmail) != strlen(node->c->email)){
        node->c->email = (char*) realloc(node->c->email, sizeof(char)*(strlen(novoEmail) + 1));
        strcpy(node->c->email, novoEmail);
        return;
    }

    strcpy(node->c->email, novoEmail);
}

void freeList(Link head){
    Link nodeAux = NULL, nextNodeAux = NULL;

    if (head == NULL){
        return;
    }

    nodeAux = head;
    nextNodeAux = head->nextNodeList;
    while (nextNodeAux != NULL){
        /*printNode(nodeAux);*/
        freeNode(nodeAux);
        nodeAux = nextNodeAux;
        nextNodeAux = nextNodeAux->nextNodeList;
    }
    /*printNode(nodeAux);*/
    freeNode(nodeAux);    
}

void printNode(Link node){
    printf("%s %s %s\n", node->c->nome, node->c->email, node->c->numeroTelefone);
}

void numOcorrenciasDominio(Link head, char *dominio){
    Link nodeAux = NULL;
    int contadorDominios = 0;
    char *dominioAux;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->nextNodeList){
        dominioAux = getDominio(nodeAux);
        /*printf("%s\n", dominioAux);*/
        if(strcmp(dominio, dominioAux) == 0){
            contadorDominios++;
        }
        free(dominioAux);
    }

    printf("%s:%d\n", dominio, contadorDominios);

}

char *getDominio(Link node){
    int i = 0, e = 0, compDominio = 0, compLocal = 0, cont = 0;
    char *dominio;

    compDominio = comprimentoDominio(node);
    compLocal = strlen(node->c->email) - compDominio;
    dominio = (char *) malloc(sizeof(char)*(compDominio + 1));
    /*printf("CompDominio = %d, compLocal = %d, compEmail = %d\n", compDominio, compLocal, compDominio + compLocal);*/

    for (i = compLocal, e = 0; node->c->email[i] != '\0'; i += 1, e += 1){
        /*printf("%c\n", node->c->email[i]);*/
        dominio[e] = node->c->email[i];
        cont++;
    }
    /*printf("Numero de caracteres : [%d]\n", cont);*/
    dominio[e] = '\0';

    return dominio;

}

int comprimentoDominio(Link node){
    int i = 0, compLocal = 0;

    for (i = 0; node->c->email[i] != '@'; i++){
        compLocal++;
    }
    compLocal++;

    return (strlen(node->c->email) - compLocal);

}


