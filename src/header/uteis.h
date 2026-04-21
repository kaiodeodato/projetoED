#ifndef UTEIS_H
#define UTEIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void limparBufferEntrada(void);
void lerString(char *mensagem, char *destino, int tamanho);
void lerFloat(char *mensagem, float *destino, float min, float max);
void lerInteiro(char *mensagem, int *destino, int min, int max);
void limparTela();
void pausarTela();
int Aleatorio(int min, int max);
void gerarTimestampFormatado(char *buffer, int tamanho);
int compararStrings(const char *a, const char *b);
void copiarStringSeguro(char *destino, const char *origem, int tamanho);
int stringVazia(const char *str);
void gerarNomeFicheiroComTimestamp(char *destino, int tamanho, const char *prefixo, const char *extensao);
void configurarAmbienteUTF8();

#endif
