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
void copiarStringSeguro(char *destino, const char *origem, int tamanho);
void gerarNomeFicheiroComTimestamp(char *destino, int tamanho, const char *prefixo, const char *extensao);
void configurarAmbienteUTF8();

#endif
