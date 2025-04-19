

typedef enum {
    casaVazia,     // Casa vazia (ainda não processada)
    casaBranca,     // Casa branca (letra maiúscula)
    casaRiscada    // Casa riscada (#)
} EstadoCasa;

typedef struct {
    char simbolo;    // Símbolo original (minúscula)
    EstadoCasa estado;
} Casa;

typedef struct {
    Casa tabuleiro[tamanho][tamanho];
} Tabuleiro;


