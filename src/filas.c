#include <stdio.h>
#include <stdlib.h>
#include "filas.h"

// Inicializa a fila, definindo início, fim e tamanho como vazios
void inicializarFila(FILA *fila) {
    if (fila == NULL) {
        return;
    }

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
}
// Verifica se a fila está vazia
int filaEstaVazia(FILA *fila) {
    if (fila == NULL) {
        return 1;
    }

    return fila->inicio == NULL;
}
// Retorna o tamanho atual da fila
int obterTamanhoFila(FILA *fila) {
    if (fila == NULL) {
        return 0;
    }

    return fila->tamanho;
}
// Adiciona um cliente ao final da fila, criando um novo elemento e atualizando a estrutura
int enfileirarCliente(FILA *fila, CLIENTE *cliente) {
    ELEMENTO *novoElemento;

    if (fila == NULL || cliente == NULL) {
        return 0;
    }

    novoElemento = criarElementoFila(cliente);
    if (novoElemento == NULL) {
        return 0;
    }

    if (filaEstaVazia(fila)) {
        fila->inicio = novoElemento;
        fila->fim = novoElemento;
    } else {
        fila->fim->seguinte = novoElemento;
        fila->fim = novoElemento;
    }

    fila->tamanho++;
    return 1;
}
// Remove e retorna o cliente no início da fila, atualizando a estrutura da fila
CLIENTE *desenfileirarCliente(FILA *fila) {
    ELEMENTO *elementoRemovido;
    CLIENTE *cliente;

    if (fila == NULL || filaEstaVazia(fila)) {
        return NULL;
    }

    elementoRemovido = fila->inicio;
    cliente = elementoRemovido->cliente;

    fila->inicio = elementoRemovido->seguinte;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(elementoRemovido);
    fila->tamanho--;

    return cliente;
}
// Remove um cliente da fila pelo seu id, ajustando os ponteiros e o tamanho da fila
int removerClienteDaFilaPorId(FILA *fila, int idCliente) {
    ELEMENTO *atual;
    ELEMENTO *anterior;

    if (fila == NULL || filaEstaVazia(fila)) {
        return 0;
    }

    atual = fila->inicio;
    anterior = NULL;

    while (atual != NULL) {
        if (atual->cliente != NULL && atual->cliente->id == idCliente) {
            if (anterior == NULL) {
                fila->inicio = atual->seguinte;
            } else {
                anterior->seguinte = atual->seguinte;
            }

            if (atual == fila->fim) {
                fila->fim = anterior;
            }

            free(atual);
            fila->tamanho--;

            if (fila->inicio == NULL) {
                fila->fim = NULL;
            }

            return 1;
        }

        anterior = atual;
        atual = atual->seguinte;
    }

    return 0;
}
// Liberta toda a memória da fila, removendo todos os elementos e reinicializando a estrutura
void libertarFila(FILA *fila) {
    ELEMENTO *atual;
    ELEMENTO *seguinte;

    if (fila == NULL) {
        return;
    }

    atual = fila->inicio;

    while (atual != NULL) {
        seguinte = atual->seguinte;
        free(atual);
        atual = seguinte;
    }

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
}
// Cria e inicializa um novo elemento da fila associado a um cliente
ELEMENTO *criarElementoFila(CLIENTE *cliente) {
    ELEMENTO *novoElemento;

    if (cliente == NULL) {
        return NULL;
    }

    novoElemento = (ELEMENTO *)malloc(sizeof(ELEMENTO));
    if (novoElemento == NULL) {
        return NULL;
    }

    novoElemento->cliente = cliente;
    novoElemento->seguinte = NULL;

    return novoElemento;
}