typedef struct{
    enum{
        ERR,
        ERR_PROGRAM,
        ERR_REPEAT,
        ERR_UNTIL,
        ERR_CONST,
        ERR_VAR,
        ERR_BEGIN,
        ERR_ELSE,
        ERR_END,
        ERR_IF,
        ERR_THEN,
        ERR_WHILE,
        ERR_DO,
        ERR_FOR,
        ERR_OF,
        ERR_WRITE,
        ERR_STRING,
        ERR_READ,
        ERR_CASE,
        ERR_ID,
        ERR_NUM,
        ERR_INTO,
        ERR_DOWNTO,
        ERR_PV,
        ERR_PT,
        ERR_DPT,
        ERR_VIR,
        ERR_AFF,
        ERR_INF,
        ERR_INFEG,
        ERR_SUP,
        ERR_SUPEG,
        ERR_DIFF,
        ERR_PO,
        ERR_PF,
        ERR_PLUS,
        ERR_MINUS,
        ERR_MULT,
        ERR_EG,
        ERR_DIV,
        ERR_INTLIT,
        ERR_FACT,
        ERR_INST,
        ERR_EOF
    } error_type;
} Erreurs;

enum{
    not_declared,
    double_dec,
    const_aff,
    div_zero,
    prog_id
}err_sem;

void Erreur(int cod_err);
void Erreur_sem(int cod);

