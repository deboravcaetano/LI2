#include <stdio.h>


void truncW (char t[], int n){
    int i,j,cont=0;

    for(i=0,j=0;t[i]!='\0';i++){
        if(cont<n){
            t[j]=t[i];
            cont++;
            j++;
        } else {
            if(t[i]==' '){
                cont=0;
                t[j]=t[i];
                j++;
            }

        }
    t[j]='\0';
    }
}

int main() {
    char texto[] = "liberdade igualdade fraternidade"; 
    int n = 4; 

    printf("Texto original: %s\n", texto);


    truncW(texto, n);

    printf("Texto truncado: %s\n", texto);

    return 0;
}


