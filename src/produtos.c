#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"
#include "define.h"
#include "uteis.h"

PRODUTO *gerarProdutoAleatorio(BASE_PRODUTOS *base) {
    int indice;

    if (base == NULL || base->dados == NULL || base->tamanho <= 0) {
        return NULL;
    }

    indice = Aleatorio(0, base->tamanho - 1);
    return &base->dados[indice];
}

void ordenarProdutosPorPreco(PRODUTO *produtos, int quantidade) {
    int i;
    int j;

    if (produtos == NULL || quantidade <= 1) {
        return;
    }

    for (i = 0; i < quantidade - 1; i++) {
        for (j = 0; j < quantidade - 1 - i; j++) {
            if (produtos[j].preco > produtos[j + 1].preco) {
                trocarProdutos(&produtos[j], &produtos[j + 1]);
            }
        }
    }
}

int calcularTempoTotalProdutos(const PRODUTO *produtos, int quantidade) {
    int i;
    int tempoTotal = 0;

    if (produtos == NULL || quantidade <= 0) {
        return 0;
    }

    for (i = 0; i < quantidade; i++) {
        tempoTotal += produtos[i].tempoDeProcura;
    }

    return tempoTotal;
}

float calcularValorTotalProdutos(const PRODUTO *produtos, int quantidade) {
    int i;
    float valorTotal = 0.0f;

    if (produtos == NULL || quantidade <= 0) {
        return 0.0f;
    }

    for (i = 0; i < quantidade; i++) {
        valorTotal += produtos[i].preco;
    }

    return valorTotal;
}

void trocarProdutos(PRODUTO *a, PRODUTO *b) {
    PRODUTO temp;

    if (a == NULL || b == NULL) {
        return;
    }

    temp = *a;
    *a = *b;
    *b = temp;
}

char *obterTextoControloCaixa(const CAIXA *caixa) {
    if (caixa == NULL) {
        return "N/A";
    }

    if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
        return "MANUAL";
    }

    return "AUTO";
}

int gerarProximoIdProdutoBase(BASE_PRODUTOS *base) {
    if (base == NULL || base->tamanho <= 0) {
        return 100001;
    }

    return base->dados[base->tamanho - 1].id + 1;
}