#include <string.h>
#include "define.h"
#include "estatisticas.h"
#include "colaboradores.h"

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
}

void registarClienteGerado(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalClientesGerados++;
}

void registarClienteAtendido(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalClientesAtendidos++;
}

void registarProdutoVendido(ESTATISTICAS *estatisticas, int quantidade, float valor) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalProdutosVendidos += quantidade;
    estatisticas->totalValorVendido += valor;
}

void registarProdutoOferecido(ESTATISTICAS *estatisticas, float valor) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalProdutosOferecidos++;
    estatisticas->totalValorOferecido += valor;
}

void registarMudancaFila(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalMudancasFila++;
}

void registarAberturaAutomatica(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalAberturasAutomaticas++;
}

void registarEncerramentoAutomatico(ESTATISTICAS *estatisticas) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalEncerramentosAutomaticos++;
}

void atualizarTempoEspera(ESTATISTICAS *estatisticas, int tempoEspera) {
    if (estatisticas == NULL) {
        return;
    }

    estatisticas->somaTemposEspera += tempoEspera;
}

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

void atualizarEstatisticasCaixas(SISTEMA *sistema) {
    if (sistema == NULL || sistema->caixas == NULL) {
        return;
    }

    atualizarMelhorCaixaClientes(sistema);
    atualizarMelhorCaixaProdutos(sistema);
}

void determinarMelhoresCaixas(SISTEMA *sistema) {
    if (sistema == NULL) {
        return;
    }

    atualizarEstatisticasCaixas(sistema);
}

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