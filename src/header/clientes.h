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
int calcularTempoTotalComprasCliente(const CLIENTE *cliente);
int calcularTempoTotalPagamentoCliente(const CLIENTE *cliente);
float calcularValorTotalComprasCliente(const CLIENTE *cliente);
void atualizarEstadoCliente(CLIENTE *cliente, ESTADO_CLIENTE novoEstado);
int clienteEstaEmAtendimento(const CLIENTE *cliente);
void registarEntradaFilaCliente(CLIENTE *cliente, int instanteAtual);
int calcularTempoEsperaCliente(const CLIENTE *cliente, int instanteAtual);
void iniciarAtendimentoCliente(CLIENTE *cliente, int instanteAtual);
void atualizarTempoAtendimentoCliente(CLIENTE *cliente);
int clienteTerminouAtendimento(const CLIENTE *cliente);
void finalizarAtendimentoCliente(CLIENTE *cliente, int instanteAtual);
int clienteRecebeuOferta(const CLIENTE *cliente);
int clienteTemDireitoAOferta(const SISTEMA *sistema, const CLIENTE *cliente, int instanteAtual);
void aplicarOfertaCliente(CLIENTE *cliente);
int clienteJaMudouDeFila(const CLIENTE *cliente);
void marcarClienteMudouDeFila(CLIENTE *cliente);
void libertarCliente(CLIENTE *cliente);
void limparCamposCliente(CLIENTE *cliente);
int obterQuantidadeProdutosCliente(const SISTEMA *sistema);
int obterQuantidadeNovosClientesNoCiclo(const SISTEMA *sistema);
void calcularCamposDerivadosCliente(CLIENTE *cliente);
int obterIndiceProdutoMaisBarato(const CLIENTE *cliente);
int gerarProximoIdClienteBase(BASE_CLIENTES *base);
int contarClientesNasCaixas(const SISTEMA *sistema);

#endif