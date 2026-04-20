#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

size_t calcularMemoriaCliente(const CLIENTE *cliente);
size_t calcularMemoriaFila(const FILA *fila);
size_t calcularMemoriaHash(const HASHTABLE *tabela);
size_t calcularMemoriaListaCompras(const LISTA_CLIENTES_COMPRANDO *lista);
size_t calcularMemoriaLogs(const LISTA_LOGS *logs);
size_t calcularMemoriaSistema(const SISTEMA *sistema);
size_t calcularMemoriaDesperdicadaSistema(const SISTEMA *sistema);
void gerarRelatorioMemoria(const SISTEMA *sistema, const char *nomeFicheiro);
size_t calcularMemoriaHistoricoClientesCaixa(const HISTORICO_CLIENTES_CAIXA *historico);
size_t calcularMemoriaBases(const SISTEMA *sistema);
size_t calcularMemoriaDesperdicadaBases(const SISTEMA *sistema);

#endif