// Autor: Nikola Sovilj Datum: 06.06.2021.

#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "Token.h"
#include "IR.h"
#include "InstructionCreation.h"
#include "LivenessAnalysis.h"
#include "Types.h"

using namespace std;


int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\multiply.mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			lex.printTokens();
			cout << "Lexical analysis finished successfully!" << endl;
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		SyntaxAnalysis syntax(lex);

		retVal = syntax.Do();

		if (retVal)
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		Instructions& instructions = getInstructions();
		Variables& variables = getVariables();


		// 1) build instruction list (some nodes from pseudo code)
		createVariables(lex);
		createUseDef(lex);
		cout << endl;
		createSuccPred(lex);
		cout << endl;
		cout << "Variables list:" << endl;
		printVariables(variables);
		cout << endl << endl << endl;
		cout << "Instruction list (flow control graph):" << endl;
		printInstructions(instructions);
		cout << endl << endl;

		// 2) do liveness analysis of variables using control flow graph
		livenessAnalysis(instructions);
		cout << endl;
		cout << "Instruction list after LIVENESS ANALYSIS:" << endl;
		printInstructions(instructions);
		cout << endl << endl;


		cout << "END" << endl;

		getchar();
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
