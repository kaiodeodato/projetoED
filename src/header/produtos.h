#ifndef PRODUTOS_H
#define PRODUTOS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

PRODUTO *gerarProdutoAleatorio(BASE_PRODUTOS *base);
void ordenarProdutosPorPreco(PRODUTO *produtos, int quantidade);
int calcularTempoTotalProdutos(const PRODUTO *produtos, int quantidade);
float calcularValorTotalProdutos(const PRODUTO *produtos, int quantidade);
void trocarProdutos(PRODUTO *a, PRODUTO *b);
int gerarProximoIdProdutoBase(BASE_PRODUTOS *base);

#endif