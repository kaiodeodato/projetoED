#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uteis.h"
#include <locale.h>

void limparBufferEntrada(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void lerString(char *mensagem, char *destino, int tamanho) {
    if (destino == NULL || tamanho <= 0) {
        return;
    }

    if (mensagem != NULL) {
        printf("%s", mensagem);
    }

    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }

    destino[strcspn(destino, "\r\n")] = '\0';
}

void lerFloat(char *mensagem, float *destino, float min, float max) {
    float valor;
    int lido;

    if (destino == NULL) {
        return;
    }

    while (1) {
        if (mensagem != NULL) {
            printf("%s", mensagem);
        }

        lido = scanf("%f", &valor);

        if (lido != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }

        limparBufferEntrada();

        if (valor < min || valor > max) {
            printf("Valor fora do intervalo permitido.\n");
            continue;
        }

        *destino = valor;
        return;
    }
}

void lerInteiro(char *mensagem, int *destino, int min, int max) {
    int valor;
    int lido;

    if (destino == NULL) {
        return;
    }

    while (1) {
        if (mensagem != NULL) {
            printf("%s", mensagem);
        }

        lido = scanf("%d", &valor);

        if (lido != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }

        limparBufferEntrada();

        if (valor < min || valor > max) {
            printf("Valor fora do intervalo permitido.\n");
            continue;
        }

        *destino = valor;
        return;
    }
}

void limparTela() {
    system("cls");
}

void pausarTela() {
    printf("Prima ENTER para continuar...");
    getchar();
}

int Aleatorio(int min, int max) {
    if (min > max) {
        return min;
    }

    return min + rand() % (max - min + 1);
}

void gerarTimestampFormatado(char *buffer, int tamanho) {
    time_t agora;
    struct tm *infoTempo;

    if (buffer == NULL || tamanho <= 0) {
        return;
    }

    agora = time(NULL);
    infoTempo = localtime(&agora);

    if (infoTempo == NULL) {
        buffer[0] = '\0';
        return;
    }

    strftime(buffer, tamanho, "%Y%m%d_%H%M%S", infoTempo);
}

void copiarStringSeguro(char *destino, const char *origem, int tamanho) {
    if (destino == NULL || origem == NULL || tamanho <= 0) {
        return;
    }

    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

void gerarNomeFicheiroComTimestamp(char *destino, int tamanho, const char *prefixo, const char *extensao) {
    char timestamp[30];

    if (destino == NULL || tamanho <= 0 || prefixo == NULL || extensao == NULL) {
        return;
    }

    gerarTimestampFormatado(timestamp, sizeof(timestamp));
    snprintf(destino, tamanho, "%s%s%s", prefixo, timestamp, extensao);
}

void configurarAmbienteUTF8() {
        system("chcp 65001 > nul");
        setlocale(LC_ALL, "pt_PT.UTF-8");
}