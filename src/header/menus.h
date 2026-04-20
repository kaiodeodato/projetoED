#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void mostrarMenuPrincipal();
int lerOpcaoMenu();
void executarOpcaoMenu(SISTEMA *sistema, int opcao);
void mostrarMenuGestao();
void adicionarClienteBaseMenu(SISTEMA *sistema);
void adicionarProdutoBaseMenu(SISTEMA *sistema);
void abrirCaixaManual(SISTEMA *sistema);
void fecharCaixaManual(SISTEMA *sistema);
void mostrarCabecalho(const char *titulo);
void mostrarEstadoResumidoSistema(const SISTEMA *sistema);
void executarMenuGestao(SISTEMA *sistema);
void alterarVelocidadeSimulacao(SISTEMA *sistema);
char *obterTextoVelocidade(int velocidade);

#endif