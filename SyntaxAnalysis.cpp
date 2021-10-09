// Autor: Nikola Sovilj Datum: 06.06.2021.

#include "SyntaxAnalysis.h"
#include "Types.h"

#include <iostream>

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex) : lexicalAnalysis(lex), errorFound(false)
{
	tokenIterator = lexicalAnalysis.getTokenList().begin();
}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	q();

	// check if anything has been analyzed:
	if (--tokenIterator == lexicalAnalysis.getTokenList().begin())
		return false;
	else
		return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token& token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (currentToken.getType() == t)
	{
		currentToken.printTokenValue();
		if (t != T_END_OF_FILE)
			currentToken = getNextToken();
	}
	else
	{
		printSyntaxError(currentToken);
		errorFound = true;
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


void SyntaxAnalysis::q()
{
	s();
	eat(T_SEMI_COL);
	l();
}


void SyntaxAnalysis::s()
{

	if (currentToken.getType() == T_MEM) 
	{
		eat(T_MEM);
		eat(T_M_ID);
		eat(T_NUM);
	}

	else if (currentToken.getType() == T_REG) 
	{
		eat(T_REG);
		eat(T_R_ID);
	}

	else if (currentToken.getType() == T_FUNC)
	{
		eat(T_FUNC);
		eat(T_ID);
	}

	else if (currentToken.getType() == T_ID)
	{
		eat(T_ID);
		eat(T_COL);
		e();
	}

	else
	{
		e();
	}	
}


void SyntaxAnalysis::l()
{
	if (currentToken.getType() == T_END_OF_FILE) 
	{

		eat(T_END_OF_FILE);
	}

	else 
	{
		q();
	}
}


void SyntaxAnalysis::e() // u e grupu produkcija sam dodao nove produkcije za 4 dodatne instrukcije (mult, div, lui, jr)
{
	if (currentToken.getType() == T_ADD)
	{
		eat(T_ADD);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}

	else if (currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}

	else if (currentToken.getType() == T_SUB)
	{
		eat(T_SUB);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}

	else if (currentToken.getType() == T_LA)
	{
		eat(T_LA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_M_ID);
	}

	else if (currentToken.getType() == T_LW)
	{
		eat(T_LW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
	}

	else if (currentToken.getType() == T_LI)
	{
		eat(T_LI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}

	else if (currentToken.getType() == T_SW)
	{
		eat(T_SW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
	}

	else if (currentToken.getType() == T_B)
	{
		eat(T_B);
		eat(T_ID);
	}

	else if (currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
	}

	else if (currentToken.getType() == T_NOP)
	{
		eat(T_NOP);
	}

	else if (currentToken.getType() == T_MULT)
	{
		eat(T_MULT);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}

	else if (currentToken.getType() == T_DIV)
	{
		eat(T_DIV);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}

	else if (currentToken.getType() == T_LUI)
	{
		eat(T_LUI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}

	else if (currentToken.getType() == T_JR)
	{
		eat(T_JR);
		eat(T_R_ID);
	}
}
