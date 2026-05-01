#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"
#include "define.h"
#include "uteis.h"

// Seleciona aleatoriamente um produto da base de produtos e devolve o seu ponteiro
PRODUTO *gerarProdutoAleatorio(BASE_PRODUTOS *base) {
    int indice;

    if (base == NULL || base->dados == NULL || base->tamanho <= 0) {
        return NULL;
    }

    indice = Aleatorio(0, base->tamanho - 1);
    return &base->dados[indice];
}
// Ordena os produtos por preço em ordem crescente usando algoritmo de bubble sort
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
// Calcula o tempo total de procura de um conjunto de produtos
int calcularTempoTotalProdutos(PRODUTO *produtos, int quantidade) {
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
// Calcula o valor total de um conjunto de produtos somando os preços
float calcularValorTotalProdutos(PRODUTO *produtos, int quantidade) {
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
// Troca dois produtos de posição na memória (swap)
void trocarProdutos(PRODUTO *a, PRODUTO *b) {
    PRODUTO temp;

    if (a == NULL || b == NULL) {
        return;
    }

    temp = *a;
    *a = *b;
    *b = temp;
}
// Retorna uma string indicando se a caixa está em controlo manual ou automático
char *obterTextoControloCaixa(CAIXA *caixa) {
    if (caixa == NULL) {
        return "N/A";
    }

    if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
        return "MANUAL";
    }

    return "AUTO";
}
// Gera o próximo ID sequencial para produtos da base, com base no último elemento existente
int gerarProximoIdProdutoBase(BASE_PRODUTOS *base) {
    if (base == NULL || base->tamanho <= 0) {
        return 100001;
    }

    return base->dados[base->tamanho - 1].id + 1;
}