#include <stdio.h>

int maiorSequencia(int n){
    int n ,maior=0;

    scanf("%d", &n);
    maior = n;

    while(n!=0){
        scanf("%d", &n);
        if(n>maior){
            maior=n;
        }
    }
    printf("O maior valor da sequência é:%d\n", maior);
    return maior;

}

int main() {
    maiorSequencia();
    return 0;
}