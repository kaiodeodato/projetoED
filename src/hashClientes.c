#include <stdio.h>
#include <stdlib.h>
#include "hashClientes.h"

void inicializarHash(HASHTABLE *tabela, int nBuckets) {
    int i;
    BUCKET *ultimo = NULL;

    if (tabela == NULL) {
        return;
    }

    tabela->inicio = NULL;
    tabela->nBuckets = 0;
    tabela->nElementos = 0;

    if (nBuckets <= 0) {
        return;
    }

    for (i = 0; i < nBuckets; i++) {
        BUCKET *novoBucket = criarBucket(i);

        if (novoBucket == NULL) {
            libertarHash(tabela);
            return;
        }

        if (tabela->inicio == NULL) {
            tabela->inicio = novoBucket;
        } else {
            ultimo->prox = novoBucket;
        }

        ultimo = novoBucket;
        tabela->nBuckets++;
    }
}

int funcaoHash(int chave, int nBuckets) {
    if (nBuckets <= 0) {
        return 0;
    }

    if (chave < 0) {
        chave = -chave;
    }

    return chave % nBuckets;
}

BUCKET *obterBucket(HASHTABLE *tabela, int indice) {
    if (tabela == NULL || indice < 0 || indice >= tabela->nBuckets) {
        return NULL;
    }

    return procurarBucket(tabela->inicio, indice);
}

int inserirClienteHash(HASHTABLE *tabela, CLIENTE *cliente) {
    int indice;
    BUCKET *bucket;
    HASHNODE *novoNo;

    if (tabela == NULL || cliente == NULL || tabela->nBuckets <= 0) {
        return 0;
    }

    if (clienteExisteHash(tabela, cliente->id)) {
        return 0;
    }

    indice = funcaoHash(cliente->id, tabela->nBuckets);
    bucket = obterBucket(tabela, indice);
    if (bucket == NULL) {
        return 0;
    }

    novoNo = criarHashNode(cliente);
    if (novoNo == NULL) {
        return 0;
    }

    novoNo->prox = bucket->clientes;
    bucket->clientes = novoNo;
    tabela->nElementos++;

    return 1;
}

CLIENTE *procurarClienteHash(const HASHTABLE *tabela, int chave) {
    int indice;
    BUCKET *bucket;
    HASHNODE *atual;

    if (tabela == NULL || tabela->nBuckets <= 0) {
        return NULL;
    }

    indice = funcaoHash(chave, tabela->nBuckets);
    bucket = procurarBucket(tabela->inicio, indice);
    if (bucket == NULL) {
        return NULL;
    }

    atual = bucket->clientes;
    while (atual != NULL) {
        if (atual->chave == chave) {
            return atual->cliente;
        }

        atual = atual->prox;
    }

    return NULL;
}

int removerClienteHash(HASHTABLE *tabela, int chave) {
    int indice;
    BUCKET *bucket;
    HASHNODE *atual;
    HASHNODE *anterior;

    if (tabela == NULL || tabela->nBuckets <= 0) {
        return 0;
    }

    indice = funcaoHash(chave, tabela->nBuckets);
    bucket = obterBucket(tabela, indice);
    if (bucket == NULL) {
        return 0;
    }

    atual = bucket->clientes;
    anterior = NULL;

    while (atual != NULL) {
        if (atual->chave == chave) {
            if (anterior == NULL) {
                bucket->clientes = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            tabela->nElementos--;
            return 1;
        }

        anterior = atual;
        atual = atual->prox;
    }

    return 0;
}

int clienteExisteHash(const HASHTABLE *tabela, int chave) {
    return procurarClienteHash(tabela, chave) != NULL;
}

void libertarHash(HASHTABLE *tabela) {
    BUCKET *atual;
    BUCKET *seguinte;

    if (tabela == NULL) {
        return;
    }

    atual = tabela->inicio;

    while (atual != NULL) {
        seguinte = atual->prox;
        libertarListaClientesBucket(atual->clientes);
        free(atual);
        atual = seguinte;
    }

    tabela->inicio = NULL;
    tabela->nBuckets = 0;
    tabela->nElementos = 0;
}

BUCKET *criarBucket(int indice) {
    BUCKET *bucket = (BUCKET *)malloc(sizeof(BUCKET));

    if (bucket == NULL) {
        return NULL;
    }

    bucket->indice = indice;
    bucket->clientes = NULL;
    bucket->prox = NULL;

    return bucket;
}

HASHNODE *criarHashNode(CLIENTE *cliente) {
    HASHNODE *novoNo;

    if (cliente == NULL) {
        return NULL;
    }

    novoNo = (HASHNODE *)malloc(sizeof(HASHNODE));
    if (novoNo == NULL) {
        return NULL;
    }

    novoNo->chave = cliente->id;
    novoNo->cliente = cliente;
    novoNo->prox = NULL;

    return novoNo;
}

BUCKET *procurarBucket(BUCKET *inicio, int indice) {
    BUCKET *atual = inicio;

    while (atual != NULL) {
        if (atual->indice == indice) {
            return atual;
        }

        atual = atual->prox;
    }

    return NULL;
}

void libertarListaClientesBucket(HASHNODE *no) {
    HASHNODE *atual = no;
    HASHNODE *seguinte;

    while (atual != NULL) {
        seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }
}