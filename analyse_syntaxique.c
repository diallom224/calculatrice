#include <stdio.h>
#include <stdlib.h>
#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"

/* Variables globales */
static int error_count = 0;

/* Fonction pour signaler une erreur syntaxique */
void erreur_syntaxe() {
    printf("Erreur de syntaxe ligne %d, colonne %d\n",
           lexeme_courant().ligne,
           lexeme_courant().colonne);
    error_count++;
    exit(1);
}

/* Prototypes des nouvelles fonctions */
int seq_terme();
int suite_seq_terme(int gauche);
int seq_facteur();
int suite_seq_facteur(int gauche);
int facteur();
Nature_Lexeme op1();
Nature_Lexeme op2();

/* Analyse et évaluation d'une EAG */
void analyser(char *fichier, int *resultat) {
    demarrer(fichier);

    /* Début de l'analyse */
    *resultat = seq_terme();

    /* Vérification de la fin du fichier */
    if (!fin_de_sequence()) {
        erreur_syntaxe();
    }

    /* Affichage du résultat */
    if (error_count == 0) {
        printf("BRAVO : Expression correcte ! Résultat = %d\n", *resultat);
    }

    /* Fermeture du fichier */
    arreter();
}

/* Analyse récursive d'une séquence de termes */
int seq_terme() {
    int gauche = seq_facteur();
    return suite_seq_terme(gauche);
}

/* Suite de seq_terme (gestion des + et -) */
int suite_seq_terme(int gauche) {
    if (lexeme_courant().nature == PLUS || lexeme_courant().nature == MOINS) {
        Nature_Lexeme operateur = op1();
        int droit = seq_facteur();

        /* Application de l'opération */
        if (operateur == PLUS) {
            gauche = gauche + droit;
        } else if (operateur == MOINS) {
            gauche = gauche - droit;
        }

        return suite_seq_terme(gauche);
    }

    return gauche;  // Fin de la séquence
}

/* Analyse récursive d'une séquence de facteurs */
int seq_facteur() {
    int gauche = facteur();
    return suite_seq_facteur(gauche);
}

/* Suite de seq_facteur (gestion des * et /) */
int suite_seq_facteur(int gauche) {
    if (lexeme_courant().nature == MUL || lexeme_courant().nature == DIV) {
        Nature_Lexeme operateur = op2();
        int droit = facteur();

        /* Application de l'opération */
        if (operateur == MUL) {
            gauche = gauche * droit;
        } else if (operateur == DIV) {
            if (droit == 0) {
                printf("Erreur : Division par zéro\n");
                exit(1);
            }
            gauche = gauche / droit;
        }

        return suite_seq_facteur(gauche);
    }

    return gauche;  // Fin de la séquence
}

/* Analyse d'un facteur */
int facteur() {
    int resultat = 0;

    if (lexeme_courant().nature == ENTIER) {
        resultat = lexeme_courant().valeur.valeur_entiere;
        avancer();
    } 
    else if (lexeme_courant().nature == PARO) {
        avancer();  // Consommer '('
        resultat = seq_terme();
        
        if (lexeme_courant().nature == PARF) {
            avancer();  // Consommer ')'
        } else {
            erreur_syntaxe();
        }
    } 
    else {
        erreur_syntaxe();
    }

    return resultat;
}

/* Analyse d'un opérateur de niveau 1 (+ ou -) */
Nature_Lexeme op1() {
    Nature_Lexeme nature = lexeme_courant().nature;

    if (nature == PLUS || nature == MOINS) {
        avancer();
        return nature;
    }

    erreur_syntaxe();
    return ERREUR;
}

/* Analyse d'un opérateur de niveau 2 (* ou /) */
Nature_Lexeme op2() {
    Nature_Lexeme nature = lexeme_courant().nature;

    if (nature == MUL || nature == DIV) {
        avancer();
        return nature;
    }

    erreur_syntaxe();
    return ERREUR;
}
