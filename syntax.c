#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "syntax.h"
#define _extern
#include "File.h"
#undef _extern
#include "codeGenerator.h"
#include "sem.h"


sym sym_cour, sym_precPCV, sym_prec;


static int index =0;
static T_TAB_IDF TAB_IDFS[100];
int ExistInTab(sym symbole)
{
    for(int i=0; i<index; i++)
    {
        if(strcmp(symbole.nom, TAB_IDFS[i].NOM)==0)
            return 1;
    }
    return 0;
}
int isVar(sym symbole)
{
    for(int i=0; i<index; i++)
    {
        if(strcmp(symbole.nom, TAB_IDFS[i].NOM)==0)
        {
            if(TAB_IDFS[i].TIDF != TVAR)
            {
                if(TAB_IDFS[i].TIDF == TPROG)
                    return 0;
                else
                    return -1;
            }
            else
                return 1;
        }
    }
    return 0;
}
int isProgId(sym symbole)
{
    for(int i=0; i<index; i++)
    {
        if(strcmp(symbole.nom, TAB_IDFS[i].NOM)==0)
        {
            if(TAB_IDFS[i].TIDF != TPROG)
                return 0;
            else
                return 1;
        }
    }
    return 0;
}

void ajouter(sym symbole,int type)
{
    strcpy(TAB_IDFS[index].NOM, symbole.nom);
    TAB_IDFS[index].TIDF = type;
    index++;
}
void ajouter_val(float val, sym symbole)
{
    for(int i=0; i<index; i++)
    {
        if(strcmp(symbole.nom, TAB_IDFS[i].NOM)==0)
            TAB_IDFS[i].value = val;
    }
}

int get_adresse(sym id)
{
    for(int j = 0; j<index; j++)
    {
        if(strcmp(id.nom, TAB_IDFS[j].NOM)==0)
        {
            return j;
        }
    }
    return -1;
}

static char* type[] = {"TPROG", "TCONST", "TVAR"};
void printTAB_IDFS()
{
    int i=0;
    printf("Printing saved program id, consts and vars:\n");
    for(i = 0; i<index; i++)
    {
        printf("symbole: %s\t\ttype: %s\t\tvalue: %.1f\n", TAB_IDFS[i].NOM, type[TAB_IDFS[i].TIDF], TAB_IDFS[i].value);
    }
}

sym lexer_toxens[100];
int taille;
int init_lexer_token_list(sym* list_source, int size_source)
{
   if(size_source> 100)
        return 0;
   int i=0;
   for(i=0; i<size_source; i++)
   {
       lexer_toxens[i] = list_source[i];
       if(lexer_toxens[i].code == INTLIT_TOKEN)
       {
           lexer_toxens[i].code = NUM_TOKEN;
       }
   }
   taille = size_source;
   return 1;
}

int t=1;
void premier_sym()
{
    sym_cour = lexer_toxens[0];
}
void Sym_suiv()
{
    if(t==taille)
        return;
    sym_cour.code = lexer_toxens[t].code;
    strcpy(sym_cour.nom, lexer_toxens[t].nom);
    t++;
}

void printLexer()
{
    printf("_________________________________\n");
    printf("CHECKING THE LEXER LIST:");
    for(int i=0; i<taille; i++)
    {
        printf("nom: %s code: %s ||\t\t", lexer_toxens[i].nom, token_names[lexer_toxens[i].code]);
        if(i%3 ==0)
                printf("\n");
    }
    printf("DONE CHECKING. NOW LET'S START.\n");
    printf("_________________________________\n");
}

float value;
void set_value(float val)
{
    value = val;
}
void Test_Symbole (int token, int COD_ERR)
{
   //printf("\nChecking sym cour: nom->%s  code->%s\n", sym_cour.nom, token_names[sym_cour.code]);
    if(sym_cour.code == token)
    {
        if(sym_cour.code == PROGRAM_TOKEN)
            sym_precPCV = sym_cour;
        else if(sym_cour.code == CONST_TOKEN)
            sym_precPCV = sym_cour;
        else if(sym_cour.code == VAR_TOKEN)
            sym_precPCV = sym_cour;

        else if(sym_cour.code == ID_TOKEN)
        {
            sym_prec = sym_cour;
        }
        else if(sym_cour.code == NUM_TOKEN)
        {
            value = atoi(sym_cour.nom);
        }
     //   printf("Done. Next...\n");
        Sym_suiv();
    }
    else
    {
       // printLexer();
        Erreur(COD_ERR);
    }
}

