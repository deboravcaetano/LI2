/**
* @file main.c
* @brief Este ficheiro contém o módulo 'main' do Projeto Final de LI2 (24/25).
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

int main() { 
    // clear do terminal
    clear();
    
    // inicialização da lista de comandos do jogo                                                                                                    
    COMANDO comandos[] = {sair, ler, gravar, riscar, desfazer, branco, help, autocompletar, autocompletarRec, verificar, regras, resolver, limpa, desfazerCoordenada, NULL};
    
    // inicialização do estado
    ESTADO estado;
    estado.looping   = true;
    estado.tabuleiro = NULL;
    estado.historico = NULL;
    estado.tab_valid = true;
    estado.caminhoFicheiro = NULL;

    // inicialização das instruções dos comandos
    char cmd = 0;
    char arg[LINE_SIZE] = {0};

    // exibir no ecrã a mensagem inicial
    mensagemInicial(estado);

    // ciclo de jogo
    while (estado.looping) {
        int res = lerLinha(&cmd, arg);

        if (res == E_EOF) {
            estado.looping = false;
        } else if (res == E_OK){
            int codigoErro = executarComando(comandos, cmd, arg, &estado);

            if (codigoErro == E_CMD_INV && cmd != '\0') {
                fprintf(stderr, NEGRITO VERMELHO "\n Erro:" RESET " Comando '%c' inválido!\n", cmd);
            }
        } else {

        }
    }
 
    // libertação da memória e histórico do jogo (findo o ciclo de jogo)
    libertarMemoria(&estado);
    libertarHistorico(&estado);
    
    return E_OK;
}