#ifndef CAIXAS_H
#define CAIXAS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"


void inicializarCaixa(CAIXA *caixa, int idCaixa);
void inicializarCaixas(SISTEMA *sistema);
CAIXA *obterCaixaPorId(SISTEMA *sistema, int idCaixa);
CAIXA *obterPrimeiraCaixaAberta(SISTEMA *sistema);
CAIXA *obterCaixaComMenorTempoEstimado(SISTEMA *sistema);
int contarCaixasAbertas(SISTEMA *sistema);
int contarCaixasEmEncerramento(SISTEMA *sistema);
int caixaEstaAberta(CAIXA *caixa);
int caixaEstaFechada(CAIXA *caixa);
int caixaEstaEmEncerramento(CAIXA *caixa);
int caixaAceitaNovosClientes(CAIXA *caixa);
int caixaTemClientes(CAIXA *caixa);
int abrirCaixa(SISTEMA *sistema, int idCaixa, int controloManual);
int encerrarCaixa(SISTEMA *sistema, int idCaixa, int controloManual);
int fecharCaixa(SISTEMA *sistema, int idCaixa, int controloManual);
int colocarCaixaEmAuto(SISTEMA *sistema, int idCaixa);
int encaminharClienteParaMelhorCaixa(SISTEMA *sistema, CLIENTE *cliente);
int adicionarClienteNaCaixa(CAIXA *caixa, CLIENTE *cliente, int instanteAtual);
int redistribuirClientesCaixaFechada(SISTEMA *sistema, int idCaixa);
int iniciarAtendimentoSeNecessario(SISTEMA *sistema, CAIXA *caixa);
int finalizarAtendimentoSeConcluido(SISTEMA *sistema, CAIXA *caixa);
int calcularTempoRestanteClienteAtual(CAIXA *caixa);
int calcularTempoEstimadoCaixa(CAIXA *caixa);
int obterNumeroClientesCaixa(CAIXA *caixa);
void atualizarTempoEstimadoCaixa(CAIXA *caixa);
void adicionarClienteAoHistoricoCaixa(CAIXA *caixa, CLIENTE *cliente);
void ativarOperadorDaCaixa(SISTEMA *sistema, int idCaixa);
void desativarOperadorDaCaixa(SISTEMA *sistema, int idCaixa);

#endif