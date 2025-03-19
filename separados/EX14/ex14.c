#include <stdio.h>

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

int main() {
    char texto[] = "abacba"; 
    char maisFrequente = charMaisFreq(texto); 

    printf("O caractere mais frequente é: %c\n", maisFrequente); 

    return 0;
}