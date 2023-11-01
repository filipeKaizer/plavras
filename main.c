// l123b - t2 (acerte a letra)
// programa para digitar letras dentro do limite de tempo
// Filipe Sacchet Kaizer
// 2023-10-17

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "tecla.h"
#include "tela.h"

// quantas palavras a gerar
#define N_PAL 10
// quantos segundos para digitar
#define TEMPO 20
// Quantas palavras tem no arquivo
#define MAX_PAL 920

// Verifica se a letra c informada é a primeira do vetor. Se for, remove.
int remove_letra(char v[16], char p);
// Remove uma palavra do vetor
void remove_palavra(char palavras[][16], int posicao[N_PAL], int digitacao[N_PAL], int ativacao[N_PAL], int index);
// Seleciona a palavra que será removida, se for a primeira letra
int seleciona_palavra(char v[][16], char letra, int ativacao[N_PAL], double tempoInicial);
// Testa a palavra
bool testa_palavra(char v[]);
// apresenta o programa
void apresentacao();
// encerra o programa
void encerramento();
// apresenta a situação corrente do jogo e verifica se e para parar.
bool terminou(double timeIni, int n_palavras, char palavras[][16], int digitacao[N_PAL], int ativacao[N_PAL]);
// executa uma partida
void jogo();
// verifica a vontade do jogador
bool quer_jogar_de_novo();
// limpa a linha de entrada
void espera_enter();
// abre arquivo e sorteia palavras
bool sorteia_palavras(char palavras[N_PAL][16], int posicao[N_PAL], int digitacao[N_PAL], int ativacao[N_PAL]);
// atualiza a tela do jogo com as palavras e o tempo
void desenha_tela(char palavras[][16], int posicao[N_PAL], int digitacao[N_PAL], int ativacao[N_PAL], int palavraSelecionada, double tempoRestante);

int main()
{
  // inicializa o gerador de números aleatórios
  srand(time(0));

  apresentacao();

  do {    
    tela_ini();
    tecla_ini();

    jogo();
    
    tecla_fim();
    tela_cor_normal();
    tela_fim();
  } while(quer_jogar_de_novo());

  encerramento();
}

void jogo()
{
  // inicializa o vetor de números a digitar
  int n_palavras = N_PAL;
  // inicializa tempo
  double tempo_inicial = tela_relogio();
  // Indice para determinar a palavra a ser digitada
  int palavraSelecionada = -1;
  // Matriz de palavras
  char palavras[N_PAL][16];
  // Vetor de ativações
  int ativacao[N_PAL];
  // Vetor de tempo de digitação
  int digitacao[N_PAL];
  // Vetor de posição horizontal
  int posicao[N_PAL];
  

  if (!sorteia_palavras(palavras, posicao, digitacao, ativacao)) {
    return;
  }
  
  while (true) {
    if (terminou(tempo_inicial, n_palavras, palavras, digitacao, ativacao)) {
      break;
    }
      
    desenha_tela(palavras, posicao, digitacao, ativacao, palavraSelecionada, tempo_inicial);
    
    char letra='\0';
    letra = tecla_le_char();
        
    if (palavraSelecionada == -1 && letra != '\0') {
      palavraSelecionada = seleciona_palavra(palavras, letra, ativacao, tempo_inicial);
    }
    if (palavraSelecionada != -1 && remove_letra(palavras[palavraSelecionada], letra) == 1) {         
      remove_palavra(palavras, posicao, ativacao, digitacao, palavraSelecionada);
      palavraSelecionada = -1;      
      n_palavras--; 
    }
  }
}

void desenha_tela(char palavras[][16], int posicao[], int digitacao[], int ativacao[], int palavraSelecionada, double tempoInicial)
{
  tela_limpa();

  double tempoAtual = (tela_relogio() - tempoInicial);

  // Cabeçalho
  tela_lincol(0, tela_ncol() / 2 - 41 / 2);
  tela_cor_letra(0, 82, 204);
  printf("As palavras estão descendo, ELIMINE-AS!!!");

  // Limite superior em unicode
  tela_cor_letra(0, 204, 167);
  for (int i = 0; i < tela_ncol() + 1; i++) {
    tela_lincol(2, i);  
    printf("\u2500");  
  }
    
  // Imprime as palavras não selecionadas de acordo com o tempo de ativacao
  tela_cor_letra(0, 205, 10);
  for (int i = 0; palavras[i][0] != '\0'; i++) {
    if (ativacao[i] < tempoAtual && tempoAtual < (ativacao[i] + digitacao[i]) && i != palavraSelecionada) {
      int linha = 3 + (tela_nlin() - 3) * (tempoAtual - ativacao[i]) / digitacao[i];
      int coluna = (tela_ncol() - strlen(palavras[i])) * posicao[i] / 100;
      tela_lincol(linha, coluna);
      printf("%s-%d", palavras[i], i);
    } 
  } 

  // Imprime a palavra selecionada
  if (palavraSelecionada != -1) {
    tela_cor_letra(180, 22, 50);
    int linha = 3 + (tela_nlin() - 3) * (tempoAtual - ativacao[palavraSelecionada]) / digitacao[palavraSelecionada];
    int coluna = (tela_ncol() - strlen(palavras[palavraSelecionada])) * posicao[palavraSelecionada] / 100;
    
    tela_lincol(linha, coluna);
    printf("%s", palavras[palavraSelecionada]); 
  }
  // Limite inferior em unicode
  tela_cor_letra(0, 204, 167);
  for (int i = 0; i < tela_ncol() + 1; i++) {
    tela_lincol(tela_nlin(), i);  
    printf("\u2500");  
  }
  
  tela_atualiza();
}

