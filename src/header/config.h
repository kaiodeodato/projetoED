#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int carregarConfiguracao(CONFIGURACAO *config, const char *nomeFicheiro);
int validarConfiguracao(const CONFIGURACAO *config);
void aplicarValoresDefault(CONFIGURACAO *config);
int lerParConfiguracao(FILE *ficheiro, char *chave, char *valor);
void aplicarValorConfiguracao(CONFIGURACAO *config, const char *chave, const char *valor);

#endif