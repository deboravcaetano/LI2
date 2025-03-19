#include <stdio.h>


int bitsUm (unsigned int n){
    int cont=0;
    while (n > 0) {
        if ( n % 2 == 1) {
           cont ++;
        }
        n = n/2;
    }
}

int main() {
    unsigned int numero;

    printf("Digite um número inteiro positivo: ");
    scanf("%u", &numero);

    bitsUm(numero);
    printf("O número de bits 1 é: %d\n", bitsUm(numero));

    return 0;
}