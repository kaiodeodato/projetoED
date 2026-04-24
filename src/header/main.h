#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarSistemaBase(SISTEMA *sistema);
int carregarDadosIniciais(SISTEMA *sistema);
void executarLoopPrincipal(SISTEMA *sistema);
void finalizarSistema(SISTEMA *sistema);
void libertarEstruturasDinamicas(SISTEMA *sistema);
void libertarClientesAtivosHash(SISTEMA *sistema);
void libertarClientesHistoricoCaixas(SISTEMA *sistema);
void libertarBases(SISTEMA *sistema);
void gerarSaidasFinais(SISTEMA *sistema);

#endif