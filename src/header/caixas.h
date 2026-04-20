#ifndef CAIXAS_H
#define CAIXAS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarCaixas(SISTEMA *sistema);
void inicializarCaixa(CAIXA *caixa, int idCaixa);
CAIXA *obterCaixaPorId(SISTEMA *sistema, int idCaixa);
CAIXA *obterPrimeiraCaixaAberta(SISTEMA *sistema);
CAIXA *obterCaixaComMenorFila(SISTEMA *sistema);
CAIXA *obterCaixaComMenorTempoEstimado(SISTEMA *sistema);
CAIXA *obterCaixaComMenosClientes(SISTEMA *sistema);
int contarCaixasAbertas(const SISTEMA *sistema);
int contarCaixasEmEncerramento(const SISTEMA *sistema);
int caixaEstaAberta(const CAIXA *caixa);
int caixaEstaFechada(const CAIXA *caixa);
int caixaEstaEmEncerramento(const CAIXA *caixa);
int caixaAceitaNovosClientes(const CAIXA *caixa);
int caixaTemClientes(const CAIXA *caixa);
int abrirCaixa(SISTEMA *sistema, int idCaixa, int controloManual);
int encerrarCaixa(SISTEMA *sistema, int idCaixa, int controloManual);
int fecharCaixa(SISTEMA *sistema, int idCaixa, int controloManual);
int encaminharClienteParaMelhorCaixa(SISTEMA *sistema, CLIENTE *cliente);
int adicionarClienteNaCaixa(CAIXA *caixa, CLIENTE *cliente, int instanteAtual);
CLIENTE *retirarClienteDaCaixa(CAIXA *caixa);
int redistribuirClientesCaixaFechada(SISTEMA *sistema, int idCaixa);
int iniciarAtendimentoSeNecessario(SISTEMA *sistema, CAIXA *caixa);
int finalizarAtendimentoSeConcluido(SISTEMA *sistema, CAIXA *caixa);
int calcularTempoRestanteClienteAtual(const CAIXA *caixa);
int calcularTempoEstimadoCaixa(const CAIXA *caixa);
float calcularMediaClientesPorCaixaAberta(const SISTEMA *sistema);
int obterNumeroClientesCaixa(const CAIXA *caixa);
int obterNumeroTotalClientesEmFilas(const SISTEMA *sistema);
void atualizarTempoEstimadoCaixa(CAIXA *caixa);
void adicionarClienteAoHistoricoCaixa(CAIXA *caixa, CLIENTE *cliente);
void ativarOperadorDaCaixa(SISTEMA *sistema, int idCaixa);
void desativarOperadorDaCaixa(SISTEMA *sistema, int idCaixa);
int existeCaixaComTempoAcimaDoLimite(SISTEMA *sistema);

#endif