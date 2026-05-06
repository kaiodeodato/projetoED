#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"
#include "define.h"
#include "uteis.h"

// Seleciona aleatoriamente um produto da base de produtos e devolve o seu ponteiro
PRODUTO *gerarProdutoAleatorio(BASE_PRODUTOS *base) {
    int indice;

    if (base == NULL || base->dados == NULL || base->tamanho <= 0) {
        return NULL;
    }

    indice = Aleatorio(0, base->tamanho - 1);
    return &base->dados[indice];
}
// Ordena os produtos por preço em ordem crescente usando algoritmo de bubble sort
void ordenarProdutosPorPreco(PRODUTO *produtos, int quantidade) {
    int i;
    int j;

    if (produtos == NULL || quantidade <= 1) {
        return;
    }

    for (i = 0; i < quantidade - 1; i++) {
        for (j = 0; j < quantidade - 1 - i; j++) {
            if (produtos[j].preco > produtos[j + 1].preco) {
                trocarProdutos(&produtos[j], &produtos[j + 1]);
            }
        }
    }
}
// Calcula o tempo total de procura de um conjunto de produtos
int calcularTempoTotalProdutos(PRODUTO *produtos, int quantidade) {
    int i;
    int tempoTotal = 0;

    if (produtos == NULL || quantidade <= 0) {
        return 0;
    }

    for (i = 0; i < quantidade; i++) {
        tempoTotal += produtos[i].tempoDeProcura;
    }

    return tempoTotal;
}
// Calcula o valor total de um conjunto de produtos somando os preços
float calcularValorTotalProdutos(PRODUTO *produtos, int quantidade) {
    int i;
    float valorTotal = 0.0f;

    if (produtos == NULL || quantidade <= 0) {
        return 0.0f;
    }

    for (i = 0; i < quantidade; i++) {
        valorTotal += produtos[i].preco;
    }

    return valorTotal;
}
// Troca dois produtos de posição na memória (swap)
void trocarProdutos(PRODUTO *a, PRODUTO *b) {
    PRODUTO temp;

    if (a == NULL || b == NULL) {
        return;
    }

    temp = *a;
    *a = *b;
    *b = temp;
}
// Retorna uma string indicando se a caixa está em controlo manual ou automático
char *obterTextoControloCaixa(CAIXA *caixa) {
    if (caixa == NULL) {
        return "N/A";
    }

    if (caixa->controloManualGerente == CAIXA_COM_CONTROLO_MANUAL) {
        return "MANUAL";
    }

    return "AUTO";
}
// Gera o próximo ID sequencial para produtos da base, com base no último elemento existente
int gerarProximoIdProdutoBase(BASE_PRODUTOS *base) {
    if (base == NULL || base->tamanho <= 0) {
        return 100001;
    }

    return base->dados[base->tamanho - 1].id + 1;
}
// Procura um produto pelo ID na base de produtos
// e apresenta os seus dados caso seja encontrado.
void procurarProdutoPorId(BASE_PRODUTOS *base) {
    int i;
    int idProduto;

    if (base == NULL || base->dados == NULL || base->tamanho <= 0) {
        printf("Base de produtos invalida.\n");
        return;
    }

    lerInteiro("Id do produto: ", &idProduto, 1, 999999999);

    for (i = 0; i < base->tamanho; i++) {

        if (base->dados[i].id == idProduto) {

            printf("\n========== PRODUTO ENCONTRADO ==========\n");
            printf("ID: %d\n", base->dados[i].id);
            printf("Nome: %s\n", base->dados[i].nome);
            printf("Preco: %.2f euros\n", base->dados[i].preco);
            printf("Tempo de procura: %d min\n", base->dados[i].tempoDeProcura);
            printf("Tempo de pagamento: %d min\n", base->dados[i].tempoDePagamento);
            printf("========================================\n");

            return;
        }
    }

    printf("Produto com ID %d nao encontrado.\n", idProduto);
}
// Mostra uma página da listagem de produtos da base,
// apresentando os dados principais e permitindo navegação paginada.
void mostrarPaginaProdutos(BASE_PRODUTOS *base, int pagina) {
    int i;
    int inicio;
    int fim;

    if (base == NULL || base->dados == NULL || base->tamanho <= 0) {
        printf("Nao existem produtos para listar.\n");
        return;
    }

    inicio = pagina * CLIENTES_POR_PAGINA;
    fim = inicio + CLIENTES_POR_PAGINA;

    if (fim > base->tamanho) {
        fim = base->tamanho;
    }

    limparTela();
    printf("\n%s", LINHA_SEPARADORA);
    printf("PRODUTOS BASE (Pagina %d)\n", pagina + 1);
    printf("%s", LINHA_SEPARADORA);

    for (i = inicio; i < fim; i++) {
        printf(
            "ID: %d | Nome: %-120s | Preco: %.2f | Procura: %d min | Pagamento: %d min\n",
            base->dados[i].id,
            base->dados[i].nome,
            base->dados[i].preco,
            base->dados[i].tempoDeProcura,
            base->dados[i].tempoDePagamento
        );
    }

    printf("\nTotal de produtos: %d\n", base->tamanho);
    printf("[A] Anterior | [P] Proxima | [S] Sair\n");
}
// Permite navegar pela lista de produtos em páginas,
// avançando, retrocedendo ou saindo da listagem.
void listarProdutosPaginado(BASE_PRODUTOS *base) {
    int pagina = 0;
    char opcao;

    if (base == NULL || base->dados == NULL || base->tamanho <= 0) {
        printf("Nao existem produtos para listar.\n");
        return;
    }

    while (1) {
        mostrarPaginaProdutos(base, pagina);

        scanf(" %c", &opcao);

        if (opcao == 'p' || opcao == 'P') {
            if ((pagina + 1) * CLIENTES_POR_PAGINA < base->tamanho) {
                pagina++;
            }
        } else if (opcao == 'a' || opcao == 'A') {
            if (pagina > 0) {
                pagina--;
            }
        } else if (opcao == 's' || opcao == 'S') {
            break;
        }
    }
}