void addID()
{
    if(strcmp(sym_precPCV.nom, "program")==0)
        ajouter(sym_prec, TPROG);

    else if(sym_precPCV.code == CONST_TOKEN)
    {
        ajouter(sym_prec, TCONST);
    }
     else if(sym_precPCV.code == VAR_TOKEN)
    {
        ajouter(sym_prec, TVAR);
    }
}

 /* Starting code generation*/

void PROGRAM() //code generated
{
  //  printLexer();
    Test_Symbole(PROGRAM_TOKEN, ERR_PROGRAM);
    Test_Symbole(ID_TOKEN, ERR_ID);
    addID();
    Test_Symbole(PV_TOKEN, ERR_PV);
    BLOCK();
    generer1(HLT);
    Test_Symbole(PT_TOKEN, ERR_PT);

    printf("\n------->Done :) \n");
    printf("\n_________________________\n");
    printTAB_IDFS(TAB_IDFS, index);
    printf("_________________________\n");
    print_code();
    SavePCodeToFile(FILE_SORTIE);
    printf("\nSaved in file \"result.rh\".   \n");
}

void BLOCK() //code generated
{
     CONSTS();
     VARS();
     printf("\n#### OFFSET IS %d. ", offset);
     printf("Memory = %d. ###\n", offset+1);
     generer2(INT, offset);
     INSTS();
}

void CONSTS() //code generated
{
    //const ID = NUM ; { ID = NUM ; } | e
    switch(sym_cour.code)
    {
        case CONST_TOKEN:
        {
            Test_Symbole(CONST_TOKEN,ERR_CONST);
            Test_Symbole(ID_TOKEN,ERR_ID); //sym_prec saved
            if(!ExistInTab(sym_prec))
                addID();
            else
                Erreur_sem(double_dec);

            int id_addr = get_adresse(sym_prec);
            if(id_addr == -1)
            {
                printf("\n### Error while generating code: Couldn't find id. ###\n");
                exit(EXIT_FAILURE);
            }
            generer2(LDA, id_addr);
            Test_Symbole(AFF_TOKEN,ERR_AFF);
            Test_Symbole(NUM_TOKEN,ERR_NUM); //value saved
            ajouter_val(value, sym_prec);
            generer2(LDI, value);
            Test_Symbole(PV_TOKEN,ERR_PV);
            generer1(STO);

            offset++;
            while(sym_cour.code == ID_TOKEN)
            {
                Test_Symbole(ID_TOKEN,ERR_ID);
                if(!ExistInTab(sym_prec))
                    addID();
                else
                    Erreur_sem(double_dec);

                int id_addr = get_adresse(sym_prec);
                if(id_addr == -1)
                {
                    printf("\n### Error while generating code: Couldn't find id. ###\n");
                    exit(EXIT_FAILURE);
                }
                generer2(LDA, id_addr);
                Test_Symbole(AFF_TOKEN,ERR_AFF);
                Test_Symbole(NUM_TOKEN,ERR_NUM);
                generer2(LDI, value);
                Test_Symbole(PV_TOKEN,ERR_PV);
                generer1(STO);
                offset++;
            }
        }
        case VAR_TOKEN:
            break;
        case BEGIN_TOKEN:
            break;
        default:
            Erreur(ERR_CONST);
    }
}

void VARS() //code generated
{
    // var ID { , ID } ; | e
    Test_Symbole(VAR_TOKEN,ERR_VAR);
    Test_Symbole(ID_TOKEN,ERR_ID);
    if(!ExistInTab(sym_prec))
        addID();
    else
        Erreur_sem(double_dec);
    int id_addr = get_adresse(sym_prec);
    if(id_addr == -1)
    {
        printf("\n### Error while generating code: Couldn't find id. ###\n");
        exit(EXIT_FAILURE);
    }
    generer2(LDA, id_addr);
    offset++;

    while(sym_cour.code == VIR_TOKEN)
    {
        //Sym_suiv();
        Test_Symbole(VIR_TOKEN, ERR_VIR);
        Test_Symbole(ID_TOKEN,ERR_ID);
        if(!ExistInTab(sym_prec))
            addID();
        else
            Erreur_sem(double_dec);
        int id_addr = get_adresse(sym_prec);
        if(id_addr == -1)
        {
            printf("\n### Error while generating code: Couldn't find id. ###\n");
            exit(EXIT_FAILURE);
        }
        generer2(LDA, id_addr);
        offset++;
    }
    Test_Symbole(PV_TOKEN,ERR_PV);
}

