#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historico.h"
#include "define.h"

// Inicializa a lista de logs, definindo início, fim e tamanho como vazios
void inicializarListaLogs(LISTA_LOGS *logs) {
    if (logs == NULL) {
        return;
    }

    logs->inicio = NULL;
    logs->fim = NULL;
    logs->tamanho = 0;
}
// Adiciona um novo registo de log à lista, incluindo instante, ação e descrição
void adicionarLog(LISTA_LOGS *logs, int instante, char *acao, char *descricao) {
    NO_LOG_ACAO *novoNo;

    if (logs == NULL || acao == NULL || descricao == NULL) {
        return;
    }

    novoNo = criarNoLog(instante, acao, descricao);
    if (novoNo == NULL) {
        return;
    }

    if (logs->inicio == NULL) {
        logs->inicio = novoNo;
        logs->fim = novoNo;
    } else {
        logs->fim->seguinte = novoNo;
        logs->fim = novoNo;
    }

    logs->tamanho++;
}
// Liberta todos os nós da lista de logs e reinicializa a estrutura
void limparListaLogs(LISTA_LOGS *logs) {
    NO_LOG_ACAO *atual;
    NO_LOG_ACAO *seguinte;

    if (logs == NULL) {
        return;
    }

    atual = logs->inicio;

    while (atual != NULL) {
        seguinte = atual->seguinte;
        free(atual);
        atual = seguinte;
    }

    logs->inicio = NULL;
    logs->fim = NULL;
    logs->tamanho = 0;
}
// Liberta a lista de logs, reutilizando a função de limpeza dos seus elementos
void libertarListaLogs(LISTA_LOGS *logs) {
    if (logs == NULL) {
        return;
    }

    limparListaLogs(logs);
}
// Cria e inicializa um novo nó de log com instante, ação e descrição
NO_LOG_ACAO *criarNoLog(int instante, char *acao, char *descricao) {
    NO_LOG_ACAO *novoNo;

    if (acao == NULL || descricao == NULL) {
        return NULL;
    }

    novoNo = (NO_LOG_ACAO *)malloc(sizeof(NO_LOG_ACAO));
    if (novoNo == NULL) {
        return NULL;
    }

    novoNo->log.instante = instante;

    strncpy(novoNo->log.acao, acao, sizeof(novoNo->log.acao) - 1);
    novoNo->log.acao[sizeof(novoNo->log.acao) - 1] = '\0';

    strncpy(novoNo->log.descricao, descricao, sizeof(novoNo->log.descricao) - 1);
    novoNo->log.descricao[sizeof(novoNo->log.descricao) - 1] = '\0';

    novoNo->seguinte = NULL;

    return novoNo;
}