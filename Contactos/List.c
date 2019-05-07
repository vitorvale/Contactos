#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NOME 1023
#define MAX_EMAIL 511
#define MAX_NUMERO_TELEFONE 63

typedef struct ListNode {
    char *nome;
    char *email;
    char *numeroTelefone;
    struct ListNode *next;
} ListNode;

typedef ListNode* Link;

Link criaContacto(char *nome, char *email, char *numeroTelefone);

Link insereContacto(Link head, char *nome, char *email, char *numeroTelefone);

void listaContactos(Link head);

int verificaNomeExistente(Link head, char *nome);

void procuraContacto(Link head, char *nome);

Link removeContacto(Link head, char *nome);

void freeNode(Link node);

void alteraEmail(Link head, char *nome, char *novoEmail);

void freeList(Link head);

void printNode(Link node);

int main(){
    Link head = NULL;
    char cmd, bufferNome[MAX_NOME], bufferEmail[MAX_EMAIL], bufferNumTelefone[MAX_NUMERO_TELEFONE];

    memset(bufferNome, '\0', sizeof(char) * MAX_NOME);
    memset(bufferEmail, '\0', sizeof(char) * MAX_EMAIL);
    memset(bufferNumTelefone, '\0', sizeof(char) * MAX_NUMERO_TELEFONE);

    while(1){
        int flagNome = 0;

        scanf("%c", &cmd);

        if (cmd == 'x'){
            freeList(head);
            break;
        }

        switch (cmd)
            {
            case 'a':
                getchar();
                scanf("%s %s %s", bufferNome, bufferEmail, bufferNumTelefone);
                /*printf("Nome - [%s]; Email - [%s]; Numero - [%s]\n", bufferNome, bufferEmail, bufferNumTelefone);*/
                flagNome = verificaNomeExistente(head, bufferNome);
                if (flagNome == 0 ){
                    /*printf("Nome - [%s]; Email - [%p]; Numero - [%p]\n", bufferNome, bufferEmail, bufferNumTelefone);*/
                    head = insereContacto(head, bufferNome, bufferEmail, bufferNumTelefone);
                    /*printNode(head);*/

                }
                break;
            case 'l':
                listaContactos(head);
                break;    
            case 'p':
                getchar();
                scanf("%s", bufferNome);
                procuraContacto(head, bufferNome);
                break;
            case 'r':
                getchar();
                scanf("%s", bufferNome);
                head = removeContacto(head, bufferNome);
                break;    
            case 'e':
                getchar();
                scanf("%s %s", bufferNome, bufferEmail);
                alteraEmail(head, bufferNome, bufferEmail);
                break;
            }    



        cmd = '\0';
    }    

    return 0;
    
}

Link criaContacto(char *nome, char *email, char *numeroTelefone){
    Link node = NULL;

    node = (Link) malloc(sizeof(ListNode));

    node->nome = (char*) malloc(sizeof(char)*(strlen(nome) + 1));
    node->email = (char*) malloc(sizeof(char)*(strlen(email) + 1));
    node->numeroTelefone = (char*) malloc(sizeof(char)*(strlen(numeroTelefone) + 1));
    strcpy(node->nome, nome);
    strcpy(node->email, email);
    strcpy(node->numeroTelefone, numeroTelefone);

    node->next = NULL;

    return node;
}

Link insereContacto(Link head, char *nome, char *email, char *numeroTelefone){
    Link nodeAux = NULL;

    if (head == NULL){
        return criaContacto(nome, email, numeroTelefone);
    }
    for (nodeAux = head; nodeAux->next != NULL; nodeAux = nodeAux->next);
    nodeAux->next = criaContacto(nome, email, numeroTelefone);

    return head;
}

void listaContactos(Link head){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->next){
        printf("%s %s %s\n", nodeAux->nome, nodeAux->email, nodeAux->numeroTelefone);
    }
}

int verificaNomeExistente(Link head, char *nome){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->next){
        if (strcmp(nodeAux->nome, nome) == 0){
            printf("Nome existente.\n");
            return 1;
        }
    }

    return 0;
}

void procuraContacto(Link head, char *nome){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->next){
        if (strcmp(nodeAux->nome, nome) == 0){
            printf("%s %s %s\n", nodeAux->nome, nodeAux->email, nodeAux->numeroTelefone);
            return;
        }
    }

    printf("Nome inexistente.\n");

}

Link removeContacto(Link head, char *nome){
    Link nodeAux = NULL, prevNode = NULL;
    int flagNome = 0;

    for (nodeAux = head, prevNode = NULL; nodeAux != NULL; prevNode = nodeAux, nodeAux = nodeAux->next){
        if (strcmp(nodeAux->nome, nome) == 0){
            if (nodeAux == head){
                head = nodeAux->next;
            }
            else{
                prevNode->next = nodeAux->next;
            }
            freeNode(nodeAux);
            flagNome++;
            break;
        }    
    }

    if (flagNome == 0){
        printf("Nome inexistente.\n");
    }

    return head;        
}

void freeNode(Link node){
    /*printf("Vai eliminar o node :\n");
    printf("%s\n", node->nome);*/
    free(node->nome);
    /*printf("%s\n", node->email);*/
    free(node->email);
    /*printf("%s\n", node->numeroTelefone);*/
    free(node->numeroTelefone);
    /*printf("Deu free das strings\n");*/
    free(node);
}

void alteraEmail(Link head, char *nome, char *novoEmail){
    Link nodeAux = NULL;

    for (nodeAux = head; nodeAux != NULL; nodeAux = nodeAux->next){
        if (strcmp(nodeAux->nome, nome) == 0){
            if (strlen(novoEmail) != strlen(nodeAux->email)){
                nodeAux->email = (char*) realloc(nodeAux->email, sizeof(char)*(strlen(novoEmail) + 1));
            }
            strcpy(nodeAux->email, novoEmail);
            return;
        }    
    }

    printf("Nome inexistente.\n");

}

void freeList(Link head){
    Link nodeAux = NULL, nextNodeAux = NULL;

    nodeAux = head;
    nextNodeAux = head->next;
    while (nextNodeAux != NULL){
        /*printNode(nodeAux);*/
        freeNode(nodeAux);
        nodeAux = nextNodeAux;
        nextNodeAux = nextNodeAux->next;
    }
    /*printNode(nodeAux);*/
    freeNode(nodeAux);    
}

void printNode(Link node){
    printf("%s %s %s\n", node->nome, node->email, node->numeroTelefone);
}