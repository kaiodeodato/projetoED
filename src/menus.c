#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menus.h"
#include "uteis.h"
#include "historico.h"
#include "config.h"
#include "ficheiros.h"
#include "caixas.h"
#include "simulacao.h"
#include "define.h"
#include "clientes.h"
#include "produtos.h"

void mostrarMenuPrincipal() {
    limparTela();
    mostrarCabecalho("MENU PRINCIPAL");
    printf("1. Iniciar simulacao\n");
    printf("2. Retomar simulacao\n");
    printf("3. Gestao administrativa\n");
    printf("4. Encerrar simulacao\n");
    printf("0. Sair\n");
    printf("%s", LINHA_SEPARADORA);
}

int lerOpcaoMenu() {
    int opcao = 0;

    lerInteiro("Opcao: ", &opcao, 0, 4);
    return opcao;
}

void executarOpcaoMenu(SISTEMA *sistema, int opcao) {
    if (sistema == NULL) {
        return;
    }
    switch (opcao) {
        case 1:
            if (sistema->estadoSimulacao == SIMULACAO_ENCERRADA) {
                reinicializarEstadoSimulacao(sistema);
            }
            executarSimulacao(sistema);
            break;
            break;

        case 2:
            if (sistema->estadoSimulacao == SIMULACAO_PAUSADA) {
                retomarSimulacao(sistema);
                executarSimulacao(sistema);
            } else {
                printf("A simulacao nao esta pausada.\n");
                pausarTela();
            }
            break;

        case 3:
            executarMenuGestao(sistema);
            break;

        case 4:
            encerrarSimulacao(sistema);
            pausarTela();
            break;

        case 0:
            adicionarLog(&sistema->logs, sistema->tempoAtual, "MENU", "Saida do programa");
            break;

        default:
            printf("Opcao invalida.\n");
            break;
    }
}

void mostrarMenuGestao() {
    limparTela();
    mostrarCabecalho("MENU DE GESTAO");
    printf("1. Adicionar cliente base\n");
    printf("2. Adicionar produto base\n");
    printf("3. Abrir caixa manualmente\n");
    printf("4. Fechar caixa manualmente\n");
    printf("5. Mostrar estado resumido\n");
    printf("6. Alterar velocidade da simulacao\n");
    printf("0. Voltar\n");
    printf("%s", LINHA_SEPARADORA);
}

void adicionarClienteBaseMenu(SISTEMA *sistema) {
    CLIENTE_BASE *novosDados;
    int novaCapacidade;
    char nome[MAX_NOME];

    if (sistema == NULL) {
        return;
    }

    lerString("Nome do cliente: ", nome, MAX_NOME);

    if (sistema->baseClientes.tamanho >= sistema->baseClientes.capacidade) {
        novaCapacidade = sistema->baseClientes.capacidade <= 0
            ? CAPACIDADE_INICIAL_CLIENTES_BASE
            : sistema->baseClientes.capacidade * FATOR_CRESCIMENTO_VETORES;

        novosDados = (CLIENTE_BASE *)realloc(
            sistema->baseClientes.dados,
            sizeof(CLIENTE_BASE) * novaCapacidade
        );

        if (novosDados == NULL) {
            printf("Erro ao alocar memoria para clientes base.\n");
            return;
        }

        sistema->baseClientes.dados = novosDados;
        sistema->baseClientes.capacidade = novaCapacidade;
    }

    sistema->baseClientes.dados[sistema->baseClientes.tamanho].id = gerarProximoIdClienteBase(&sistema->baseClientes);

    copiarStringSeguro(
        sistema->baseClientes.dados[sistema->baseClientes.tamanho].nome,
        nome,
        MAX_NOME
    );

    sistema->baseClientes.tamanho++;

    printf("Cliente base adicionado com sucesso.\n");
}