void INSTS() //code generated
{
    // begin INST { ; INST } end
    Test_Symbole(BEGIN_TOKEN,ERR_BEGIN);
    INST(); //génère son code
    while(sym_cour.code == PV_TOKEN)
    {
        Test_Symbole(PV_TOKEN,ERR_PV); //Symb_suiv()
        INST(); //génère son code
    }
    Test_Symbole(END_TOKEN,ERR_END);
}

void SI() // code generated
{
    // IF COND THEN INST [ELSE INST | e]
    Test_Symbole(IF_TOKEN,ERR_IF);
    COND(); //genere son code
    int addr_cond =PC;
    generer1(BZE); //,???)
    Test_Symbole(THEN_TOKEN, ERR_THEN);
    INST(); //genere son code
    if(sym_cour.code == ELSE_TOKEN)
    {
        INST(); //genere son code
        genererBZE(addr_cond,PC);
    }
}

void AFFECT() //code generated
{
    // AFFEC ::= ID := EXPR
    Test_Symbole(ID_TOKEN, ERR_ID);
    if(!ExistInTab(sym_prec))
        Erreur_sem(not_declared);

    int err = isVar(sym_prec);
    if(err ==0)
            Erreur_sem(const_aff);
    else if(err == -1)
            Erreur_sem(prog_id);
    int id_addr = get_adresse(sym_prec);
    if(id_addr == -1)
    {
        printf("\n### Error while generating code: Couldn't find id. ###\n");
        exit(EXIT_FAILURE);
    }
    sym affect_sym = sym_prec;
    generer2(LDA, id_addr);
    Test_Symbole(AFF_TOKEN, ERR_AFF);
    EXPR(); // get value from here + génère son code
    ajouter_val(value, affect_sym);
    generer1(STO);
}

void TANTQUE() //code generated
{
    //while cond do inst
    Test_Symbole(WHILE_TOKEN,ERR_WHILE);
    int addr_BRN = PC;
    COND(); //génère son code
    int addr_cond =PC;
    generer1(BZE);
    Test_Symbole(DO_TOKEN, ERR_DO);
    INST(); //génère son code
    generer2(BRN, addr_BRN);
    genererBZE(addr_cond,PC);

}

void COND() // code generated
{
    int op, c_token;
    EXPR();
    switch(sym_cour.code)
    {
        case EG_TOKEN:
            {
                c_token = EG_TOKEN;
                op = EQL;
                break;
            }
        case DIFF_TOKEN:
            {
                c_token = DIFF_TOKEN;
                op = NEQ;
                break;
            }
        case INF_TOKEN:
            {
                c_token = INF_TOKEN;
                op= LSS;
                break;
            }
        case SUP_TOKEN:
            {
                c_token = SUP_TOKEN;
                op = GTR;
                break;
            }
        case INFEG_TOKEN:
            {
                c_token = INFEG_TOKEN;
                op = LEQ;
                break;
            }
        case SUPEG_TOKEN:
            {
                c_token = SUPEG_TOKEN;
                op = GEQ;
                break;
            }
        default:
            {
                c_token = -1;
            }
    }
    Test_Symbole (c_token, ERR);
    EXPR();
    generer1(op);
}

void INST() //code generated
{
    // INSTS | AFFEC | SI | TANTQUE | ECRIRE; | LIRE; | e
    switch(sym_cour.code)
    {
        case BEGIN_TOKEN:
            {
                INSTS(); //génère son code
                break;
            }
        case ID_TOKEN:
            {
                AFFECT(); //génère son code
                break;
            }
        case IF_TOKEN:
            {
                SI(); //génère son code
                break;
            }
        case WHILE_TOKEN:
            {
                TANTQUE(); //génère son code
                break;
            }
        case WRITE_TOKEN:
            {
                ECRIRE(); //sym_cour = PV_TOKEN //génère son code
                break;
            }
        case READ_TOKEN:
            {
               LIRE();  //sym_cour = PV_TOKEN //génère son code
               break;
            }
        case PV_TOKEN:
            {
               //Test_Symbole(PV_TOKEN, ERR_PV);

               break;
            }
        default:
            {
               //Erreur(ERR_INST);
            }
    }
}

