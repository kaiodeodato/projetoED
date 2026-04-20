#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define.h"

typedef struct Produto {
    int id;
    char nome[MAX_NOME];
    float preco;
    int tempoDeProcura;
    int tempoDePagamento;
} PRODUTO;

typedef enum EstadoCliente {
    CLIENTE_A_COMPRAR,
    CLIENTE_EM_FILA,
    CLIENTE_EM_ATENDIMENTO,
    CLIENTE_ATENDIDO
} ESTADO_CLIENTE;

typedef enum EstadoCaixa {
    CAIXA_FECHADA,
    CAIXA_ABERTA,
    CAIXA_EM_ENCERRAMENTO
} ESTADO_CAIXA;

typedef struct Configuracao {
    int MAX_ESPERA;
    int N_CAIXAS;
    int TEMPO_ATENDIMENTO_PRODUTO;
    float MAX_PRECO;
    int MAX_FILA;
    int MIN_FILA;
    int DIFERENCA_TEMPO_CAIXAS;
    int MAX_PRODUTOS_CLIENTE;
    int MIN_NOVOS_CLIENTES_POR_CICLO;
    int MAX_NOVOS_CLIENTES_POR_CICLO;
} CONFIGURACAO;

typedef struct Estatisticas {
    int tempoSimulacao;
    int totalClientesGerados;
    int totalClientesAtendidos;
    int totalProdutosVendidos;
    float totalValorVendido;
    int totalProdutosOferecidos;
    float totalValorOferecido;
    int totalMudancasFila;
    int totalAberturasAutomaticas;
    int totalEncerramentosAutomaticos;
    float somaTemposEspera;
    float tempoMedioEspera;
    int idCaixaMaisClientes;
    int idCaixaMaisProdutos;
    int idOperadorMenosAtendimentos;
} ESTATISTICAS;

typedef struct Cliente {
    int id;
    char nome[MAX_NOME];
    int nProdutos;
    PRODUTO *produtos;
    ESTADO_CLIENTE estado;
    int idCaixaAtual;
    int instanteEntradaSistema;
    int instantePrevistoFimCompras;
    int instanteEntradaFila;
    int instanteInicioAtendimento;
    int instanteFimAtendimento;
    int tempoRestanteCompras;
    int tempoRestanteAtendimento;
    int tempoTotalCompras;
    int tempoTotalPagamento;
    int recebeuOferta;
    int mudouDeFila;
    float valorTotalCompras;
    float valorOferta;
    char nomeProdutoOferecido[MAX_NOME_PRODUTO];
} CLIENTE;

typedef struct Elemento {
    CLIENTE *cliente;
    struct Elemento *seguinte;
} ELEMENTO;

typedef struct Fila {
    int tamanho;
    ELEMENTO *inicio;
    ELEMENTO *fim;
} FILA;

typedef struct Colaborador {
    int id;
    char nome[MAX_NOME];
    int clientesAtendidos;
    int idCaixaAssociada;
    int ativo;
} COLABORADOR;

typedef struct NoHistoricoCliente {
    CLIENTE *cliente;
    struct NoHistoricoCliente *seguinte;
} NO_HISTORICO_CLIENTE;

typedef struct HistoricoClientesCaixa {
    NO_HISTORICO_CLIENTE *inicio;
    NO_HISTORICO_CLIENTE *fim;
    int tamanho;
} HISTORICO_CLIENTES_CAIXA;

typedef struct Caixa {
    int id;
    ESTADO_CAIXA estado;
    int controloManualGerente;
    int bloqueadaAutomaticamente;
    FILA fila;
    CLIENTE *clienteAtual;
    int tempoTotalEstimadoFila;
    int clientesAtendidos;
    int totalProdutosVendidos;
    float totalValorVendido;
    int totalProdutosOferecidos;
    float totalValorOferecido;

    COLABORADOR *operador;

    HISTORICO_CLIENTES_CAIXA historicoClientes;
} CAIXA;

typedef struct HashNode {
    int chave;
    CLIENTE *cliente;
    struct HashNode *prox;
} HASHNODE;

typedef struct Bucket {
    int indice;
    HASHNODE *clientes;
    struct Bucket *prox;
} BUCKET;

typedef struct HashTable {
    BUCKET *inicio;
    int nBuckets;
    int nElementos;
} HASHTABLE;

typedef struct LogAcao {
    int instante;
    char acao[100];
    char descricao[256];
} LOG_ACAO;

typedef struct NoClienteComprando {
    CLIENTE *cliente;
    struct NoClienteComprando *seguinte;
} NO_CLIENTE_COMPRANDO;

typedef struct ListaClientesComprando {
    NO_CLIENTE_COMPRANDO *inicio;
    NO_CLIENTE_COMPRANDO *fim;
    int tamanho;
} LISTA_CLIENTES_COMPRANDO;

typedef struct ClienteBase {
    int id;
    char nome[MAX_NOME];
} CLIENTE_BASE;

typedef struct BaseClientes {
    CLIENTE_BASE *dados;
    int tamanho;
    int capacidade;
} BASE_CLIENTES;

typedef struct BaseProdutos {
    PRODUTO *dados;
    int tamanho;
    int capacidade;
} BASE_PRODUTOS;

typedef struct BaseColaboradores {
    COLABORADOR *dados;
    int tamanho;
    int capacidade;
} BASE_COLABORADORES;

typedef enum EstadoSimulacao {
    SIMULACAO_PARADA,
    SIMULACAO_ATIVA,
    SIMULACAO_PAUSADA,
    SIMULACAO_ENCERRADA
} ESTADO_SIMULACAO;

typedef struct NoLogAcao {
    LOG_ACAO log;
    struct NoLogAcao *seguinte;
} NO_LOG_ACAO;

typedef struct ListaLogs {
    NO_LOG_ACAO *inicio;
    NO_LOG_ACAO *fim;
    int tamanho;
} LISTA_LOGS;

typedef struct Sistema {
    CONFIGURACAO config;

    BASE_CLIENTES baseClientes;
    BASE_PRODUTOS baseProdutos;
    BASE_COLABORADORES baseColaboradores;

    CAIXA *caixas;
    int nCaixasAbertas;

    int tempoAtual;
    ESTADO_SIMULACAO estadoSimulacao;
    int proximoIdClienteGerado;
    int velocidadeSimulacao;
    int ciclosDesdeUltimoRefresh;

    HASHTABLE clientesHash;
    ESTATISTICAS estatisticas;
    LISTA_CLIENTES_COMPRANDO clientesComprando;

    LISTA_LOGS logs;
} SISTEMA;


#endif