#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "ficheiros.h"
#include "define.h"
#include "uteis.h"
#include "colaboradores.h"

// Carrega a base de clientes a partir de um ficheiro, armazenando os dados válidos em memória
int carregarBaseClientes(BASE_CLIENTES *base, char *nomeFicheiro) {
    FILE *ficheiro;
    char linha[256];
    int totalClientes;
    CLIENTE_BASE cliente;
    int lidos;

    if (base == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    base->dados = NULL;
    base->tamanho = 0;
    base->capacidade = 0;

    totalClientes = contarClientesValidosFicheiro(nomeFicheiro);
    if (totalClientes <= 0) {
        return 0;
    }

    base->dados = (CLIENTE_BASE *)malloc(sizeof(CLIENTE_BASE) * totalClientes);
    if (base->dados == NULL) {
        return 0;
    }

    base->capacidade = totalClientes;

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        free(base->dados);
        base->dados = NULL;
        base->capacidade = 0;
        return 0;
    }

    if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
        fclose(ficheiro);
        free(base->dados);
        base->dados = NULL;
        base->capacidade = 0;
        return 0;
    }

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        cliente.id = 0;
        cliente.nome[0] = '\0';

        lidos = sscanf(linha, "%d %39[^\n]", &cliente.id, cliente.nome);
        if (lidos != 2) {
            continue;
        }

        base->dados[base->tamanho] = cliente;
        base->tamanho++;
    }

    fclose(ficheiro);
    return base->tamanho;
}
// Carrega a base de produtos a partir de um ficheiro, processando cada linha e armazenando os dados válidos em memória
int carregarBaseProdutos(BASE_PRODUTOS *base, char *nomeFicheiro) {
    FILE *ficheiro;
    char linha[256];
    int totalProdutos;

    if (base == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    base->dados = NULL;
    base->tamanho = 0;
    base->capacidade = 0;

    totalProdutos = contarProdutosValidosFicheiro(nomeFicheiro);
    if (totalProdutos <= 0) {
        return 0;
    }

    base->dados = (PRODUTO *)malloc(sizeof(PRODUTO) * totalProdutos);
    if (base->dados == NULL) {
        return 0;
    }

    base->capacidade = totalProdutos;

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        free(base->dados);
        base->dados = NULL;
        base->capacidade = 0;
        return 0;
    }

    if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
        fclose(ficheiro);
        free(base->dados);
        base->dados = NULL;
        base->capacidade = 0;
        return 0;
    }

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        PRODUTO produto;
        char nomeTemp[128];
        size_t comprimento;
        int id, tempoProcura, tempoPagamento;
        float preco;
        int pos;
        int lidos;

        produto.id = 0;
        produto.preco = 0.0f;
        produto.tempoDeProcura = 0;
        produto.tempoDePagamento = 0;
        produto.nome = NULL;
        nomeTemp[0] = '\0';

        linha[strcspn(linha, "\r\n")] = '\0';

        pos = (int)strlen(linha) - 1;
        if (pos < 0) continue;

        while (pos >= 0 && (linha[pos] == ' ' || linha[pos] == '\t')) pos--;
        if (pos < 0) continue;
        while (pos >= 0 && linha[pos] != ' ' && linha[pos] != '\t') pos--;
        tempoPagamento = atoi(&linha[pos + 1]);

        while (pos >= 0 && (linha[pos] == ' ' || linha[pos] == '\t')) pos--;
        if (pos < 0) continue;
        while (pos >= 0 && linha[pos] != ' ' && linha[pos] != '\t') pos--;
        tempoProcura = atoi(&linha[pos + 1]);

        while (pos >= 0 && (linha[pos] == ' ' || linha[pos] == '\t')) pos--;
        if (pos < 0) continue;
        while (pos >= 0 && linha[pos] != ' ' && linha[pos] != '\t') pos--;
        preco = (float)atof(&linha[pos + 1]);

        if (pos < 0) continue;

        linha[pos] = '\0';

        lidos = sscanf(linha, "%d %127[^\n]", &id, nomeTemp);
        if (lidos != 2) continue;

        comprimento = strlen(nomeTemp);
        while (comprimento > 0 &&
               (nomeTemp[comprimento - 1] == ' ' || nomeTemp[comprimento - 1] == '\t')) {
            nomeTemp[comprimento - 1] = '\0';
            comprimento--;
        }

        produto.id = id;
        produto.preco = preco;
        produto.tempoDeProcura = tempoProcura;
        produto.tempoDePagamento = tempoPagamento;

        produto.nome = (char *)malloc(strlen(nomeTemp) + 1);
        if (produto.nome == NULL) {
            int j;

            for (j = 0; j < base->tamanho; j++) {
                free(base->dados[j].nome);
                base->dados[j].nome = NULL;
            }

            free(base->dados);
            base->dados = NULL;
            base->tamanho = 0;
            base->capacidade = 0;

            fclose(ficheiro);
            return 0;
        }

