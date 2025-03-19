#include <stdio.h>

/*
Notas:
- verificar :10
- importantes: 13,16


*/

// ex1

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


// ex2

int mediaSequencia(){
    int n, j=0;
    float media=0;

    scanf("%d", &n);
    int soma = n;
    while(n!=0){
        scanf("%d", &n);
        soma+=n;
        j++;
    }

    media=soma/j;

    printf("A média da sequência é:%f\n", media);
    return media;
}

// ex3

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

// ex4

int bitsUm (unsigned int n){
    int cont=0;
    while (n > 0) {
        if ( n % 2 == 1) {
           cont ++;
        }
        n = n/2;
    }
}

// ex5

int trailingZ(unsigned int n) {
    int cont = 0;
    while (n > 0 && (n % 2 == 0)) {  // Continua enquanto o último bit é 0
        cont++;
        n = n / 2; 
    }
    return cont; 
}

/*
Nota: o 0 ou 1 que sai da primeira divisão por 2 é o do bit menos signficativo
Ex: n = 12 que em binário é 1100, trailingZ vai verificar se n em binário tem 0s da direita para a esquerda
*/


// ex6

int qDigFor(unsigned int n) {
    int count;
    if (n == 0) {
        return 1;  // Caso especial: o número 0 tem 1 dígito
    }
    for (count = 0; n > 0; count++) {
        n /= 10;
    }
    return count;
}

// ex7

char *strcat(char s1[], char s2[]) {
    int i, j;

    // Encontra o final da string s1
    for (i = 0; s1[i] != '\0'; i++); // Loop vazio: apenas avança 'i'

    // Copia s2 para o final de s1
    for (j = 0; s2[j] != '\0'; j++) {
        s1[i++] = s2[j];
    }


    s1[i] = '\0';

    return s1; // Retorna um ponteiro para a string concatenada (s1)
}

// ex8

char *strcpy(char *dest, char source[]) {
    int i;

    for (i = 0; source[i] != '\0'; i++) {
        dest[i] = source[i];
    }

    dest[i] = '\0'; 
    return dest; 
}

// ex9

int strcmp(const char s1[], const char s2[]) {
    int i;

    for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
        if (s1[i] < s2[i]) {
            return -1; // s1 é menor que s2
        } else if (s1[i] > s2[i]) {
            return 1;  // s1 é maior que s2
        }
    }

    // Se chegamos aqui, uma das strings (ou ambas) terminou
    if (s1[i] == '\0' && s2[i] == '\0') {
        return 0;  // As strings são iguais
    } else if (s1[i] == '\0') {
        return -1; // s1 é mais curta, então é menor
    } else {
        return 1;  // s2 é mais curta, então s1 é maior
    }
}

// ex10

char *strstr(char s1[], char s2[]) {
    int i, j;
    
    // Se s2 for uma string vazia, retorna s1
    if (s2[0] == '\0') {
        return NULL;
    }
    
    for (i = 0; s1[i] != '\0'; i++) {
        // Verifica se os caracteres de s2 correspondem aos de s1 a partir de i
        for (j = 0; s2[j] != '\0' && s1[i + j] == s2[j]; j++);
        
        // Se chegamos ao final de s2, encontramos a substring
        if (s2[j] == '\0') {
            return &s1[i];
        }
    }
    
    // Se não encontrar a substring, retorna NULL
    return NULL;
}

// ex11

int tamanhoString(char s[]) {
    int i;
    for(i = 0; s[i] != '\0'; i++);
    return i;
}

void strrev(char s[]) {
    int i, j;
    char t;
    int tam = tamanhoString(s);
    for (i = 0, j = tam - 1; i < j; i++, j--) {
        t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
}

// ex12

int vogal(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

void strnoV(char s[]) {
    int i, j;
    for (i = 0, j = 0; s[i] != '\0'; i++) {
        if (!vogal(s[i])) {
            s[j] = s[i];
            j++;
        }
    }
    s[j] = '\0';
}

// ver explicação folha 50 questões

// ex13

void truncW (char t[], int n){
    int i,j,cont=0;

    for(i=0,j=0; t[i]!='\0';i++){
        if(t[i]==' '){ // se acabei uma palavra
            cont = 0; // como vai começar a copiar uma nova palavra, reseto o número de caracteres copiados
            t[j]=' '; // adiciona um espaço de onde trunquei a ultima
            j++; // avanço para a posição a seguir ao novo espaço
        } else if (cont < n){
            t[j]=t[i];
            cont++;
            j++;
        }
    }
    t[j]='\0';
}

// ex14

int freqC(char ch, char s[]) {
    int i, freq = 0;
    for(i = 0; s[i]; i++) {
        if(s[i] == ch) freq++;
    }
    return freq;
}

char charMaisFreq(char s[]) {
    char maisFreq = 0;
    int i, freqMax = 0, freqAtual;
    for(i = 0; s[i]; i++) {
        if((freqAtual = freqC(s[i],s)) > freqMax) {
            freqMax = freqAtual;
            maisFreq = s[i];
        }
    }
    return maisFreq;
}

// ex15

int iguaisConsecutivos (char s[]) {
    int i, consec = 1, max = 0;
    for(i = 0; s[i]; i++) {
        if(s[i] == s[i + 1]) consec++;
        else {
            if(consec > max) max = consec;
            consec = 1;
        }
    }
    return max;
}

// ex16

// ex17

int maiorPrefixo (char s1 [], char s2 []) {
    int i;
    for(i = 0; s1[i] == s2[i] && s1[i]; i++);
    return i;
}

// ex18

int tamanhoString(char s[]) {
    int i;
    for(i = 0; s[i] != '\0'; i++);
    return i;
}

int maiorSufixo (char s1 [], char s2 []){
    int i,j,cont;
    int tam1 = tamanhoString(s1);
    int tam2 = tamanhoString(s2);
    for(i=tam1-1,j=tam2-1; s1[i] ==  s2[j] && i != 0 && j != 0; i--,j--) cont++;
    return cont;

}

// ex19

int sufPref (char s1[], char s2[]) {
    int i1, i2 = 0;
    for(i1 = 0; s1[i1]; i1++) {
        if(s1[i1] == s2[i2]) i2++;
        else i2 = 0;
    }
    return i2;
}

// ex20

int contaPal(char s[]) {
    int i, cont = 0;
    int dentroDaPalavra = 0; // vai funcionar como flag indicar se estamos dentro de uma palavra

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] != ' ' && !dentroDaPalavra) { // verifica se não estamos dentro de uma palavra (dentroDaPalavra == 0).
// Isso significa que o caractere atual é o início de uma nova palavra.
            cont++; 
            dentroDaPalavra = 1; 
        }
        // se o caractere atual for um espaço, marca que não estamos mais dentro de uma palavra
        else if (s[i] == ' ') {
            dentroDaPalavra = 0;
        }
    }

    return cont; 
} 

int contaVogais(char s[]) {
    char vogais[] = "aeiouAEIOU";
    int i, j;
    int numvogais = 0;
    
    for (i = 0; s[i] != '\0'; i++) {
        for (j = 0; vogais[j] != '\0'; j++) {
            if (s[i] == vogais[j]) {
                numvogais++;
            }
        }
    }
    return numvogais;
}






