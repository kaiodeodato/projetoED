#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

size_t calcularMemoriaCliente(CLIENTE *cliente);
size_t calcularMemoriaFila(FILA *fila);
size_t calcularMemoriaHash(HASHTABLE *tabela);
size_t calcularMemoriaListaCompras(LISTA_CLIENTES_COMPRANDO *lista);
size_t calcularMemoriaLogs(LISTA_LOGS *logs);
size_t calcularMemoriaSistema(SISTEMA *sistema);
size_t calcularMemoriaDesperdicadaSistema(SISTEMA *sistema);
void gerarRelatorioMemoria(SISTEMA *sistema, char *nomeFicheiro);
size_t calcularMemoriaHistoricoClientesCaixa(HISTORICO_CLIENTES_CAIXA *historico);
size_t calcularMemoriaBases(SISTEMA *sistema);
size_t calcularMemoriaDesperdicadaBases(SISTEMA *sistema);

#endif