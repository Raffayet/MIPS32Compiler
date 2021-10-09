// Autor: Nikola Sovilj Datum: 06.06.2021.


#include "InstructionCreation.h"
#include "IR.h"
#include "LexicalAnalysis.h"
#include <string>

using namespace std;

static Instructions instructions;

static InstructionList instructionList;

static Variables variables;

typedef TokenList::iterator it;

typedef Instructions::iterator it2;

map<Instruction*, Variable*> instructionsAfterLabels;  //mapa ciji su kljucevi instrukcije, a vrednosti labele. 
												  //U mapi se nalaze samo instrukcije koje se nalaze odmah posle deklaracije labele
												  //neophodno zbog popunjavanja lista prethodnika i sledbenika u slucaju
												  //instrukcija koje predstavljaju skok

typedef map<Instruction*, Variable*>::iterator it3;

Instructions& getInstructions()
{
	return instructions;
}


Variables& getVariables()
{
	return variables;
}


static Instruction* makeInstruction(unsigned int pPos, InstructionType tType, Variable* dDst, Variable* sSrc1, Variable* sSrc2)
{
	Instruction* p = new Instruction();
	p->m_position = pPos;
	p->m_type = tType;

	if (dDst != nullptr)
		p->m_def.push_back(dDst);

	if (sSrc1 != nullptr)
		p->m_use.push_back(sSrc1);

	if (sSrc2 != nullptr)
		p->m_use.push_back(sSrc2);

	return p;
}

void checkInstructionsAfterLabel(TokenList::iterator it, Instruction* instr) //provera koje instrukcije se nalaze odmah posle deklaracije labele
{
	auto possibleLabel = prev(it, 2);	//koristi se za proveru da li neka instrukcija sledi odmah posle labele
													// (zbog popunjavanja succ i pred listi)
	if (possibleLabel->getType() == T_ID)
	{
		for (Variable* v : variables)
		{
			if (v->getName() == possibleLabel->getValue())
			{
				instructionsAfterLabels.insert(pair<Instruction*, Variable*>(instr, v));
			}
		}
	}
}

void createVariables(LexicalAnalysis& lex)	//kreira listu promenljivih
{
	int variablePosition = 0;

	for (TokenList::iterator it = lex.getTokenList().begin(); it != lex.getTokenList().end(); it++)
	{
		if (it->getType() == T_ID || it->getType() == T_R_ID || it->getType() == T_M_ID)
		{
			Variable* v = new Variable(it->getValue(), variablePosition);
			variables.push_back(v);
			variablePosition++;
		}
	}
}

