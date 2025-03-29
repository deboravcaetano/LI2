#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_VALORES 10000
#define MAX_GRUPOS 100

typedef struct {
    int pos;      
    int ord;      
    int grp;      
    double ordRel; 
    int val;      
    int prm;      
} Tabela;


double valor_referencia(int num_grupos) {
    double p = 0.95;
    double df = num_grupos - 1;
    double a = (p < 0.5) ? sqrt(-2.0 * log(p)) : sqrt(-2.0 * log(1.0 - p));
    double poly = 2.515517 + 0.802853 * a + 0.010328 * a * a;
    double q = 1.0 + 1.432788 * a + 0.189269 * a * a + 0.001308 * a * a * a;
    double z = (p < 0.5) ? -(a - poly / q) : (a - poly / q);
    double x = df * pow(1.0 - 2.0 / (9.0 * df) + z * sqrt(2.0 / (9.0 * df)), 3.0);
    return x;
}


int lerDados(Tabela tabela[], int grupos[], int *totalValores, int *numGrupos) {
    if((scanf("%d", numGrupos))!=1) return 1; 

    for (int g = 1; g <= *numGrupos; g++) { 
        int N;
        if ((scanf("%d", &N))!=1) return 1; 
        grupos[g] = N;

        for (int i = 0; i < N; i++) { 
            if((scanf("%d", &tabela[*totalValores].val))!=1) return 1; 
            tabela[*totalValores].grp = g; 
            (*totalValores)++; 
        }
    }
    return 1;
}

void ordenarTabela(Tabela tabela[], int totalValores) {
    for (int i = 0; i < totalValores-1; i++) {
        for (int j = 0; j < totalValores-i-1; j++) {
            if (tabela[j].val > tabela[j+1].val) {
                Tabela temp = tabela[j];
                tabela[j] = tabela[j+1];
                tabela[j+1] = temp;
            } // quando troca dois elementos, toda a estrutura é trocada
        }
    }
}


void calcularTabela(Tabela tabela[], int totalValores) {
    ordenarTabela(tabela, totalValores);

    for (int i = 0; i < totalValores; i++) {
        tabela[i].pos = i;      // posição no array começa em 0
        tabela[i].ord = i + 1;  // ordem (sempre pos + 1) 
        
        int j = i;
        while (j > 0 && tabela[j-1].val == tabela[i].val) {
            j--; // retrocede até ao val que é igual e aparece primeiro
        }
        tabela[i].prm = j;
    }

    // calcular OrdRel (média das ordens para valores iguais)
    for (int i = 0; i < totalValores; i++) {
        int start = i;
        while (i < totalValores - 1 && tabela[i+1].val == tabela[i].val) { //percorre da primeira ocorrencia até à ultima igual
            i++; // descobrimos o indice da ultima igual
        }
        double media = (tabela[start].ord + tabela[i].ord) / 2.0;
        for (int j = start; j <= i; j++) { // preenche todas essas linhas com o mesmo valor
            tabela[j].ordRel = media;
        }
    }
}

void imprimirTabela(Tabela tabela[], int totalValores) {

    printf(" Pos  Ord  Grp     OrdRel  Val  Prm\n");
    for (int i = 0; i < totalValores; i++) {
        printf("%4d %4d %4d %10.1f %4d %4d\n",  
               tabela[i].pos,
               tabela[i].ord,
               tabela[i].grp,
               tabela[i].ordRel,
               tabela[i].val,
               tabela[i].prm);
    }
}


double calcularMedias(Tabela tabela[], int totalValores, int numGrupos, int grupos[], double somaOrdRelGrupos[]) {
    double mediaGlobal = (totalValores + 1) / 2.0;

    printf("\n  Grp MediaOrdem\n");
    for (int g = 1; g <= numGrupos; g++) {
        somaOrdRelGrupos[g] = 0;
        int cont = 0;
        
        for (int i = 0; i < totalValores; i++) {
            if (tabela[i].grp == g) {
                somaOrdRelGrupos[g] += tabela[i].ordRel;
                cont++;
            }
        }
        printf(" %4d    %7.1f\n", g, somaOrdRelGrupos[g] / grupos[g]);  
    }
    
    printf("Todos    %7.1f\n", mediaGlobal);  
    return mediaGlobal;
}

void calcCompX(int numGrupos, int totalValores, int grupos[], double somaOrdRelGrupos[], double mediaGlobal) {
    double S = 0.0;

    for (int g = 1; g <= numGrupos; g++) {
        double mediaGrupo = somaOrdRelGrupos[g] / grupos[g];
        S += grupos[g] * pow(mediaGrupo - mediaGlobal, 2);
    }

    double X = ((double)(totalValores - 1) / totalValores) *
               ((12.0 * S) / (totalValores * totalValores - 1));

    printf("\nCalc: %.2f\n", X);

    double refValor = valor_referencia(numGrupos);
    
    printf(" Ref: %.2f\n", refValor);
    
    if (X < refValor) {
        printf("Nao\n");
    } else {
        printf("Sim\n");
    }
}

int main() {
    Tabela tabela[MAX_VALORES]; 

    int grupos[MAX_GRUPOS]; 

    double somaOrdRelGrupos[MAX_GRUPOS] = {0}; 

    int totalValores = 0, numGrupos = 0;

    lerDados(tabela, grupos, &totalValores, &numGrupos);

    calcularTabela(tabela, totalValores);

    imprimirTabela(tabela, totalValores);

    double mediaGlobal = calcularMedias(tabela, totalValores, numGrupos, grupos, somaOrdRelGrupos);

    calcCompX(numGrupos, totalValores, grupos, somaOrdRelGrupos, mediaGlobal);

    return 0;
}




