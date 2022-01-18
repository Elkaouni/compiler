#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
#include "file.h"
#include <ctype.h>
#include <string.h>

//static char car_cour;
static char *keywords[] = {"program", "const", "var", "begin",
    "end"  , "if", "then", "while", "do","read", "write", };

static int keywords_value[]={PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN, BEGIN_TOKEN,
         END_TOKEN  , IF_TOKEN, THEN_TOKEN, WHILE_TOKEN, DO_TOKEN , READ_TOKEN ,
         WRITE_TOKEN, };

static char car_cour;

void get_next_char()
{
	if (reading)
	{
		char c = reading;
		car_cour= c;
		reading = 0;
		return;
	}
	car_cour= fgetc(file);
	if (car_cour == '\n')
	{
		ligne++;
	}
}

void skip_space()
{
	get_next_char();
	//while (car_cour == ' ' || car_cour == '\n' || car_cour == '\t' || car_cour == '\r' || car_cour == '\f')
	while ( car_cour== ' ' || car_cour=='\n'
         || car_cour=='\t'|| car_cour=='\r'
         || car_cour=='\f')

	{
		get_next_char();
	}
}

int lire_num()
{
	char *value = calloc(2, sizeof(char));
    char *c = calloc(2, sizeof(char)); // to stringfy car_cour

	while (isdigit(car_cour))
	{
        c[0] = car_cour;
        c[1] = '\0';
		strcat(value, c);
		get_next_char();
	}
    reading = car_cour;
	free(c);
	return atoi(value);
}

char *get_id()
{
	char *value = calloc(1, sizeof(char));
	value[0] = '\0';

	while (isalnum(car_cour))
	{
		char *s = calloc(2, sizeof(char));
        s[0] = car_cour;
        s[1] = '\0';

		value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
		strcat(value, s);
		get_next_char();
	}
	reading = car_cour;
	return value;
}

char *lire_string()
{
	get_next_char();
	char *val = calloc(1, sizeof(char));
	val[0] = '\0';
	while (car_cour != '"')
	{
		char *s = calloc(2, sizeof(char));
        s[0] = car_cour;
        s[1] = '\0';
        val = realloc(val, (strlen(val) + strlen(s) + 1) * sizeof(char));
		strcat(val, s);
		get_next_char();
	}
	get_next_char();
	return val;
}

Token *Symb_suiv(Token *token)
{
	skip_space();
	char *token_value = calloc(2, sizeof(char));
	token_value[0] = car_cour;
	token_value[1] = '\0';

	switch (car_cour)
	{
	case EOF:
		return token_init(EOF_TOKEN, "EOF");

	case '+':
		return token_init(PLUS_TOKEN, token_value);
	case ';':
		return token_init(PV_TOKEN, token_value);
	case '.':
		return token_init(PT_TOKEN, token_value);
	case ',':
		return token_init(VIR_TOKEN, token_value);
	case '(':
		return token_init(PO_TOKEN, token_value);
	case ')':
		return token_init(PF_TOKEN, token_value);
	case '-':
		return token_init(MINUS_TOKEN, token_value);
	case '/':
		return token_init(DIV_TOKEN, token_value);
	case '*':
		return token_init(MULT_TOKEN, token_value);
	default:
	{
		if (car_cour == ':')
		{
			char *value = calloc(3, sizeof(char));
			value[0] = car_cour;
 			get_next_char();
			if (car_cour == '=') //affectation
			{
                value[1] = car_cour;
                value[2] = '\0';
				return token_init(AFF_TOKEN, value);
			}
			free(value);
			reading = car_cour;

		}
		if (car_cour == '<')
		{
			char *value = calloc(1, sizeof(char));
			value[0] = car_cour;
			get_next_char();
			if (car_cour == '=') //<=
			{
				char *s = calloc(2, sizeof(char));
                s[0] = car_cour;
                s[1] = '\0';
				value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
				strcat(value, s);
				return token_init(INFEG_TOKEN, token_value);
			}
			if (car_cour == '>')
			{
				char *s = calloc(2, sizeof(char));
                s[0] = car_cour;
                s[1] = '\0';
				value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
				strcat(value, s);
				return token_init(DIFF_TOKEN, token_value);
			}
			free(value);
			reading = car_cour;

			return token_init(INF_TOKEN, token_value);
		}
		if (car_cour == '>')
		{
			char *value = calloc(1, sizeof(char));
			value[0] = car_cour;
			get_next_char();
			if (car_cour == '=')
			{
				char *s = calloc(2, sizeof(char));
                s[0] = car_cour;
                s[1] = '\0';
				value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
				strcat(value, s);
				return token_init(SUPEG_TOKEN, token_value);
			}
			free(value);
			reading = car_cour;

			return token_init(SUP_TOKEN, token_value);
		}
		if (isdigit(car_cour))
		{
			int number = lire_num();

			int length = snprintf(NULL, 0, "%d", number);
            char *str = malloc(length + 1);
            snprintf(str, length + 1, "%d", number);

			return token_init(INTLIT_TOKEN, str);
		}
		if (isalpha(car_cour))
		{
			char *identificateur = get_id();
			int length = sizeof(keywords) / sizeof(keywords[0]);
			for (int i = 0; i < length; i++)
			{
				if (!strcmp(identificateur, keywords[i]))
				{
					return token_init(keywords_value[i], identificateur);
				}
			}

			return token_init(ID_TOKEN, identificateur);
		}
		if (car_cour == '"')
		{
			char *string = lire_string();
			return token_init(STRING_TOKEN, string);
		}
		return token_init(ERR_TOKEN, token_value);
	}
	};
}


