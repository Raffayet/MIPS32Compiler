// Autor: Nikola Sovilj Datum: 06.06.2021.

#include "LivenessAnalysis.h"
#include "Variable.h"
#include "IR.h"
#include "Types.h"


bool variableExists(Variable* variable, Variables variables)
{
	bool ret = false;

	Variables::iterator it;

	for (it = variables.begin(); it != variables.end(); it++)
	{
		if (*it == variable)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void livenessAnalysis(Instructions instructions)
{

	for (Instruction* instr : instructions) {

		instr->m_in.clear();
		instr->m_out.clear();
	}

	bool ponovi = true;

	while (ponovi) {

		ponovi = false;

		for (Instructions::reverse_iterator it = instructions.rbegin(); it != instructions.rend(); it++) { // iterira se od nazad kako bi se prvo dobavile succ liste za formiranje in i out

			Instruction* instr = *it;		

			Instruction* kopija = *it;		//kopije in i out

			for (Variable* var : instr->m_in) {

				kopija->m_in.push_back(var);
				kopija->m_out.push_back(var);
			}

			for (Instruction* succ : instr->m_succ) {

				for (Variable* var : succ->m_in) {

					instr->m_out.push_back(var);

					if (!variableExists(var, instr->m_def)) {

						instr->m_in.push_back(var);
					}
				}
			}

			for (Variable* var : instr->m_use) {

				instr->m_in.push_back(var);
			}

			for (Variable* var : instr->m_out) {

				if (!variableExists(var, instr->m_def)) {

					instr->m_in.push_back(var);
				}
			}

			instr->m_in.sort();
			instr->m_in.unique();

			instr->m_out.sort();
			instr->m_out.unique();

			if (kopija->m_in != instr->m_in || kopija->m_out != instr->m_out) {

				ponovi = true;
			}
		}
	}
}
