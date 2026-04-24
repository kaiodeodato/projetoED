#include <stdio.h>
#include <stdlib.h>
#include "caixas.h"
#include "clientes.h"
#include "filas.h"
#include "colaboradores.h"
#include "estatisticas.h"
#include "hashClientes.h"

void inicializarCaixa(CAIXA *caixa, int idCaixa) {
    if (caixa == NULL) {
        return;
    }

    caixa->id = idCaixa;
    caixa->estado = CAIXA_FECHADA;
    caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
    caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;

    inicializarFila(&caixa->fila);

    caixa->clienteAtual = NULL;
    caixa->tempoTotalEstimadoFila = 0;
    caixa->clientesAtendidos = 0;
    caixa->totalProdutosVendidos = 0;
    caixa->totalValorVendido = 0.0f;
    caixa->totalProdutosOferecidos = 0;
    caixa->totalValorOferecido = 0.0f;
    caixa->operador = NULL;

    caixa->historicoClientes.inicio = NULL;
    caixa->historicoClientes.fim = NULL;
    caixa->historicoClientes.tamanho = 0;
}

void inicializarCaixas(SISTEMA *sistema) {
    int i;
    int quantidadeCaixasAbertasInicial;

    if (sistema == NULL || sistema->config.N_CAIXAS <= 0) {
        return;
    }

    sistema->caixas = (CAIXA *)malloc(sizeof(CAIXA) * sistema->config.N_CAIXAS);
    if (sistema->caixas == NULL) {
        sistema->nCaixasAbertas = 0;
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        inicializarCaixa(&sistema->caixas[i], i);
    }

    quantidadeCaixasAbertasInicial = N_CAIXAS_INICIALMENTE_ABERTAS;
    if (quantidadeCaixasAbertasInicial > sistema->config.N_CAIXAS) {
        quantidadeCaixasAbertasInicial = sistema->config.N_CAIXAS;
    }

    sistema->nCaixasAbertas = 0;

    for (i = 0; i < quantidadeCaixasAbertasInicial; i++) {
        sistema->caixas[i].estado = CAIXA_ABERTA;
        sistema->caixas[i].controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
        sistema->caixas[i].bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
        sistema->nCaixasAbertas++;
    }
}

CAIXA *obterCaixaPorId(SISTEMA *sistema, int idCaixa) {
    if (sistema == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    if (idCaixa < 0 || idCaixa >= sistema->config.N_CAIXAS) {
        return NULL;
    }

    return &sistema->caixas[idCaixa];
}

CAIXA *obterPrimeiraCaixaAberta(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        if (caixaEstaAberta(&sistema->caixas[i])) {
            return &sistema->caixas[i];
        }
    }

    return NULL;
}

CAIXA *obterCaixaComMenorTempoEstimado(SISTEMA *sistema) {
    int i;
    CAIXA *melhorCaixa = NULL;
    int menorTempo = 0;

    if (sistema == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];
        int tempoEstimado;

        if (!caixaAceitaNovosClientes(caixa)) {
            continue;
        }

        tempoEstimado = calcularTempoEstimadoCaixa(caixa);

        if (melhorCaixa == NULL || tempoEstimado < menorTempo ||
            (tempoEstimado == menorTempo && caixa->id < melhorCaixa->id)) {
            melhorCaixa = caixa;
            menorTempo = tempoEstimado;
        }
    }

    return melhorCaixa;
}

int contarCaixasAbertas(const SISTEMA *sistema) {
    int i;
    int contador = 0;

    if (sistema == NULL || sistema->caixas == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        if (caixaEstaAberta(&sistema->caixas[i])) {
            contador++;
        }
    }

    return contador;
}

int contarCaixasEmEncerramento(const SISTEMA *sistema) {
    int i;
    int contador = 0;

    if (sistema == NULL || sistema->caixas == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        if (caixaEstaEmEncerramento(&sistema->caixas[i])) {
            contador++;
        }
    }

    return contador;
}

int caixaEstaAberta(const CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_ABERTA;
}

int caixaEstaFechada(const CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_FECHADA;
}

int caixaEstaEmEncerramento(const CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_EM_ENCERRAMENTO;
}

int caixaAceitaNovosClientes(const CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_ABERTA;
}

int caixaTemClientes(const CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->clienteAtual != NULL || !filaEstaVazia(&caixa->fila);
}

int abrirCaixa(SISTEMA *sistema, int idCaixa, int controloManual) {
    CAIXA *caixa;

    if (sistema == NULL) {
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return 0;
    }

    if (caixaEstaAberta(caixa)) {
        if (controloManual) {
            caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
            caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
        }
        return 1;
    }

    if (caixaEstaEmEncerramento(caixa) && caixaTemClientes(caixa)) {
        caixa->estado = CAIXA_ABERTA;
        if (controloManual) {
            caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
            caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
        } else {
            caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
            caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
        }
        sistema->nCaixasAbertas = contarCaixasAbertas(sistema);
        ativarOperadorDaCaixa(sistema, idCaixa);
        atualizarTempoEstimadoCaixa(caixa);
        return 1;
    }

    caixa->estado = CAIXA_ABERTA;
    caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;

    if (controloManual) {
        caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
    } else {
        caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
    }

    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);
    ativarOperadorDaCaixa(sistema, idCaixa);
    atualizarTempoEstimadoCaixa(caixa);

    return 1;
}

