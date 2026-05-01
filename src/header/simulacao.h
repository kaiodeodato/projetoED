#ifndef SIMULACAO_H
#define SIMULACAO_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarSimulacao(SISTEMA *sistema);
void executarSimulacao(SISTEMA *sistema);
void cicloSimulacao(SISTEMA *sistema);
void atualizarClientesEmComprasSimulacao(SISTEMA *sistema);
void moverClientesParaFilas(SISTEMA *sistema);
void atualizarCaixasSimulacao(SISTEMA *sistema);
void finalizarAtendimentosSimulacao(SISTEMA *sistema);
void iniciarNovosAtendimentosSimulacao(SISTEMA *sistema);
void aplicarOfertasSimulacao(SISTEMA *sistema);
void avaliarMudancasFilaSimulacao(SISTEMA *sistema);
void verificarAberturaCaixasSimulacao(SISTEMA *sistema);
void verificarEncerramentoCaixasSimulacao(SISTEMA *sistema);
void gerarNovosClientesSimulacao(SISTEMA *sistema);
void atualizarEstatisticasSimulacao(SISTEMA *sistema);
void pausarSimulacao(SISTEMA *sistema);
void retomarSimulacao(SISTEMA *sistema);
void encerrarSimulacao(SISTEMA *sistema);
void processarClientesTerminadosEmCompras(SISTEMA *sistema);
CAIXA *obterCaixaDestinoMudanca(SISTEMA *sistema, CAIXA *caixaAtual);
CLIENTE *obterClienteNaPosicaoFila(FILA *fila, int posicao);
int tentarMoverClienteDeFila(SISTEMA *sistema, CAIXA *caixaOrigem, CLIENTE *cliente);
CAIXA *obterCaixaParaAberturaAutomatica(SISTEMA *sistema);
CAIXA *obterCaixaParaEncerramentoAutomatico(SISTEMA *sistema);
void mostrarPainelSimulacao(SISTEMA *sistema);
void processarTeclaSimulacao(SISTEMA *sistema);
char obterCharLoading(int passo);
int obterDelayPorVelocidade(int velocidade);
char *obterTextoControloCaixa(CAIXA *caixa);
void reinicializarEstadoSimulacao(SISTEMA *sistema);

#endif