#ifndef HASHCLIENTES_H
#define HASHCLIENTES_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarHash(HASHTABLE *tabela, int nBuckets);
int funcaoHash(int chave, int nBuckets);
BUCKET *obterBucket(HASHTABLE *tabela, int indice);
int inserirClienteHash(HASHTABLE *tabela, CLIENTE *cliente);
CLIENTE *procurarClienteHash(HASHTABLE *tabela, int chave);
int removerClienteHash(HASHTABLE *tabela, int chave);
int clienteExisteHash(HASHTABLE *tabela, int chave);
void libertarHash(HASHTABLE *tabela);
BUCKET *criarBucket(int indice);
HASHNODE *criarHashNode(CLIENTE *cliente);
BUCKET *procurarBucket(BUCKET *inicio, int indice);
void libertarListaClientesBucket(HASHNODE *no);

#endif