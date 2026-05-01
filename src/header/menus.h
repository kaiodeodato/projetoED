#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void mostrarMenuPrincipal();
int lerOpcaoMenu(int min, int max);
void executarOpcaoMenu(SISTEMA *sistema, int opcao);
void mostrarMenuGestao();
void adicionarClienteBaseMenu(SISTEMA *sistema);
void adicionarProdutoBaseMenu(SISTEMA *sistema);
void abrirCaixaManual(SISTEMA *sistema);
void fecharCaixaManual(SISTEMA *sistema);
void mostrarCabecalho(char *titulo);
void mostrarEstadoResumidoSistema(SISTEMA *sistema);
void executarMenuGestao(SISTEMA *sistema);
void alterarVelocidadeSimulacao(SISTEMA *sistema);
char *obterTextoVelocidade(int velocidade);
void pesquisarClienteAtivoMenu(SISTEMA *sistema);
void listarTodosClientesMenu(SISTEMA *sistema);
void mostrarEstatisticasSimulacaoMenu(SISTEMA *sistema);
void mostrarRelatorioMemoriaMenu(SISTEMA *sistema);
int colocarCaixaEmAutoUI(SISTEMA *sistema);

#endif