void EXPR() //code generated
{
    // TERM { [+ | -] TERM }
    int op;
    float valeur=0;
    TERM(); //recupère value //genere son code
    valeur = value;

    while(sym_cour.code == PLUS_TOKEN || sym_cour.code == MINUS_TOKEN)
    {
        op = sym_cour.code;
        Sym_suiv();
        printf("Checked PLUS_TOKEN / Minus_token\n");

        TERM(); //recupère value //genere son code
        int val = value;
        switch(op)
        {
            case PLUS_TOKEN:
                op = ADD;
                valeur += val;
                break;
            case MINUS_TOKEN:
                op = SUB;
                valeur -= val;
                break;
        }
        generer1(op);
    }
    set_value(valeur);
}

void TERM() // code generated
{
    // FACT { [* | /] FACT }
    int op;
    float valeur=0;
    FACT(); // get value //genere son code
    valeur = value;
    while(sym_cour.code == MULT_TOKEN || sym_cour.code == DIV_TOKEN)
    {
        op = sym_cour.code;
        Sym_suiv();
        //printf("Checked mult_token / div_token");
        FACT(); // get value //genere son code
        int val =value;
        switch(op)
        {
            case MULT_TOKEN:
                op = MUL;
                valeur = valeur * val;
                break;
            case DIV_TOKEN:
                op = DIV;
                if(!val)
                    Erreur_sem(div_zero);
                valeur = valeur / val;
                break;
        }
        generer1(op);
    }
    set_value(valeur);
}

float get_value_id()
{
    for(int i=0; i<index; i++)
    {
        if(strcmp(sym_cour.nom, TAB_IDFS[i].NOM)==0)
        {
            value = TAB_IDFS[i].value;
        }
    }
}
void FACT() //generate code
{
    // ID | NUM | ( EXPR )
    switch(sym_cour.code)
    {
        case ID_TOKEN:
            {
                Test_Symbole(ID_TOKEN, ERR_ID);
                if(!ExistInTab(sym_prec))
                    Erreur_sem(not_declared);
                if(isProgId(sym_prec))
                    Erreur_sem(prog_id);

                int id_addr = get_adresse(sym_prec);
                if(id_addr == -1)
                {
                    printf("\n### Error while generating code: Couldn't find id. ###\n");
                    exit(EXIT_FAILURE);
                }
                generer2(LDA, id_addr);
                generer1(LDV);

                get_value_id();
                printf("value of %s is %f.\n", sym_prec.nom, value);
                break;
            }
        case NUM_TOKEN:
            {
                Test_Symbole(NUM_TOKEN, ERR_NUM); //get value
                generer2(LDI, value);
                break;
            }
        case PO_TOKEN:
            {
                //Sym_suiv();
                Test_Symbole(PO_TOKEN, ERR_PO);
                EXPR();
                Test_Symbole(PF_TOKEN, ERR_PF);
            }
        default:
            {
                Erreur(ERR_FACT);
            }
    }
}

void REPETER()
{
    //REPETER ::= REPEAT INST UNTIL COND
    Test_Symbole(REPEAT_TOKEN, ERR_REPEAT);
    INST();
    Test_Symbole(UNTIL_TOKEN, ERR_UNTIL);
    COND();
}

void POUR()
{
    //POUR ::= FOR ID DO:= NUM [INTO|DOWNTO] NUM DO INST
    Test_Symbole(FOR_TOKEN, ERR_FOR);
    Test_Symbole(ID_TOKEN, ERR_ID);
    if(!ExistInTab(sym_prec))
        Erreur_sem(not_declared);
    int err = isVar(sym_prec);
    if(err ==0)
            Erreur_sem(const_aff);
    else if(err == -1)
            Erreur_sem(prog_id);

    Test_Symbole(DO_TOKEN, ERR_DO);
    Test_Symbole(AFF_TOKEN, ERR_AFF);
    Test_Symbole(NUM_TOKEN, ERR_NUM);
    switch(sym_cour.code)
    {
        case INTO_TOKEN:
        {
            Test_Symbole(INTO_TOKEN, ERR_INTO);
            break;
        }
        case DOWNTO_TOKEN:
        {
            Test_Symbole(DOWNTO_TOKEN, ERR_DOWNTO);
            break;
        }
        default:
            Erreur(ERR_FOR);
    }
    Test_Symbole(NUM_TOKEN, ERR_NUM);
    Test_Symbole(DO_TOKEN, ERR_DO);
    INST();
}

