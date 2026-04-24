#ifndef FILAS_H
#define FILAS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarFila(FILA *fila);
int filaEstaVazia(const FILA *fila);
int obterTamanhoFila(const FILA *fila);
int enfileirarCliente(FILA *fila, CLIENTE *cliente);
CLIENTE *desenfileirarCliente(FILA *fila);
int removerClienteDaFilaPorId(FILA *fila, int idCliente);
void libertarFila(FILA *fila);
ELEMENTO *criarElementoFila(CLIENTE *cliente);

#endif