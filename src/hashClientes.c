#include <stdio.h>
#include <stdlib.h>
#include "hashClientes.h"

// Inicializa a tabela hash criando a lista de buckets com o número especificado
void inicializarHash(HASHTABLE *tabela, int nBuckets) {
    int i;

    if (tabela == NULL) {
        return;
    }

    tabela->nBuckets = HASH_N_BUCKETS;
    tabela->nElementos = 0;

    for (i = 0; i < tabela->nBuckets; i++) {
        tabela->buckets[i].indice = i;
        tabela->buckets[i].clientes = NULL;
    }
}
// Calcula o índice do bucket na tabela hash a partir da chave, garantindo valor não negativo
int funcaoHash(int chave, int nBuckets) {
    if (nBuckets <= 0) {
        return 0;
    }

    if (chave < 0) {
        chave = -chave;
    }

    return chave % nBuckets;
}
// Retorna o bucket correspondente ao índice indicado na tabela hash
BUCKET *obterBucket(HASHTABLE *tabela, int indice) {
    if (tabela == NULL || indice < 0 || indice >= tabela->nBuckets) {
        return NULL;
    }

    return &tabela->buckets[indice];
}
// Insere um cliente na tabela hash, evitando duplicados e adicionando-o ao bucket correspondente
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
// Procura e retorna um cliente na tabela hash com base na chave (id), ou NULL se não existir
CLIENTE *procurarClienteHash(HASHTABLE *tabela, int chave) {
    int indice;
    HASHNODE *atual;

    if (tabela == NULL || tabela->nBuckets <= 0) {
        return NULL;
    }

    indice = funcaoHash(chave, tabela->nBuckets);
    atual = tabela->buckets[indice].clientes;

    while (atual != NULL) {
        if (atual->chave == chave) {
            return atual->cliente;
        }

        atual = atual->prox;
    }

    return NULL;
}
// Remove um cliente da tabela hash com base na chave, ajustando os ponteiros e o número de elementos
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
// Verifica se um cliente existe na tabela hash com base na chave
int clienteExisteHash(HASHTABLE *tabela, int chave) {
    return procurarClienteHash(tabela, chave) != NULL;
}
// Liberta toda a memória da tabela hash, incluindo buckets e listas de clientes
void libertarHash(HASHTABLE *tabela) {
    int i;

    if (tabela == NULL) {
        return;
    }

    for (i = 0; i < tabela->nBuckets; i++) {
        libertarListaClientesBucket(tabela->buckets[i].clientes);
        tabela->buckets[i].clientes = NULL;
    }

    tabela->nBuckets = 0;
    tabela->nElementos = 0;
}
// Cria e inicializa um novo nó da tabela hash associado a um cliente
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
// Liberta a memória de todos os nós da lista de clientes de um bucket
void libertarListaClientesBucket(HASHNODE *no) {
    HASHNODE *atual = no;
    HASHNODE *seguinte;

    while (atual != NULL) {
        seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }
}