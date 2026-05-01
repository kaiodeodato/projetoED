#ifndef FICHEIROS_H
#define FICHEIROS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int carregarBaseClientes(BASE_CLIENTES *base, char *nomeFicheiro);
int carregarBaseProdutos(BASE_PRODUTOS *base, char *nomeFicheiro);
int carregarBaseColaboradores(BASE_COLABORADORES *base, char *nomeFicheiro);
int guardarBaseClientes(BASE_CLIENTES *base, char *nomeFicheiro);
int guardarBaseProdutos(BASE_PRODUTOS *base, char *nomeFicheiro);
int escreverHistoricoCSV(LISTA_LOGS *logs, char *nomeFicheiro);
int escreverRelatorioEstatisticas(SISTEMA *sistema, char *nomeFicheiro);
int escreverRelatorioCaixa(CAIXA *caixa, char *nomeFicheiro);
void escreverRelatoriosTodasCaixas(SISTEMA *sistema);
int garantirCapacidadeClientes(BASE_CLIENTES *base);
int garantirCapacidadeProdutos(BASE_PRODUTOS *base);
int garantirCapacidadeColaboradores(BASE_COLABORADORES *base);
void limparPastaRelatorios(void);

#endif