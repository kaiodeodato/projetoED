#ifndef FICHEIROS_H
#define FICHEIROS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int carregarBaseClientes(BASE_CLIENTES *base, const char *nomeFicheiro);
int carregarBaseProdutos(BASE_PRODUTOS *base, const char *nomeFicheiro);
int carregarBaseColaboradores(BASE_COLABORADORES *base, const char *nomeFicheiro);
int guardarBaseClientes(const BASE_CLIENTES *base, const char *nomeFicheiro);
int guardarBaseProdutos(const BASE_PRODUTOS *base, const char *nomeFicheiro);
int escreverHistoricoCSV(const LISTA_LOGS *logs, const char *nomeFicheiro);
int escreverRelatorioEstatisticas(const SISTEMA *sistema, const char *nomeFicheiro);
int escreverRelatorioCaixa(const CAIXA *caixa, const char *nomeFicheiro);
void escreverRelatoriosTodasCaixas(const SISTEMA *sistema);
int garantirCapacidadeClientes(BASE_CLIENTES *base);
int garantirCapacidadeProdutos(BASE_PRODUTOS *base);
int garantirCapacidadeColaboradores(BASE_COLABORADORES *base);
void limparPastaRelatorios(void);

#endif