#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"
#include "define.h"
#include "uteis.h"

PRODUTO *obterProdutoPorIndice(BASE_PRODUTOS *base, int indice) {
    if (base == NULL || base->dados == NULL) {
        return NULL;
    }

    if (indice < 0 || indice >= base->tamanho) {
        return NULL;
    }

    return &base->dados[indice];
}

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

int garantirCapacidadeBaseProdutos(BASE_PRODUTOS *base) {
    PRODUTO *novosDados;
    int novaCapacidade;

    if (base == NULL) {
        return 0;
    }

    if (base->dados == NULL) {
        base->dados = (PRODUTO *)malloc(sizeof(PRODUTO) * CAPACIDADE_INICIAL_PRODUTOS_BASE);
        if (base->dados == NULL) {
            return 0;
        }

        base->capacidade = CAPACIDADE_INICIAL_PRODUTOS_BASE;
        return 1;
    }

    if (base->tamanho < base->capacidade) {
        return 1;
    }

    novaCapacidade = base->capacidade * FATOR_CRESCIMENTO_VETORES;
    novosDados = (PRODUTO *)realloc(base->dados, sizeof(PRODUTO) * novaCapacidade);
    if (novosDados == NULL) {
        return 0;
    }

    base->dados = novosDados;
    base->capacidade = novaCapacidade;
    return 1;
}

void inicializarBaseProdutos(BASE_PRODUTOS *base) {
    if (base == NULL) {
        return;
    }

    base->dados = NULL;
    base->tamanho = 0;
    base->capacidade = 0;
}

void removerQuebraLinha(char *texto) {
    if (texto == NULL) {
        return;
    }

    texto[strcspn(texto, "\r\n")] = '\0';
}

void copiarNomeProduto(char *destino, const char *origem) {
    int inicio = 0;
    int fim;

    if (destino == NULL || origem == NULL) {
        return;
    }

    while (origem[inicio] == ' ' || origem[inicio] == '\t') {
        inicio++;
    }

    fim = (int)strlen(origem + inicio) - 1;
    while (fim >= 0 &&
           ((origem + inicio)[fim] == ' ' || (origem + inicio)[fim] == '\t')) {
        fim--;
    }

    if (fim < 0) {
        destino[0] = '\0';
        return;
    }

    {
        int tamanho = fim + 1;
        if (tamanho >= MAX_NOME) {
            tamanho = MAX_NOME - 1;
        }

        strncpy(destino, origem + inicio, tamanho);
        destino[tamanho] = '\0';
    }
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