#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "type.h"
#include "erros.h"

ESTADO estadoTeste;

void inicializarestadoTeste() {
    // Inicializa um tabuleiro  para testes (2x2)
    estadoTeste.linhas = 2;
    estadoTeste.colunas = 2;
    estadoTeste.tabuleiro = malloc(estadoTeste.linhas * sizeof(char *));
    for (int i = 0; i < estadoTeste.linhas; i++) {
        estadoTeste.tabuleiro[i] = malloc(estadoTeste.colunas * sizeof(char));
        for (int j = 0; j < estadoTeste.colunas; j++) {
            estadoTeste.tabuleiro[i][j] = 'a';
        }
    }
    estadoTeste.historico = NULL;
}

void limparEstadoTeste() {
    for (int i = 0; i < estadoTeste.linhas; i++) {
        free(estadoTeste.tabuleiro[i]);
    }
    free(estadoTeste.tabuleiro);
    libertarHistorico(&estadoTeste);
}

/**
 * @brief Testa todas as funções dos comandos com parâmetro cmd inválido.
 */

void testeComComandoInvalido() {
    ESTADO estadoTeste = {0};

    CU_ASSERT_EQUAL(ler('l',"nivel/5" ,&estadoTeste), E_OK);

    CU_ASSERT_EQUAL(gravar('x', "a1", &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(sair('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(ler('x', "a1", &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(riscar('x', "a1", &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(branco('x', "a1", &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(desfazer('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(verificar('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(autocompletar('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(autocompletarRec('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(resolver('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(limpa('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(desfazerCoordenada('x', "a1", &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(regras('x', NULL, &estadoTeste), E_CMD_INV);
    CU_ASSERT_EQUAL(help('x', NULL, &estadoTeste), E_CMD_INV);

    libertarMemoria(&estadoTeste);

}

/**
 * @brief Testa funções que deveriam receber argumento sem argumento.
 */
void testeComArgumentoVazio() {
    ESTADO estadoTeste = {0};

    CU_ASSERT_EQUAL(ler('l',"nivel/5" ,&estadoTeste), E_OK);

    CU_ASSERT_EQUAL(gravar('g', NULL, &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(ler('l', NULL, &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(riscar('r', NULL, &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(branco('b', NULL, &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(desfazerCoordenada('m', NULL, &estadoTeste), E_ARG);

    libertarMemoria(&estadoTeste);
}


/**
 * @brief Testa funções que não deveriam receber argumento com argumento.
 */
void testeComArgumento() {
    ESTADO estadoTeste = {0};

    CU_ASSERT_EQUAL(ler('l',"nivel/5" ,&estadoTeste), E_OK);

    CU_ASSERT_EQUAL(sair('s', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(desfazer('d', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(verificar('v', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(autocompletar('a', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(autocompletarRec('A', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(resolver('R', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(limpa('L', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(regras('w', "a1", &estadoTeste), E_ARG);
    CU_ASSERT_EQUAL(help('h', "a1", &estadoTeste), E_ARG);

    libertarMemoria(&estadoTeste);
      
}

/**
 * @brief Testa funções com tabuleiro não carregado.
 */
void testeComTabuleiroVazio() {
    ESTADO estado_vazio = {0};

    CU_ASSERT_EQUAL(riscar('r', "a1", &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(branco('b', "a1", &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(desfazer('d', NULL, &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(verificar('v', NULL, &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(autocompletar('a', NULL, &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(autocompletarRec('A', NULL, &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(resolver('R', NULL, &estado_vazio), E_TAB_NC);
    CU_ASSERT_EQUAL(desfazerCoordenada('m', "a1", &estado_vazio), E_TAB_NC);

    libertarMemoria(&estado_vazio);

}

/**
 * @brief Testa funções com formato de coordenadas inválido
 */
void testeComFormatoCoordenadasInvalido() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"nivel/5" ,&estado), E_OK);

    CU_ASSERT_EQUAL(riscar('r', "1A", &estado), E_COORD);
    CU_ASSERT_EQUAL(branco('b', "1A", &estado), E_COORD);
    CU_ASSERT_EQUAL(desfazerCoordenada('m', "1A", &estado), E_COORD);
    CU_ASSERT_EQUAL(riscar('r', "a1 2", &estado), E_COORD);
    CU_ASSERT_EQUAL(branco('b', "a1 2", &estado), E_COORD);
    CU_ASSERT_EQUAL(desfazerCoordenada('m', "a1 2", &estado), E_COORD);

    libertarMemoria(&estado);
}

/**
 * @brief Testa funções com coordenadas fora dos limites do tabuleiro.
 */
void testeComCoordenadasInvalidas() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"nivel/5" ,&estado), E_OK);

    CU_ASSERT_EQUAL(riscar('r', "z9", &estado), E_COORD);
    CU_ASSERT_EQUAL(branco('b', "z9", &estado), E_COORD);
    CU_ASSERT_EQUAL(desfazerCoordenada('m', "z9", &estado), E_COORD);

    libertarMemoria(&estado);
}


/**
 * @brief Testa função ler.
 */
void testarLer() {
    ESTADO estado = {0};
    ESTADO estado1 = {0};
    ESTADO estado2 = {0};
 
    // Ficheiro inválido
    CU_ASSERT_EQUAL(ler('l',"tests/malFormatado.txt",&estado), E_FICHEIRO);
    CU_ASSERT_EQUAL(ler('l',"tests/ExHistoricoE",&estado2), E_FICHEIRO);
    
    // Ficheiro não existe
    CU_ASSERT_EQUAL(ler('l',"naoExiste",&estado), E_FICHEIRO);

    // Ficheiro válido
    CU_ASSERT_EQUAL(ler('l',"tests/ExLer",&estado), E_OK);
    CU_ASSERT_EQUAL(estado.linhas,2);
    CU_ASSERT_EQUAL(estado.colunas,2);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0], '#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0], 'c');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1], 'b');
    CU_ASSERT_TRUE(estado.historico != NULL);
    CU_ASSERT_STRING_EQUAL(estado.caminhoFicheiro, "tests/ExLer");

    // Gravar estado antes de abrir um novo
    CU_ASSERT_EQUAL(riscar('r',"b1",&estado),E_OK);
    CU_ASSERT_EQUAL(ler('l',"tests/ExLer2",&estado1), E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1], '#');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);

    libertarMemoria(&estado2);
    libertarHistorico(&estado2);

}

/**
 * @brief Testa função riscar.
 */
void testarRiscar() {
    ESTADO estado = {0};
    ESTADO estado1 = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExRiscar",&estado), E_OK);

    char original = estado.tabuleiro[0][0]; // Guarda o valor antes de riscar
    CU_ASSERT_EQUAL(riscar('r', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0], '#'); // Verifica se riscou
    CU_ASSERT_NOT_EQUAL(original, '#');     

    CU_ASSERT_FALSE(tabuleiroResolvido(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExRiscar2",&estado1), E_OK);

    CU_ASSERT_EQUAL(riscar('r', "b2", &estado1), E_OK);
    CU_ASSERT_EQUAL(estado1.tabuleiro[1][1], '#'); // Verifica se riscou
    CU_ASSERT_TRUE(tabuleiroResolvido(&estado1));

    CU_ASSERT_EQUAL(ler('l',"nivel/2",&estado1), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "k11", &estado1), E_OK);
    CU_ASSERT_EQUAL(estado1.tabuleiro[10][10], '#'); // Verifica se riscou

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);
}

/**
 * @brief Testa função branco.
 */
void testarBranco() {
    ESTADO estado = {0};
    ESTADO estado1 = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExBranco",&estado), E_OK);

    char original = estado.tabuleiro[0][0]; // Guarda o valor antes de riscar
    CU_ASSERT_EQUAL(branco('b', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0], 'A'); // Verifica se pintou
    CU_ASSERT_NOT_EQUAL(original, 'A');     

    CU_ASSERT_FALSE(tabuleiroResolvido(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExBranco2",&estado1), E_OK);

    char original1 = estado1.tabuleiro[1][0]; // Guarda o valor antes de riscar
    CU_ASSERT_EQUAL(branco('b', "a2", &estado1), E_OK);
    CU_ASSERT_EQUAL(estado1.tabuleiro[1][0], 'C'); // Verifica se riscou
    CU_ASSERT_NOT_EQUAL(original1, 'C');     
    CU_ASSERT_TRUE(tabuleiroResolvido(&estado1));

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);
}

/**
* @brief Testa função desfazerCoordenada.
*/
void testaDesfazerCoordenada() {
    ESTADO estado = {0};
    ESTADO estado1 = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExMinuscula",&estado), E_OK);
    char original1 = estado.tabuleiro[0][0]; 
    CU_ASSERT_EQUAL(desfazerCoordenada('m',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'a');
    CU_ASSERT_NOT_EQUAL(original1, 'a'); 

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExMinuscula2",&estado1), E_OK);
    CU_ASSERT_EQUAL(desfazerCoordenada('m',"a1",&estado1),E_COORD);
    CU_ASSERT_EQUAL(estado1.tabuleiro[0][0],'a');

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);
}

/**
* @brief Testa função autocompletar.
*/
void testarAutocompletar() {
    ESTADO estado = {0};
    ESTADO estado1 = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExAutocompletar",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(autocompletar('a', NULL, &estado),E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'D');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'C');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'B');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'e');

    CU_ASSERT_FALSE(tabuleiroResolvido(&estado));

    CU_ASSERT_EQUAL(branco('b',"c1",&estado),E_OK);
    CU_ASSERT_TRUE(tabuleiroResolvido(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExAutocompletar2",&estado1), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado1),E_OK);
    CU_ASSERT_EQUAL(riscar('r',"b1",&estado1),E_OK);
    CU_ASSERT_EQUAL(autocompletar('a', NULL, &estado1),E_TAB_INV);

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);

    CU_ASSERT_EQUAL(ler('l',"nivel/5",&estado1), E_OK);
    CU_ASSERT_EQUAL(autocompletar('a', NULL, &estado1),E_TAB_NM);

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);
}

void testaAutocompletarRec() {
    ESTADO estado = {0};
    ESTADO estado1 = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExCompletRec",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(autocompletarRec('A',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'D');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'C');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'B');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'E');
    CU_ASSERT_TRUE(tabuleiroResolvido(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExAutocompletar2",&estado1), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado1),E_OK);
    CU_ASSERT_EQUAL(riscar('r',"b1",&estado1),E_OK);
    CU_ASSERT_EQUAL(autocompletarRec('A', NULL, &estado1),E_TAB_INV);

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);

    CU_ASSERT_EQUAL(ler('l',"nivel/6",&estado1), E_OK);
    CU_ASSERT_EQUAL(autocompletarRec('A', NULL, &estado1),E_TAB_NM);

    libertarMemoria(&estado1);
    libertarHistorico(&estado1);

}

void testaVerificar() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExVerifica",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a2",&estado),E_OK);
    CU_ASSERT_EQUAL(verificar('v',NULL,&estado),E_OK);
    CU_ASSERT_FALSE(estado.tab_valid);

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExVerifica",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(branco('b',"a2",&estado),E_OK);
    CU_ASSERT_EQUAL(branco('b',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(verificar('v',NULL,&estado),E_OK);
    CU_ASSERT_TRUE(estado.tab_valid);

    libertarMemoria(&estado);
    libertarHistorico(&estado);


}

void testaDesfazer() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExDesfazer",&estado), E_OK);
    CU_ASSERT_EQUAL(desfazer('d',NULL,&estado),E_HIST_VAZIO);

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExDesfazer",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(desfazer('d',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'a');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExDesfazer",&estado), E_OK);
    CU_ASSERT_EQUAL(branco('b',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(autocompletarRec('A',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(desfazer('d',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'A');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'d');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}


void testarResolver() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExResolver",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(riscar('r',"b1",&estado),E_OK);
    CU_ASSERT_EQUAL(resolver('R',NULL,&estado),E_TAB_INV);

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExResolver",&estado), E_OK);
    CU_ASSERT_EQUAL(resolver('R',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'A');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'B');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'C');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'D');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'E');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"nivel/6",&estado), E_OK);
    CU_ASSERT_EQUAL(resolver('R',NULL,&estado),E_OK);

    libertarMemoria(&estado);
    libertarHistorico(&estado);
}

void testarSair() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExResolver",&estado), E_OK);
    
    // guarda o stdin
    FILE *org_stdin = stdin;
    
    // simula a entrada do utilizador com 's' (sim)
    char *input_y = "s\n";
    FILE *yes = fmemopen((void*)input_y, strlen(input_y), "r"); // abre um stream de memória equivalente ao stdin

    // simula a entrada do utilizador com 'n' (não)
    char *input_n = "n\n";
    FILE *no  = fmemopen((void*)input_n, strlen(input_n), "r"); // abre um stream de memória equivalente ao stdin

    // simula a entrada do utilizador com 'n' (não)
    char *input_x = "x\n";
    FILE *x  = fmemopen((void*)input_x, strlen(input_x), "r"); // abre um stream de memória equivalente ao stdin
    
    stdin = yes; // redireciona stdin para o stream 'yes'
    CU_ASSERT_EQUAL(sair('s', NULL, &estado), E_OK);
    CU_ASSERT_FALSE(estado.looping);

    stdin = no; // redireciona stdin para o stream 'no'
    CU_ASSERT_EQUAL(sair('s', NULL, &estado), E_OK);
    CU_ASSERT_FALSE(estado.looping);

    stdin = x; // redireciona stdin para o stream com entrada inválida
    CU_ASSERT_EQUAL(sair('s', NULL, &estado), E_OK);
    CU_ASSERT_FALSE(estado.looping);
    

    stdin = org_stdin; // restaura o stdin
    fclose(yes); // fecha o stream temporário 'yes'
    fclose(no);  // fecha o stream temporário 'no'
    fclose(x);  // fecha o stream temporário 'x'

    libertarMemoria(&estado);
}


void verificarConteudoArquivo(const char *filename, ESTADO *e) {
    FILE *file = fopen(filename, "r");
    int linhas, colunas;
    fscanf(file, "%d %d", &linhas, &colunas);
    CU_ASSERT_EQUAL(linhas, e->linhas);
    CU_ASSERT_EQUAL(colunas, e -> colunas);
    // Adicionar verificações para o tabuleiro e histórico
    fclose(file);
}

void testarGravar() {
    ESTADO estado = {0};
    
    CU_ASSERT_EQUAL(ler('l',"tests/ExGravar1",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_EQUAL(branco('b',"b1",&estado),E_OK);
    CU_ASSERT_EQUAL(gravar('g',"tests/ExGravar1",&estado),E_OK);


    verificarConteudoArquivo("tests/ExGravar1" , &estado);

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExGravar1",&estado), E_OK);
    CU_ASSERT_EQUAL(desfazer('d',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(desfazer('d',NULL,&estado),E_OK);
    CU_ASSERT_EQUAL(gravar('g',"tests/ExGravar1",&estado),E_OK);


    verificarConteudoArquivo("tests/ExGravar1" , &estado);

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}


void testarRiscarLetrasIguaisBrancas() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExAutocompletar",&estado), E_OK);
    CU_ASSERT_EQUAL(branco('b',"a2",&estado),E_OK);
    CU_ASSERT_TRUE(riscarLetrasIguaisBrancas(&estado));

    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'d');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'e');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'C');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'#');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l',"tests/ExRiscarIguais",&estado), E_OK);
    CU_ASSERT_EQUAL(branco('b',"a1",&estado),E_OK);
    CU_ASSERT_TRUE(riscarLetrasIguaisBrancas(&estado));

    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'A');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'c');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'f');


    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarPintarVizinhasRiscadas() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"tests/ExAutocompletar",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_TRUE(pintarVizinhasDeRiscadas(&estado));

    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'D');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'e');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'C');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'c');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}


void testarPintarCasasQueIsolamBrancas() {
    ESTADO estado = {0};
    
    CU_ASSERT_EQUAL(ler('l',"tests/ExGravar1",&estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r',"a1",&estado),E_OK);
    CU_ASSERT_FALSE(pintarCasasQueIsolamLetras(&estado));

    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'#');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'d');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'d');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'C');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'t');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarVerificarBrancasRepetidas() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    
    CU_ASSERT_EQUAL(branco('b', "a1", &estado), E_OK);
    CU_ASSERT_TRUE(verificarBrancasRepetidas(&estado, 0));

    CU_ASSERT_EQUAL(branco('b', "c1", &estado), E_OK);
    CU_ASSERT_FALSE(verificarBrancasRepetidas(&estado, 0));

    CU_ASSERT_EQUAL(branco('b', "a2", &estado), E_OK);
    CU_ASSERT_FALSE(verificarBrancasRepetidas(&estado, 0));

    

    
    libertarMemoria(&estado);
    libertarHistorico(&estado);
}

void testarVizinhasRiscadas() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    
    CU_ASSERT_EQUAL(riscar('r', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "a2", &estado), E_OK);
    CU_ASSERT_FALSE(verificarVizinhasRiscadas(&estado, 0));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarVerificarCaminho() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    
    CU_ASSERT_EQUAL(riscar('r', "c1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "b2", &estado), E_OK);
    CU_ASSERT_FALSE(verificarCaminho(&estado, 0));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    
    CU_ASSERT_EQUAL(riscar('r', "c1", &estado), E_OK);
    CU_ASSERT_TRUE(verificarCaminho(&estado, 0));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarVerificaRegras() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    
    CU_ASSERT_EQUAL(riscar('r', "c1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "b2", &estado), E_OK);
    CU_ASSERT_EQUAL(branco('b', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(branco('b', "a2", &estado), E_OK);
    CU_ASSERT_FALSE(verificaRegras(&estado, 0));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarTabuleiroResolvido() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    
    CU_ASSERT_EQUAL(riscar('r', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "b1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "c1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "a2", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "b2", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "c2", &estado), E_OK);
    CU_ASSERT_FALSE(tabuleiroResolvido(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    CU_ASSERT_EQUAL(branco('b', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(branco('b', "b1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "c1", &estado), E_OK);
    CU_ASSERT_EQUAL(riscar('r', "a2", &estado), E_OK);
    CU_ASSERT_EQUAL(branco('b', "b2", &estado), E_OK);
    CU_ASSERT_EQUAL(branco('b', "c2", &estado), E_OK);
    CU_ASSERT_TRUE(tabuleiroResolvido(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);
}

void testarLeConteudoTabuleiro() {
    ESTADO estado = {0};

    estado.linhas = 2;
    estado.colunas = 3;
    estado.looping   = true;
    estado.tabuleiro = NULL;
    estado.historico = NULL;
    estado.tab_valid = true;
    estado.caminhoFicheiro = NULL;

    estado.tabuleiro = malloc(estado.linhas * sizeof(char *));
    for (int i = 0; i < estado.linhas; i++) {
        estado.tabuleiro[i] = malloc(estado.colunas * sizeof(char));
    }

    FILE *fileTeste = fopen("tests/ExRiscarIguais", "r");
    if (fileTeste == NULL) {
        printf("Erro ao abrir arquivo de teste!\n");
        return;
    }
    CU_ASSERT_TRUE(lerConteudoTabuleiro(fileTeste, &estado));
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'c');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'f');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarLerTabuleiroDoArquivo() {
    ESTADO estado = {0};

    FILE *fileTeste = fopen("tests/ExRiscarIguais", "r");
    if (fileTeste == NULL) {
        printf("Erro ao abrir arquivo de teste!\n");
        return;
    }
    CU_ASSERT_TRUE(lerTabuleiroDoArquivo(fileTeste, &estado));
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][1],'b');
    CU_ASSERT_EQUAL(estado.tabuleiro[0][2],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][0],'a');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][1],'c');
    CU_ASSERT_EQUAL(estado.tabuleiro[1][2],'f');

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarLerHistoricoJogadas() {
    ESTADO estado = {0};
    estado.linhas = 2;
    estado.colunas = 3;

    ler('l', "tests/ExHistorico", &estado);
    
    CU_ASSERT_PTR_NOT_NULL_FATAL(estado.historico);
    
    CU_ASSERT_EQUAL(estado.historico -> x, 1);
    CU_ASSERT_EQUAL(estado.historico -> y, 1);
    CU_ASSERT_EQUAL(estado.historico -> c, 'd');
    CU_ASSERT_EQUAL(estado.historico -> inicioComando, 0);

    
    JOGADA_SALVA *segundaJogada = estado.historico -> ant;
    CU_ASSERT_PTR_NOT_NULL_FATAL(segundaJogada);
    CU_ASSERT_EQUAL(segundaJogada -> x, 0);
    CU_ASSERT_EQUAL(segundaJogada -> y, 1);
    CU_ASSERT_EQUAL(segundaJogada -> c, 'b');
    CU_ASSERT_EQUAL(segundaJogada -> inicioComando, 0);

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarTabuleiroCarregado() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    CU_ASSERT_TRUE(tabuleiroCarregado(&estado));

    libertarMemoria(&estado);

}

void testarHistoricoVazio() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);
    CU_ASSERT_TRUE(verificarHistoricoVazio(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

    CU_ASSERT_EQUAL(ler('l', "tests/ExHistorico", &estado), E_OK);
    CU_ASSERT_FALSE(verificarHistoricoVazio(&estado));

    libertarMemoria(&estado);
    libertarHistorico(&estado);

}

void testarGravarTabuleiro() {
    ESTADO estado = {0};

    // Alocar e preencher tabuleiro
    estado.tabuleiro = malloc(estado.linhas * sizeof(char *));
    for (int i = 0; i < estado.linhas; i++) {
        estado.tabuleiro[i] = malloc(estado.colunas * sizeof(char));
        for (int j = 0; j < estado.colunas; j++) {
            estado.tabuleiro[i][j] = 'a' + i + j;  
        }
    }

    FILE *arquivoTeste = fopen("tests/arquivo.txt", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(arquivoTeste);
    
    gravarTabuleiro(arquivoTeste, &estado);
    fclose(arquivoTeste);

    arquivoTeste = fopen("tests/arquivo.txt", "r");
    char linha[10];
    for (int i = 0; i < estado.linhas; i++) {
        CU_ASSERT_PTR_NOT_NULL(fgets(linha, sizeof(linha), arquivoTeste));
        for (int j = 0; j < estado.colunas; j++) {
            CU_ASSERT_EQUAL(linha[j], estado.tabuleiro[i][j]);
        }
        CU_ASSERT_EQUAL(linha[estado.colunas], '\n');
    }
    fclose(arquivoTeste);

    libertarMemoria(&estado);

}

void testarLibertarMemoria() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscarIguais", &estado), E_OK);

    libertarMemoria(&estado);

    CU_ASSERT_PTR_NULL(estado.tabuleiro);
    CU_ASSERT_PTR_NULL(estado.caminhoFicheiro);
}

void testarAlocarMemoria() {
    ESTADO estado = {0};
    estado.linhas = 3;
    estado.colunas = 3;
    
    CU_ASSERT_TRUE(alocarMemoria(&estado));
    CU_ASSERT_PTR_NOT_NULL(estado.tabuleiro);
    for (int i = 0; i < estado.linhas; i++) {
        CU_ASSERT_PTR_NOT_NULL(estado.tabuleiro[i]);
    }

    libertarMemoria(&estado);

    
}

void testarLibertarHistorico() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExHistorico", &estado), E_OK);
    libertarHistorico(&estado);
    CU_ASSERT_PTR_NULL(estado.historico);
    libertarMemoria(&estado);

}

void testarLibertarCopiaEstado() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExHistorico", &estado), E_OK);

    ESTADO *copia = criaCopiaEstado(&estado);
    libertarHistorico(&estado);
    libertarMemoria(&estado);

    libertarCopiaEstado(copia);
    libertarHistorico(copia);

    CU_ASSERT_PTR_NULL(copia -> tabuleiro);
    CU_ASSERT_PTR_NULL(copia -> historico);
    CU_ASSERT_PTR_NULL(copia -> caminhoFicheiro);
    
    free(copia);
}

void testarToIntNumber() {
    CU_ASSERT_EQUAL(toInt_Number('0'), 0);
    CU_ASSERT_EQUAL(toInt_Number('1'), 1);
    CU_ASSERT_EQUAL(toInt_Number('2'), 2);
    CU_ASSERT_EQUAL(toInt_Number('3'), 3);
    CU_ASSERT_EQUAL(toInt_Number('4'), 4);
    CU_ASSERT_EQUAL(toInt_Number('5'), 5);
    CU_ASSERT_EQUAL(toInt_Number('6'), 6);
    CU_ASSERT_EQUAL(toInt_Number('7'), 7);
    CU_ASSERT_EQUAL(toInt_Number('8'), 8);
    CU_ASSERT_EQUAL(toInt_Number('9'), 9);
}

void testarToLower() {
    CU_ASSERT_EQUAL(toLower('A'), 'a');
    CU_ASSERT_EQUAL(toLower('B'), 'b');
    CU_ASSERT_EQUAL(toLower('c'), 'c');
    CU_ASSERT_EQUAL(toLower('d'), 'd');
}

void testarRestaurarEstado() {
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l', "tests/ExRiscar", &estado), E_OK);

    ESTADO copia = {0};

    restaurarEstado(&copia, &estado);

    CU_ASSERT_EQUAL(copia.looping, estado.looping);
    CU_ASSERT_EQUAL(copia.tab_valid, estado.tab_valid);

    for (int i = 0; i < copia.linhas; i++) {
        for (int j = 0; j < copia.colunas; j++) {
            CU_ASSERT_EQUAL(copia.tabuleiro[i][j], estado.tabuleiro[i][j]);
        }
    }

    restaurarEstado(&copia, &estado);

    CU_ASSERT_STRING_EQUAL(copia.caminhoFicheiro, estado.caminhoFicheiro)
    
    libertarHistorico(&estado);
    libertarMemoria(&estado);

    libertarHistorico(&copia);
    libertarMemoria(&copia);
}

void testarGuardarEstado() {
	ESTADO estado = {0};
	
	CU_ASSERT_EQUAL(ler('l',"nivel/4",&estado),E_OK);
	CU_ASSERT_FALSE(guardarEstado(&estado,-1,2,'C'));
	
	libertarMemoria(&estado);
	
	CU_ASSERT_EQUAL(ler('l',"nivel/4",&estado),E_OK);
	CU_ASSERT_FALSE(guardarEstado(&estado,2,-2,'C'));
	
	libertarMemoria(&estado);
	
	CU_ASSERT_EQUAL(ler('l',"nivel/4",&estado),E_OK);
	CU_ASSERT_TRUE(guardarEstado(&estado,0,0,'b'));
	
	libertarMemoria(&estado);
	libertarHistorico(&estado);
}

void testarRemoverMarcadorComandoCasoComAnterior() {
    ESTADO estado = {0};

    JOGADA_SALVA *jogada1 = malloc(sizeof(JOGADA_SALVA));
    JOGADA_SALVA *jogada2 = malloc(sizeof(JOGADA_SALVA));
    JOGADA_SALVA *jogada3 = malloc(sizeof(JOGADA_SALVA));
    

    jogada1 -> inicioComando = false;
    jogada1 -> ant = NULL;
    
    jogada2 -> inicioComando = true;
    jogada2 -> ant = jogada1;
    
    jogada3 -> inicioComando = false;
    jogada3 -> ant = jogada2;
    
    estado.historico = jogada3;
    
    removerMarcadorComando(&estado, jogada2, jogada1);

    CU_ASSERT_PTR_EQUAL(estado.historico, jogada3);
    CU_ASSERT_PTR_EQUAL(jogada3 -> ant, jogada2);
    CU_ASSERT_PTR_EQUAL(jogada1 -> ant, jogada1);

    free(jogada1);
    free(jogada3);
}

void testarExecutaComando() {
    COMANDO comandos[] = {sair, ler, gravar, riscar, desfazer, branco, help, autocompletar, autocompletarRec, verificar, regras, resolver, limpa, desfazerCoordenada, NULL};
    ESTADO estado = {0};

    CU_ASSERT_EQUAL(ler('l',"nivel/4",&estado),E_OK);

    CU_ASSERT_EQUAL(executarComando(comandos, 'r', "a1", &estado), E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0], '#');
    CU_ASSERT_EQUAL(executarComando(comandos, 'd', "", &estado), E_OK);
    CU_ASSERT_EQUAL(estado.tabuleiro[0][0], 'b');

    CU_ASSERT_EQUAL(executarComando(comandos, 'z', "a1", &estado), E_CMD_INV);

    libertarHistorico(&estado);
    libertarMemoria(&estado);
}

void testarClear() {
    CU_ASSERT_EQUAL(clear(), E_OK);
}

void testarMensagemInicial() {
    CU_ASSERT_EQUAL(mensagemInicial(), E_OK);
}

int init_suite(void) {
    inicializarestadoTeste();
    return 0;
}

int clean_suite(void) {
    limparEstadoTeste();
    return 0;
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        fprintf(stderr, "Erro ao inicializar CUnit!\n");
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("Suite de Testes Funcionais", init_suite, clean_suite);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Registro dos testes
    CU_add_test(suite, "Testa função mensagemInicial", testarMensagemInicial);
    CU_add_test(suite, "Testa função clear", testarClear);
    CU_add_test(suite, "Comando inválido", testeComComandoInvalido);
    CU_add_test(suite, "Argumento vazio", testeComArgumentoVazio);
    CU_add_test(suite, "Argumento não vazio", testeComArgumento);
    CU_add_test(suite, "Tabuleiro não carregado", testeComTabuleiroVazio);
    CU_add_test(suite, "Formato argumento inválido", testeComFormatoCoordenadasInvalido);
    CU_add_test(suite, "Coordenadas fora do tabuleiro", testeComCoordenadasInvalidas);
    CU_add_test(suite, "Testa função riscar", testarRiscar);
    CU_add_test(suite, "Testa função ler", testarLer);
    CU_add_test(suite, "Testa função branco", testarBranco);
    CU_add_test(suite, "Testa função desfazerCoordenada", testaDesfazerCoordenada);
    CU_add_test(suite, "Testa função autocompletar", testarAutocompletar);
    CU_add_test(suite, "Testa função autocompletarRec", testaAutocompletarRec);
    CU_add_test(suite, "Testa função verifica", testaVerificar);
    CU_add_test(suite, "Testa função desfazer", testaDesfazer);
    CU_add_test(suite, "Testa função resolver", testarResolver);
    CU_add_test(suite, "Testa função sair", testarSair);
    CU_add_test(suite, "Testa função gravar", testarGravar);
    CU_add_test(suite, "Testa função riscarLetrasIguaisBrancas", testarRiscarLetrasIguaisBrancas);
    CU_add_test(suite, "Testa função pintarVizinhasDeRiscadas", testarPintarVizinhasRiscadas);
    CU_add_test(suite, "Testa função pintarCasasQueIsolamBrancas", testarPintarCasasQueIsolamBrancas);
    CU_add_test(suite, "Testa função verificarBrancasRepetidas", testarVerificarBrancasRepetidas);
    CU_add_test(suite, "Testa função verificarVizinhasRiscadas", testarVizinhasRiscadas);
    CU_add_test(suite, "Testa função verificarCaminho", testarVerificarCaminho);
    CU_add_test(suite, "Testa função verificaRegras", testarVerificaRegras);
    CU_add_test(suite, "Testa função tabuleiroResolvido", testarTabuleiroResolvido);
    CU_add_test(suite, "Testa função leConteudoTabuleiro", testarLeConteudoTabuleiro);
    CU_add_test(suite, "Testa função leTrabuleiroDoArquivo", testarLerTabuleiroDoArquivo);
    CU_add_test(suite, "Testa função lerHistoricoJogadas", testarLerHistoricoJogadas);
    CU_add_test(suite, "Testa função tabuleiroCarregado", testarTabuleiroCarregado);
    CU_add_test(suite, "Testa função verificarHistoricoVazio", testarHistoricoVazio);
    CU_add_test(suite, "Testa função gravarTabuleiro", testarGravarTabuleiro);
    CU_add_test(suite, "Testa função libertarMemoria", testarLibertarMemoria);
    CU_add_test(suite, "Testa função alocarMemoria", testarAlocarMemoria);
    CU_add_test(suite, "Testa função libertarHistorico", testarLibertarHistorico);
    CU_add_test(suite, "Testa função libertarCopiaEstado", testarLibertarCopiaEstado);
    CU_add_test(suite, "Testa função toInt_Number", testarToIntNumber);
    CU_add_test(suite, "Testa função toLower", testarToLower);
    CU_add_test(suite, "Testa função restaurarEstado", testarRestaurarEstado);
    CU_add_test(suite, "Testa função guardarEstado", testarGuardarEstado);
    CU_add_test(suite, "Testa função removerMarcadorComando", testarRemoverMarcadorComandoCasoComAnterior);
    CU_add_test(suite, "Testa função executarComando", testarExecutaComando);

    // Execução dos testes
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}