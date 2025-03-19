#include <stdio.h>

int segundoMaior(){
    int n, maior=0, smaior=0;

    scanf("%d", &n);
    maior=n;

    while(n!=0){
        scanf("%d", &n);
        if(n>maior){
            smaior = maior;
            maior = n;
        }
    }

    printf("%d", smaior);

    return smaior;
}

int main() {
    segundoMaior();
    return 0;
}