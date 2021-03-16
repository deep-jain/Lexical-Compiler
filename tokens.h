#ifndef TOKENS_H_
#define TOKENS_H_

#include <string>
#include <iostream>
using std::istream;
using std::ostream;
using std::string;

enum TokenType
{

	PRINT,
	SET,
	IF,
	LOOP,
	BEGIN,
	END,

	ID,

	ICONST,
	SCONST,

	PLUS,
	MINUS,
	STAR,
	SLASH,
	LPAREN,
	RPAREN,
	SC,
	NL,

	ERR,

	DONE
};

class Token
{
	TokenType tt;
	string lexeme;
	int lnum;

public:
	Token()
	{
		tt = ERR;
		lnum = -1;
	}
	Token(TokenType tt, string lexeme, int line)
	{
		this->tt = tt;
		this->lexeme = lexeme;
		this->lnum = line;
	}

	bool operator==(const TokenType tt) const { return this->tt == tt; }
	bool operator!=(const TokenType tt) const { return this->tt != tt; }

	TokenType GetTokenType() const { return tt; }
	string GetLexeme() const { return lexeme; }
	int GetLinenum() const { return lnum; }
};

extern ostream &operator<<(ostream &out, const Token &tok);

extern Token getNextToken(istream &in, int &linenum);

#endif