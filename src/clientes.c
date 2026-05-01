#include "clientes.h"
#include "produtos.h"
#include "hashClientes.h"
#include "listaDeCompras.h"
#include "estatisticas.h"
#include "uteis.h"

// Cria e inicializa um novo cliente, alocando memória e preparando seus dados
CLIENTE *criarCliente(SISTEMA *sistema) {
    CLIENTE *cliente;

    if (sistema == NULL) {
        return NULL;
    }

    cliente = (CLIENTE *)malloc(sizeof(CLIENTE));
    if (cliente == NULL) {
        return NULL;
    }

    inicializarCliente(cliente, sistema);
    return cliente;
}
// Inicializa os dados básicos de um cliente, atribuindo id, estado inicial e instante de entrada no sistema
void inicializarCliente(CLIENTE *cliente, SISTEMA *sistema) {
    if (cliente == NULL || sistema == NULL) {
        return;
    }

    limparCamposCliente(cliente);

    cliente->id = sistema->proximoIdClienteGerado++;
    cliente->estado = CLIENTE_A_COMPRAR;
    cliente->idCaixaAtual = ID_CAIXA_INVALIDO;
    cliente->instanteEntradaSistema = sistema->tempoAtual;
}
// Gera novos clientes aleatoriamente no sistema, inserindo-os nas estruturas e atualizando estatísticas
int gerarNovosClientes(SISTEMA *sistema) {
    int quantidade;
    int i;
    int totalGerados = 0;

    if (sistema == NULL) {
        return 0;
    }

    if (sistema->clientesHash.nElementos >= MAX_CLIENTES_MERCADO) {
        return 0;
    }

    if (Aleatorio(1, 100) > 25) {
        return 0;
    }

    quantidade = obterQuantidadeNovosClientesNoCiclo(sistema);

    for (i = 0; i < quantidade; i++) {
        CLIENTE *cliente;

        if (sistema->clientesHash.nElementos >= MAX_CLIENTES_MERCADO) {
            break;
        }

        cliente = gerarClienteAleatorio(sistema);

        if (cliente == NULL) {
            continue;
        }

        if (!inserirClienteHash(&sistema->clientesHash, cliente)) {
            libertarCliente(cliente);
            continue;
        }

        if (!inserirClienteOrdenadoListaCompras(&sistema->clientesComprando, cliente)) {
            removerClienteHash(&sistema->clientesHash, cliente->id);
            libertarCliente(cliente);
            continue;
        }

        registarClienteGerado(&sistema->estatisticas);
        totalGerados++;
    }

    return totalGerados;
}
// Gera um cliente aleatório a partir da base, atribuindo produtos e calculando tempos de compras e atendimento
CLIENTE *gerarClienteAleatorio(SISTEMA *sistema) {
    CLIENTE *cliente;
    CLIENTE_BASE *clienteBase;
    int indice;

    if (sistema == NULL) {
        return NULL;
    }

    if (sistema->baseClientes.tamanho <= 0 || sistema->baseProdutos.tamanho <= 0) {
        return NULL;
    }

    indice = Aleatorio(0, sistema->baseClientes.tamanho - 1);
    clienteBase = &sistema->baseClientes.dados[indice];

    cliente = criarCliente(sistema);
    if (cliente == NULL) {
        return NULL;
    }

    cliente->id = clienteBase->id;
    copiarStringSeguro(cliente->nome, clienteBase->nome, MAX_NOME);

    gerarProdutosCliente(cliente, sistema);

    if (cliente->produtos == NULL || cliente->nProdutos <= 0) {
        libertarCliente(cliente);
        return NULL;
    }

    ordenarProdutosClientePorPreco(cliente);
    calcularCamposDerivadosCliente(cliente);

    cliente->tempoRestanteCompras = cliente->tempoTotalCompras;
    cliente->tempoRestanteAtendimento = cliente->tempoTotalPagamento;
    cliente->instantePrevistoFimCompras = cliente->instanteEntradaSistema + cliente->tempoTotalCompras;

    return cliente;
}
// Gera e atribui uma lista de produtos aleatórios a um cliente com base na base de produtos
void gerarProdutosCliente(CLIENTE *cliente, SISTEMA *sistema) {
    int quantidade;
    int i;

    if (cliente == NULL || sistema == NULL) {
        return;
    }

    if (sistema->baseProdutos.tamanho <= 0) {
        return;
    }

    quantidade = obterQuantidadeProdutosCliente(sistema);
    if (quantidade <= 0) {
        return;
    }

    cliente->produtos = (PRODUTO *)malloc(sizeof(PRODUTO) * quantidade);
    if (cliente->produtos == NULL) {
        cliente->nProdutos = 0;
        return;
    }

    cliente->nProdutos = quantidade;

    for (i = 0; i < quantidade; i++) {
        PRODUTO *produtoBase = gerarProdutoAleatorio(&sistema->baseProdutos);

        if (produtoBase == NULL) {
            cliente->nProdutos = i;
            break;
        }

        cliente->produtos[i] = *produtoBase;
    }

    if (cliente->nProdutos <= 0) {
        free(cliente->produtos);
        cliente->produtos = NULL;
    }
}
// Ordena os produtos do cliente por preço, se houver mais de um produto
void ordenarProdutosClientePorPreco(CLIENTE *cliente) {
    if (cliente == NULL || cliente->produtos == NULL || cliente->nProdutos <= 1) {
        return;
    }

    ordenarProdutosPorPreco(cliente->produtos, cliente->nProdutos);
}
// Calcula o tempo total de compras do cliente somando o tempo de todos os produtos
int calcularTempoTotalComprasCliente(CLIENTE *cliente) {
    if (cliente == NULL || cliente->produtos == NULL || cliente->nProdutos <= 0) {
        return 0;
    }

    return calcularTempoTotalProdutos(cliente->produtos, cliente->nProdutos);
}
// Calcula o tempo total de pagamento somando o tempo de pagamento de todos os produtos do cliente
int calcularTempoTotalPagamentoCliente(CLIENTE *cliente) {
    int i;
    int total = 0;

    if (cliente == NULL || cliente->produtos == NULL || cliente->nProdutos <= 0) {
        return 0;
    }

    for (i = 0; i < cliente->nProdutos; i++) {
        total += cliente->produtos[i].tempoDePagamento;
    }

    return total;
}
// Calcula o valor total das compras do cliente somando os preços de todos os produtos
float calcularValorTotalComprasCliente(CLIENTE *cliente) {
    if (cliente == NULL || cliente->produtos == NULL || cliente->nProdutos <= 0) {
        return 0.0f;
    }

    return calcularValorTotalProdutos(cliente->produtos, cliente->nProdutos);
}
// Atualiza o estado do cliente para o novo estado indicado
void atualizarEstadoCliente(CLIENTE *cliente, ESTADO_CLIENTE novoEstado) {
    if (cliente == NULL) {
        return;
    }

    cliente->estado = novoEstado;
}
// Verifica se o cliente está atualmente em atendimento
int clienteEstaEmAtendimento(CLIENTE *cliente) {
    if (cliente == NULL) {
        return 0;
    }

    return cliente->estado == CLIENTE_EM_ATENDIMENTO;
}
// Regista a entrada do cliente na fila, atualizando o instante, removendo a associação à caixa e alterando o estado
void registarEntradaFilaCliente(CLIENTE *cliente, int instanteAtual) {
    if (cliente == NULL) {
        return;
    }

    cliente->instanteEntradaFila = instanteAtual;
    cliente->idCaixaAtual = ID_CAIXA_INVALIDO;
    atualizarEstadoCliente(cliente, CLIENTE_EM_FILA);
}
// Calcula o tempo de espera do cliente desde que entrou na fila até ao instante atual
int calcularTempoEsperaCliente(CLIENTE *cliente, int instanteAtual) {
    if (cliente == NULL) {
        return 0;
    }

    if (cliente->instanteEntradaFila < 0) {
        return 0;
    }

    if (instanteAtual < cliente->instanteEntradaFila) {
        return 0;
    }

    return instanteAtual - cliente->instanteEntradaFila;
}
// Inicia o atendimento do cliente, registando o instante, definindo o tempo restante e atualizando o estado
void iniciarAtendimentoCliente(CLIENTE *cliente, int instanteAtual) {
    if (cliente == NULL) {
        return;
    }

    cliente->instanteInicioAtendimento = instanteAtual;
    cliente->tempoRestanteAtendimento = cliente->tempoTotalPagamento;
    atualizarEstadoCliente(cliente, CLIENTE_EM_ATENDIMENTO);
}
// Atualiza o tempo restante de atendimento do cliente a cada ciclo de simulação, garantindo que não fique negativo
void atualizarTempoAtendimentoCliente(CLIENTE *cliente) {
    if (cliente == NULL) {
        return;
    }

    if (!clienteEstaEmAtendimento(cliente)) {
        return;
    }

    if (cliente->tempoRestanteAtendimento > 0) {
        cliente->tempoRestanteAtendimento -= CICLO_SIMULACAO_MINUTOS;
        if (cliente->tempoRestanteAtendimento < 0) {
            cliente->tempoRestanteAtendimento = 0;
        }
    }
}
// Verifica se o cliente terminou o atendimento (estado em atendimento e tempo restante igual ou inferior a zero)
int clienteTerminouAtendimento(CLIENTE *cliente) {
    if (cliente == NULL) {
        return 0;
    }

    return clienteEstaEmAtendimento(cliente) && cliente->tempoRestanteAtendimento <= 0;
}
// Finaliza o atendimento do cliente, registando o instante final, zerando o tempo restante e atualizando o estado
void finalizarAtendimentoCliente(CLIENTE *cliente, int instanteAtual) {
    if (cliente == NULL) {
        return;
    }

    cliente->instanteFimAtendimento = instanteAtual;
    cliente->tempoRestanteAtendimento = 0;
    atualizarEstadoCliente(cliente, CLIENTE_ATENDIDO);
    cliente->idCaixaAtual = ID_CAIXA_INVALIDO;
}
// Verifica se o cliente recebeu uma oferta durante o atendimento
int clienteRecebeuOferta(CLIENTE *cliente) {
    if (cliente == NULL) {
        return 0;
    }

    return cliente->recebeuOferta == CLIENTE_RECEBEU_OFERTA;
}
// Verifica se o cliente tem direito a oferta com base no tempo de espera e se ainda não recebeu
int clienteTemDireitoAOferta(SISTEMA *sistema, CLIENTE *cliente, int instanteAtual) {
    int tempoEspera;

    if (sistema == NULL || cliente == NULL) {
        return 0;
    }

    if (clienteRecebeuOferta(cliente)) {
        return 0;
    }

    tempoEspera = calcularTempoEsperaCliente(cliente, instanteAtual);
    return tempoEspera > sistema->config.MAX_ESPERA;
}
// Aplica uma oferta ao cliente, oferecendo o produto mais barato caso ainda não tenha recebido
void aplicarOfertaCliente(CLIENTE *cliente) {
    int indiceMaisBarato;

    if (cliente == NULL) {
        return;
    }

    if (clienteRecebeuOferta(cliente)) {
        return;
    }

    indiceMaisBarato = obterIndiceProdutoMaisBarato(cliente);
    if (indiceMaisBarato == INDICE_INVALIDO) {
        return;
    }

    cliente->recebeuOferta = CLIENTE_RECEBEU_OFERTA;
    cliente->valorOferta = cliente->produtos[indiceMaisBarato].preco;
    copiarStringSeguro(
        cliente->nomeProdutoOferecido,
        cliente->produtos[indiceMaisBarato].nome,
        MAX_NOME_PRODUTO
    );
}
// Verifica se o cliente já mudou de fila anteriormente
int clienteJaMudouDeFila(CLIENTE *cliente) {
    if (cliente == NULL) {
        return 0;
    }

    return cliente->mudouDeFila == CLIENTE_JA_MUDOU_DE_FILA;
}
// Marca que o cliente já mudou de fila
void marcarClienteMudouDeFila(CLIENTE *cliente) {
    if (cliente == NULL) {
        return;
    }

    cliente->mudouDeFila = CLIENTE_JA_MUDOU_DE_FILA;
}
// Liberta a memória associada ao cliente e aos seus produtos
void libertarCliente(CLIENTE *cliente) {
    if (cliente == NULL) {
        return;
    }

    if (cliente->produtos != NULL) {
        free(cliente->produtos);
        cliente->produtos = NULL;
    }

    free(cliente);
}
// Limpa e inicializa todos os campos do cliente com valores padrão
void limparCamposCliente(CLIENTE *cliente) {
    if (cliente == NULL) {
        return;
    }

    cliente->id = 0;
    cliente->nome[0] = '\0';
    cliente->nProdutos = 0;
    cliente->produtos = NULL;
    cliente->estado = CLIENTE_A_COMPRAR;
    cliente->idCaixaAtual = ID_CAIXA_INVALIDO;
    cliente->instanteEntradaSistema = 0;
    cliente->instantePrevistoFimCompras = 0;
    cliente->instanteEntradaFila = INDICE_INVALIDO;
    cliente->instanteInicioAtendimento = INDICE_INVALIDO;
    cliente->instanteFimAtendimento = INDICE_INVALIDO;
    cliente->tempoRestanteCompras = 0;
    cliente->tempoRestanteAtendimento = 0;
    cliente->tempoTotalCompras = 0;
    cliente->tempoTotalPagamento = 0;
    cliente->recebeuOferta = CLIENTE_NAO_RECEBEU_OFERTA;
    cliente->mudouDeFila = CLIENTE_NAO_MUDOU_DE_FILA;
    cliente->valorTotalCompras = 0.0f;
    cliente->valorOferta = 0.0f;
    cliente->nomeProdutoOferecido[0] = '\0';
}
// Gera uma quantidade aleatória de produtos para o cliente dentro dos limites definidos na configuração
int obterQuantidadeProdutosCliente(SISTEMA *sistema) {
    int maxProdutos;

    if (sistema == NULL) {
        return 0;
    }

    maxProdutos = sistema->config.MAX_PRODUTOS_CLIENTE;
    if (maxProdutos < MIN_PRODUTOS_CLIENTE_DEFAULT) {
        maxProdutos = MAX_PRODUTOS_CLIENTE_DEFAULT;
    }

    return Aleatorio(MIN_PRODUTOS_CLIENTE_DEFAULT, maxProdutos);
}
// Gera uma quantidade aleatória de novos clientes para o ciclo atual, respeitando os limites configurados
int obterQuantidadeNovosClientesNoCiclo(SISTEMA *sistema) {
    int minimo;
    int maximo;

    if (sistema == NULL) {
        return 0;
    }

    minimo = sistema->config.MIN_NOVOS_CLIENTES_POR_CICLO;
    maximo = sistema->config.MAX_NOVOS_CLIENTES_POR_CICLO;

    if (minimo < 0) {
        minimo = MIN_NOVOS_CLIENTES_POR_CICLO_DEFAULT;
    }

    if (maximo < minimo) {
        maximo = MAX_NOVOS_CLIENTES_POR_CICLO_DEFAULT;
    }

    return Aleatorio(minimo, maximo);
}
// Calcula e atualiza os campos derivados do cliente (tempo de compras, tempo de pagamento e valor total)
void calcularCamposDerivadosCliente(CLIENTE *cliente) {
    if (cliente == NULL) {
        return;
    }

    cliente->tempoTotalCompras = calcularTempoTotalComprasCliente(cliente);
    cliente->tempoTotalPagamento = calcularTempoTotalPagamentoCliente(cliente);
    cliente->valorTotalCompras = calcularValorTotalComprasCliente(cliente);
}
// Retorna o índice do produto mais barato na lista de produtos do cliente
int obterIndiceProdutoMaisBarato(CLIENTE *cliente) {
    int i;
    int indiceMaisBarato;

    if (cliente == NULL || cliente->produtos == NULL || cliente->nProdutos <= 0) {
        return INDICE_INVALIDO;
    }

    indiceMaisBarato = 0;

    for (i = 1; i < cliente->nProdutos; i++) {
        if (cliente->produtos[i].preco < cliente->produtos[indiceMaisBarato].preco) {
            indiceMaisBarato = i;
        }
    }

    return indiceMaisBarato;
}
// Gera o próximo id sequencial para cliente base, baseado no último elemento existente
int gerarProximoIdClienteBase(BASE_CLIENTES *base) {
    if (base == NULL || base->tamanho <= 0) {
        return 000001;
    }

    return base->dados[base->tamanho - 1].id + 1;
}
// Conta o total de clientes nas caixas, incluindo os em atendimento e nas filas
int contarClientesNasCaixas(SISTEMA *sistema) {
    int i;
    int total = 0;

    if (sistema == NULL || sistema->caixas == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        total += sistema->caixas[i].fila.tamanho;

        if (sistema->caixas[i].clienteAtual != NULL) {
            total++;
        }
    }

    return total;
}
// Conta o número total de clientes no sistema somando os que estão a comprar, os que estão nas filas dos caixas e os que estão a ser atendidos no momento.
int contarClientesSistema(SISTEMA *sistema) {
    int total = 0;

    total += sistema->clientesComprando.tamanho;

    for (int i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];

        total += caixa->fila.tamanho;

        if (caixa->clienteAtual != NULL) {
            total++;
        }
    }

    return total;
}