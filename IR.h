// Autor: Nikola Sovilj Datum: 06.06.2021.

#ifndef __IR__
#define __IR__

#include "Types.h"

using namespace std;


/**
 * This class represents one variable from program code.
 */

/**
 * Use this function to print all variables in list.
 * @param variables list of variables
 */


class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}

	string getName() 
	{
		return m_name;
	}

	int getPosition()
	{
		return m_position;
	}

	Regs getAssignment()
	{
		return m_assignment;
	}

private:
	VariableType m_type;
	string m_name;
	int m_position;
	Regs m_assignment;
};

typedef std::list<Variable*> Variables;
/**
 * This type represents list of variables from program code.
 */



/**
 * This class represents one instruction in program code.
 */
class Instruction
{

private:
	std::string value;
	InstructionType instructionType;
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}

	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

	InstructionType Instruction::getType()
	{
		return instructionType;
	}

	string Instruction::getValue()
	{
		return value;
	}
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

typedef std::list<Instruction> InstructionList;

void printVariable(Variable* variable);

void printVariables(Variables variables);

void printInstructions(Instructions instrs);

static void printInstructionsPos(Instructions instrs);

void createInstructionList();

bool variableExists(Variable* variable, Variables variables);

#endif
