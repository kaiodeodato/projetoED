#ifndef PRODUTOS_H
#define PRODUTOS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

PRODUTO *gerarProdutoAleatorio(BASE_PRODUTOS *base);
int gerarProximoIdProdutoBase(BASE_PRODUTOS *base);
void procurarProdutoPorId(BASE_PRODUTOS *base);
void mostrarPaginaProdutos(BASE_PRODUTOS *base, int pagina);
void listarProdutosPaginado(BASE_PRODUTOS *base);
void editarProduto(SISTEMA *sistema);

#endif