#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "simulacao.h"
#include "clientes.h"
#include "caixas.h"
#include "filas.h"
#include "listaDeCompras.h"
#include "estatisticas.h"
#include "historico.h"
#include "define.h"
#include "uteis.h"
#include "hashClientes.h"

// Inicializa a simulação, resetando tempo, estado, estatísticas e registando o início no log
void inicializarSimulacao(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    sistema->tempoAtual = 0;
    sistema->estadoSimulacao = SIMULACAO_PARADA;
    sistema->proximoIdClienteGerado = 1;
    sistema->nCaixasAbertas = contarCaixasAbertas(sistema);

    inicializarEstatisticas(&sistema->estatisticas);

    adicionarLog(&sistema->logs,sistema->tempoAtual,"SIMULACAO","Simulacao preparada");
}
// Executa o ciclo principal da simulação, processando eventos, atualizando a interface e controlando pausa e velocidade
// estrutura principal de execução da simulação.
// A função executarSimulacao separa a lógica da simulação da renderização da interface.
// A lógica é executada a cada ciclo, mas a interface só é atualizada a cada 60 ciclos.
void executarSimulacao(SISTEMA *sistema) {
    int passoLoading = 0;
    int estavaPausada;

    if (sistema == NULL) {
        return;
    }

    if (sistema->estadoSimulacao == SIMULACAO_ENCERRADA) {
        return;
    }

    estavaPausada = (sistema->estadoSimulacao == SIMULACAO_PAUSADA);

    sistema->estadoSimulacao = SIMULACAO_ATIVA;
    sistema->ciclosDesdeUltimoRefresh = 0;

    if (!estavaPausada) {
        adicionarLog(&sistema->logs,sistema->tempoAtual,"SIMULACAO","Simulacao iniciada");
    }

    mostrarPainelSimulacao(sistema);
    printf("\nLoading: %c", obterCharLoading(passoLoading));
    printf("\nPrima 'P' para pausar e voltar ao menu.\n");

    while (sistema->estadoSimulacao == SIMULACAO_ATIVA) {
        cicloSimulacao(sistema);
        sistema->ciclosDesdeUltimoRefresh++;

        if (sistema->ciclosDesdeUltimoRefresh >= CICLOS_POR_REFRESH_TELA) {
            mostrarPainelSimulacao(sistema);

            passoLoading++;
            if (passoLoading >= 4) {
                passoLoading = 0;
            }

            printf("\nLoading: %c", obterCharLoading(passoLoading));
            printf("\nPrima 'P' para pausar e voltar ao menu.\n");

            sistema->ciclosDesdeUltimoRefresh = 0;
        }

        processarTeclaSimulacao(sistema);
        //A velocidade da simulação não altera a lógica nem o tempo simulado, apenas o tempo real de execução, sendo controlada através do delay (Sleep) entre ciclos.
        Sleep(obterDelayPorVelocidade(sistema->velocidadeSimulacao));
    }
}
// Executa um ciclo da simulação, atualizando clientes, caixas, eventos e avançando o tempo do sistema
// coração da simulação
// A simulação é baseada em ciclos discretos de tempo.
// Cada execução da função cicloSimulacao representa 1 minuto de tempo simulado.
void cicloSimulacao(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    if (sistema->estadoSimulacao != SIMULACAO_ATIVA) {
        return;
    }

    atualizarClientesEmComprasSimulacao(sistema);
    moverClientesParaFilas(sistema);
    atualizarCaixasSimulacao(sistema);
    finalizarAtendimentosSimulacao(sistema);
    iniciarNovosAtendimentosSimulacao(sistema);
    aplicarOfertasSimulacao(sistema);
    avaliarMudancasFilaSimulacao(sistema);
    {
        int caixasAbertasAntes = contarCaixasAbertas(sistema);

        verificarAberturaCaixasSimulacao(sistema);
        reforcarCaixasPosFecho(sistema);

        if (contarCaixasAbertas(sistema) == caixasAbertasAntes &&
            contarCaixasEmEncerramento(sistema) == 0) {
            verificarEncerramentoCaixasSimulacao(sistema);
        }
    }
    gerarNovosClientesSimulacao(sistema);
    atualizarEstatisticasSimulacao(sistema);

    sistema->tempoAtual += CICLO_SIMULACAO_MINUTOS;
}
// Atualiza o estado dos clientes em compras durante a simulação
void atualizarClientesEmComprasSimulacao(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    atualizarClientesEmCompras(&sistema->clientesComprando, sistema->tempoAtual);
}
// Move clientes que terminaram as compras para as filas das caixas durante a simulação
void moverClientesParaFilas(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    processarClientesTerminadosEmCompras(sistema);
}
// Atualiza o tempo de atendimento dos clientes atualmente em atendimento em todas as caixas
void atualizarCaixasSimulacao(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];

        if (caixa->clienteAtual != NULL) {
            atualizarTempoAtendimentoCliente(caixa->clienteAtual);
        }
    }
}
// Finaliza os atendimentos concluídos nas caixas durante a simulação
void finalizarAtendimentosSimulacao(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        finalizarAtendimentoSeConcluido(sistema, &sistema->caixas[i]);
    }
}
// Inicia novos atendimentos nas caixas caso existam clientes na fila
void iniciarNovosAtendimentosSimulacao(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        iniciarAtendimentoSeNecessario(sistema, &sistema->caixas[i]);
    }
}
// Aplica ofertas aos clientes em atendimento que cumprem os critérios definidos
void aplicarOfertasSimulacao(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CLIENTE *clienteAtual = sistema->caixas[i].clienteAtual;

        if (clienteAtual == NULL) {
            continue;
        }

        if (clienteTemDireitoAOferta(sistema, clienteAtual, clienteAtual->instanteInicioAtendimento)) {
            aplicarOfertaCliente(clienteAtual, sistema);
        }
    }
}
// Avalia e executa possíveis mudanças de fila entre caixas durante a simulação
void avaliarMudancasFilaSimulacao(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixaOrigem = &sistema->caixas[i];
        CAIXA *caixaDestino;
        int posicao;

        if (!caixaEstaAberta(caixaOrigem)) {
            continue;
        }

        if (obterTamanhoFila(&caixaOrigem->fila) < 3) {
            continue;
        }

        caixaDestino = obterCaixaDestinoMudanca(sistema, caixaOrigem);
        if (caixaDestino == NULL) {
            continue;
        }

        for (posicao = 3; posicao <= obterTamanhoFila(&caixaOrigem->fila); posicao++) {
            CLIENTE *cliente = obterClienteNaPosicaoFila(&caixaOrigem->fila, posicao);

            if (cliente == NULL) {
                continue;
            }

            if (clienteJaMudouDeFila(cliente)) {
                continue;
            }

            if (tentarMoverClienteDeFila(sistema, caixaOrigem, cliente)) {
                break;
            }
        }
    }
}
// Verifica condições para abertura automática de caixas e abre uma nova caixa se necessário
void verificarAberturaCaixasSimulacao(SISTEMA *sistema) {
    CAIXA *caixa;
    int i;
    int totalClientesFila = 0;
    int caixasAbertas = 0;
    float mediaClientesPorFila;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        if (!caixaEstaAberta(&sistema->caixas[i])) {
            continue;
        }

        totalClientesFila += obterTamanhoFila(&sistema->caixas[i].fila);
        caixasAbertas++;
    }

    if (caixasAbertas <= 0) {
        return;
    }

    mediaClientesPorFila = (float)totalClientesFila / caixasAbertas;

    if (mediaClientesPorFila <= sistema->config.MAX_FILA) {
        return;
    }

    caixa = obterCaixaParaAberturaAutomatica(sistema);
    if (caixa == NULL) {
        return;
    }

    if (abrirCaixa(sistema, caixa->id, CAIXA_SEM_CONTROLO_MANUAL)) {
        registarAberturaAutomatica(&sistema->estatisticas);
    }
}
// Verifica condições para encerramento automático de caixas e encerra uma caixa se aplicável
void verificarEncerramentoCaixasSimulacao(SISTEMA *sistema) {
    CAIXA *caixa;
    int clientesNasCaixas;
    int caixasOperacionais;
    float mediaClientesPorCaixa;

    if (sistema == NULL) {
        return;
    }

    if (contarCaixasAbertas(sistema) <= 1) {
        return;
    }

    if (contarCaixasEmEncerramento(sistema) > 0) {
        return;
    }

    clientesNasCaixas = contarClientesNasCaixas(sistema);
    caixasOperacionais = contarCaixasOperacionais(sistema);

    if (caixasOperacionais <= 0) {
        return;
    }

    mediaClientesPorCaixa = (float)clientesNasCaixas / caixasOperacionais;

    if (mediaClientesPorCaixa >= sistema->config.MIN_FILA) {
        return;
    }

    caixa = obterCaixaParaEncerramentoAutomatico(sistema);
    if (caixa == NULL) {
        return;
    }

    if (encerrarCaixa(sistema, caixa->id, CAIXA_SEM_CONTROLO_MANUAL)) {
        registarEncerramentoAutomatico(&sistema->estatisticas);
    }
}
// Gera novos clientes durante a simulação e contabiliza quantos foram criados
void gerarNovosClientesSimulacao(SISTEMA *sistema) {
    int totalGerados;
    int horaAtual;

    if (sistema == NULL) {
        return;
    }

    horaAtual = (sistema->tempoAtual / 60) % 24;

    if (horaAtual < HORARIO_ABERTURA || horaAtual >= HORARIO_FECHO) {
        return;
    }

    totalGerados = gerarNovosClientes(sistema);
}
// Atualiza as estatísticas globais da simulação com base no estado atual do sistema
void atualizarEstatisticasSimulacao(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    sistema->estatisticas.tempoSimulacao = sistema->tempoAtual;
    calcularTempoMedioEspera(&sistema->estatisticas);
    atualizarEstatisticasCaixas(sistema);
    determinarOperadorMenosAtendimentos(sistema);
    determinarOperadorMaisAtendimentos(sistema);
}
// Coloca a simulação em estado de pausa e regista o evento no log
void pausarSimulacao(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }
    sistema->estadoSimulacao = SIMULACAO_PAUSADA;
    adicionarLog(&sistema->logs,sistema->tempoAtual,"SIMULACAO","Simulacao pausada");
}
// Retoma a simulação e regista o evento no log do sistema
void retomarSimulacao(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }
    adicionarLog(&sistema->logs,sistema->tempoAtual,"SIMULACAO","Simulacao retomada");
}
// Encerra a simulação, atualiza o estado do sistema e regista o evento no log
void encerrarSimulacao(SISTEMA *sistema) {
    char confirmacao;
    if (sistema == NULL) {
        return;
    }

    printf("\nTem a certeza que deseja encerrar a simulacao? (S/N): ");
    scanf(" %c", &confirmacao);

    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Encerramento cancelado.\n");
        return;
    }

    sistema->estadoSimulacao = SIMULACAO_ENCERRADA;
    adicionarLog(&sistema->logs,sistema->tempoAtual,"SIMULACAO","Simulacao encerrada");
    printf("Simulacao encerrada com sucesso.\n");
}
// Processa clientes que terminaram as compras, removendo-os da lista e encaminhando-os para as caixas
void processarClientesTerminadosEmCompras(SISTEMA *sistema) {
    CLIENTE *cliente;

    if (sistema == NULL) {
        return;
    }

    cliente = obterPrimeiroClienteListaCompras(&sistema->clientesComprando);

    while (cliente != NULL && clienteTerminouComprasLista(cliente, sistema->tempoAtual)) {
        cliente = removerPrimeiroClienteListaCompras(&sistema->clientesComprando);

        if (cliente == NULL) {
            break;
        }

        if (!encaminharClienteParaMelhorCaixa(sistema, cliente)) {
            inserirClienteOrdenadoListaCompras(&sistema->clientesComprando, cliente);
            break;
        }

        cliente = obterPrimeiroClienteListaCompras(&sistema->clientesComprando);
    }
}
// Determina a melhor caixa de destino para mudança de fila com base no tempo estimado de atendimento
CAIXA *obterCaixaDestinoMudanca(SISTEMA *sistema, CAIXA *caixaAtual) {
    int i;
    CAIXA *melhorCaixa = NULL;
    int tempoAtualCaixa;
    int melhorTempo = 0;

    if (sistema == NULL || caixaAtual == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    tempoAtualCaixa = calcularTempoEstimadoCaixa(caixaAtual);

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];
        int tempoCaixa;

        if (caixa->id == caixaAtual->id) {
            continue;
        }

        if (!caixaAceitaNovosClientes(caixa)) {
            continue;
        }

        tempoCaixa = calcularTempoEstimadoCaixa(caixa);

        if (tempoAtualCaixa - tempoCaixa <= sistema->config.DIFERENCA_TEMPO_CAIXAS) {
            continue;
        }

        if (melhorCaixa == NULL || tempoCaixa < melhorTempo) {
            melhorCaixa = caixa;
            melhorTempo = tempoCaixa;
        }
    }

    return melhorCaixa;
}
// Retorna o cliente na posição indicada da fila, ou NULL se a posição não existir
CLIENTE *obterClienteNaPosicaoFila(FILA *fila, int posicao) {
    ELEMENTO *atual;
    int indice = 1;

    if (fila == NULL || posicao <= 0) {
        return NULL;
    }

    atual = fila->inicio;

    while (atual != NULL) {
        if (indice == posicao) {
            return atual->cliente;
        }

        atual = atual->seguinte;
        indice++;
    }

    return NULL;
}
// Tenta mover um cliente de uma fila para outra caixa mais adequada, revertendo a operação em caso de falha
int tentarMoverClienteDeFila(SISTEMA *sistema, CAIXA *caixaOrigem, CLIENTE *cliente) {
    CAIXA *caixaDestino;

    if (sistema == NULL || caixaOrigem == NULL || cliente == NULL) {
        return 0;
    }

    caixaDestino = obterCaixaDestinoMudanca(sistema, caixaOrigem);
    if (caixaDestino == NULL) {
        return 0;
    }

    if (!removerClienteDaFilaPorId(&caixaOrigem->fila, cliente->id)) {
        return 0;
    }

    if (!adicionarClienteNaCaixa(caixaDestino, cliente, sistema->tempoAtual)) {
        enfileirarCliente(&caixaOrigem->fila, cliente);
        return 0;
    }

    marcarClienteMudouDeFila(cliente);
    registarMudancaFila(&sistema->estatisticas);

    return 1;
}
// Procura uma caixa elegível para abertura automática, respeitando regras de estado e controlo manual
CAIXA *obterCaixaParaAberturaAutomatica(SISTEMA *sistema) {
    int i;

    if (sistema == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];

        if (!caixaEstaFechada(caixa)) {
            continue;
        }

        if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
            continue;
        }

        if (caixa->bloqueadaAutomaticamente == CAIXA_BLOQUEADA_AUTOMATICAMENTE) {
            continue;
        }

        return caixa;
    }

    return NULL;
}
// Seleciona a caixa mais adequada para encerramento automático com base na menor quantidade de clientes
CAIXA *obterCaixaParaEncerramentoAutomatico(SISTEMA *sistema) {
    int i;
    CAIXA *melhor = NULL;
    int menorQuantidade = 0;

    if (sistema == NULL || sistema->caixas == NULL) {
        return NULL;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];
        int quantidade;

        if (!caixaEstaAberta(caixa)) {
            continue;
        }

        if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
            continue;
        }

        quantidade = obterNumeroClientesCaixa(caixa);

        if (melhor == NULL || quantidade < menorQuantidade) {
            melhor = caixa;
            menorQuantidade = quantidade;
        }
    }

    return melhor;
}
// Exibe o painel principal da simulação com estado geral, estatísticas e informação detalhada das caixas
void mostrarPainelSimulacao(SISTEMA *sistema) {
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

    printf("\nESTADO GERAL DO SISTEMA\n");
    printf("%s", LINHA_SEPARADORA);

   
    int horaAtual = (sistema->tempoAtual / 60) % 24;

    printf("Mercado:              %s\n",(horaAtual >= HORARIO_ABERTURA && horaAtual < HORARIO_FECHO) ? "ABERTO" : "FECHADO");
    printf("%s", LINHA_SEPARADORA);
    printf("Velocidade:              %dx\n", sistema->velocidadeSimulacao);
    printf("Estado da simulacao:     %s\n",obterTextoEstadoSimulacao(sistema->estadoSimulacao));
    printf("Clientes em compras:     %d\n", sistema->clientesComprando.tamanho);
    printf("Clientes ativos (hash):  %d\n", sistema->clientesHash.nElementos);
    printf("Caixas abertas:          %d\n", sistema->nCaixasAbertas);

    printf("\nESTATISTICAS\n");
    printf("%s", LINHA_SEPARADORA);

    printf("Clientes gerados:        %d\n", sistema->estatisticas.totalClientesGerados);
    printf("Clientes atendidos:      %d\n", sistema->estatisticas.totalClientesAtendidos);
    printf("Produtos oferecidos:     %d\n", sistema->estatisticas.totalProdutosOferecidos);
    printf("Tempo medio de espera:   %.2f min\n", sistema->estatisticas.tempoMedioEspera);
    printf("Clientes nas caixas:     %d\n", contarClientesNasCaixas(sistema));

    printf("\nESTADO DAS CAIXAS\n");
    printf("%s", LINHA_SEPARADORA);

    if (sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];

        printf(
            "Caixa %d | Estado=%-14s | Abertura=%-7s | Fila=%d | EmAtendimento=%d | Total=%d | Atendidos=%d\n",
            caixa->id,
            obterTextoEstadoCaixa(caixa->estado),
            obterTextoControloCaixa(caixa),
            caixa->fila.tamanho,
            caixa->clienteAtual != NULL ? 1 : 0,
            caixa->fila.tamanho + (caixa->clienteAtual != NULL ? 1 : 0),
            caixa->clientesAtendidos
        );
    }
}
// Converte o estado da simulação para texto,
// permitindo apresentar estados legíveis ao utilizador.
char *obterTextoEstadoSimulacao(ESTADO_SIMULACAO estado) {

    switch (estado) {

        case SIMULACAO_PARADA:
            return "PARADA";

        case SIMULACAO_ATIVA:
            return "ATIVA";

        case SIMULACAO_PAUSADA:
            return "PAUSADA";

        case SIMULACAO_ENCERRADA:
            return "ENCERRADA";

        default:
            return "DESCONHECIDO";
    }
}
// Converte o estado de uma caixa para texto,
// facilitando a apresentação legível do estado ao utilizador.
char *obterTextoEstadoCaixa(ESTADO_CAIXA estado) {
    switch (estado) {
        case CAIXA_FECHADA: return "FECHADA";
        case CAIXA_ABERTA: return "ABERTA";
        case CAIXA_EM_ENCERRAMENTO: return "ENCERRAMENTO";
        default: return "DESCONHECIDO";
    }
}
// Lê entrada do teclado durante a simulação e pausa a execução caso a tecla 'P' seja pressionada
void processarTeclaSimulacao(SISTEMA *sistema) {
    char tecla;

    if (sistema == NULL) {
        return;
    }

    // _kbhit(): retorna 1 se uma tecla foi pressionada, 0 caso contrário
    // evita bloquear a execução esperando input
    if (!_kbhit()) {
        return;
    }
    // lê a tecla pressionada sem necessidade de ENTER
    tecla = (char)_getch();

    if (tecla == 'p' || tecla == 'P') {
        pausarSimulacao(sistema);
    }
}
// Retorna o caractere usado para animar o indicador de loading da simulação
char obterCharLoading(int passo) {
    switch (passo) {
        case 0: return '|';
        case 1: return '/';
        case 2: return '-';
        case 3: return '\\';
        default: return '|';
    }
}
// Calcula o atraso (delay) da simulação com base na velocidade, garantindo um valor mínimo de 1 ms
int obterDelayPorVelocidade(int velocidade) {
    int delay;

    if (velocidade <= 0) {
        return SLEEP_SIMULACAO_BASE_MS;
    }

    delay = SLEEP_SIMULACAO_BASE_MS / velocidade;

    if (delay < 1) {
        delay = 1;
    }

    return delay;
}
// Reinicializa completamente o estado da simulação, limpando clientes, filas, caixas e estatísticas
void reinicializarEstadoSimulacao(SISTEMA *sistema) {
    int i;

    if (sistema == NULL) {
        return;
    }

    for (i = 0; i < sistema->clientesHash.nBuckets; i++) {
    HASHNODE *noAtual = sistema->clientesHash.buckets[i].clientes;

    while (noAtual != NULL) {
        if (noAtual->cliente != NULL) {
            libertarCliente(noAtual->cliente);
            noAtual->cliente = NULL;
        }

        noAtual = noAtual->prox;
    }
}

    libertarHash(&sistema->clientesHash);
    inicializarHash(&sistema->clientesHash, HASH_N_BUCKETS);

    libertarListaClientesComprando(&sistema->clientesComprando);
    inicializarListaClientesComprando(&sistema->clientesComprando);

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            libertarFila(&sistema->caixas[i].fila);
            inicializarFila(&sistema->caixas[i].fila);

            sistema->caixas[i].clienteAtual = NULL;
            sistema->caixas[i].tempoTotalEstimadoFila = 0;
            sistema->caixas[i].clientesAtendidos = 0;
            sistema->caixas[i].totalProdutosVendidos = 0;
            sistema->caixas[i].totalValorVendido = 0.0f;
            sistema->caixas[i].totalProdutosOferecidos = 0;
            sistema->caixas[i].totalValorOferecido = 0.0f;
            sistema->caixas[i].controloManualGerente = CAIXA_SEM_CONTROLO_MANUAL;
            sistema->caixas[i].bloqueadaAutomaticamente = CAIXA_NAO_BLOQUEADA_AUTOMATICAMENTE;
            sistema->caixas[i].estado = CAIXA_FECHADA;

            while (sistema->caixas[i].historicoClientes.inicio != NULL) {
                NO_HISTORICO_CLIENTE *tmp = sistema->caixas[i].historicoClientes.inicio;
                sistema->caixas[i].historicoClientes.inicio = tmp->seguinte;

                if (tmp->cliente != NULL) {
                    libertarCliente(tmp->cliente);
                }

                free(tmp);
            }

            sistema->caixas[i].historicoClientes.fim = NULL;
            sistema->caixas[i].historicoClientes.tamanho = 0;
        }
    }

    sistema->nCaixasAbertas = 0;

    for (i = 0; i < N_CAIXAS_INICIALMENTE_ABERTAS && i < sistema->config.N_CAIXAS; i++) {
        sistema->caixas[i].estado = CAIXA_ABERTA;
        sistema->nCaixasAbertas++;
        ativarOperadorDaCaixa(sistema, i);
    }

    for (; i < sistema->config.N_CAIXAS; i++) {
        desativarOperadorDaCaixa(sistema, i);
    }

    sistema->tempoAtual = 0;
    sistema->estadoSimulacao = SIMULACAO_PARADA;
    sistema->proximoIdClienteGerado = 1;
    sistema->ciclosDesdeUltimoRefresh = 0;

    inicializarEstatisticas(&sistema->estatisticas);
    adicionarLog(&sistema->logs, 0, "SIMULACAO", "Simulacao reinicializada");
}