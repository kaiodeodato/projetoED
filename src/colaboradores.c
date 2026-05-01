#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "colaboradores.h"
#include "uteis.h"
#include "define.h"

// Retorna o colaborador com o id indicado a partir da base, ou NULL se não existir
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
// Retorna o colaborador associado a uma caixa específica, ou NULL se não existir
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
// Associa colaboradores às caixas por índice, ativando-os apenas nas caixas abertas
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
// Ativa o colaborador e associa-o à caixa indicada
void ativarColaborador(COLABORADOR *colaborador, int idCaixa) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->ativo = 1;
    colaborador->idCaixaAssociada = idCaixa;
}
// Desativa o colaborador, marcando-o como inativo
void desativarColaborador(COLABORADOR *colaborador) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->ativo = 0;
}
// Incrementa o contador de clientes atendidos pelo colaborador
void incrementarClientesAtendidosColaborador(COLABORADOR *colaborador) {
    if (colaborador == NULL) {
        return;
    }

    colaborador->clientesAtendidos++;
}
