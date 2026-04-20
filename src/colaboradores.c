#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "colaboradores.h"
#include "uteis.h"
#include "define.h"

COLABORADOR *obterColaboradorPorId(SISTEMA *sistema, int id) {
    int i;

    if (sistema == NULL || sistema->baseColaboradores.dados == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->baseColaboradores.tamanho; i++) {
        if (sistema->baseColaboradores.dados[i].id == id) {
            return &sistema->baseColaboradores.dados[i];
        }
    }

    return NULL;
}

COLABORADOR *obterColaboradorPorCaixa(SISTEMA *sistema, int idCaixa) {
    int i;

    if (sistema == NULL || sistema->baseColaboradores.dados == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->baseColaboradores.tamanho; i++) {
        if (sistema->baseColaboradores.dados[i].idCaixaAssociada == idCaixa) {
            return &sistema->baseColaboradores.dados[i];
        }
    }

    return NULL;
}

COLABORADOR *obterOperadorComMenosAtendimentos(SISTEMA *sistema) {
    int i;
    COLABORADOR *menor = NULL;

    if (sistema == NULL || sistema->baseColaboradores.dados == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->baseColaboradores.tamanho; i++) {
        COLABORADOR *atual = &sistema->baseColaboradores.dados[i];

        if (menor == NULL || atual->clientesAtendidos < menor->clientesAtendidos) {
            menor = atual;
        }
    }

    return menor;
}

void associarColaboradoresAsCaixas(SISTEMA *sistema) {
    int i;
    int limite;

    if (sistema == NULL || sistema->caixas == NULL || sistema->baseColaboradores.dados == NULL) {
        return;
    }

    limite = sistema->config.N_CAIXAS;
    if (limite > sistema->baseColaboradores.tamanho) {
        limite = sistema->baseColaboradores.tamanho;
    }

    for (i = 0; i < limite; i++) {
        COLABORADOR *colaborador = &sistema->baseColaboradores.dados[i];

        colaborador->idCaixaAssociada = i;
        sistema->caixas[i].operador = colaborador;

        if (sistema->caixas[i].estado == CAIXA_ABERTA) {
            colaborador->ativo = 1;
        } else {
            colaborador->ativo = 0;
        }
    }
}

void ativarColaborador(COLABORADOR *colaborador, int idCaixa) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->ativo = 1;
    colaborador->idCaixaAssociada = idCaixa;
}

void desativarColaborador(COLABORADOR *colaborador) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->ativo = 0;
}

void incrementarClientesAtendidosColaborador(COLABORADOR *colaborador) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->clientesAtendidos++;
}

void inicializarColaborador(COLABORADOR *colaborador) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->id = 0;
    colaborador->nome[0] = '\0';
    colaborador->clientesAtendidos = 0;
    colaborador->idCaixaAssociada = ID_CAIXA_INVALIDO;
    colaborador->ativo = 0;
}

int garantirCapacidadeBaseColaboradores(BASE_COLABORADORES *base) {
    COLABORADOR *novosDados;
    int novaCapacidade;

    if (base == NULL) {
        return 0;
    }

    if (base->dados == NULL) {
        base->dados = (COLABORADOR *)malloc(sizeof(COLABORADOR) * CAPACIDADE_INICIAL_COLABORADORES_BASE);
        if (base->dados == NULL) {
            return 0;
        }

        base->capacidade = CAPACIDADE_INICIAL_COLABORADORES_BASE;
        return 1;
    }

    if (base->tamanho < base->capacidade) {
        return 1;
    }

    novaCapacidade = base->capacidade * FATOR_CRESCIMENTO_VETORES;
    novosDados = (COLABORADOR *)realloc(base->dados, sizeof(COLABORADOR) * novaCapacidade);
    if (novosDados == NULL) {
        return 0;
    }

    base->dados = novosDados;
    base->capacidade = novaCapacidade;
    return 1;
}