int encerrarCaixa(SISTEMA *sistema, int idCaixa, int controloManual) {
    CAIXA *caixa;

    if (sistema == NULL) {
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return 0;
    }

    if (caixaEstaFechada(caixa)) {
        return 0;
    }

    if (caixaEstaEmEncerramento(caixa)) {
        return 1;
    }

    caixa->estado = CAIXA_EM_ENCERRAMENTO;

    if (controloManual) {
        caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
        redistribuirClientesCaixaFechada(sistema, idCaixa);
    } else {
        caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
    }

    atualizarTempoEstimadoCaixa(caixa);
    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);

    if (!caixaTemClientes(caixa)) {
        return fecharCaixa(sistema, idCaixa, controloManual);
    }

    return 1;
}

int fecharCaixa(SISTEMA *sistema, int idCaixa, int controloManual) {
    CAIXA *caixa;

    if (sistema == NULL) {
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return 0;
    }

    if (caixaEstaFechada(caixa)) {
        return 1;
    }

    if (caixa->clienteAtual != NULL) {
        return 0;
    }

    if (!filaEstaVazia(&caixa->fila)) {
        redistribuirClientesCaixaFechada(sistema, idCaixa);
    }

    caixa->estado = CAIXA_FECHADA;
    caixa->tempoTotalEstimadoFila = 0;
    caixa->bloqueadaAutomaticamente = controloManual ? CAIXA_BLOQUEADA_AUTOMATICAMENTE : CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;

    if (!controloManual) {
        caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
    }

    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);
    desativarOperadorDaCaixa(sistema, idCaixa);

    return 1;
}

int encaminharClienteParaMelhorCaixa(SISTEMA *sistema, CLIENTE *cliente) {
    CAIXA *caixaDestino;

    if (sistema == NULL || cliente == NULL) {
        return 0;
    }

    caixaDestino = obterCaixaComMenorTempoEstimado(sistema);
    if (caixaDestino == NULL) {
        caixaDestino = obterPrimeiraCaixaAberta(sistema);
    }

    if (caixaDestino == NULL) {
        return 0;
    }

    return adicionarClienteNaCaixa(caixaDestino, cliente, sistema->tempoAtual);
}

int adicionarClienteNaCaixa(CAIXA *caixa, CLIENTE *cliente, int instanteAtual) {
    if (caixa == NULL || cliente == NULL) {
        return 0;
    }

    if (!caixaAceitaNovosClientes(caixa)) {
        return 0;
    }

    registarEntradaFilaCliente(cliente, instanteAtual);
    cliente->idCaixaAtual = caixa->id;

    if (!enfileirarCliente(&caixa->fila, cliente)) {
        return 0;
    }

    atualizarTempoEstimadoCaixa(caixa);
    return 1;
}

int redistribuirClientesCaixaFechada(SISTEMA *sistema, int idCaixa) {
    CAIXA *caixaOrigem;
    CLIENTE *cliente;
    int totalRedistribuido = 0;

    if (sistema == NULL) {
        return 0;
    }

    caixaOrigem = obterCaixaPorId(sistema, idCaixa);
    if (caixaOrigem == NULL) {
        return 0;
    }

    while (!filaEstaVazia(&caixaOrigem->fila)) {
        cliente = desenfileirarCliente(&caixaOrigem->fila);
        if (cliente == NULL) {
            continue;
        }

        cliente->idCaixaAtual = ID_CAIXA_INVALIDO;

        if (!encaminharClienteParaMelhorCaixa(sistema, cliente)) {
            enfileirarCliente(&caixaOrigem->fila, cliente);
            break;
        }

        totalRedistribuido++;
    }

    atualizarTempoEstimadoCaixa(caixaOrigem);
    return totalRedistribuido;
}

int iniciarAtendimentoSeNecessario(SISTEMA *sistema, CAIXA *caixa) {
    CLIENTE *cliente;

    if (sistema == NULL || caixa == NULL) {
        return 0;
    }

    if (caixa->clienteAtual != NULL) {
        return 0;
    }

    if (filaEstaVazia(&caixa->fila)) {
        if (caixaEstaEmEncerramento(caixa)) {
            fecharCaixa(sistema, caixa->id, 0);
        }
        atualizarTempoEstimadoCaixa(caixa);
        return 0;
    }

    if (!caixaEstaAberta(caixa) && !caixaEstaEmEncerramento(caixa)) {
        return 0;
    }

    cliente = desenfileirarCliente(&caixa->fila);
    if (cliente == NULL) {
        atualizarTempoEstimadoCaixa(caixa);
        return 0;
    }

    caixa->clienteAtual = cliente;
    cliente->idCaixaAtual = caixa->id;
    iniciarAtendimentoCliente(cliente, sistema->tempoAtual);

    atualizarTempoEstimadoCaixa(caixa);
    return 1;
}

