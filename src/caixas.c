#include <stdio.h>
#include <stdlib.h>
#include "caixas.h"
#include "clientes.h"
#include "filas.h"
#include "colaboradores.h"
#include "estatisticas.h"
#include "hashClientes.h"

// Inicializa uma caixa com estado fechado, controlo automático e todos os dados internos zerados
void inicializarCaixa(CAIXA *caixa, int idCaixa) {
    if (caixa == NULL) return;

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
// Aloca e inicializa todas as caixas do sistema, abrindo automaticamente um número inicial definido
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
// Retorna um ponteiro para a caixa com o id indicado, validando limites e existência
CAIXA *obterCaixaPorId(SISTEMA *sistema, int idCaixa) {
    if (sistema == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    if (idCaixa < 0 || idCaixa >= sistema->config.N_CAIXAS) {
        return NULL;
    }

    return &sistema->caixas[idCaixa];
}
// Retorna um ponteiro para a caixa com o id indicado, validando limites e existência
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
// Retorna a caixa disponível com menor tempo estimado de espera, priorizando o menor id em caso de empate
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
// Conta e retorna o número de caixas atualmente abertas no sistema
int contarCaixasAbertas(SISTEMA *sistema) {
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
// Conta e retorna o número de caixas que estão em processo de encerramento
int contarCaixasEmEncerramento(SISTEMA *sistema) {
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
// Verifica se a caixa está aberta, retornando 1 se sim ou 0 caso contrário
int caixaEstaAberta(CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_ABERTA;
}
// Verifica se a caixa está fechada, retornando 1 se sim ou 0 caso contrário
int caixaEstaFechada(CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_FECHADA;
}
// Verifica se a caixa está em processo de encerramento, retornando 1 se sim ou 0 caso contrário
int caixaEstaEmEncerramento(CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_EM_ENCERRAMENTO;
}
// Verifica se a caixa pode aceitar novos clientes (apenas quando está aberta)
int caixaAceitaNovosClientes(CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->estado == CAIXA_ABERTA;
}
// Verifica se a caixa possui clientes, seja em atendimento ou na fila
int caixaTemClientes(CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return caixa->clienteAtual != NULL || !filaEstaVazia(&caixa->fila);
}
// Abre a caixa, respeitando o controlo manual e impedindo ações automáticas se estiver bloqueada pelo gerente
int abrirCaixa(SISTEMA *sistema, int idCaixa, int controloManual) {
    CAIXA *caixa;

    if (sistema == NULL) {
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return 0;
    }

    if (controloManual) {
        caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
        caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
    } else {
        if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
            return 0;
        }
    }

    if (caixaEstaAberta(caixa)) {
        return 1;
    }

    if (caixaEstaEmEncerramento(caixa) && caixaTemClientes(caixa)) {
        caixa->estado = CAIXA_ABERTA;
    } else {
        caixa->estado = CAIXA_ABERTA;
    }

    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);
    ativarOperadorDaCaixa(sistema, idCaixa);
    atualizarTempoEstimadoCaixa(caixa);

    return 1;
}
// Coloca a caixa em encerramento (ou fecha diretamente), respeitando o controlo manual e redistribuindo clientes se necessário
int encerrarCaixa(SISTEMA *sistema, int idCaixa, int controloManual) {
    CAIXA *caixa;

    if (sistema == NULL) {
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return 0;
    }

    if (controloManual) {
        caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
        caixa->bloqueadaAutomaticamente = CAIXA_BLOQUEADA_AUTOMATICAMENTE;
    } else if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
        return 0;
    } else {
        caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
        caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
    }

    if (caixaEstaFechada(caixa)) {
        caixa->estado = CAIXA_FECHADA;
        caixa->tempoTotalEstimadoFila = 0;
        sistema->nCaixasAbertas = contarCaixasAbertas(sistema);
        desativarOperadorDaCaixa(sistema, idCaixa);
        return 1;
    }

    if (caixaEstaEmEncerramento(caixa)) {
        return 1;
    }

    caixa->estado = CAIXA_EM_ENCERRAMENTO;

    if (controloManual) {
        redistribuirClientesCaixaFechada(sistema, idCaixa);
    }

    atualizarTempoEstimadoCaixa(caixa);
    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);

    if (!caixaTemClientes(caixa)) {
        return fecharCaixa(sistema, idCaixa, controloManual);
    }

    return 1;
}
// Fecha a caixa definitivamente, respeitando controlo manual e redistribuindo clientes antes do encerramento
int fecharCaixa(SISTEMA *sistema, int idCaixa, int controloManual) {
    CAIXA *caixa;

    if (sistema == NULL) {
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        return 0;
    }

    if (controloManual) {
        caixa->controloManualGerente = CAIXA_COM_CONTROLO_MANUAL;
        caixa->bloqueadaAutomaticamente = CAIXA_BLOQUEADA_AUTOMATICAMENTE;
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

    if (!controloManual && caixa->controloManualGerente != CAIXA_COM_CONTROLO_MANUAL) {
        caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
        caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
    }

    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);
    desativarOperadorDaCaixa(sistema, idCaixa);

    return 1;
}
// Coloca a caixa em modo automático, removendo o controlo manual e permitindo gestão automática
int colocarCaixaEmAuto(SISTEMA *sistema, int idCaixa) {
    CAIXA *caixa;

    if (sistema == NULL) {
        printf("Sistema invalido.\n");
        return 0;
    }

    if (idCaixa < 0 || idCaixa >= sistema->config.N_CAIXAS) {
        printf("Id de caixa invalido.\n");
        return 0;
    }

    caixa = obterCaixaPorId(sistema, idCaixa);
    if (caixa == NULL) {
        printf("Caixa nao encontrada.\n");
        return 0;
    }

    if (caixa->controloManualGerente == CAIXA_SEM_CONTROLO_MANUAL) {
        printf("A caixa %d ja esta em modo automatico.\n", idCaixa);
        return 1;
    }

    caixa->controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
    caixa->bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;

    printf("Caixa %d agora esta em modo automatico.\n", idCaixa);

    return 1;
}
// Encaminha o cliente para a melhor caixa disponível (menor tempo estimado ou primeira aberta)
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
// Adiciona um cliente à fila da caixa, atualizando seu estado e o tempo estimado de espera
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
// Redistribui os clientes da fila de uma caixa para outras disponíveis, ao fechá-la
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
// Inicia o atendimento do próximo cliente na fila se a caixa estiver disponível, fechando-a se necessário
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
// Finaliza o atendimento de um cliente concluído, atualizando estatísticas e fechando a caixa se necessário
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
// Retorna o tempo restante de atendimento do cliente atual da caixa, ou 0 se não houver ou já terminou
int calcularTempoRestanteClienteAtual(CAIXA *caixa) {
    if (caixa == NULL || caixa->clienteAtual == NULL) {
        return 0;
    }

    if (caixa->clienteAtual->tempoRestanteAtendimento > 0) {
        return caixa->clienteAtual->tempoRestanteAtendimento;
    }

    return 0;
}
// Calcula o tempo total estimado de espera na caixa somando o atendimento atual e os pagamentos dos clientes na fila
int calcularTempoEstimadoCaixa(CAIXA *caixa) {
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
// Retorna o número total de clientes na caixa, incluindo o cliente em atendimento e os que estão na fila
int obterNumeroClientesCaixa(CAIXA *caixa) {
    if (caixa == NULL) {
        return 0;
    }

    return obterTamanhoFila(&caixa->fila) + (caixa->clienteAtual != NULL ? 1 : 0);
}
// Atualiza o tempo total estimado da fila da caixa com base no estado atual
void atualizarTempoEstimadoCaixa(CAIXA *caixa) {
    if (caixa == NULL) {
        return;
    }

    caixa->tempoTotalEstimadoFila = calcularTempoEstimadoCaixa(caixa);
}
// Adiciona um cliente ao histórico da caixa, mantendo a lista encadeada de atendimentos
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
// Associa e ativa um colaborador na caixa, tornando-o responsável pelo atendimento
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
// Desativa o colaborador associado à caixa, removendo-o do atendimento
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
