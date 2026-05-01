#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structs.h"
#include "define.h"
#include "config.h"
#include "ficheiros.h"
#include "produtos.h"
#include "colaboradores.h"
#include "caixas.h"
#include "hashClientes.h"
#include "historico.h"
#include "listaDeCompras.h"
#include "estatisticas.h"
#include "simulacao.h"
#include "menus.h"
#include "memoria.h"
#include "uteis.h"
#include "filas.h"
#include "clientes.h"
#include "main.h"

// Função principal que inicializa o sistema, carrega dados, executa a simulação e liberta recursos no final
int main(void) {
    configurarAmbienteUTF8();
    SISTEMA sistema;

    srand((unsigned int)time(NULL));

    inicializarSistemaBase(&sistema);

    if (!carregarDadosIniciais(&sistema)) {
        printf("Erro ao carregar os dados iniciais do sistema.\n");
        pausarTela();
        finalizarSistema(&sistema);
        return 1;
    }

    inicializarSimulacao(&sistema);
    executarLoopPrincipal(&sistema);
    finalizarSistema(&sistema);
    return 0;
}
// Inicializa a estrutura base do sistema, definindo configurações padrão e resetando todos os dados e estruturas
void inicializarSistemaBase(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    aplicarValoresDefault(&sistema->config);

    sistema->baseClientes.dados = NULL;
    sistema->baseClientes.tamanho = 0;
    sistema->baseClientes.capacidade = 0;

    sistema->baseProdutos.dados = NULL;
    sistema->baseProdutos.tamanho = 0;
    sistema->baseProdutos.capacidade = 0;

    sistema->baseColaboradores.dados = NULL;
    sistema->baseColaboradores.tamanho = 0;
    sistema->baseColaboradores.capacidade = 0;

    sistema->caixas = NULL;
    sistema->nCaixasAbertas = 0;

    sistema->tempoAtual = 0;
    sistema->estadoSimulacao = SIMULACAO_PARADA;
    sistema->proximoIdClienteGerado = 1;
    sistema->velocidadeSimulacao = VELOCIDADE_SIMULACAO_DEFAULT;
    sistema->ciclosDesdeUltimoRefresh = 0;

    sistema->clientesHash.inicio = NULL;
    sistema->clientesHash.nBuckets = 0;
    sistema->clientesHash.nElementos = 0;

    inicializarEstatisticas(&sistema->estatisticas);
    inicializarListaClientesComprando(&sistema->clientesComprando);
    inicializarListaLogs(&sistema->logs);
}
// Carrega todos os dados iniciais do sistema (configuração, bases, hash e caixas), validando cada etapa
int carregarDadosIniciais(SISTEMA *sistema) {
    if (sistema == NULL) {
        return 0;
    }

    if (!carregarConfiguracao(&sistema->config, FICHEIRO_CONFIGURACAO)) {
        printf("Aviso: configuracao invalida ou nao encontrada. A usar valores default.\n");
        aplicarValoresDefault(&sistema->config);
    }

    if (!carregarBaseClientes(&sistema->baseClientes, FICHEIRO_CLIENTES)) {
        printf("Erro ao carregar a base de clientes.\n");
        return 0;
    }

    if (!carregarBaseProdutos(&sistema->baseProdutos, FICHEIRO_PRODUTOS)) {
        printf("Erro ao carregar a base de produtos.\n");
        return 0;
    }

    if (!carregarBaseColaboradores(&sistema->baseColaboradores, FICHEIRO_COLABORADORES)) {
        printf("Erro ao carregar a base de colaboradores.\n");
        return 0;
    }

    inicializarHash(&sistema->clientesHash, HASH_N_BUCKETS_INICIAL);
    if (sistema->clientesHash.nBuckets <= 0) {
        printf("Erro ao inicializar a hash de clientes.\n");
        return 0;
    }

    inicializarCaixas(sistema);
    if (sistema->caixas == NULL) {
        printf("Erro ao inicializar as caixas.\n");
        return 0;
    }

    associarColaboradoresAsCaixas(sistema);

    adicionarLog(&sistema->logs, sistema->tempoAtual, "SISTEMA", "Sistema inicializado");
    adicionarLog(&sistema->logs, sistema->tempoAtual, "SISTEMA", "Dados iniciais carregados");

    return 1;
}
// Executa o loop principal do programa, gerindo o menu e impedindo saída enquanto a simulação estiver ativa ou pausada
void executarLoopPrincipal(SISTEMA *sistema) {
    int opcao = -1;

    if (sistema == NULL) {
        return;
    }

    while (opcao != 0) {
        mostrarMenuPrincipal();
        opcao = lerOpcaoMenu(0, 6);

        if (opcao == 0 &&
            (sistema->estadoSimulacao == SIMULACAO_ATIVA || sistema->estadoSimulacao == SIMULACAO_PAUSADA)) {
            printf("Encerre a simulacao antes de sair.\n");
            pausarTela();
            opcao = -1;
            continue;
        }

        executarOpcaoMenu(sistema, opcao);
    }
}
// Finaliza o sistema, gerando saídas finais e libertando toda a memória alocada
void finalizarSistema(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    gerarSaidasFinais(sistema);
    libertarEstruturasDinamicas(sistema);
}
// Gera todos os ficheiros de saída finais (histórico, estatísticas, relatórios e memória), atualizando dados antes de gravar
void gerarSaidasFinais(SISTEMA *sistema) {
    char nomeHistorico[TAM_NOME_FICHEIRO];
    char nomeEstatisticas[TAM_NOME_FICHEIRO];
    char nomeMemoria[TAM_NOME_FICHEIRO];

    if (sistema == NULL) {
        return;
    }

    calcularTempoMedioEspera(&sistema->estatisticas);
    atualizarEstatisticasCaixas(sistema);
    determinarOperadorMenosAtendimentos(sistema);

    guardarBaseClientes(&sistema->baseClientes, FICHEIRO_CLIENTES);
    guardarBaseProdutos(&sistema->baseProdutos, FICHEIRO_PRODUTOS);

    gerarNomeFicheiroComTimestamp(
        nomeHistorico,
        TAM_NOME_FICHEIRO,
        PASTA_RELATORIOS PREFIXO_FICHEIRO_HISTORICO,
        EXTENSAO_CSV
    );

    gerarNomeFicheiroComTimestamp(
        nomeEstatisticas,
        TAM_NOME_FICHEIRO,
        PASTA_RELATORIOS PREFIXO_FICHEIRO_ESTATISTICAS,
        EXTENSAO_TXT
    );

    gerarNomeFicheiroComTimestamp(
        nomeMemoria,
        TAM_NOME_FICHEIRO,
        PASTA_RELATORIOS "Memoria_",
        EXTENSAO_TXT
    );

    limparPastaRelatorios();
    escreverHistoricoCSV(&sistema->logs, nomeHistorico);
    escreverRelatorioEstatisticas(sistema, nomeEstatisticas);
    escreverRelatoriosTodasCaixas(sistema);
    gerarRelatorioMemoria(sistema, nomeMemoria);
}
// Liberta todas as estruturas dinâmicas do sistema, incluindo caixas, filas, listas, hash e bases de dados
void libertarEstruturasDinamicas(SISTEMA *sistema) {
    int i;

    if (sistema == NULL) {
        return;
    }

    libertarClientesAtivosHash(sistema);
    libertarClientesHistoricoCaixas(sistema);

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            libertarFila(&sistema->caixas[i].fila);
        }

        free(sistema->caixas);
        sistema->caixas = NULL;
    }

    libertarListaClientesComprando(&sistema->clientesComprando);
    libertarHash(&sistema->clientesHash);
    libertarListaLogs(&sistema->logs);

    libertarBases(sistema);
}
// Liberta os clientes ativos na hash que ainda não foram atendidos, evitando libertar os já processados
void libertarClientesAtivosHash(SISTEMA *sistema) {
    BUCKET *bucketAtual;

    if (sistema == NULL) {
        return;
    }

    bucketAtual = sistema->clientesHash.inicio;

    while (bucketAtual != NULL) {
        HASHNODE *noAtual = bucketAtual->clientes;

        while (noAtual != NULL) {
            if (noAtual->cliente != NULL &&
                noAtual->cliente->estado != CLIENTE_ATENDIDO) {
                
                libertarCliente(noAtual->cliente);
                noAtual->cliente = NULL;
            }

            noAtual = noAtual->prox;
        }

        bucketAtual = bucketAtual->prox;
    }
}
// Liberta todos os clientes armazenados no histórico das caixas e limpa a estrutura associada
void libertarClientesHistoricoCaixas(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        NO_HISTORICO_CLIENTE *atual = sistema->caixas[i].historicoClientes.inicio;

        while (atual != NULL) {
            NO_HISTORICO_CLIENTE *seguinte = atual->seguinte;

            if (atual->cliente != NULL) {
                libertarCliente(atual->cliente);
                atual->cliente = NULL;
            }

            free(atual);
            atual = seguinte;
        }

        sistema->caixas[i].historicoClientes.inicio = NULL;
        sistema->caixas[i].historicoClientes.fim = NULL;
        sistema->caixas[i].historicoClientes.tamanho = 0;
    }
}
// Liberta a memória das bases de clientes, produtos e colaboradores e reinicializa os seus estados
void libertarBases(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    if (sistema->baseClientes.dados != NULL) {
        free(sistema->baseClientes.dados);
        sistema->baseClientes.dados = NULL;
    }

    if (sistema->baseProdutos.dados != NULL) {
        free(sistema->baseProdutos.dados);
        sistema->baseProdutos.dados = NULL;
    }

    if (sistema->baseColaboradores.dados != NULL) {
        free(sistema->baseColaboradores.dados);
        sistema->baseColaboradores.dados = NULL;
    }

    sistema->baseClientes.tamanho = 0;
    sistema->baseClientes.capacidade = 0;

    sistema->baseProdutos.tamanho = 0;
    sistema->baseProdutos.capacidade = 0;

    sistema->baseColaboradores.tamanho = 0;
    sistema->baseColaboradores.capacidade = 0;
}