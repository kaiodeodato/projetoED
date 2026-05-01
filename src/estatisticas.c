#include <string.h>
#include "define.h"
#include "estatisticas.h"
#include "colaboradores.h"

// Inicializa todas as estatísticas da simulação com valores padrão
void inicializarEstatisticas(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->tempoSimulacao = 0;
    estatisticas->totalClientesGerados = 0;
    estatisticas->totalClientesAtendidos = 0;
    estatisticas->totalProdutosVendidos = 0;
    estatisticas->totalValorVendido = 0.0f;
    estatisticas->totalProdutosOferecidos = 0;
    estatisticas->totalValorOferecido = 0.0f;
    estatisticas->totalMudancasFila = 0;
    estatisticas->totalAberturasAutomaticas = 0;
    estatisticas->totalEncerramentosAutomaticos = 0;
    estatisticas->somaTemposEspera = 0.0f;
    estatisticas->tempoMedioEspera = 0.0f;
    estatisticas->idCaixaMaisClientes = INDICE_INVALIDO;
    estatisticas->idCaixaMaisProdutos = INDICE_INVALIDO;
    estatisticas->idOperadorMenosAtendimentos = INDICE_INVALIDO;
    estatisticas->idOperadorMaisAtendimentos  = INDICE_INVALIDO;
}
// Incrementa o contador de clientes gerados na simulação
void registarClienteGerado(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalClientesGerados++;
}
// Incrementa o contador de clientes atendidos na simulação
void registarClienteAtendido(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalClientesAtendidos++;
}
// Regista a venda de produtos, atualizando quantidade total e valor total vendido
void registarProdutoVendido(ESTATISTICAS *estatisticas, int quantidade, float valor) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalProdutosVendidos += quantidade;
    estatisticas->totalValorVendido += valor;
}
// Regista um produto oferecido, incrementando a contagem e o valor total das ofertas
void registarProdutoOferecido(ESTATISTICAS *estatisticas, float valor) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalProdutosOferecidos++;
    estatisticas->totalValorOferecido += valor;
}
// Incrementa o contador de mudanças de fila realizadas pelos clientes
void registarMudancaFila(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalMudancasFila++;
}
// Incrementa o contador de aberturas automáticas de caixas
void registarAberturaAutomatica(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalAberturasAutomaticas++;
}
// Incrementa o contador de encerramentos automáticos de caixas
void registarEncerramentoAutomatico(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalEncerramentosAutomaticos++;
}
// Acumula o tempo de espera total para cálculo posterior da média
void atualizarTempoEspera(ESTATISTICAS *estatisticas, int tempoEspera) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->somaTemposEspera += tempoEspera;
}
// Calcula o tempo médio de espera com base na soma dos tempos e no número de clientes atendidos
void calcularTempoMedioEspera(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    if (estatisticas->totalClientesAtendidos <= 0) {
        estatisticas->tempoMedioEspera = 0.0f;
        return;
    }

    estatisticas->tempoMedioEspera =
        estatisticas->somaTemposEspera / estatisticas->totalClientesAtendidos;
}
// Atualiza estatísticas agregadas das caixas, como a que mais atendeu clientes e produtos
void atualizarEstatisticasCaixas(SISTEMA *sistema) {
    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    atualizarMelhorCaixaClientes(sistema);
    atualizarMelhorCaixaProdutos(sistema);
}
// Determina o operador com menos atendimentos e regista o seu id nas estatísticas
void determinarOperadorMenosAtendimentos(SISTEMA *sistema) {
    int i;
    COLABORADOR *pior = NULL;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    sistema->estatisticas.idOperadorMenosAtendimentos = -1;

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        COLABORADOR *atual = sistema->caixas[i].operador;

        if (atual == NULL) {
            continue;
        }

        if (pior == NULL || atual->clientesAtendidos < pior->clientesAtendidos) {
            pior = atual;
        }
    }

    if (pior != NULL) {
        sistema->estatisticas.idOperadorMenosAtendimentos = pior->id;
    }
}
// Determina o operador com mais atendimentos e regista o seu id nas estatísticas
void determinarOperadorMaisAtendimentos(SISTEMA *sistema) {
    int i;
    COLABORADOR *melhor = NULL;

    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    sistema->estatisticas.idOperadorMaisAtendimentos = INDICE_INVALIDO;

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        COLABORADOR *atual = sistema->caixas[i].operador;

        if (atual == NULL) {
            continue;
        }

        if (melhor == NULL || atual->clientesAtendidos > melhor->clientesAtendidos) {
            melhor = atual;
        }
    }

    if (melhor != NULL) {
        sistema->estatisticas.idOperadorMaisAtendimentos = melhor->id;
    }
}
// Identifica a caixa com mais clientes atendidos e regista o seu id nas estatísticas
void atualizarMelhorCaixaClientes(SISTEMA *sistema) {
    int i;
    int maxClientes = -1;
    int id = INDICE_INVALIDO;

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];

        if (caixa->clientesAtendidos > maxClientes) {
            maxClientes = caixa->clientesAtendidos;
            id = caixa->id;
        }
    }

    sistema->estatisticas.idCaixaMaisClientes = id;
}
// Identifica a caixa com maior número de produtos vendidos e regista o seu id nas estatísticas
void atualizarMelhorCaixaProdutos(SISTEMA *sistema) {
    int i;
    int maxProdutos = -1;
    int id = INDICE_INVALIDO;

    for (i = 0; i < sistema->config.N_CAIXAS; i++) {
        CAIXA *caixa = &sistema->caixas[i];

        if (caixa->totalProdutosVendidos > maxProdutos) {
            maxProdutos = caixa->totalProdutosVendidos;
            id = caixa->id;
        }
    }

    sistema->estatisticas.idCaixaMaisProdutos = id;
}