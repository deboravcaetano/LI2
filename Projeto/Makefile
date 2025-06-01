# Compilador
CC = gcc

# Flags padrão de compilação
BASE_CFLAGS = -Wall -Wextra -pedantic -O0 -fsanitize=address -fno-omit-frame-pointer -g

# Flags de cobertura
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Flags de link
LDFLAGS = -lcunit

# Arquivos fonte
SRC = modulos/tabuleiro.c modulos/estado.c modulos/regras.c modulos/verificacao.c modulos/memoria.c modulos/simples.c modulos/auxiliares.c modulos/comandos.c
TEST_SRC = modulos/testesC.c $(SRC)

# Objetos principais e de teste
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

# Alvo padrão
all: jogo

# Compilação padrão do jogo
jogo: modulos/main.o $(OBJ)
	$(CC) $(BASE_CFLAGS) -o $@ $^
	find . -name '*.o' -type f -delete

# Compilação para testes com cobertura
testar: CFLAGS := $(BASE_CFLAGS) $(GCOV_FLAGS)
testar: $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	./testar
	gcov -b -c $(SRC)
	find . \( -name '*.o' -o -name '*.gcda' -o -name '*.gcno' \) -type f -delete

# Limpeza
clean:
	rm -f *.gcov testar jogo