void adicionarProdutoBaseMenu(SISTEMA *sistema) {
    PRODUTO *novosDados;
    int novaCapacidade;
    char nome[MAX_NOME_PRODUTO];
    float preco;
    int tempoProcura;
    int tempoPagamento;

    if (sistema == NULL) {
        return;
    }

    lerString("Nome do produto: ", nome, MAX_NOME_PRODUTO);
    lerFloat("Preco: ", &preco, 0.01f, 999999.0f);
    lerInteiro("Tempo de procura: ", &tempoProcura, 1, 999999);
    lerInteiro("Tempo de pagamento: ", &tempoPagamento, 1, 999999);

    if (sistema->baseProdutos.tamanho >= sistema->baseProdutos.capacidade) {
        novaCapacidade = sistema->baseProdutos.capacidade <= 0
            ? CAPACIDADE_INICIAL_PRODUTOS_BASE
            : sistema->baseProdutos.capacidade * FATOR_CRESCIMENTO_VETORES;

        novosDados = (PRODUTO *)realloc(
            sistema->baseProdutos.dados,
            sizeof(PRODUTO) * novaCapacidade
        );

        if (novosDados == NULL) {
            printf("Erro ao alocar memoria para produtos base.\n");
            return;
        }

        sistema->baseProdutos.dados = novosDados;
        sistema->baseProdutos.capacidade = novaCapacidade;
    }

    sistema->baseProdutos.dados[sistema->baseProdutos.tamanho].id =
        gerarProximoIdProdutoBase(&sistema->baseProdutos);

    copiarStringSeguro(
        sistema->baseProdutos.dados[sistema->baseProdutos.tamanho].nome,
        nome,
        MAX_NOME_PRODUTO
    );

    sistema->baseProdutos.dados[sistema->baseProdutos.tamanho].preco = preco;
    sistema->baseProdutos.dados[sistema->baseProdutos.tamanho].tempoDeProcura = tempoProcura;
    sistema->baseProdutos.dados[sistema->baseProdutos.tamanho].tempoDePagamento = tempoPagamento;

    sistema->baseProdutos.tamanho++;

    adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Produto base adicionado");
    printf("Produto base adicionado com sucesso.\n");
}

void abrirCaixaManual(SISTEMA *sistema) {
    int idCaixa;

    if (sistema == NULL) {
        return;
    }

    if (sistema->config.N_CAIXAS <= 0) {
        printf("Nao existem caixas configuradas.\n");
        return;
    }

    lerInteiro(
        "Id da caixa a abrir: ",
        &idCaixa,
        0,
        sistema->config.N_CAIXAS - 1
    );

    if (abrirCaixa(sistema, idCaixa, CAIXA_COM_CONTROLO_MANUAL)) {
        adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Caixa aberta manualmente");
        printf("Caixa %d aberta com sucesso.\n", idCaixa);
    } else {
        printf("Nao foi possivel abrir a caixa %d.\n", idCaixa);
    }
}

void fecharCaixaManual(SISTEMA *sistema) {
    int idCaixa;

    if (sistema == NULL) {
        return;
    }

    if (sistema->config.N_CAIXAS <= 0) {
        printf("Nao existem caixas configuradas.\n");
        return;
    }

    lerInteiro(
        "Id da caixa a fechar: ",
        &idCaixa,
        0,
        sistema->config.N_CAIXAS - 1
    );

    if (encerrarCaixa(sistema, idCaixa, CAIXA_COM_CONTROLO_MANUAL)) {
        adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Caixa colocada em encerramento manual");
        printf("Caixa %d em encerramento.\n", idCaixa);
    } else {
        printf("Nao foi possivel encerrar a caixa %d.\n", idCaixa);
    }
}

void mostrarCabecalho(const char *titulo) {
    printf("\n%s", LINHA_SEPARADORA);
    if (titulo != NULL) {
        printf("%s\n", titulo);
    }
    printf("%s", LINHA_SEPARADORA);
}

