#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void inicializarEstatisticas(ESTATISTICAS *estatisticas);
void registarClienteGerado(ESTATISTICAS *estatisticas);
void registarClienteAtendido(ESTATISTICAS *estatisticas);
void registarProdutoVendido(ESTATISTICAS *estatisticas, int quantidade, float valor);
void registarProdutoOferecido(ESTATISTICAS *estatisticas, float valor);
void registarMudancaFila(ESTATISTICAS *estatisticas);
void registarAberturaAutomatica(ESTATISTICAS *estatisticas);
void registarEncerramentoAutomatico(ESTATISTICAS *estatisticas);
void atualizarTempoEspera(ESTATISTICAS *estatisticas, int tempoEspera);
void calcularTempoMedioEspera(ESTATISTICAS *estatisticas);
void atualizarEstatisticasCaixas(SISTEMA *sistema);
void determinarOperadorMenosAtendimentos(SISTEMA *sistema);
void atualizarMelhorCaixaClientes(SISTEMA *sistema);
void atualizarMelhorCaixaProdutos(SISTEMA *sistema);

#endif