void apresentacao()
{
  printf("Você deve digitar os números que aparecerão na tela.\n");
  printf("A ordem não é importante.\n");
  printf("Tecle <enter> para iniciar. ");
  espera_enter();
}

void encerramento()
{
  printf("Até a próxima.\n");
}

bool terminou(double timeIni, int n_palavras, char palavras[][16], int digitacao[], int ativacao[]) 
{
  double tempoAtual = (tela_relogio() - timeIni);

  bool terminou = false;
  for (int i = 0; i < palavras[i][0] != '\0'; i++){
    if (tempoAtual > (ativacao[i] + digitacao[i])) {
      terminou = true;
      break;
    }
  }
  
  if (terminou) {
    // Game over pelo tempo
    tela_limpa();
    
    int linha = tela_nlin() / 2;
    int coluna = tela_ncol() / 2 - 18 / 2;

    tela_cor_letra(205, 10, 0);
    tela_lincol(linha, coluna);
    printf("Tempo esgotado! :(");

    tela_atualiza();
    return true;
    
  } else if (n_palavras == 0) {
    tela_limpa();
    
    int linha = tela_nlin() / 2;
    int coluna = tela_ncol() / 2 - 11 / 2;

    tela_cor_letra(10, 205, 0);
    tela_lincol(linha, coluna);
    printf("PARABENS!!!");

    tela_atualiza();
    return true;
  } else {
    return false;
  }
}

bool quer_jogar_de_novo()
{
  // limpa a entrada
  espera_enter();

  printf("Digite 's' para jogar de novo ");
  while (true) {
    char c = getchar();
    if (c == '\n') {
      return false;
    } else if (c == 's' || c == 'S') {
      espera_enter();
      return true;
    }
  }
}

void espera_enter()
{
  while (getchar() != '\n') {
    ; /* não faz nada */
  }
}

int remove_letra(char v[16], char p)
{
  if (v[0] == p){
    char letraRemovida = p;
    int i = 0;
    if (v[1] == '\0') {
      v[0] = '\0';
      return 1;
    }
    for (/*Nada*/; v[i+1] != '\0'; i++){
      v[i] = v[i+1]; 
    }
    v[i] = '\0';
  }
  return 0;
}

void remove_palavra(char palavras[][16], int posicao[], int digitacao[], int ativacao[], int palavraIndex)
{
  int i=0;
  for (i = palavraIndex; palavras[i+1][0] != '\0' && i < MAX_PAL; i++) {
    strcpy(palavras[i], palavras[i+1]);
    posicao[i] = posicao[i+1];
    digitacao[i] = digitacao[i+1];
    ativacao[i] = ativacao[i+1];
  }
  //palavras[i+1][0] = '\0';
  palavras[i][0] = '\0';
}

bool sorteia_palavras(char palavras[][16], int posicao[], int digitacao[], int ativacao[])
{
  FILE *arquivo;
  char palavraLida[16];
  char palavrasArquivo[920][16];
 
  // Limpa vetor de palavras para evitar lixos
  for (int i = 0; i < N_PAL; i++) {
    for (int j = 0; j < 16; j++) {
      palavras[i][j] = '\0';
    }
  }

  arquivo = fopen("palavras", "r");
  
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return false;
  } else {
    //Lê todas as palavras do arquivo
    for (int i = 0; i< MAX_PAL; i++){
      fscanf(arquivo, "%s", palavraLida);
      strcpy(palavrasArquivo[i], palavraLida);
    }
    
    int i=0; 
    while (i < N_PAL) {
      int n = rand() % MAX_PAL;
      if (testa_palavra(palavrasArquivo[n])) {         
        // copia palavra aleatoria
        strcpy(palavras[i], palavrasArquivo[n]);

        // sorteia valores da palavra
        posicao[i] = rand() % 101;
        ativacao[i] = rand() % 21;
        digitacao[i] = (rand() % 26) + 5;
        
        i++;
      } 
    }
    fclose(arquivo);
    return true;
  }
}

bool testa_palavra(char palavra[])
{
  for (int i = 0; palavra[i] != '\0'; i++) {
    // Se houver maiuscula, troca para minuscula.
    if (palavra[i] >= 'A' && palavra[i] <= 'Z') {
      palavra[i] += 32; // As maiuscula - minusculas da tabela Ascii
    }
    
    if (!(palavra[i] >= 'a' && palavra[i] <= 'z')) {
      return false;
      break;
    }
  }
  return true;
}

int seleciona_palavra(char v[][16], char letra, int ativacao[N_PAL], double tempoInicial) 
{
  double tempoAtual = (tela_relogio() - tempoInicial);
  
  for (int i = 0; i < N_PAL; i++){
    if (v[i][0] == letra && ativacao[i] <= tempoAtual) {
      return i; // Achou uma, retorna seu indice
    }
  } 
  return -1; // Achou nenhuma, retorna -1
}
