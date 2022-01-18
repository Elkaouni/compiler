#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

char* error_names[] = {"ERR",   "ERR_PROGRAM",
        "ERR_REPEAT", "ERR_UNTIL", "ERR_CONST",  "ERR_VAR",
        "ERR_BEGIN",  "ERR_END",   "ERR_IF",     "ERR_THEN",
        "ERR_WHILE",  "ERR_DO",    "ERR_FOR",    "ERR_OF",
        "ERR_WRITE",  "ERR_STRING","ERR_READ",   "ERR_CASE",
        "ERR_STRING", "ERR_ID",    "ERR_NUM",    "ERR_INTO",
        "ERR_DOWNTO", "ERR_PV",    "ERR_PT",     "ERR_DPT",
        "ERR_VIR",    "ERR_AFF",   "ERR_INF",    "ERR_INFEG",
        "ERR_SUP",    "ERR_SUPEG", "ERR_DIFF",   "ERR_PO",
        "ERR_PF",     "ERR_PLUS",  "ERR_MINUS",  "ERR_MULT",
        "ERR_EG",     "ERR_DIV",   "ERR_INTLIT", "ERR_FACT",
        "ERR_INST",   "ERR_EOF"
        };


void Erreur(int cod_err)
{
        printf("\n### Error type: %s. Wrong tokens used. ###\n", error_names[cod_err]);
        exit(EXIT_FAILURE);
}


void Erreur_sem(int cod)
{
    printf("\n### Semantic error: ");
    switch(cod)
    {
        case not_declared:
            printf("ID wasn't previously declared.");
            break;
        case double_dec:
            printf("Beware of the double declaration.");
            break;
        case const_aff:
            printf("You tried to change the value of a CONST.");
            break;
        case div_zero:
            printf("You tried to divide by zero!");
            break;
        case prog_id:
            printf("You can't use the program ID.");
            break;
    }
    printf(" ###\n");
    exit(EXIT_FAILURE);
}