strcpy(produto.nome, nomeTemp);

        base->dados[base->tamanho] = produto;
        base->tamanho++;
        }

    fclose(ficheiro);
    return base->tamanho;
}
// Carrega a base de colaboradores a partir de um ficheiro, inicializando os dados e armazenando em memória
int carregarBaseColaboradores(BASE_COLABORADORES *base, char *nomeFicheiro) {
    FILE *ficheiro;
    char linha[256];
    int totalColaboradores;
    COLABORADOR colaborador;
    int lidos;

    if (base == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    base->dados = NULL;
    base->tamanho = 0;
    base->capacidade = 0;

    totalColaboradores = contarColaboradoresValidosFicheiro(nomeFicheiro);
    if (totalColaboradores <= 0) {
        return 0;
    }

    base->dados = (COLABORADOR *)malloc(sizeof(COLABORADOR) * totalColaboradores);
    if (base->dados == NULL) {
        return 0;
    }

    base->capacidade = totalColaboradores;

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        free(base->dados);
        base->dados = NULL;
        base->capacidade = 0;
        return 0;
    }

    if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
        fclose(ficheiro);
        free(base->dados);
        base->dados = NULL;
        base->capacidade = 0;
        return 0;
    }

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        colaborador.id = 0;
        colaborador.nome[0] = '\0';

        lidos = sscanf(linha, "%d %39[^\n]", &colaborador.id, colaborador.nome);

        if (lidos != 2) {
            continue;
        }

        colaborador.clientesAtendidos = 0;
        colaborador.idCaixaAssociada = ID_CAIXA_INVALIDO;
        colaborador.ativo = 0;

        base->dados[base->tamanho] = colaborador;
        base->tamanho++;
    }

    fclose(ficheiro);
    return base->tamanho;
}
// Guarda a base de clientes num ficheiro, escrevendo todos os registos formatados
int guardarBaseClientes(BASE_CLIENTES *base, char *nomeFicheiro) {
    FILE *ficheiro;
    int i;

    if (base == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return 0;
    }

    fprintf(ficheiro, "id     nome\n");

    for (i = 0; i < base->tamanho; i++) {
        fprintf(ficheiro, "%06d %s\n", base->dados[i].id, base->dados[i].nome);
    }

    fclose(ficheiro);
    return 1;
}
// Guarda a base de produtos num ficheiro, escrevendo todos os registos com formatação detalhada
int guardarBaseProdutos(BASE_PRODUTOS *base, char *nomeFicheiro) {
    FILE *ficheiro;
    int i;

    if (base == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return 0;
    }

    fprintf(ficheiro, "id     nome                       preco   tempoDeProcura  tempoDePagamento\n");

    for (i = 0; i < base->tamanho; i++) {
        fprintf(
            ficheiro,
            "%d %-26s %-7.2f %-15d %-15d\n",
            base->dados[i].id,
            base->dados[i].nome,
            base->dados[i].preco,
            base->dados[i].tempoDeProcura,
            base->dados[i].tempoDePagamento
        );
    }

    fclose(ficheiro);
    return 1;
}
// Escreve o histórico de ações num ficheiro CSV, incluindo instante e descrição de cada log
int escreverHistoricoCSV(LISTA_LOGS *logs, char *nomeFicheiro) {
    FILE *ficheiro;
    NO_LOG_ACAO *atual;

    if (logs == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return 0;
    }

    fprintf(ficheiro, "%s", CABECALHO_CSV_HISTORICO);

    atual = logs->inicio;
    while (atual != NULL) {
        fprintf(ficheiro, "%d;%s\n", atual->log.instante, atual->log.descricao);
        atual = atual->seguinte;
    }

    fclose(ficheiro);
    return 1;
}
// Gera um relatório de estatísticas da simulação em ficheiro, incluindo métricas gerais, operadores e clientes de destaque
int escreverRelatorioEstatisticas(SISTEMA *sistema, char *nomeFicheiro) {
    FILE *ficheiro;
    COLABORADOR *operador;
    COLABORADOR *operadorMais;
    CLIENTE *clienteMaisGastou = NULL;
    CLIENTE *clienteMenosGastou = NULL;
    int i;

    if (sistema == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return 0;
    }

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            NO_HISTORICO_CLIENTE *atual = sistema->caixas[i].historicoClientes.inicio;

            while (atual != NULL) {
                CLIENTE *cliente = atual->cliente;

                if (cliente != NULL) {
                    if (clienteMaisGastou == NULL ||
                        cliente->valorTotalCompras > clienteMaisGastou->valorTotalCompras) {
                        clienteMaisGastou = cliente;
                    }

                    if (clienteMenosGastou == NULL ||
                        cliente->valorTotalCompras < clienteMenosGastou->valorTotalCompras) {
                        clienteMenosGastou = cliente;
                    }
                }

                atual = atual->seguinte;
            }
        }
    }

    fprintf(ficheiro, "\n%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "RESUMO GERAL\n");
    fprintf(ficheiro, "%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "Tempo de simulacao: %d\n", sistema->estatisticas.tempoSimulacao);
    fprintf(ficheiro, "Clientes gerados: %d\n", sistema->estatisticas.totalClientesGerados);
    fprintf(ficheiro, "Clientes atendidos: %d\n", sistema->estatisticas.totalClientesAtendidos);

    fprintf(ficheiro, "\n%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "VENDAS\n");
    fprintf(ficheiro, "%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "Produtos vendidos: %d\n", sistema->estatisticas.totalProdutosVendidos);
    fprintf(ficheiro, "Valor total vendido: %.2f\n", sistema->estatisticas.totalValorVendido);
    fprintf(ficheiro, "Produtos oferecidos: %d\n", sistema->estatisticas.totalProdutosOferecidos);
    fprintf(ficheiro, "Valor total oferecido: %.2f\n", sistema->estatisticas.totalValorOferecido);
    fprintf(ficheiro, "Receita liquida: %.2f\n", sistema->estatisticas.totalValorVendido - sistema->estatisticas.totalValorOferecido);

    fprintf(ficheiro, "\n%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "FILAS E ESPERA\n");
    fprintf(ficheiro, "%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "Mudancas de fila: %d\n", sistema->estatisticas.totalMudancasFila);
    fprintf(ficheiro, "Soma tempos espera: %.2f\n", sistema->estatisticas.somaTemposEspera);
    fprintf(ficheiro, "Tempo medio espera: %.2f\n", sistema->estatisticas.tempoMedioEspera);

    fprintf(ficheiro, "\n%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "CAIXAS\n");
    fprintf(ficheiro, "%s", LINHA_SEPARADORA);
    fprintf(ficheiro, "Aberturas automaticas: %d\n", sistema->estatisticas.totalAberturasAutomaticas);
    fprintf(ficheiro, "Encerramentos automaticos: %d\n", sistema->estatisticas.totalEncerramentosAutomaticos);
    fprintf(ficheiro, "Caixa com mais clientes: %d\n", sistema->estatisticas.idCaixaMaisClientes);
    fprintf(ficheiro, "Caixa com mais produtos: %d\n", sistema->estatisticas.idCaixaMaisProdutos);

    operador = obterColaboradorPorId(
        (SISTEMA *)sistema,
        sistema->estatisticas.idOperadorMenosAtendimentos
    );

    operadorMais = obterColaboradorPorId(
        (SISTEMA *)sistema,
        sistema->estatisticas.idOperadorMaisAtendimentos
    );

    if (operador != NULL) {
        fprintf(
            ficheiro,
            "Operador com menos atendimentos: %d - %s (%d atendimentos)\n",
            operador->id,
            operador->nome,
            operador->clientesAtendidos
        );
    } else {
        fprintf(
            ficheiro,
            "Operador com menos atendimentos: %d\n",
            sistema->estatisticas.idOperadorMenosAtendimentos
        );
    }

    if (operadorMais != NULL) {
        fprintf(
            ficheiro,
            "Operador com mais atendimentos: %d - %s (%d atendimentos)\n",
            operadorMais->id,
            operadorMais->nome,
            operadorMais->clientesAtendidos
        );
    } else {
        fprintf(
            ficheiro,
            "Operador com mais atendimentos: %d\n",
            sistema->estatisticas.idOperadorMaisAtendimentos
        );
    }

    if (clienteMaisGastou != NULL) {
    char nomeClienteMais[MAX_NOME] = "Desconhecido";

    for (i = 0; i < sistema->baseClientes.tamanho; i++) {
        if (sistema->baseClientes.dados[i].id == clienteMaisGastou->id) {
            copiarStringSeguro(nomeClienteMais, sistema->baseClientes.dados[i].nome, MAX_NOME);
            break;
        }
    }

        fprintf(
            ficheiro,
            "Cliente que mais gastou: %06d - %s (%.2f)\n",
            clienteMaisGastou->id,
            nomeClienteMais,
            clienteMaisGastou->valorTotalCompras
        );
    } else {
        fprintf(ficheiro, "Cliente que mais gastou: nenhum cliente atendido\n");
    }

    if (clienteMenosGastou != NULL) {
    char nomeClienteMenos[MAX_NOME] = "Desconhecido";

    for (i = 0; i < sistema->baseClientes.tamanho; i++) {
        if (sistema->baseClientes.dados[i].id == clienteMenosGastou->id) {
            copiarStringSeguro(nomeClienteMenos, sistema->baseClientes.dados[i].nome, MAX_NOME);
            break;
        }
    }

    fprintf(
        ficheiro,
        "Cliente que menos gastou: %06d - %s (%.2f)\n",
        clienteMenosGastou->id,
        nomeClienteMenos,
        clienteMenosGastou->valorTotalCompras
    );
} else {
        fprintf(ficheiro, "Cliente que menos gastou: nenhum cliente atendido\n");
    }

    fclose(ficheiro);
    return 1;
}
// Garante capacidade suficiente na base de clientes, alocando ou expandindo memória conforme necessário
int garantirCapacidadeClientes(BASE_CLIENTES *base) {
    CLIENTE_BASE *novosDados;
    int novaCapacidade;

    if (base == NULL) {
        return 0;
    }

    if (base->dados == NULL) {
        base->dados = (CLIENTE_BASE *)malloc(sizeof(CLIENTE_BASE) * CAPACIDADE_INICIAL_CLIENTES_BASE);
        if (base->dados == NULL) {
            return 0;
        }

        base->capacidade = CAPACIDADE_INICIAL_CLIENTES_BASE;
        return 1;
    }

    if (base->tamanho < base->capacidade) {
        return 1;
    }

    novaCapacidade = base->capacidade + FATOR_CRESCIMENTO_VETORES;
    novosDados = (CLIENTE_BASE *)realloc(base->dados, sizeof(CLIENTE_BASE) * novaCapacidade);
    if (novosDados == NULL) {
        return 0;
    }

    base->dados = novosDados;
    base->capacidade = novaCapacidade;
    return 1;
}
// Garante capacidade suficiente na base de produtos, alocando ou expandindo memória conforme necessário
int garantirCapacidadeProdutos(BASE_PRODUTOS *base) {
    PRODUTO *novosDados;
    int novaCapacidade;

    if (base == NULL) {
        return 0;
    }

    if (base->dados == NULL) {
        base->dados = (PRODUTO *)malloc(sizeof(PRODUTO) * CAPACIDADE_INICIAL_PRODUTOS_BASE);
        if (base->dados == NULL) {
            return 0;
        }

        base->capacidade = CAPACIDADE_INICIAL_PRODUTOS_BASE;
        return 1;
    }

    if (base->tamanho < base->capacidade) {
        return 1;
    }

    novaCapacidade = base->capacidade + FATOR_CRESCIMENTO_VETORES;
    novosDados = (PRODUTO *)realloc(base->dados, sizeof(PRODUTO) * novaCapacidade);
    if (novosDados == NULL) {
        return 0;
    }

    base->dados = novosDados;
    base->capacidade = novaCapacidade;
    return 1;
}
// Garante capacidade suficiente na base de colaboradores, alocando ou expandindo memória conforme necessário
int garantirCapacidadeColaboradores(BASE_COLABORADORES *base) {
    COLABORADOR *novosDados;
    int novaCapacidade;

    if (base == NULL) {
        return 0;
    }

    if (base->dados == NULL) {
        base->dados = (COLABORADOR *)malloc(sizeof(COLABORADOR) * CAPACIDADE_INICIAL_COLABORADORES_BASE);
        if (base->dados == NULL) {
            return 0;
        }

        base->capacidade = CAPACIDADE_INICIAL_COLABORADORES_BASE;
        return 1;
    }

    if (base->tamanho < base->capacidade) {
        return 1;
    }

    novaCapacidade = base->capacidade + FATOR_CRESCIMENTO_VETORES;
    novosDados = (COLABORADOR *)realloc(base->dados, sizeof(COLABORADOR) * novaCapacidade);
    if (novosDados == NULL) {
        return 0;
    }

    base->dados = novosDados;
    base->capacidade = novaCapacidade;
    return 1;
}
// Gera um relatório detalhado de uma caixa em ficheiro, incluindo operador, clientes atendidos e estatísticas
// Gera um relatório detalhado de uma caixa em ficheiro, incluindo operador, clientes atendidos e estatísticas
int escreverRelatorioCaixa(SISTEMA *sistema, CAIXA *caixa, char *nomeFicheiro) {
    FILE *ficheiro;
    NO_HISTORICO_CLIENTE *atual;

    if (sistema == NULL || caixa == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return 0;
    }

    fprintf(ficheiro, "Caixa %d\n", caixa->id);

    if (caixa->operador != NULL) {
        fprintf(ficheiro, "Codigo do atendente: %d\n", caixa->operador->id);
        fprintf(ficheiro, "Nome do atendente: %s\n", caixa->operador->nome);
    } else {
        fprintf(ficheiro, "Codigo do atendente: N/A\n");
        fprintf(ficheiro, "Nome do atendente: N/A\n");
    }

    fprintf(ficheiro, "\nPessoas atendidas:\n");

    atual = caixa->historicoClientes.inicio;

    if (atual == NULL) {
        fprintf(ficheiro, "Nenhum cliente atendido.\n");
    } else {
        while (atual != NULL) {
            if (atual->cliente != NULL) {
                char nomeCliente[MAX_NOME] = "Desconhecido";
                int i;

                for (i = 0; i < sistema->baseClientes.tamanho; i++) {
                    if (sistema->baseClientes.dados[i].id == atual->cliente->id) {
                        copiarStringSeguro(
                            nomeCliente,
                            sistema->baseClientes.dados[i].nome,
                            MAX_NOME
                        );
                        break;
                    }
                }

                fprintf(
                    ficheiro,
                    "  %06d - %s\n",
                    atual->cliente->id,
                    nomeCliente
                );
            }

            atual = atual->seguinte;
        }
    }

    fprintf(ficheiro, "\nClientes atendidos: %d\n", caixa->clientesAtendidos);
    fprintf(ficheiro, "Produtos vendidos: %d\n", caixa->totalProdutosVendidos);
    fprintf(ficheiro, "Valor vendido: %.2f\n", caixa->totalValorVendido);
    fprintf(ficheiro, "Produtos oferecidos: %d\n", caixa->totalProdutosOferecidos);
    fprintf(ficheiro, "Valor oferecido: %.2f\n", caixa->totalValorOferecido);

    fclose(ficheiro);
    return 1;
}
// Gera relatórios individuais para todas as caixas, criando um ficheiro para cada uma
void escreverRelatoriosTodasCaixas(SISTEMA *sistema) {
    int i;
    char nomeFicheiro[TAM_NOME_FICHEIRO];

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        snprintf(
            nomeFicheiro,
            TAM_NOME_FICHEIRO,
            "%s%s%d%s",
            PASTA_RELATORIOS,
            PREFIXO_FICHEIRO_CAIXA,
            sistema->caixas[i].id,
            EXTENSAO_TXT
        );

        escreverRelatorioCaixa(sistema, &sistema->caixas[i], nomeFicheiro);
    }
}
// Remove todos os ficheiros da pasta de relatórios e recria a pasta limpa
void limparPastaRelatorios(void) {
    char comando[TAM_NOME_FICHEIRO * 2];

    snprintf(
        comando,
        sizeof(comando),
        "rmdir /s /q \"%s\"",
        PASTA_RELATORIOS
    );

    system(comando);
    _mkdir(PASTA_RELATORIOS);
}
// Conta o número de clientes válidos presentes no ficheiro de clientes
int contarClientesValidosFicheiro(char *nomeFicheiro) {
    FILE *ficheiro;
    char linha[256];
    int total = 0;
    CLIENTE_BASE cliente;
    int lidos;

    if (nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        return 0;
    }

    if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
        fclose(ficheiro);
        return 0;
    }

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        cliente.id = 0;
        cliente.nome[0] = '\0';

        lidos = sscanf(linha, "%d %39[^\n]", &cliente.id, cliente.nome);

        if (lidos == 2) {
            total++;
        }
    }

    fclose(ficheiro);
    return total;
}
// Conta o número de produtos válidos presentes no ficheiro de produtos
int contarProdutosValidosFicheiro(char *nomeFicheiro) {
    FILE *ficheiro;
    char linha[256];
    int total = 0;

    if (nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        return 0;
    }

    if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
        fclose(ficheiro);
        return 0;
    }

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        int id, tempoProcura, tempoPagamento;
        float preco;
        char nomeTemp[128];
        int lidos;
        int pos;

        linha[strcspn(linha, "\r\n")] = '\0';

        pos = (int)strlen(linha) - 1;
        if (pos < 0) {
            continue;
        }

        while (pos >= 0 && (linha[pos] == ' ' || linha[pos] == '\t')) pos--;
        if (pos < 0) continue;
        while (pos >= 0 && linha[pos] != ' ' && linha[pos] != '\t') pos--;
        tempoPagamento = atoi(&linha[pos + 1]);

        while (pos >= 0 && (linha[pos] == ' ' || linha[pos] == '\t')) pos--;
        if (pos < 0) continue;
        while (pos >= 0 && linha[pos] != ' ' && linha[pos] != '\t') pos--;
        tempoProcura = atoi(&linha[pos + 1]);

        while (pos >= 0 && (linha[pos] == ' ' || linha[pos] == '\t')) pos--;
        if (pos < 0) continue;
        while (pos >= 0 && linha[pos] != ' ' && linha[pos] != '\t') pos--;
        preco = (float)atof(&linha[pos + 1]);

        if (pos < 0) {
            continue;
        }

        linha[pos] = '\0';

        lidos = sscanf(linha, "%d %127[^\n]", &id, nomeTemp);

        if (lidos == 2) {
            total++;
        }
    }

    fclose(ficheiro);
    return total;
}
// Conta o número de colaboradores válidos presentes no ficheiro de colaboradores
int contarColaboradoresValidosFicheiro(char *nomeFicheiro) {
    FILE *ficheiro;
    char linha[256];
    int total = 0;
    COLABORADOR colaborador;
    int lidos;

    if (nomeFicheiro == NULL) {
        return 0;
    }

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        return 0;
    }

    if (fgets(linha, sizeof(linha), ficheiro) == NULL) {
        fclose(ficheiro);
        return 0;
    }

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        colaborador.id = 0;
        colaborador.nome[0] = '\0';

        lidos = sscanf(linha, "%d %39[^\n]", &colaborador.id, colaborador.nome);

        if (lidos == 2) {
            total++;
        }
    }

    fclose(ficheiro);
    return total;
}