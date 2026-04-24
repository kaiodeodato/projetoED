#ifndef COLABORADORES_H
#define COLABORADORES_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

COLABORADOR *obterColaboradorPorId(SISTEMA *sistema, int id);
COLABORADOR *obterColaboradorPorCaixa(SISTEMA *sistema, int idCaixa);
void associarColaboradoresAsCaixas(SISTEMA *sistema);
void ativarColaborador(COLABORADOR *colaborador, int idCaixa);
void desativarColaborador(COLABORADOR *colaborador);
void incrementarClientesAtendidosColaborador(COLABORADOR *colaborador);

#endif