void mostrarEstadoResumidoSistema(const SISTEMA *sistema) {
    int i;
    int totalMinutos;
    int dias;
    int horas;
    int minutos;

    if (sistema == NULL) {
        return;
    }

    limparTela();

    totalMinutos = sistema->tempoAtual;
    dias = totalMinutos / 1440;
    horas = (totalMinutos % 1440) / 60;
    minutos = totalMinutos % 60;

    printf("%s", LINHA_SEPARADORA);
    printf("SIMULACAO EM EXECUCAO\n");
    printf("%s", LINHA_SEPARADORA);

    printf(
        "Tempo de simulacao: %d min (%d dia(s), %02d:%02d)\n",
        sistema->tempoAtual,
        dias,
        horas,
        minutos
    );
    printf("Velocidade: %dx\n", sistema->velocidadeSimulacao);
    printf("Estado: %d\n", sistema->estadoSimulacao);
    printf("Clientes em compras: %d\n", sistema->clientesComprando.tamanho);
    printf("Clientes ativos na hash: %d\n", sistema->clientesHash.nElementos);
    printf("Caixas abertas: %d\n", sistema->nCaixasAbertas);
    printf("Clientes gerados: %d\n", sistema->estatisticas.totalClientesGerados);
    printf("Clientes atendidos: %d\n", sistema->estatisticas.totalClientesAtendidos);
    printf("Produtos oferecidos: %d\n", sistema->estatisticas.totalProdutosOferecidos);
    printf("Tempo medio de espera: %.2f min\n", sistema->estatisticas.tempoMedioEspera);

    printf("\n%s", LINHA_SEPARADORA);
    printf("ESTADO DAS CAIXAS\n");
    printf("%s", LINHA_SEPARADORA);

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            printf(
                "Caixa %d | Estado=%d | Abertura=%s | Fila=%d | Atendimento=%s | Atendidos=%d\n",
                sistema->caixas[i].id,
                sistema->caixas[i].estado,
                obterTextoControloCaixa(&sistema->caixas[i]),
                sistema->caixas[i].fila.tamanho,
                sistema->caixas[i].clienteAtual != NULL ? "SIM" : "NAO",
                sistema->caixas[i].clientesAtendidos
            );
        }
    }

    printf("\nLoading: -");
    printf("\nPrima 'P' para pausar e voltar ao menu.\n");

    pausarTela();
}

void executarMenuGestao(SISTEMA *sistema) {
    int opcao = -1;

    if (sistema == NULL) {
        return;
    }

    while (opcao != 0) {
        mostrarMenuGestao();
        lerInteiro("Opcao: ", &opcao, 0, 6);

        switch (opcao) {
            case 1:
                adicionarClienteBaseMenu(sistema);
                pausarTela();
                break;

            case 2:
                adicionarProdutoBaseMenu(sistema);
                pausarTela();
                break;

            case 3:
                abrirCaixaManual(sistema);
                pausarTela();
                break;

            case 4:
                fecharCaixaManual(sistema);
                pausarTela();
                break;

            case 5:
                mostrarEstadoResumidoSistema(sistema);
                pausarTela();
                break;

            case 6:
                alterarVelocidadeSimulacao(sistema);
                break;

            case 0:
                adicionarLog(&sistema->logs, sistema->tempoAtual, "MENU", "Saida do menu de gestao");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
}

void alterarVelocidadeSimulacao(SISTEMA *sistema) {
    int opcao;

    if (sistema == NULL) {
        return;
    }

    limparTela();
    mostrarCabecalho("VELOCIDADE DA SIMULACAO");
    printf("Velocidade atual: %s\n\n", obterTextoVelocidade(sistema->velocidadeSimulacao));
    printf("1. 1x (normal)\n");
    printf("2. 5x\n");
    printf("3. 10x\n");
    printf("4. 100x\n");
    printf("5. 500x\n");
    printf("0. Cancelar\n");
    printf("%s", LINHA_SEPARADORA);

    lerInteiro("Opcao: ", &opcao, 0, 5);

    switch (opcao) {
        case 1:
            sistema->velocidadeSimulacao = VELOCIDADE_SIMULACAO_1X;
            break;
        case 2:
            sistema->velocidadeSimulacao = VELOCIDADE_SIMULACAO_5X;
            break;
        case 3:
            sistema->velocidadeSimulacao = VELOCIDADE_SIMULACAO_10X;
            break;
        case 4:
            sistema->velocidadeSimulacao = VELOCIDADE_SIMULACAO_100X;
            break;
        case 5:
            sistema->velocidadeSimulacao = VELOCIDADE_SIMULACAO_500X;
            break;
        case 0:
            return;
        default:
            return;
    }

    adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Velocidade da simulacao alterada");

    printf(
        "Velocidade alterada com sucesso para %s.\n",
        obterTextoVelocidade(sistema->velocidadeSimulacao)
    );
    pausarTela();
}

char *obterTextoVelocidade(int velocidade) {
    switch (velocidade) {
        case VELOCIDADE_SIMULACAO_1X:
            return "1x";
        case VELOCIDADE_SIMULACAO_5X:
            return "5x";
        case VELOCIDADE_SIMULACAO_10X:
            return "10x";
        case VELOCIDADE_SIMULACAO_100X:
            return "100x";
        case VELOCIDADE_SIMULACAO_500X:
            return "500x";
        default:
            return "desconhecida";
    }
}