void createUseDef(LexicalAnalysis& lex)			
{
	unsigned int variablePosition = 0;
	unsigned int instructionPosition = 0;

	for (TokenList::iterator it = lex.getTokenList().begin(); it != lex.getTokenList().end(); it++)
	{
		if (it->getType() == T_ADD || it->getType() == T_ADDI || it->getType() == T_SUB) //grupisao sam po broju promenljivih po instrukciji
		{
			auto t1 = next(it, 1);				//auto promenljive sluze za dobavljanje svih promenljivih koje ulaze u sastav instrukcije
			auto comma1 = next(it, 2);			//svaki poziv next() funkcije iterira do sledeceg tokena
			auto t2 = next(it, 3);
			auto comma2 = next(it, 4);
			auto t3 = next(it, 5);
			Variable* dst = new Variable(t1->getValue(), variablePosition);
			variablePosition++;
			Variable* src1 = new Variable(t2->getValue(), variablePosition);
			variablePosition++;
			Variable* src2 = new Variable(t3->getValue(), variablePosition);
			variablePosition++;
			Instruction* instr;
			if (it->getType() == T_ADD)
			{
				instr = makeInstruction(instructionPosition, I_ADD, dst, src1, src2);
				checkInstructionsAfterLabel(it, instr);
			}

			else if (it->getType() == T_ADDI)
			{
				instr = makeInstruction(instructionPosition, I_ADDI, dst, src1, src2);
				checkInstructionsAfterLabel(it, instr);
			}

			else
			{
				instr = makeInstruction(instructionPosition, I_SUB, dst, src1, src2);
				checkInstructionsAfterLabel(it, instr);
			}
			instructions.push_back(instr);
			instructionPosition++;
		}

		else if (it->getType() == T_B || it->getType() == T_JR)
		{
			auto t1 = next(it, 1);
			Variable* dst = new Variable(t1->getValue(), variablePosition);
			variablePosition++;
			Instruction* instr;
			if (it->getType() == T_B)
			{
				instr = makeInstruction(instructionPosition, I_B, dst, nullptr, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}

			else
			{
				instr = makeInstruction(instructionPosition, I_JR, dst, nullptr, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}
			instructions.push_back(instr);
			instructionPosition++;
		}

		else if (it->getType() == T_BLTZ)
		{
			auto t1 = next(it, 1);
			auto comma = next(it, 2);
			auto t2 = next(it, 3);
			Variable* dst = new Variable(t1->getValue(), variablePosition);
			variablePosition++;
			Variable* src1 = new Variable(t2->getValue(), variablePosition);
			variablePosition++;
			Instruction* instr = makeInstruction(instructionPosition, I_BLTZ, dst, src1, nullptr);
			checkInstructionsAfterLabel(it, instr);
			instructions.push_back(instr);
			instructionPosition++;
		}

		else if (it->getType() == T_LA || it->getType() == T_LI || it->getType() == T_MULT || it->getType() == T_DIV || it->getType() == T_LUI)
		{
			auto t1 = next(it, 1);
			auto comma = next(it, 2);
			auto t2 = next(it, 3);
			Variable* dst = new Variable(t1->getValue(), variablePosition);
			variablePosition++;
			Variable* src1 = new Variable(t2->getValue(), variablePosition);
			variablePosition++;
			Instruction* instr;

			if (it->getType() == T_LA)
			{
				instr = makeInstruction(instructionPosition, I_LA, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}

			else if (it->getType() == T_MULT)
			{
				instr = makeInstruction(instructionPosition, I_MULT, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}

			else if (it->getType() == T_LI)
			{
				instr = makeInstruction(instructionPosition, I_LI, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}

			else if (it->getType() == T_LUI)
			{
				instr = makeInstruction(instructionPosition, I_LUI, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}

			else
			{
				instr = makeInstruction(instructionPosition, I_DIV, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}
			instructions.push_back(instr);
			instructionPosition++;
		}

		else if (it->getType() == T_LW || it->getType() == T_SW)
		{
			auto t1 = next(it, 1);
			auto comma1 = next(it, 2);
			auto zero = next(it, 3);
			auto leftPar = next(it, 4);
			auto t2 = next(it, 5);
			Variable* dst = new Variable(t1->getValue(), variablePosition);
			variablePosition++;
			Variable* src1 = new Variable(t2->getValue(), variablePosition);
			variablePosition++;
			Instruction* instr;
			if (it->getType() == T_LW)
			{
				instr = makeInstruction(instructionPosition, I_LW, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}
			else
			{
				instr = makeInstruction(instructionPosition, I_SW, dst, src1, nullptr);
				checkInstructionsAfterLabel(it, instr);
			}
			instructions.push_back(instr);
			instructionPosition++;
		}

		else if (it->getType() == T_NOP)
		{
			Instruction* instr = makeInstruction(instructionPosition, I_NOP, nullptr, nullptr, nullptr);
			checkInstructionsAfterLabel(it, instr);
			instructions.push_back(instr);
			instructionPosition++;
		}
	}
}

string instructionTypeToString(InstructionType instrtp)		//sluzi za ispis tipa instrukcije na konzolu
{
	if (instrtp == I_LA)
	{
		return "la";
	}

	else if (instrtp == I_LW)
	{
		return "lw";
	}

	else if (instrtp == I_LI)
	{
		return "li";
	}

	else if (instrtp == I_ADD)
	{
		return "add";
	}

	else if (instrtp == I_ADDI)
	{
		return "addi";
	}

	else if (instrtp == I_BLTZ)
	{
		return "bltz";
	}

	else if (instrtp == I_B)
	{
		return "b";
	}

	else if (instrtp == I_SUB)
	{
		return "sub";
	}

	else if (instrtp == I_SW)
	{
		return "sw";
	}

	else if (instrtp == I_MULT)
	{
		return "mult";
	}

	else if (instrtp == I_DIV)
	{
		return "div";
	}

	else if (instrtp == I_NOP)
	{
		return "nop";
	}

	else if (instrtp == I_JR)
	{
		return "jr";
	}

	else if (instrtp == I_LUI)
	{
		return "lui";
	}

	else
	{
		"Ova instrukcija ne postoji u gramatici jezika!";
	}
}

void printVariableName(Variable* variable)
{
	printf("%s", variable->getName().c_str());
}

void printVariablesNames(Variables variables)
{
	Variables::iterator iter;

	for (iter = variables.begin(); iter != variables.end(); iter++)
	{
		printVariableName(*iter);
		printf(" ");
	}
	printf("\n");
}

static void printInstructionsPos(Instructions instrs)
{
	Instructions::iterator iter;

	for (iter = instrs.begin(); iter != instrs.end(); iter++)
	{
		(*iter)->m_pred.unique();
		(*iter)->m_succ.unique();
		cout << instructionTypeToString((*iter)->m_type);	//ispis tipa instrukcije u vidu stringa
		cout << " (" <<(*iter)->m_position << ") ";	//svaka instrukcija u konzoli je ispisana: tip instrukcije (pozicija)				
	}
}

static void printInstruction(Instruction* instr)
{

	instr->m_pred.unique();
	instr->m_succ.unique();
	printf("%d\n\n", instr->m_position);

	cout << "Type: \n\n" << instructionTypeToString(instr->m_type) << " ("<< instr->m_position << ")"<< endl;

	/*if (instr->dst != NULL)
	{
		printf("Destination:\t");
		printVariable(instr->dst);
		printf("\n");
	}

	if (instr->src1 != NULL)
	{
		printf("Source 1:\t");
		printVariable(instr->src1);
		printf("\n");
	}

	if (instr->src2 != NULL)
	{
		printf("Source 2:\t");
		printVariable(instr->src2);
		printf("\n");
	}*/

	printf("\nPRED:\t");
	printInstructionsPos(instr->m_pred);
	printf("\n");

	printf("\nSUCC:\t");
	printInstructionsPos(instr->m_succ);
	printf("\n");

	printf("\nUSE:\t");
	printVariablesNames(instr->m_use);
	printf("\n");

	printf("DEF:\t");
	printVariablesNames(instr->m_def);
	printf("\n");

	printf("IN:\t");
	printVariablesNames(instr->m_in);
	printf("\n");

	printf("OUT:\t");
	printVariablesNames(instr->m_out);
	printf("\n");
}

void printInstructions(Instructions instrs)
{
	Instructions::iterator iter;

	for (iter = instrs.begin(); iter != instrs.end(); iter++)
	{
		Instruction* instr = *iter;

		printf("-----------------------------------------------\n");
		printInstruction(instr);
		printf("-----------------------------------------------\n");
	}
}

void printVariable(Variable* variable)
{
	printf("(Name: %s, Pos: %d, Assignment: %d) ", variable->getName().c_str(), variable->getPosition(), variable->getAssignment());
}

void printVariables(Variables variables)
{
	Variables::iterator iter;

	for (iter = variables.begin(); iter != variables.end(); iter++)
	{
		printVariable(*iter);
		printf("\n");
	}
	printf("\n");
}

void createInstructionList()
{
	for (Instruction* instr : instructions)
	{
		instructionList.push_back(*instr);
	}
}

void createSuccPred(LexicalAnalysis& lex)
{
	for (Instructions::iterator it2 = instructions.begin(); it2 != instructions.end(); it2++)
	{	
		Instruction* instr = *it2;

		if (instr->m_type == I_ADD || instr->m_type == I_ADDI || instr->m_type == I_LA || instr->m_type == I_LI
			|| instr->m_type == I_LW || instr->m_type == I_NOP || instr->m_type == I_SUB || instr->m_type == I_SW
			|| instr->m_type == I_MULT || instr->m_type == I_DIV || instr->m_type == I_LUI)
		{
			if (it2 == instructions.begin()) //proverava da li je prva instrukcija u pitanju (onda nema prethodnika)
			{
				it2++;
				Instruction* instrSucc = *it2;
				instr->m_succ.push_back(instrSucc);
				it2--;
			}

			else if (instr->m_position == instructions.size() - 1) //proverava da li je posledjnja instrukcija (nema sledbenika)
			{
				it2--;
				Instruction* instrPred = *it2;
				instr->m_pred.push_back(instrPred);
				it2++;
			}

			else
			{
				it2--;
				Instruction* instrPred = *it2;
				instr->m_pred.push_back(instrPred);
				it2++;
				
				it2++;
				Instruction* instrSucc = *it2;
				instr->m_succ.push_back(instrSucc);
				it2--;
			}
		}

		else if (instr->m_type == I_B)
		{
			bool emptySuccList = true;	

			if (it2 != instructions.begin())
			{
				it2--;
				Instruction* instrPred = *it2;
				instr->m_pred.push_back(instrPred);
				it2++;
			}

			for (map<Instruction*, Variable*>::iterator it3 = instructionsAfterLabels.begin(); 
				it3 != instructionsAfterLabels.end(); it3++)
			{
				Instruction* instrSucc = it3->first;		//instrukcija koja se nalazi odmah posle date labele

				for (Variable* va : instr->m_def)
				{
					if (va->getName() == it3->second->getName())
					{
						if (emptySuccList)
						{
							instr->m_succ.push_back(instrSucc);
							emptySuccList = false;
						}

						instrSucc->m_pred.push_back(instr);
					}
				}
			}
		}

		else if (instr->m_type == I_BLTZ)
		{
			if (it2 != instructions.begin())
			{
				it2--;
				Instruction* instrPred = *it2;
				instr->m_pred.push_back(instrPred);
				it2++;
			}

			for (map<Instruction*, Variable*>::iterator it3 = instructionsAfterLabels.begin();
				it3 != instructionsAfterLabels.end(); it3++)
			{
				Instruction* instrSucc = it3->first;

				for (Variable* va : instr->m_use)
				{
					if (va->getName() == it3->second->getName())
					{
						instr->m_succ.push_back(instrSucc);
						instrSucc->m_pred.push_back(instr);
					}
				}
			}
		}
	}
}