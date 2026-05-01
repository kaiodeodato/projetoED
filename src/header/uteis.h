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
void gerarTimestampFormatado(char *buffer, int tamanho);
void copiarStringSeguro(char *destino, char *origem, int tamanho);
void gerarNomeFicheiroComTimestamp(char *destino, int tamanho, char *prefixo, char *extensao);
void configurarAmbienteUTF8();
int Aleatorio(int min, int max);

#endif
