#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

typedef int (*FILTRO_CLIENTE)(const CLIENTE *cliente);

void listarClientesMenu(SISTEMA *sistema, const char *titulo, FILTRO_CLIENTE filtro);
void listarTodosClientesMenu(SISTEMA *sistema);
void listarClientesNasCaixasMenu(SISTEMA *sistema);
void listarClientesComprandoMenu(SISTEMA *sistema);
void mostrarPaginaClientes(CLIENTE **lista, int total, int pagina, const char *titulo);
int carregarClientesEmLista(SISTEMA *sistema, CLIENTE ***lista, FILTRO_CLIENTE filtro);
int filtroTodosClientes(const CLIENTE *cliente);
int filtroClientesNasCaixas(const CLIENTE *cliente);
int filtroClientesComprando(const CLIENTE *cliente);

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
void mostrarEstatisticasSimulacaoMenu(SISTEMA *sistema);
void mostrarRelatorioMemoriaMenu(SISTEMA *sistema);
int colocarCaixaEmAutoUI(SISTEMA *sistema);
void mostrarMenuClientes();
void executarMenuClientes(SISTEMA *sistema);
void mostrarMenuProdutos();
void executarMenuProdutos(SISTEMA *sistema);
void mostrarMenuCaixas();
void executarMenuCaixas(SISTEMA *sistema);
void mostrarMenuRelatorios();
void executarMenuRelatorios(SISTEMA *sistema);

#endif