void CAS()
{
    //CAS ::= CASE ID OF NUM : INST { NUM: INST} [ELSE INST |e] END
    Test_Symbole(CASE_TOKEN, ERR_CASE);
    Test_Symbole(ID_TOKEN, ERR_ID);
    if(!ExistInTab(sym_prec))
        Erreur_sem(not_declared);
    if(isProgId(sym_prec))
        Erreur_sem(prog_id);

    Test_Symbole(OF_TOKEN, ERR_OF);
    Test_Symbole(NUM_TOKEN, ERR_NUM);
    Test_Symbole(DPT_TOKEN, ERR_DPT);
    INST();
    while(sym_cour.code == NUM_TOKEN)
    {
        //Sym_suiv();
        Test_Symbole(NUM_TOKEN, ERR_NUM);
        Test_Symbole(DPT_TOKEN, ERR_DPT);
        INST();
    }

    if(sym_cour.code == ELSE_TOKEN)
    {
        Test_Symbole(END_TOKEN, ERR_END);
    }
}

void LIRE() //code generated
{
     // Read(ID*,{ID}*);
     Test_Symbole(READ_TOKEN, ERR_READ);
     Test_Symbole(PO_TOKEN, ERR_PO);
     Test_Symbole(ID_TOKEN, ERR_ID);
     if(!ExistInTab(sym_prec))
        Erreur_sem(not_declared);
     int err = isVar(sym_prec);
     if(err ==0)
        Erreur_sem(const_aff);
     else if(err == -1)
        Erreur_sem(prog_id);

     int id_addr = get_adresse(sym_prec);
     if(id_addr == -1)
     {
         printf("\n### Error while generating code: Couldn't find id. ###\n");
         exit(EXIT_FAILURE);
     }
     generer2(LDA, id_addr);
     generer1(INN);

     while(sym_cour.code== VIR_TOKEN){
        //Sym_suiv();
        Test_Symbole(VIR_TOKEN, ERR_VIR);
        Test_Symbole(ID_TOKEN, ERR_ID);
        if(!ExistInTab(sym_prec))
            Erreur_sem(not_declared);
        int err = isVar(sym_prec);
        if(err ==0)
                Erreur_sem(const_aff);
        else if(err == -1)
                Erreur_sem(prog_id);

        int id_addr = get_adresse(sym_prec);
        if(id_addr == -1)
        {
            printf("\n### Error while generating code: Couldn't find id. ###\n");
            exit(EXIT_FAILURE);
        }
        generer2(LDA, id_addr);
        generer1(INN);
     }
     Test_Symbole(PF_TOKEN, ERR_PF);
     //Test_Symbole(PV_TOKEN, ERR_PV);
     if(sym_cour.code != PV_TOKEN)
        Erreur(ERR_PV);
}

void ECRIRE() //code generated
{
     // write(ID*,{ID}*);
     Test_Symbole(WRITE_TOKEN, ERR_READ);
     Test_Symbole(PO_TOKEN, ERR_PO);
     Test_Symbole(ID_TOKEN, ERR_ID);
     if(!ExistInTab(sym_prec))
        Erreur_sem(not_declared);

     int id_addr = get_adresse(sym_prec);
     if(id_addr == -1)
     {
         printf("\n### Error while generating code: Couldn't find id. ###\n");
         exit(EXIT_FAILURE);
     }
     generer2(LDA, id_addr);
     generer1(LDV);
     generer1(PRN);

     while(sym_cour.code== VIR_TOKEN){
        //Sym_suiv();
        Test_Symbole(VIR_TOKEN, ERR_VIR);
        Test_Symbole(ID_TOKEN, ERR_ID);
        if(!ExistInTab(sym_prec))
            Erreur_sem(not_declared);
        if(isProgId(sym_prec))
            Erreur_sem(prog_id);

        int id_addr = get_adresse(sym_prec);
         if(id_addr == -1)
         {
             printf("\n### Error while generating code: Couldn't find id. ###\n");
             exit(EXIT_FAILURE);
         }
         generer2(LDA, id_addr);
         generer1(LDV);
         generer1(PRN);
     }
     Test_Symbole(PF_TOKEN, ERR_PF);
     if(sym_cour.code != PV_TOKEN)
        Erreur(ERR_PV);
     //Test_Symbole(PV_TOKEN, ERR_PV);
}
