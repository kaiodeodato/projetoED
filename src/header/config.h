#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int carregarConfiguracao(CONFIGURACAO *config, char *nomeFicheiro);
int validarConfiguracao(CONFIGURACAO *config);
void aplicarValoresDefault(CONFIGURACAO *config);
int lerParConfiguracao(FILE *ficheiro, char *chave, char *valor);
void aplicarValorConfiguracao(CONFIGURACAO *config, char *chave, char *valor);

#endif