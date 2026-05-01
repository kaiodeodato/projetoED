#ifndef HISTORICO_H
#define HISTORICO_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarListaLogs(LISTA_LOGS *logs);
void adicionarLog(LISTA_LOGS *logs, int instante, char *acao, char *descricao);
void limparListaLogs(LISTA_LOGS *logs);
void libertarListaLogs(LISTA_LOGS *logs);
NO_LOG_ACAO *criarNoLog(int instante, char *acao, char *descricao);

#endif