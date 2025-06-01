/**
* @file regras.c
* @brief Este ficheiro contém o módulo 'regras' do Projeto Final de LI2 (24/25).
* 
* Projeto Final - Puzzle Game
* Laboratórios de Informática II (UMinho 24/25) 
*
* (c) Débora Luana Pinto Caetano    <a112332@alunos.uminho.pt>, 2025-06-01
* (c) Marcos Bernardo da Silva Lobo <a110959@alunos.uminho.pt>, 2025-06-01
* (c) Maria Inês da Cruz Machado    <a111464@alunos.uminho.pt>, 2025-06-01
*/

#include "type.h"
#include "erros.h"

/**
 * @brief Função que risca todas as letras iguais a uma letra branca na mesma linha e/ou coluna.
 */

bool riscarLetrasIguaisBrancas(ESTADO *e) {
    // regista se ocorrem alterações no tabuleiro
    bool mudou = false;

    // iteração sobre todas as coordenadas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {

            // se a coordenada for maiúscula/branca
            if (isUpper(e -> tabuleiro[i][j])) {
                // converte em minúscula
                char brancaMin = toLower(e -> tabuleiro[i][j]);

                // procura outras letras iguais na mesma linha
                for (int k = 0; k < e -> colunas; k++) { // linha fixa (i), colunas variáveis (k)
                    if (e -> tabuleiro[i][k] == brancaMin) { 
                        // guarda o estado antes de modificar
                        if (!guardarEstado(e, i, k, e -> tabuleiro[i][k])) { // erro: falha ao guardar estado
                            fprintf(stderr, " Erro ao guardar estado em %c%d\n", 'a'+k, i+1);
                            return false;
                        }

                        // risca a casa igual
                        e -> tabuleiro[i][k] = '#';
                        
                        // regista que houve uma alteração
                        mudou = true;
                    } 
                }

                // procura outras letras iguais na mesma linha
                for (int k = 0; k < e -> linhas; k++) { // coluna fixa (j), linhas variáveis (k)
                    if (e -> tabuleiro[k][j] == brancaMin) { 
                        // guarda o estado antes de modificar
                        if (!guardarEstado(e, k, j, e -> tabuleiro[k][j])) { // erro: falha ao guardar estado
                            fprintf(stderr, " Erro ao guardar estado em %c%d\n", 'a'+j, i+1);
                            return false;
                        }

                        // risca a casa igual
                        e -> tabuleiro[k][j] = '#';
                        
                        // regista que houve uma alteração
                        mudou = true;
                    }
                }
            }

        }
    }

   return mudou;
}

/**
 * @brief Função que converte todas as vizinhas ortogonais de uma casa riscada em brancas.
 */

bool pintarVizinhasDeRiscadas(ESTADO *e) {
    // regista se ocorrem alterações no tabuleiro
    bool mudou = false;

    // iteração sobre todas as coordenadas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {

            // se a coordenada estiver riscada
            if (e -> tabuleiro[i][j] == '#') {
                // inicializa os arrays com as direções ortogonais
                // NOTA: os movimentos ortogonais variam num só eixo: (-1, 0) (1, 0) (0, -1) (0, 1)
                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, -1, 1};

                for (int d = 0; d < 4; d++) {
                    // variação das coordenadas, de acordo com os arrays das direções ortogonais
                    int ci = i + dx[d];
                    int cj = j + dy[d];

                    // se a variação das coordenadas for corente (estiver dentro do tabuleiro)
                    if (ci >= 0 && ci < e -> linhas && cj >= 0 && cj < e -> colunas) {
                        // se a vizinha for minúscula
                        if (isLower(e -> tabuleiro[ci][cj])) {
                            // guarda o estado antes de modificar
                            if (!guardarEstado(e, ci, cj, e -> tabuleiro[ci][cj])) { // erro: falha ao guardar estado
                                fprintf(stderr, " Erro ao guardar estado em %c%d\n", 'a'+cj, ci+1);
                                return false;
                            }

                            // converte a casa em branco
                            e -> tabuleiro[ci][cj] = toUpper(e -> tabuleiro[ci][cj]);
                            
                            // regista que houve uma alteração
                            mudou = true;
                        }
                    }
                }
            }

        }
    }
    
    return mudou;
}

