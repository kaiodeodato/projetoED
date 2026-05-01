#include <stdio.h>
#include <stdlib.h>
#include "memoria.h"

// Calcula a memória ocupada por um cliente, incluindo a estrutura e os seus produtos
size_t calcularMemoriaCliente(CLIENTE *cliente) {
    size_t memoria = 0;

    if (cliente == NULL) {
        return 0;
    }

    memoria += sizeof(CLIENTE);

    if (cliente->produtos != NULL && cliente->nProdutos > 0) {
        memoria += sizeof(PRODUTO) * cliente->nProdutos;
    }

    return memoria;
}
// Calcula a memória ocupada pela fila, incluindo a estrutura e todos os seus elementos
size_t calcularMemoriaFila(FILA *fila) {
    size_t memoria = 0;
    ELEMENTO *atual;

    if (fila == NULL) {
        return 0;
    }

    memoria += sizeof(FILA);

    atual = fila->inicio;
    while (atual != NULL) {
        memoria += sizeof(ELEMENTO);
        atual = atual->seguinte;
    }

    return memoria;
}
// Calcula a memória ocupada pela tabela hash, incluindo buckets e nós de clientes
size_t calcularMemoriaHash(HASHTABLE *tabela) {
    size_t memoria = 0;
    BUCKET *bucketAtual;

    if (tabela == NULL) {
        return 0;
    }

    memoria += sizeof(HASHTABLE);

    bucketAtual = tabela->inicio;
    while (bucketAtual != NULL) {
        HASHNODE *noAtual = bucketAtual->clientes;

        memoria += sizeof(BUCKET);

        while (noAtual != NULL) {
            memoria += sizeof(HASHNODE);
            noAtual = noAtual->prox;
        }

        bucketAtual = bucketAtual->prox;
    }

    return memoria;
}
// Calcula a memória ocupada pela lista de clientes em compras, incluindo a estrutura e todos os nós
size_t calcularMemoriaListaCompras(LISTA_CLIENTES_COMPRANDO *lista) {
    size_t memoria = 0;
    NO_CLIENTE_COMPRANDO *atual;

    if (lista == NULL) {
        return 0;
    }

    memoria += sizeof(LISTA_CLIENTES_COMPRANDO);

    atual = lista->inicio;
    while (atual != NULL) {
        memoria += sizeof(NO_CLIENTE_COMPRANDO);
        atual = atual->seguinte;
    }

    return memoria;
}
// Calcula a memória ocupada pela lista de logs, incluindo a estrutura e todos os nós
size_t calcularMemoriaLogs(LISTA_LOGS *logs) {
    size_t memoria = 0;
    NO_LOG_ACAO *atual;

    if (logs == NULL) {
        return 0;
    }

    memoria += sizeof(LISTA_LOGS);

    atual = logs->inicio;
    while (atual != NULL) {
        memoria += sizeof(NO_LOG_ACAO);
        atual = atual->seguinte;
    }

    return memoria;
}
// Calcula a memória total ocupada pelo sistema, incluindo estruturas principais, caixas, listas, hash e clientes
size_t calcularMemoriaSistema(SISTEMA *sistema) {
    size_t memoria = 0;
    int i;
    BUCKET *bucketAtual;

    if (sistema == NULL) {
        return 0;
    }

    memoria += sizeof(SISTEMA);
    memoria += calcularMemoriaBases(sistema);

    if (sistema->caixas != NULL && sistema->config.N_CAIXAS > 0) {
        memoria += sizeof(CAIXA) * sistema->config.N_CAIXAS;

        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            memoria += calcularMemoriaFila(&sistema->caixas[i].fila) - sizeof(FILA);
            memoria += calcularMemoriaHistoricoClientesCaixa(&sistema->caixas[i].historicoClientes);
        }
    }

    memoria += calcularMemoriaHash(&sistema->clientesHash) - sizeof(HASHTABLE);
    memoria += calcularMemoriaListaCompras(&sistema->clientesComprando) - sizeof(LISTA_CLIENTES_COMPRANDO);
    memoria += calcularMemoriaLogs(&sistema->logs) - sizeof(LISTA_LOGS);

    bucketAtual = sistema->clientesHash.inicio;
    while (bucketAtual != NULL) {
        HASHNODE *noAtual = bucketAtual->clientes;

        while (noAtual != NULL) {
            if (noAtual->cliente != NULL) {
                memoria += calcularMemoriaCliente(noAtual->cliente);
            }

            noAtual = noAtual->prox;
        }

        bucketAtual = bucketAtual->prox;
    }

    return memoria;
}
// Calcula a memória desperdiçada no sistema, delegando o cálculo às bases de dados
size_t calcularMemoriaDesperdicadaSistema(SISTEMA *sistema) {
    if (sistema == NULL) {
        return 0;
    }

    return calcularMemoriaDesperdicadaBases(sistema);
}
// Gera um relatório estimado de memória do sistema, detalhando consumo por estruturas e memória desperdiçada
void gerarRelatorioMemoria(SISTEMA *sistema, char *nomeFicheiro) {
    FILE *ficheiro;
    size_t memoriaClientesAtivos = 0;
    size_t memoriaFilas = 0;
    size_t memoriaHash = 0;
    size_t memoriaListaCompras = 0;
    size_t memoriaLogs = 0;
    size_t memoriaCaixas = 0;
    size_t memoriaTotal;
    size_t memoriaDesperdicada;
    int i;
    BUCKET *bucketAtual;

    if (sistema == NULL || nomeFicheiro == NULL) {
        return;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return;
    }

    bucketAtual = sistema->clientesHash.inicio;
    while (bucketAtual != NULL) {
        HASHNODE *noAtual = bucketAtual->clientes;

        while (noAtual != NULL) {
            if (noAtual->cliente != NULL) {
                memoriaClientesAtivos += calcularMemoriaCliente(noAtual->cliente);
            }

            noAtual = noAtual->prox;
        }

        bucketAtual = bucketAtual->prox;
    }

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            memoriaCaixas += sizeof(CAIXA);
            memoriaFilas += calcularMemoriaFila(&sistema->caixas[i].fila) - sizeof(FILA);
            memoriaCaixas += calcularMemoriaHistoricoClientesCaixa(&sistema->caixas[i].historicoClientes);
        }
    }

    memoriaHash = calcularMemoriaHash(&sistema->clientesHash);
    memoriaListaCompras = calcularMemoriaListaCompras(&sistema->clientesComprando);
    memoriaLogs = calcularMemoriaLogs(&sistema->logs);
    memoriaTotal = calcularMemoriaSistema(sistema);
    memoriaDesperdicada = calcularMemoriaDesperdicadaSistema(sistema);

    fprintf(ficheiro, "RELATORIO DE MEMORIA (ESTIMATIVO)\n");
    fprintf(ficheiro, "================================\n\n");

    fprintf(
        ficheiro,
        "Nota: os valores apresentados correspondem a uma estimativa baseada nas estruturas\n"
        "de dados dinamicas geridas pelo programa, nao incluindo overhead interno do malloc,\n"
        "fragmentacao de memoria ou consumo adicional do runtime.\n\n"
    );

    fprintf(ficheiro, "Memoria total estimada das estruturas do sistema: %zu bytes\n", memoriaTotal);
    fprintf(ficheiro, "Memoria desperdicada estimada nas bases dinamicas: %zu bytes\n\n", memoriaDesperdicada);

    fprintf(ficheiro, "Detalhe por estrutura:\n");
    fprintf(ficheiro, "  Bases carregadas: %zu bytes\n", calcularMemoriaBases(sistema));
    fprintf(ficheiro, "  Caixas (vetor + historicos): %zu bytes\n", memoriaCaixas);
    fprintf(ficheiro, "  Filas: %zu bytes\n", memoriaFilas);
    fprintf(ficheiro, "  Hash de clientes: %zu bytes\n", memoriaHash);
    fprintf(ficheiro, "  Lista de compras: %zu bytes\n", memoriaListaCompras);
    fprintf(ficheiro, "  Logs: %zu bytes\n", memoriaLogs);
    fprintf(ficheiro, "  Clientes ativos e produtos associados: %zu bytes\n", memoriaClientesAtivos);

    fclose(ficheiro);
}
// Calcula a memória ocupada pelo histórico de clientes de uma caixa, incluindo a estrutura e todos os nós
size_t calcularMemoriaHistoricoClientesCaixa(HISTORICO_CLIENTES_CAIXA *historico) {
    size_t memoria = 0;
    NO_HISTORICO_CLIENTE *atual;

    if (historico == NULL) {
        return 0;
    }

    memoria += sizeof(HISTORICO_CLIENTES_CAIXA);

    atual = historico->inicio;
    while (atual != NULL) {
        memoria += sizeof(NO_HISTORICO_CLIENTE);
        atual = atual->seguinte;
    }

    return memoria;
}
// Calcula a memória ocupada pelas bases de clientes, produtos e colaboradores, incluindo capacidade alocada
size_t calcularMemoriaBases(SISTEMA *sistema) {
    size_t memoria = 0;

    if (sistema == NULL) {
        return 0;
    }

    memoria += sizeof(BASE_CLIENTES);
    memoria += sizeof(BASE_PRODUTOS);
    memoria += sizeof(BASE_COLABORADORES);

    if (sistema->baseClientes.dados != NULL && sistema->baseClientes.capacidade > 0) {
        memoria += sizeof(CLIENTE_BASE) * sistema->baseClientes.capacidade;
    }

    if (sistema->baseProdutos.dados != NULL && sistema->baseProdutos.capacidade > 0) {
        memoria += sizeof(PRODUTO) * sistema->baseProdutos.capacidade;
    }

    if (sistema->baseColaboradores.dados != NULL && sistema->baseColaboradores.capacidade > 0) {
        memoria += sizeof(COLABORADOR) * sistema->baseColaboradores.capacidade;
    }

    return memoria;
}
// Calcula a memória desperdiçada nas bases, considerando a capacidade alocada não utilizada
size_t calcularMemoriaDesperdicadaBases(SISTEMA *sistema) {
    size_t memoria = 0;
    int elementosNaoUsados;

    if (sistema == NULL) {
        return 0;
    }

    elementosNaoUsados = sistema->baseClientes.capacidade - sistema->baseClientes.tamanho;
    if (elementosNaoUsados > 0) {
        memoria += sizeof(CLIENTE_BASE) * elementosNaoUsados;
    }

    elementosNaoUsados = sistema->baseProdutos.capacidade - sistema->baseProdutos.tamanho;
    if (elementosNaoUsados > 0) {
        memoria += sizeof(PRODUTO) * elementosNaoUsados;
    }

    elementosNaoUsados = sistema->baseColaboradores.capacidade - sistema->baseColaboradores.tamanho;
    if (elementosNaoUsados > 0) {
        memoria += sizeof(COLABORADOR) * elementosNaoUsados;
    }

    return memoria;
}