#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
���ⲿ���У��㽫ʹ��ͼת���㷨�ֹ�ʵ��һ��С�͵Ĵʷ���������
* �����������룺�洢���ı��ļ��е��ַ����У��ַ�ȡ��ASCII�ַ������ļ��п��ܰ������ּǺţ��ؼ���if������C���Ա�׼�ı�ʶ�����ո�����س���\n��
* ���������������ӡ����ʶ��ı�ʶ�������ࡢ���кš��к���Ϣ��
��ʾ��������������ı��ļ���
ifx if iif  if
iff    if
������Ӧ���ǣ�
ID(ifx) (1, 1)
IF        (1, 5)
ID(iif)  (1, 8)
IF       (1, 13)
ID(iff) (2, 1)
IF       (2, 8)
*/
#define MAX_TOKEN_SIZE (100)

typedef enum kind {
	IF_TOKEN, 
	ID_TOKEN, 
	BAD_TOKEN,
	END_OF_LINE_TOKEN
} TokenKind;

typedef struct token {
	TokenKind k;
	int pos;
	char str[MAX_TOKEN_SIZE];
}Token;

typedef enum {
	INITIAL_STATUS,
	ID_FS_STATUS,
	ID_ACC_STATUS,
	IF_I_STATUS,
	IF_F_STATUS
} LexerStatus;

static char *st_line;
static int st_line_pos; // where is the scanner in
static int line_num = 0;

//////////////////////////////
// Judge function
int isdigit(char c) {
	if (c >= '0' && c <= '9')
		return 1;
	else
		return 0;
}

int isletter(char c) {
	if ((c >= 'a' && c <= 'z') | (c >= 'A' && c <= 'Z'))
		return 1;
	else
		return 0;
}

int isspace(char c) {
	if ((c == ' ') | (c == '\n')) 
		return 1;
	else 
		return 0;
}

//////////////////////////////
// handle function
void get_token(Token *token) {
	char current_char;
	LexerStatus status = INITIAL_STATUS;
	token->k = BAD_TOKEN;
	int pos = st_line_pos;

	while (st_line[pos] != '\0') {
		current_char = st_line[pos];
		if (status == INITIAL_STATUS) {// ��һ�����ص����⣬�Ǿ��ǣ���ʵ���Ⲣ����һ��DFA������һ��NFA
			if (isspace(current_char)) {
				if (current_char == '\n') {
					token->k = END_OF_LINE_TOKEN;
					return;
				}
				pos++;
				st_line_pos++;
				continue;
			}

			if (current_char == 'i' && strlen(st_line + pos) >= 3) {
				if (st_line[pos + 1] == 'f' && isspace(st_line[pos + 2]))
					status = IF_I_STATUS;
				else
					status = ID_FS_STATUS;
			}
			else if ((current_char == '_') | isletter(current_char)) {
				status = ID_FS_STATUS;
			}
			else {
				st_line_pos = pos;
				break; // bad token
			}
		}
		else if (status == IF_I_STATUS) {
			if (current_char == 'f') status = IF_F_STATUS;
			else
			{
				st_line_pos = pos;
				break; // bad token
			}
		}
		else if (status == ID_FS_STATUS) {
			if (isdigit(current_char) | isletter(current_char) | (current_char == '_'))
				status = ID_ACC_STATUS;
			else {
				st_line_pos = pos;
				break; // bad token
			}
				
		}
		else if (status == ID_ACC_STATUS) {
			if (isspace(current_char)) {
				token->k = ID_TOKEN;
				sscanf((st_line + st_line_pos), "%s", token->str);
				token->pos = st_line_pos + 1;
				st_line_pos = pos;
				return;
			}	
		}
		else if (status == IF_F_STATUS) {
			if (isspace(current_char)) {
				token->k = IF_TOKEN;
				token->pos = st_line_pos + 1;
				st_line_pos = pos;
				return;
			}
			else {
				st_line_pos = pos;
				break; // bad token
			}
		}
		pos++;
	}
}

//////////////////////////////////
// parse_line
void set_line(char *line) {
	st_line = line;
	st_line_pos = 0;
}

void parse_line(char *buf) {
	Token token;

	set_line(buf);

	for (;;) {
		get_token(&token);
		if (token.k == BAD_TOKEN) {
			printf("bad token!\n");
		}
		else if (token.k == END_OF_LINE_TOKEN){
			break;
		}
		else if (token.k == IF_TOKEN) {
			printf("IF    (%d, %d)\n", line_num, token.pos);
		}	
		else if (token.k == ID_TOKEN) {
			printf("ID(%s)  (%d, %d)\n", token.str, line_num, token.pos);
		}
			
	}
}

int main() {
	char buf[1024];
	while (fgets(buf, 1024, stdin) != NULL) {
		line_num++;
		parse_line(buf);
	}
	return 0;
}