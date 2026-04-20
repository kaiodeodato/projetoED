#include <stdio.h>
#include <stdlib.h>
#include "listaDeCompras.h"

void inicializarListaClientesComprando(LISTA_CLIENTES_COMPRANDO *lista) {
    if (lista == NULL) {
        return;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int listaComprasEstaVazia(const LISTA_CLIENTES_COMPRANDO *lista) {
    if (lista == NULL) {
        return 1;
    }

    return lista->inicio == NULL;
}

int inserirClienteOrdenadoListaCompras(LISTA_CLIENTES_COMPRANDO *lista, CLIENTE *cliente) {
    NO_CLIENTE_COMPRANDO *novoNo;
    NO_CLIENTE_COMPRANDO *atual;
    NO_CLIENTE_COMPRANDO *anterior;

    if (lista == NULL || cliente == NULL) {
        return 0;
    }

    novoNo = criarNoClienteComprando(cliente);
    if (novoNo == NULL) {
        return 0;
    }

    if (listaComprasEstaVazia(lista)) {
        lista->inicio = novoNo;
        lista->fim = novoNo;
        lista->tamanho++;
        return 1;
    }

    atual = lista->inicio;
    anterior = NULL;

    while (atual != NULL &&
           atual->cliente != NULL &&
           atual->cliente->instantePrevistoFimCompras <= cliente->instantePrevistoFimCompras) {
        anterior = atual;
        atual = atual->seguinte;
    }

    if (anterior == NULL) {
        novoNo->seguinte = lista->inicio;
        lista->inicio = novoNo;
    } else {
        anterior->seguinte = novoNo;
        novoNo->seguinte = atual;
    }

    if (atual == NULL) {
        lista->fim = novoNo;
    }

    lista->tamanho++;
    return 1;
}

CLIENTE *obterPrimeiroClienteListaCompras(const LISTA_CLIENTES_COMPRANDO *lista) {
    if (lista == NULL || listaComprasEstaVazia(lista)) {
        return NULL;
    }

    return lista->inicio->cliente;
}

CLIENTE *removerPrimeiroClienteListaCompras(LISTA_CLIENTES_COMPRANDO *lista) {
    NO_CLIENTE_COMPRANDO *noRemovido;
    CLIENTE *cliente;

    if (lista == NULL || listaComprasEstaVazia(lista)) {
        return NULL;
    }

    noRemovido = lista->inicio;
    cliente = noRemovido->cliente;

    lista->inicio = noRemovido->seguinte;
    if (lista->inicio == NULL) {
        lista->fim = NULL;
    }

    free(noRemovido);
    lista->tamanho--;

    return cliente;
}

void atualizarClientesEmCompras(LISTA_CLIENTES_COMPRANDO *lista, int instanteAtual) {
    NO_CLIENTE_COMPRANDO *atual;

    if (lista == NULL) {
        return;
    }

    atual = lista->inicio;

    while (atual != NULL) {
        if (atual->cliente != NULL) {
            atual->cliente->tempoRestanteCompras =
                atual->cliente->instantePrevistoFimCompras - instanteAtual;

            if (atual->cliente->tempoRestanteCompras < 0) {
                atual->cliente->tempoRestanteCompras = 0;
            }
        }

        atual = atual->seguinte;
    }
}

int clienteTerminouComprasLista(const CLIENTE *cliente, int instanteAtual) {
    if (cliente == NULL) {
        return 0;
    }

    return instanteAtual >= cliente->instantePrevistoFimCompras;
}

void libertarListaClientesComprando(LISTA_CLIENTES_COMPRANDO *lista) {
    NO_CLIENTE_COMPRANDO *atual;
    NO_CLIENTE_COMPRANDO *seguinte;

    if (lista == NULL) {
        return;
    }

    atual = lista->inicio;

    while (atual != NULL) {
        seguinte = atual->seguinte;
        free(atual);
        atual = seguinte;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

NO_CLIENTE_COMPRANDO *criarNoClienteComprando(CLIENTE *cliente) {
    NO_CLIENTE_COMPRANDO *novoNo;

    if (cliente == NULL) {
        return NULL;
    }

    novoNo = (NO_CLIENTE_COMPRANDO *)malloc(sizeof(NO_CLIENTE_COMPRANDO));
    if (novoNo == NULL) {
        return NULL;
    }

    novoNo->cliente = cliente;
    novoNo->seguinte = NULL;

    return novoNo;
}