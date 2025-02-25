#include <stdio.h>
#include <stdlib.h>
#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <fichier d'expression>\n", argv[0]);
        return 1;
    }

    int resultat;
    analyser(argv[1], &resultat);  // Passer les deux arguments à analyser
    //printf("Résultat de l'analyse : %d\n", resultat);

    return 0;
}