int finalizarAtendimentoSeConcluido(SISTEMA *sistema, CAIXA *caixa) {
    CLIENTE *cliente;

    if (sistema == NULL || caixa == NULL || caixa->clienteAtual == NULL) {
        return 0;
    }

    cliente = caixa->clienteAtual;

    if (!clienteTerminouAtendimento(cliente)) {
        atualizarTempoEstimadoCaixa(caixa);
        return 0;
    }

    finalizarAtendimentoCliente(cliente, sistema->tempoAtual);

    caixa->clientesAtendidos++;
    caixa->totalProdutosVendidos += cliente->nProdutos;
    caixa->totalValorVendido += cliente->valorTotalCompras;

    if (clienteRecebeuOferta(cliente)) {
        caixa->totalProdutosOferecidos++;
        caixa->totalValorOferecido += cliente->valorOferta;
        registarProdutoOferecido(&sistema->estatisticas, cliente->valorOferta);
    }

    adicionarClienteAoHistoricoCaixa(caixa, cliente);

    registarClienteAtendido(&sistema->estatisticas);
    registarProdutoVendido(&sistema->estatisticas, cliente->nProdutos, cliente->valorTotalCompras);
    atualizarTempoEspera(&sistema->estatisticas,
                         calcularTempoEsperaCliente(cliente, cliente->instanteInicioAtendimento));

    if (caixa->operador != NULL) {
        incrementarClientesAtendidosColaborador(caixa->operador);
    }

    removerClienteHash(&sistema->clientesHash, cliente->id);

    caixa->clienteAtual = NULL;
    atualizarTempoEstimadoCaixa(caixa);

    if (caixaEstaEmEncerramento(caixa) && !caixaTemClientes(caixa)) {
        fecharCaixa(sistema, caixa->id, 0);
    }

    return 1;
}

int calcularTempoRestanteClienteAtual(const CAIXA *caixa) {
    if (caixa == NULL || caixa->clienteAtual == NULL) {
        return 0;
    }

    if (caixa->clienteAtual->tempoRestanteAtendimento > 0) {
        return caixa->clienteAtual->tempoRestanteAtendimento;
    }

    return 0;
}

int calcularTempoEstimadoCaixa(const CAIXA *caixa) {
    ELEMENTO *atual;
    int tempoTotal = 0;

    if (caixa == NULL) {
        return 0;
    }

    tempoTotal += calcularTempoRestanteClienteAtual(caixa);

    atual = caixa->fila.inicio;
    while (atual != NULL) {
        if (atual->cliente != NULL) {
            tempoTotal += atual->cliente->tempoTotalPagamento;
        }
        atual = atual->seguinte;
    }

    return tempoTotal;
}

int obterNumeroClientesCaixa(const CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return obterTamanhoFila(&caixa->fila) + (caixa->clienteAtual != NULL ? 1 : 0);
}

void atualizarTempoEstimadoCaixa(CAIXA *caixa) {
    if (caixa == NULL) {
        return;
    }

    caixa->tempoTotalEstimadoFila = calcularTempoEstimadoCaixa(caixa);
}

void adicionarClienteAoHistoricoCaixa(CAIXA *caixa, CLIENTE *cliente) {
    NO_HISTORICO_CLIENTE *novoNo;

    if (caixa == NULL || cliente == NULL) {
        return;
    }

    novoNo = (NO_HISTORICO_CLIENTE *)malloc(sizeof(NO_HISTORICO_CLIENTE));
    if (novoNo == NULL) {
        return;
    }

    novoNo->cliente = cliente;
    novoNo->seguinte = NULL;

    if (caixa->historicoClientes.fim == NULL) {
        caixa->historicoClientes.inicio = novoNo;
        caixa->historicoClientes.fim = novoNo;
    } else {
        caixa->historicoClientes.fim->seguinte = novoNo;
        caixa->historicoClientes.fim = novoNo;
    }

    caixa->historicoClientes.tamanho++;
}

void ativarOperadorDaCaixa(SISTEMA *sistema, int idCaixa) {
    CAIXA *caixa;
    COLABORADOR *colaborador;

    if (sistema == NULL) {
        return;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return;
    }

    colaborador = obterColaboradorPorCaixa(sistema, idCaixa);
    if (colaborador == NULL) {
        return;
    }

    ativarColaborador(colaborador, idCaixa);
    caixa->operador = colaborador;
}

void desativarOperadorDaCaixa(SISTEMA *sistema, int idCaixa) {
    CAIXA *caixa;
    COLABORADOR *colaborador;

    if (sistema == NULL) {
        return;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return;
    }

    colaborador = obterColaboradorPorCaixa(sistema, idCaixa);
    if (colaborador != NULL) {
        desativarColaborador(colaborador);
    }

    caixa->operador = colaborador;
}
