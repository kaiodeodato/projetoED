#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "config.h"

// Carrega a configuração a partir de um ficheiro, aplicando valores padrão, leitura e validação final
int carregarConfiguracao(CONFIGURACAO *config, char *nomeFicheiro) {
    FILE *ficheiro;
    char chave[64];
    char valor[64];

    if (config == NULL || nomeFicheiro == NULL) {
        return 0;
    }

    aplicarValoresDefault(config);

    ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        return 0;
    }

    while (lerParConfiguracao(ficheiro, chave, valor)) {
        aplicarValorConfiguracao(config, chave, valor);
    }

    fclose(ficheiro);

    return validarConfiguracao(config);
}
// Valida os parâmetros da configuração, garantindo que todos os valores estão dentro de limites aceitáveis
int validarConfiguracao(CONFIGURACAO *config) {
    if (config == NULL) {
        return 0;
    }

    if (config->MAX_ESPERA <= 0) {
        return 0;
    }

    if (config->N_CAIXAS <= 0) {
        return 0;
    }

    if (config->TEMPO_ATENDIMENTO_PRODUTO < 2) {
        return 0;
    }

    if (config->MAX_PRECO <= 0.0f) {
        return 0;
    }

    if (config->MAX_FILA < 0) {
        return 0;
    }

    if (config->MIN_FILA < 0) {
        return 0;
    }

    if (config->MIN_FILA > config->MAX_FILA) {
        return 0;
    }

    if (config->DIFERENCA_TEMPO_CAIXAS < 0) {
        return 0;
    }

    if (config->MAX_PRODUTOS_CLIENTE <= 0) {
        return 0;
    }

    if (config->MIN_NOVOS_CLIENTES_POR_CICLO < 0) {
        return 0;
    }

    if (config->MAX_NOVOS_CLIENTES_POR_CICLO < config->MIN_NOVOS_CLIENTES_POR_CICLO) {
        return 0;
    }

    return 1;
}
// Define os valores padrão da configuração do sistema
void aplicarValoresDefault(CONFIGURACAO *config) {
    if (config == NULL) {
        return;
    }

    config->MAX_ESPERA = 120;
    config->N_CAIXAS = 6;
    config->TEMPO_ATENDIMENTO_PRODUTO = 6;
    config->MAX_PRECO = 40.0f;
    config->MAX_FILA = 7;
    config->MIN_FILA = 3;
    config->DIFERENCA_TEMPO_CAIXAS = DIFERENCA_TEMPO_CAIXAS_DEFAULT;
    config->MAX_PRODUTOS_CLIENTE = MAX_PRODUTOS_CLIENTE_DEFAULT;
    config->MIN_NOVOS_CLIENTES_POR_CICLO = MIN_NOVOS_CLIENTES_POR_CICLO_DEFAULT;
    config->MAX_NOVOS_CLIENTES_POR_CICLO = MAX_NOVOS_CLIENTES_POR_CICLO_DEFAULT;
}
// Lê um par chave-valor do ficheiro de configuração
int lerParConfiguracao(FILE *ficheiro, char *chave, char *valor) {
    if (ficheiro == NULL || chave == NULL || valor == NULL) {
        return 0;
    }

    return fscanf(ficheiro, "%63s %63s", chave, valor) == 2;
}
// Aplica um valor à configuração com base na chave lida do ficheiro
void aplicarValorConfiguracao(CONFIGURACAO *config, char *chave, char *valor) {
    if (config == NULL || chave == NULL || valor == NULL) {
        return;
    }

    if (strcmp(chave, "MAX_ESPERA") == 0) {
        config->MAX_ESPERA = atoi(valor);
    } else if (strcmp(chave, "N_CAIXAS") == 0) {
        config->N_CAIXAS = atoi(valor);
    } else if (strcmp(chave, "TEMPO_ATENDIMENTO_PRODUTO") == 0) {
        config->TEMPO_ATENDIMENTO_PRODUTO = atoi(valor);
    } else if (strcmp(chave, "MAX_PRECO") == 0) {
        config->MAX_PRECO = (float)atof(valor);
    } else if (strcmp(chave, "MAX_FILA") == 0) {
        config->MAX_FILA = atoi(valor);
    } else if (strcmp(chave, "MIN_FILA") == 0) {
        config->MIN_FILA = atoi(valor);
    } else if (strcmp(chave, "DIFERENCA_TEMPO_CAIXAS") == 0) {
        config->DIFERENCA_TEMPO_CAIXAS = atoi(valor);
    } else if (strcmp(chave, "MAX_PRODUTOS_CLIENTE") == 0) {
        config->MAX_PRODUTOS_CLIENTE = atoi(valor);
    } else if (strcmp(chave, "MIN_NOVOS_CLIENTES_POR_CICLO") == 0) {
        config->MIN_NOVOS_CLIENTES_POR_CICLO = atoi(valor);
    } else if (strcmp(chave, "MAX_NOVOS_CLIENTES_POR_CICLO") == 0) {
        config->MAX_NOVOS_CLIENTES_POR_CICLO = atoi(valor);
    }
}