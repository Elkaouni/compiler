static char* token_names[] = { "PROGRAM_TOKEN", "REPEAT_TOKEN",
		"UNTIL_TOKEN" , "CONST_TOKEN" ,	"VAR_TOKEN"   , "BEGIN_TOKEN" ,
		"ELSE_TOKEN"  , "END_TOKEN"   ,	"IF_TOKEN"    , "THEN_TOKEN"  ,
		"WHILE_TOKEN" ,	"DO_TOKEN"    ,	"FOR_TOKEN"   ,	"OF_TOKEN"    ,
		"READ_TOKEN"  ,	"CASE_TOKEN"  ,	"WRITE_TOKEN" ,	"STRING_TOKEN",
		"ID_TOKEN"    ,	"NUM_TOKEN"   ,	"INTO_TOKEN"  ,	"DOWNTO_TOKEN",
		"PV_TOKEN"    ,	"PT_TOKEN"    ,	"DPT_TOKEN"   ,	"VIR_TOKEN"   ,
		"AFF_TOKEN"   ,	"INF_TOKEN"   ,	"INFEG_TOKEN" ,	"SUP_TOKEN"   ,
		"SUPEG_TOKEN" ,	"DIFF_TOKEN"  ,	"PO_TOKEN"    ,	"PF_TOKEN"    ,
		"PLUS_TOKEN"  ,	"MINUS_TOKEN" ,	"MULT_TOKEN"  ,	"EG_TOKEN"    ,
		"DIV_TOKEN"   ,	"INTLIT_TOKEN",	"EOF_TOKEN"   , "ERR_TOKEN"
    };

typedef struct
{
	enum
	{
		PROGRAM_TOKEN,
		REPEAT_TOKEN,
		UNTIL_TOKEN,
		CONST_TOKEN,
		VAR_TOKEN,
		BEGIN_TOKEN,
		ELSE_TOKEN,
		END_TOKEN,
		IF_TOKEN,
		THEN_TOKEN,
		WHILE_TOKEN,
		DO_TOKEN,
		FOR_TOKEN,
		OF_TOKEN,
		READ_TOKEN,
		CASE_TOKEN,
		WRITE_TOKEN,
		STRING_TOKEN,
		ID_TOKEN,
		NUM_TOKEN,
		INTO_TOKEN,
		DOWNTO_TOKEN,
		PV_TOKEN,
		PT_TOKEN,
		DPT_TOKEN,
		VIR_TOKEN,
		AFF_TOKEN,
		INF_TOKEN,
		INFEG_TOKEN,
		SUP_TOKEN,
		SUPEG_TOKEN,
		DIFF_TOKEN,
		PO_TOKEN,
		PF_TOKEN,
		PLUS_TOKEN,
		MINUS_TOKEN,
		MULT_TOKEN,
		EG_TOKEN,
		DIV_TOKEN,
		INTLIT_TOKEN, // NUMBERS
		EOF_TOKEN,
        ERR_TOKEN
	} type;
	char *value;
} Token;

Token *token_init(int type, char *value);
