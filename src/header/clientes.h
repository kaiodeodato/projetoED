#ifndef CLIENTES_H
#define CLIENTES_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

CLIENTE *criarCliente(SISTEMA *sistema);
void inicializarCliente(CLIENTE *cliente, SISTEMA *sistema);
int gerarNovosClientes(SISTEMA *sistema);
CLIENTE *gerarClienteAleatorio(SISTEMA *sistema);
void gerarProdutosCliente(CLIENTE *cliente, SISTEMA *sistema);
void ordenarProdutosClientePorPreco(CLIENTE *cliente);
int calcularTempoTotalComprasCliente(CLIENTE *cliente);
int calcularTempoTotalPagamentoCliente(CLIENTE *cliente);
float calcularValorTotalComprasCliente(CLIENTE *cliente);
void atualizarEstadoCliente(CLIENTE *cliente, ESTADO_CLIENTE novoEstado);
int clienteEstaEmAtendimento(CLIENTE *cliente);
void registarEntradaFilaCliente(CLIENTE *cliente, int instanteAtual);
int calcularTempoEsperaCliente(CLIENTE *cliente, int instanteAtual);
void iniciarAtendimentoCliente(CLIENTE *cliente, int instanteAtual);
void atualizarTempoAtendimentoCliente(CLIENTE *cliente);
int clienteTerminouAtendimento(CLIENTE *cliente);
void finalizarAtendimentoCliente(CLIENTE *cliente, int instanteAtual);
int clienteRecebeuOferta(CLIENTE *cliente);
int clienteTemDireitoAOferta(SISTEMA *sistema, CLIENTE *cliente, int instanteAtual);
void aplicarOfertaCliente(CLIENTE *cliente);
int clienteJaMudouDeFila(CLIENTE *cliente);
void marcarClienteMudouDeFila(CLIENTE *cliente);
void libertarCliente(CLIENTE *cliente);
void limparCamposCliente(CLIENTE *cliente);
int obterQuantidadeProdutosCliente(SISTEMA *sistema);
int obterQuantidadeNovosClientesNoCiclo(SISTEMA *sistema);
void calcularCamposDerivadosCliente(CLIENTE *cliente);
int obterIndiceProdutoMaisBarato(CLIENTE *cliente);
int gerarProximoIdClienteBase(BASE_CLIENTES *base);
int contarClientesNasCaixas(SISTEMA *sistema);
int contarClientesSistema(SISTEMA *sistema);

#endif