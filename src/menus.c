#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menus.h"
#include "uteis.h"
#include "historico.h"
#include "config.h"
#include "ficheiros.h"
#include "main.h"
#include "caixas.h"
#include "simulacao.h"
#include "define.h"
#include "clientes.h"
#include "produtos.h"
#include "hashClientes.h"
#include "colaboradores.h"
#include "memoria.h"

// Lê e retorna uma opção válida do menu dentro do intervalo especificado
int lerOpcaoMenu(int min, int max) {
    int opcao = 0;

    lerInteiro("Opcao: ", &opcao, min, max);
    return opcao;
}
// Mostra o menu principal na tela com as opções disponíveis ao utilizador
void mostrarMenuPrincipal() {
    limparTela();
    mostrarCabecalho("MENU PRINCIPAL");
    printf("1. Iniciar simulacao\n");
    printf("2. Retomar simulacao\n");
    printf("3. Encerrar simulacao\n");
    printf("4. Mostrar estado resumido\n");
    printf("5. Gestao administrativa\n");
    printf("6. Alterar velocidade da simulacao\n");
    printf("0. Sair\n");
    printf("%s", LINHA_SEPARADORA);
}
// Executa a opção escolhida no menu principal, controlando ações da simulação e gestão do sistema
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

        case 2:
            if (sistema->estadoSimulacao == SIMULACAO_PAUSADA) {
                retomarSimulacao(sistema);
                executarSimulacao(sistema);
            } else {
                printf("Nao ha simulacao pausada para retomar.\n");
                pausarTela();
            }
            break;

         case 3:
            encerrarSimulacao(sistema);
            gerarSaidasFinais(sistema);
            pausarTela();
            break;

        case 4:
            mostrarEstadoResumidoSistema(sistema);
            adicionarLog(&sistema->logs, sistema->tempoAtual, "MENU", "Mostrar Estado Resumido Sistema");
            break;

        case 5:
            executarMenuGestao(sistema);
            break;

        case 6:
            alterarVelocidadeSimulacao(sistema);
            adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Alterar Velocidade Simulação");
            break;

        case 0:
            adicionarLog(&sistema->logs, sistema->tempoAtual, "MENU", "Saida do programa");
            break;

        default:
            printf("Opcao invalida.\n");
            break;
    }
}
// Mostra o menu de gestão administrativa com opções de configuração e controlo do sistema
void mostrarMenuGestao() {
    limparTela();
    mostrarCabecalho("MENU DE GESTAO");
    printf("1. Adicionar cliente base\n");
    printf("2. Adicionar produto base\n");
    printf("3. Abrir caixa manualmente\n");
    printf("4. Fechar caixa manualmente\n");
    printf("5. Passar caixa a AUTO\n");
    printf("6. Pesquisar cliente ativo\n");
    printf("7. Listar todos os clientes\n");
    printf("8. Mostrar estatisticas da simulacao\n");
    printf("9. Mostrar relatorio de memoria\n");
    printf("0. Voltar\n");
    printf("%s", LINHA_SEPARADORA);
}
// Executa o menu de gestão, permitindo operações administrativas enquanto regista ações no log do sistema
void executarMenuGestao(SISTEMA *sistema) {
    int opcao = -1;

    if (sistema == NULL) {
        return;
    }

    while (opcao != 0) {
        mostrarMenuGestao();
        lerInteiro("Opcao: ", &opcao, 0, 9);

        switch (opcao) {
            case 1:
                adicionarClienteBaseMenu(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Adicionar Cliente");
                pausarTela();
                break;

            case 2:
                adicionarProdutoBaseMenu(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Adicionar Produto");
                pausarTela();
                break;

            case 3:
                abrirCaixaManual(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Abrir Caixa Manual");
                pausarTela();
                break;

            case 4:
                fecharCaixaManual(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Fechar Caixa Manual");
                pausarTela();
                break;
            
            case 5:
                colocarCaixaEmAutoUI(sistema);
                adicionarLog(&sistema->logs, sistema->tempoAtual, "MENU", "Caixa colocada em controlo automatico");
                pausarTela();
                break;

            case 6:
                pesquisarClienteAtivoMenu(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Pesquisar Cliente");
                pausarTela();
                break;

            case 7:
                listarTodosClientesMenu(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Listar Todos os Clientes");
                pausarTela();
                break;
            
            case 8:
                mostrarEstatisticasSimulacaoMenu(sistema);
                adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Mostrar Estatisticas Simulaçãoo");
                pausarTela();
                break;
            
            case 9:
                mostrarRelatorioMemoriaMenu(sistema);
                 adicionarLog(&sistema->logs,sistema->tempoAtual,"MENU","Mostrar Relatório Memoria Simulação");
                pausarTela();
                break;

            case 0:
                adicionarLog(&sistema->logs, sistema->tempoAtual, "MENU", "Saída do menu de gestão");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
}
// Adiciona um novo cliente à base de dados, garantindo capacidade dinâmica e gravando o resultado no ficheiro
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

        novosDados = (CLIENTE_BASE *)realloc(sistema->baseClientes.dados,sizeof(CLIENTE_BASE) * novaCapacidade);

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

    if (guardarBaseClientes(&sistema->baseClientes, FICHEIRO_CLIENTES)) {
        printf("Cliente base adicionado e guardado com sucesso.\n");
    } else {
        printf("Cliente base adicionado, mas erro ao guardar no ficheiro.\n");
    }
}
// Adiciona um novo produto à base de dados, gerindo memória dinâmica e persistindo os dados em ficheiro
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

        novosDados = (PRODUTO *)realloc(sistema->baseProdutos.dados,sizeof(PRODUTO) * novaCapacidade);

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

    if (guardarBaseProdutos(&sistema->baseProdutos, FICHEIRO_PRODUTOS)) {
        printf("Produto base adicionado e guardado com sucesso.\n");
    } else {
        printf("Produto base adicionado, mas erro ao guardar no ficheiro.\n");
    }
}
// Abre manualmente uma caixa selecionada pelo utilizador, registando a ação no sistema
void abrirCaixaManual(SISTEMA *sistema) {
    int idCaixa;

    if (sistema == NULL) {
        return;
    }

    if (sistema->config.N_CAIXAS <= 0) {
        printf("Nao existem caixas configuradas.\n");
        return;
    }

    lerInteiro("Id da caixa a abrir: ",&idCaixa,0,sistema->config.N_CAIXAS - 1);

    if (abrirCaixa(sistema, idCaixa, CAIXA_COM_CONTROLO_MANUAL)) {
        adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Caixa aberta manualmente");
        printf("Caixa %d aberta com sucesso.\n", idCaixa);
    } else {
        printf("Nao foi possivel abrir a caixa %d.\n", idCaixa);
    }
}
// Coloca uma caixa em encerramento manual, registando a ação e validando a operação
void fecharCaixaManual(SISTEMA *sistema) {
    int idCaixa;

    if (sistema == NULL) {
        return;
    }

    if (sistema->config.N_CAIXAS <= 0) {
        printf("Nao existem caixas configuradas.\n");
        return;
    }

    lerInteiro("Id da caixa a fechar: ",&idCaixa,0,sistema->config.N_CAIXAS - 1);

    if (encerrarCaixa(sistema, idCaixa, CAIXA_COM_CONTROLO_MANUAL)) {
        adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Caixa colocada em encerramento manual");
        printf("Caixa %d em encerramento.\n", idCaixa);
    } else {
        printf("Nao foi possivel encerrar a caixa %d.\n", idCaixa);
    }
}
// Imprime um cabeçalho formatado com um título e linhas separadoras no terminal
void mostrarCabecalho(char *titulo) {
    printf("\n%s", LINHA_SEPARADORA);
    if (titulo != NULL) {
        printf("%s\n", titulo);
    }
    printf("%s", LINHA_SEPARADORA);
}
// Mostra um resumo do estado atual da simulação, incluindo tempo, estatísticas e estado das caixas
void mostrarEstadoResumidoSistema(SISTEMA *sistema) {
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
    printf("Clientes nas caixas:     %d\n", contarClientesNasCaixas(sistema));

    printf("\n%s", LINHA_SEPARADORA);
    printf("ESTADO DAS CAIXAS\n");
    printf("%s", LINHA_SEPARADORA);

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            printf(
                "Caixa %d | Estado=%d | Abertura=%s | Fila=%d | Atendimento=%d | Total=%d | Atendidos=%d\n",
                sistema->caixas[i].id,
                sistema->caixas[i].estado,
                obterTextoControloCaixa(&sistema->caixas[i]),
                sistema->caixas[i].fila.tamanho,
                sistema->caixas[i].clienteAtual != NULL ? 1 : 0,
                sistema->caixas[i].fila.tamanho + (sistema->caixas[i].clienteAtual != NULL ? 1 : 0),
                sistema->caixas[i].clientesAtendidos
            );
        }
    }

    printf("\nLoading: -");
    printf("\nPrima 'P' para pausar e voltar ao menu.\n");

    pausarTela();
}
// Pesquisa e mostra os dados de um cliente ativo na hash, incluindo estado, caixa e produtos
void pesquisarClienteAtivoMenu(SISTEMA *sistema) {
    int idCliente;
    CLIENTE *cliente;

    if (sistema == NULL) {
        return;
    }

    lerInteiro("Id do cliente: ", &idCliente, 1, 999999999);

    cliente = procurarClienteHash(&sistema->clientesHash, idCliente);

    if (cliente == NULL) {
        printf("Cliente nao encontrado entre os clientes ativos.\n");
        return;
    }

    printf("Cliente encontrado:\n");
    printf("Id: %06d\n", cliente->id);
    printf("Nome: %s\n", cliente->nome);
    printf("Estado: ");

    if (cliente->estado == CLIENTE_A_COMPRAR) {
        printf("A comprar\n");
    } else if (cliente->estado == CLIENTE_EM_FILA) {
        printf("Em fila\n");
    } else if (cliente->estado == CLIENTE_EM_ATENDIMENTO) {
        printf("Em atendimento\n");
    } else if (cliente->estado == CLIENTE_ATENDIDO) {
        printf("Atendido\n");
    } else {
        printf("Desconhecido\n");
    }

    printf("Caixa: ");
    if (cliente->idCaixaAtual == ID_CAIXA_INVALIDO) {
        printf("Nenhuma\n");
    } else {
        printf("%d\n", cliente->idCaixaAtual);
    }
        
    printf("Numero de produtos: %d\n", cliente->nProdutos);

    int i;

    printf("\nProdutos:\n");

    for (i = 0; i < cliente->nProdutos; i++) {
        printf("  - %s (%.2f)€\n",
            cliente->produtos[i].nome,
            cliente->produtos[i].preco);
    }
}
// Lista todos os clientes ativos na hash com paginação, permitindo navegação entre páginas no menu
void listarTodosClientesMenu(SISTEMA *sistema) {
    CLIENTE **lista;
    BUCKET *bucketAtual;
    int total = 0;
    int i, pagina = 0;
    char opcao;

    if (sistema == NULL) {
        return;
    }

    if (sistema->clientesHash.nElementos == 0) {
        printf("Nao existem clientes ativos neste momento.\n");
        return;
    }

    lista = (CLIENTE **)malloc(sizeof(CLIENTE *) * sistema->clientesHash.nElementos);

    if (lista == NULL) {
        printf("Erro de memoria.\n");
        return;
    }

    bucketAtual = sistema->clientesHash.inicio;

    while (bucketAtual != NULL) {
        HASHNODE *noAtual = bucketAtual->clientes;

        while (noAtual != NULL) {
            if (noAtual->cliente != NULL) {
                lista[total++] = noAtual->cliente;
            }
            noAtual = noAtual->prox;
        }

        bucketAtual = bucketAtual->prox;
    }

    while (1) {
        limparTela();
        printf("\n%s", LINHA_SEPARADORA);
        printf("CLIENTES ATIVOS (Pagina %d)\n", pagina + 1);
        printf("%s", LINHA_SEPARADORA);

        int inicio = pagina * 30;
        int fim = inicio + 30;

        if (fim > total) {
            fim = total;
        }

        for (i = inicio; i < fim; i++) {
            CLIENTE *cliente = lista[i];

            printf("ID: %06d | Nome: %-50s", cliente->id, cliente->nome);

            if (cliente->idCaixaAtual != ID_CAIXA_INVALIDO) {
                printf(" | Estado: NA CAIXA | Caixa: %d\n", cliente->idCaixaAtual);
            } else {
                printf(" | Estado: A COMPRAR\n");
            }
        }

        printf("\nTotal: %d\n", total);
        printf("[A] Anterior | [P] Proxima | [S] Sair\n");

        scanf(" %c", &opcao);

        if (opcao == 'p' || opcao == 'P') {
            if ((pagina + 1) * 30 < total) {
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

    free(lista);
}
// Permite alterar a velocidade da simulação através de um menu de opções predefinidas
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
    printf(
        "Velocidade alterada com sucesso para %s.\n",
        obterTextoVelocidade(sistema->velocidadeSimulacao)
    );
    pausarTela();
}
// Converte o valor da velocidade da simulação para uma string legível
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
// Mostra o menu de estatísticas da simulação, calculando e exibindo métricas e dados de operadores e clientes
void mostrarEstatisticasSimulacaoMenu(SISTEMA *sistema) {
    COLABORADOR *operador;
    COLABORADOR *operadorMais;
    CLIENTE *clienteMaisGastou = NULL;
    CLIENTE *clienteMenosGastou = NULL;
    int i;

    if (sistema == NULL) {
        return;
    }

    atualizarEstatisticasSimulacao(sistema);

    if (sistema->caixas != NULL) {
        for (i = 0; i < sistema->config.N_CAIXAS; i++) {
            NO_HISTORICO_CLIENTE *atual = sistema->caixas[i].historicoClientes.inicio;

            while (atual != NULL) {
                CLIENTE *cliente = atual->cliente;

                if (cliente != NULL) {
                    if (clienteMaisGastou == NULL || cliente->valorTotalCompras > clienteMaisGastou->valorTotalCompras) {
                        clienteMaisGastou = cliente;
                    }

                    if (clienteMenosGastou == NULL || cliente->valorTotalCompras < clienteMenosGastou->valorTotalCompras) {
                        clienteMenosGastou = cliente;
                    }
                }

                atual = atual->seguinte;
            }
        }
    }
    limparTela();
    mostrarCabecalho("ESTATISTICAS DA SIMULACAO");

    printf("Tempo de simulacao: %d\n", sistema->estatisticas.tempoSimulacao);
    printf("Clientes gerados: %d\n", sistema->estatisticas.totalClientesGerados);
    printf("Clientes atendidos: %d\n", sistema->estatisticas.totalClientesAtendidos);
    printf("Produtos vendidos: %d\n", sistema->estatisticas.totalProdutosVendidos);
    printf("Valor total vendido: %.2f\n", sistema->estatisticas.totalValorVendido);
    printf("Produtos oferecidos: %d\n", sistema->estatisticas.totalProdutosOferecidos);
    printf("Valor total oferecido: %.2f\n", sistema->estatisticas.totalValorOferecido);
    printf("Receita liquida: %.2f\n",sistema->estatisticas.totalValorVendido - sistema->estatisticas.totalValorOferecido);
    printf("Mudancas de fila: %d\n", sistema->estatisticas.totalMudancasFila);
    printf("Aberturas automaticas: %d\n", sistema->estatisticas.totalAberturasAutomaticas);
    printf("Encerramentos automaticos: %d\n", sistema->estatisticas.totalEncerramentosAutomaticos);
    printf("Soma tempos espera: %.2f\n", sistema->estatisticas.somaTemposEspera);
    printf("Tempo medio espera: %.2f\n", sistema->estatisticas.tempoMedioEspera);
    printf("Caixa com mais clientes: %d\n", sistema->estatisticas.idCaixaMaisClientes);
    printf("Caixa com mais produtos: %d\n", sistema->estatisticas.idCaixaMaisProdutos);

    operador = obterColaboradorPorId(
        sistema,
        sistema->estatisticas.idOperadorMenosAtendimentos
    );

      operadorMais = obterColaboradorPorId(
        (SISTEMA *)sistema,
        sistema->estatisticas.idOperadorMaisAtendimentos
    );


    if (operador != NULL) {
        printf(
            "Operador com menos atendimentos: %d - %s (%d atendimentos)\n",
            operador->id,
            operador->nome,
            operador->clientesAtendidos
        );
    } else {
        printf(
            "Operador com menos atendimentos: %d\n",
            sistema->estatisticas.idOperadorMenosAtendimentos
        );
    }

    if (operadorMais != NULL) {
        printf(
            "Operador com mais atendimentos: %d - %s (%d atendimentos)\n",
            operadorMais->id,
            operadorMais->nome,
            operadorMais->clientesAtendidos
        );
    } else {
        printf(
            "Operador com mais atendimentos: %d\n",
            sistema->estatisticas.idOperadorMaisAtendimentos
        );
    }

    if (clienteMaisGastou != NULL) {
        printf(
            "Cliente que mais gastou: %06d - %s (%.2f)\n",
            clienteMaisGastou->id,
            clienteMaisGastou->nome,
            clienteMaisGastou->valorTotalCompras
        );
    } else {
        printf("Cliente que mais gastou: nenhum cliente atendido\n");
    }

    if (clienteMenosGastou != NULL) {
        printf(
            "Cliente que menos gastou: %06d - %s (%.2f)\n",
            clienteMenosGastou->id,
            clienteMenosGastou->nome,
            clienteMenosGastou->valorTotalCompras
        );
    } else {
        printf("Cliente que menos gastou: nenhum cliente atendido\n");
    }
}
// Mostra no menu o relatório de memória estimada do sistema, detalhando consumo por estruturas
void mostrarRelatorioMemoriaMenu(SISTEMA *sistema) {
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

    if (sistema == NULL) {
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

    limparTela();
    mostrarCabecalho("RELATORIO DE MEMORIA");

    printf("Memoria total estimada das estruturas do sistema: %zu bytes\n", memoriaTotal);
    printf("Memoria desperdicada estimada nas bases dinamicas: %zu bytes\n\n", memoriaDesperdicada);

    printf("Detalhe por estrutura:\n");
    printf("  Bases carregadas: %zu bytes\n", calcularMemoriaBases(sistema));
    printf("  Caixas (vetor + historicos): %zu bytes\n", memoriaCaixas);
    printf("  Filas: %zu bytes\n", memoriaFilas);
    printf("  Hash de clientes: %zu bytes\n", memoriaHash);
    printf("  Lista de compras: %zu bytes\n", memoriaListaCompras);
    printf("  Logs: %zu bytes\n", memoriaLogs);
    printf("  Clientes ativos e produtos associados: %zu bytes\n", memoriaClientesAtivos);
}
// Interface para colocar uma caixa em modo automático, registando a ação no log do sistema
int colocarCaixaEmAutoUI(SISTEMA *sistema) {
    int idCaixa;

    if (sistema == NULL) {
        printf("Sistema invalido.\n");
        return 0;
    }

    if (sistema->config.N_CAIXAS <= 0) {
        printf("Nao existem caixas configuradas.\n");
        return 0;
    }

    lerInteiro("Id da caixa: ",&idCaixa,0,sistema->config.N_CAIXAS - 1);

    if (colocarCaixaEmAuto(sistema, idCaixa)) {
        adicionarLog(&sistema->logs, sistema->tempoAtual, "GESTAO", "Caixa colocada em controlo automatico");
        return 1;
    }

    printf("Nao foi possivel alterar a caixa %d.\n", idCaixa);
    return 0;
}