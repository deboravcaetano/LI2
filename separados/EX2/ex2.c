#include <stdio.h>

int mediaSequencia(){
    int n, j=0;
    float media=0;

    scanf("%d", &n);
    float soma = n;
    while(n!=0){
        scanf("%d", &n);
        soma+=n;
        j++;
    }

    media=soma/j;

    printf("A média da sequência é:%f\n", media);
    return media;
}


int main() {
    mediaSequencia();
    return 0;
}