/**
 * @brief Função que percorre todas as letras a partir de (i, j) e as marca como visitadas.
 * 
 * Se alguma(s) casa(s) estiver(em) isolada(s) por '#', não são marcadas como visitadas.
 * Desse modo, sabe-se se exite um caminho ortogonal entre todas as casas.
 */

void marcaLetras(ESTADO *e, int i, int j, bool visitado[e -> linhas][e -> colunas]) {
    // valida se as coordenadas estão no tabuleiro, já foram visitadas ou estão riscadas
    if (i < 0 || i >= e -> linhas || j < 0 || j >= e -> colunas || visitado[i][j] || (e -> tabuleiro[i][j]) == '#') {
        return;
    }

    // marca a casa como visitada
    visitado[i][j] = 1;

    // inicializa os arrays com as direções ortogonais
    // NOTA: os movimentos ortogonais variam num só eixo: (-1, 0) (1, 0) (0, -1) (0, 1)
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    // aplica recursivamente a função às vizinhas ortogonais
    for (int d = 0; d < 4; d++) marcaLetras(e, i + dx[d], j + dy[d], visitado);
}

/**
 * @brief Função que verifica se todas as letras estão conectadas.
 * 
 * @see marcaLetras()
 */

bool todasConectadas(ESTADO *e) {
    // inicializa a matriz visistados a zeros
    bool visitado[e -> linhas][e -> colunas];
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {
            visitado[i][j] = 0;
        }
    }

    // marca o início da verificação
    bool encontrouPrimeiraLetra = false;

    // iteração sobre todas as coordenadas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {

            // verifica se a coordenada está riscada
            if ((e -> tabuleiro[i][j]) != '#' ) {
                // se a verificação não tiver começado
                if (!encontrouPrimeiraLetra) {
                    // marca todas as letras conectadas à primeira como visitadas
                    marcaLetras(e, i, j, visitado); 
                    // inicia a verificação
                    encontrouPrimeiraLetra = true;
                }
                // com a verificação iniciada, se uma casa não tiver sido visitada, não existe um caminho ortogonal
                else if (!visitado[i][j]) return false;
            }

        }
    }

    return true;
}

/**
 * @brief Função que pinta uma célula de branco quando seria impossível que esta fosse riscada por isolar letras.
 * 
 * @see guardarEstado()
 * @see todasConectadas()
 */


bool pintarCasasQueIsolamLetras(ESTADO *e) {
    // regista se ocorrem alterações no tabuleiro
    bool mudou = false;

    // iteração sobre todas as coordenadas do tabuleiro
    for (int i = 0; i < e -> linhas; i++) {
        for (int j = 0; j < e -> colunas; j++) {

            // se a coordenada tiver uma letra minúscula
            if (isLower(e -> tabuleiro[i][j])) {
                // guarda a letra original
                char original = e -> tabuleiro[i][j];

                // guarda o estado antes de simular riscar
                if (!guardarEstado(e, i, j, original)) { // erro: falha ao guardar estado
                    fprintf(stderr, " Erro ao guardar estado em %c%d\n", 'a'+j, i+1);
                    return false;
                }

                // simula riscar a letra
                e -> tabuleiro[i][j] = '#';  

                // se não houver conexão entre todas as casas
                if (!todasConectadas(e)) {
                    // guarda o estado antes de modificar
                    if (!guardarEstado(e, i, j, '#')) { // erro: falha ao guardar estado
                        fprintf(stderr, " Erro ao guardar estado em %c%d\n", 'a'+ j, i + 1);
                        return false;
                    }

                    // pinta a casa de branco
                    e -> tabuleiro[i][j] = toUpper(original);  

                    // regista que houve uma alteração
                    mudou = true;
                }

                // se houver conexão entre todas as casas
                else {
                    // guarda o estado antes de modificar
                    if (!guardarEstado(e, i, j, '#')) { // erro: falha ao guardar estado
                        fprintf(stderr, " Erro ao guardar estado em %c%d\n", 'a'+ j, i + 1);
                        return false;
                    }

                    // mantém a letra original
                    e -> tabuleiro[i][j] = original;

                    // regista que não houve alterações
                    mudou = false;
                }
            }

        }
    }
    
    return mudou;
}