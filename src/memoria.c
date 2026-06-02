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

    if (cliente->idsProdutos != NULL && cliente->nProdutos > 0) {
        memoria += sizeof(int) * cliente->nProdutos;
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
    int i;

    if (tabela == NULL) {
        return 0;
    }

    memoria += sizeof(HASHTABLE);

    for (i = 0; i < tabela->nBuckets; i++) {
        HASHNODE *noAtual = tabela->buckets[i].clientes;

        while (noAtual != NULL) {
            memoria += sizeof(HASHNODE);
            noAtual = noAtual->prox;
        }
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

    for (i = 0; i < sistema->clientesHash.nBuckets; i++) {
        HASHNODE *noAtual = sistema->clientesHash.buckets[i].clientes;

        while (noAtual != NULL) {
            if (noAtual->cliente != NULL) {
                memoria += calcularMemoriaCliente(noAtual->cliente);
            }

            noAtual = noAtual->prox;
        }
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
    size_t memoriaDesperdicadaNomes;
    int i;

    if (sistema == NULL || nomeFicheiro == NULL) {
        return;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return;
    }

    for (i = 0; i < sistema->clientesHash.nBuckets; i++) {
        HASHNODE *noAtual = sistema->clientesHash.buckets[i].clientes;

        while (noAtual != NULL) {
            if (noAtual->cliente != NULL) {
                memoriaClientesAtivos += calcularMemoriaCliente(noAtual->cliente);
            }

            noAtual = noAtual->prox;
        }
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
    memoriaDesperdicadaNomes = calcularMemoriaDesperdicadaNomes(sistema);

    fprintf(ficheiro, "RELATORIO DE MEMORIA (ESTIMATIVO)\n");
    fprintf(ficheiro, "================================\n\n");

    fprintf(
        ficheiro,
        "Nota: os valores apresentados correspondem a uma estimativa baseada nas estruturas\n"
        "de dados dinamicas geridas pelo programa, nao incluindo overhead interno do malloc,\n"
        "fragmentacao de memoria ou consumo adicional do runtime.\n\n"
    );

    fprintf(ficheiro, "Memoria total estimada das estruturas do sistema: ");
    escreverMemoriaFormatada(ficheiro, memoriaTotal);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "Memoria desperdicada estimada nas bases dinamicas: ");
    escreverMemoriaFormatada(ficheiro, memoriaDesperdicada);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "Memoria desperdicada estimada nos Nomes nas bases dinamicas: ");
    escreverMemoriaFormatada(ficheiro, memoriaDesperdicadaNomes);
    fprintf(ficheiro, "\n\n");

    fprintf(ficheiro, "Ocupacao das bases dinamicas em unidades:\n");

    fprintf(
        ficheiro,
        "  Clientes: %d ocupados / %d capacidade / %d livres\n",
        sistema->baseClientes.tamanho,
        sistema->baseClientes.capacidade,
        calcularEspacoLivreClientes(sistema)
    );

    fprintf(
        ficheiro,
        "  Produtos: %d ocupados / %d capacidade / %d livres\n",
        sistema->baseProdutos.tamanho,
        sistema->baseProdutos.capacidade,
        calcularEspacoLivreProdutos(sistema)
    );

    fprintf(
        ficheiro,
        "  Colaboradores: %d ocupados / %d capacidade / %d livres\n\n",
        sistema->baseColaboradores.tamanho,
        sistema->baseColaboradores.capacidade,
        calcularEspacoLivreColaboradores(sistema)
    );

    fprintf(ficheiro, "Detalhe por estrutura:\n");

    fprintf(ficheiro, "  Bases carregadas: ");
    escreverMemoriaFormatada(ficheiro, calcularMemoriaBases(sistema));
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "  Caixas (vetor + historicos): ");
    escreverMemoriaFormatada(ficheiro, memoriaCaixas);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "  Filas: ");
    escreverMemoriaFormatada(ficheiro, memoriaFilas);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "  Hash de clientes: ");
    escreverMemoriaFormatada(ficheiro, memoriaHash);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "  Lista de compras: ");
    escreverMemoriaFormatada(ficheiro, memoriaListaCompras);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "  Logs: ");
    escreverMemoriaFormatada(ficheiro, memoriaLogs);
    fprintf(ficheiro, "\n");

    fprintf(ficheiro, "  Clientes da simulacao e produtos associados: ");
    escreverMemoriaFormatada(ficheiro, memoriaClientesAtivos);
    fprintf(ficheiro, "\n");

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
// Calcula a memória desperdiçada nos arrays fixos de nomes,
// considerando o espaço reservado mas não utilizado pelas strings.
size_t calcularMemoriaDesperdicadaNomes(SISTEMA *sistema) {
    size_t memoria = 0;
    int i;
    size_t usados;

    if (sistema == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->baseClientes.tamanho; i++) {
        usados = strlen(sistema->baseClientes.dados[i].nome) + 1;
        memoria += MAX_NOME - usados;
    }

    for (i = 0; i < sistema->baseColaboradores.tamanho; i++) {
        usados = strlen(sistema->baseColaboradores.dados[i].nome) + 1;
        memoria += MAX_NOME - usados;
    }

    return memoria;
}
// Calcula o número de posições livres disponíveis na base de clientes
int calcularEspacoLivreClientes(SISTEMA *sistema) {
    if (sistema == NULL) return 0;
    return sistema->baseClientes.capacidade - sistema->baseClientes.tamanho;
}
// Calcula o número de posições livres disponíveis na base de produtos
int calcularEspacoLivreProdutos(SISTEMA *sistema) {
    if (sistema == NULL) return 0;
    return sistema->baseProdutos.capacidade - sistema->baseProdutos.tamanho;
}
// Calcula o número de posições livres disponíveis na base de colaboradores
int calcularEspacoLivreColaboradores(SISTEMA *sistema) {
    if (sistema == NULL) return 0;
    return sistema->baseColaboradores.capacidade - sistema->baseColaboradores.tamanho;
}
// Mostra um valor de memória formatado em bytes, KB ou MB
void imprimirMemoriaFormatada(size_t bytes) {
    if (bytes >= 1024 * 1024) {
        printf("%.2f MB", (double)bytes / (1024.0 * 1024.0));
    }
    else if (bytes >= 1024) {
        printf("%.2f KB", (double)bytes / 1024.0);
    }
    else {
        printf("%zu bytes", bytes);
    }
}
// Escreve um valor de memória formatado em bytes, KB ou MB para um ficheiro
void escreverMemoriaFormatada(FILE *ficheiro, size_t bytes) {
    if (ficheiro == NULL) {
        return;
    }

    if (bytes >= 1024 * 1024) {
        fprintf(ficheiro, "%.2f MB", (double)bytes / (1024.0 * 1024.0));
    } else if (bytes >= 1024) {
        fprintf(ficheiro, "%.2f KB", (double)bytes / 1024.0);
    } else {
        fprintf(ficheiro, "%zu bytes", bytes);
    }
}