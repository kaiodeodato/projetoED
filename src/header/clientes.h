#ifndef CLIENTES_H
#define CLIENTES_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

CLIENTE *criarCliente(SISTEMA *sistema);
void inicializarCliente(CLIENTE *cliente, SISTEMA *sistema);
int gerarNovosClientes(SISTEMA *sistema);
void gerarProdutosCliente(CLIENTE *cliente, SISTEMA *sistema);
void atualizarEstadoCliente(CLIENTE *cliente, ESTADO_CLIENTE novoEstado);
int clienteEstaEmAtendimento(CLIENTE *cliente);
CLIENTE *gerarClienteAleatorio(SISTEMA *sistema);
void registarEntradaFilaCliente(CLIENTE *cliente, int instanteAtual);
int calcularTempoEsperaCliente(CLIENTE *cliente, int instanteAtual);
void iniciarAtendimentoCliente(CLIENTE *cliente, int instanteAtual);
void atualizarTempoAtendimentoCliente(CLIENTE *cliente);
int clienteTerminouAtendimento(CLIENTE *cliente);
void finalizarAtendimentoCliente(CLIENTE *cliente, int instanteAtual);
int clienteRecebeuOferta(CLIENTE *cliente);
int clienteTemDireitoAOferta(SISTEMA *sistema, CLIENTE *cliente, int instanteAtual);
int clienteJaMudouDeFila(CLIENTE *cliente);
void marcarClienteMudouDeFila(CLIENTE *cliente);
void libertarCliente(CLIENTE *cliente);
void limparCamposCliente(CLIENTE *cliente);
int obterQuantidadeProdutosCliente(SISTEMA *sistema);
int obterQuantidadeNovosClientesNoCiclo(SISTEMA *sistema);
int gerarProximoIdClienteBase(BASE_CLIENTES *base);
int contarClientesNasCaixas(SISTEMA *sistema);
void ordenarProdutosClientePorPreco(CLIENTE *cliente, SISTEMA *sistema);
int calcularTempoTotalComprasCliente(CLIENTE *cliente, SISTEMA *sistema);
int calcularTempoTotalPagamentoCliente(CLIENTE *cliente, SISTEMA *sistema);
float calcularValorTotalComprasCliente(CLIENTE *cliente, SISTEMA *sistema);
void calcularCamposDerivadosCliente(CLIENTE *cliente, SISTEMA *sistema);
void aplicarOfertaCliente(CLIENTE *cliente, SISTEMA *sistema);
int obterIndiceProdutoMaisBarato(CLIENTE *cliente, SISTEMA *sistema);

#endif