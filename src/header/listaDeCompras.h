#ifndef LISTADECOMPRAS_H
#define LISTADECOMPRAS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarListaClientesComprando(LISTA_CLIENTES_COMPRANDO *lista);
int listaComprasEstaVazia(LISTA_CLIENTES_COMPRANDO *lista);
int inserirClienteOrdenadoListaCompras(LISTA_CLIENTES_COMPRANDO *lista, CLIENTE *cliente);
CLIENTE *obterPrimeiroClienteListaCompras(LISTA_CLIENTES_COMPRANDO *lista);
CLIENTE *removerPrimeiroClienteListaCompras(LISTA_CLIENTES_COMPRANDO *lista);
void atualizarClientesEmCompras(LISTA_CLIENTES_COMPRANDO *lista, int instanteAtual);
int clienteTerminouComprasLista(CLIENTE *cliente, int instanteAtual);
void libertarListaClientesComprando(LISTA_CLIENTES_COMPRANDO *lista);
NO_CLIENTE_COMPRANDO *criarNoClienteComprando(CLIENTE *cliente);

#endif