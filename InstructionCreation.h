// Autor: Nikola Sovilj Datum: 06.06.2021.

#ifndef __TEST_H__
#define __TEST_H__

#include "IR.h"
#include "LexicalAnalysis.h"

/**
 * @brief Use this function to get instructions list
 *
 * @return pointer to instructions list
 */
Instructions& getInstructions();


/**
 * @brief Use this function to get variables list
 *
 * @return pointer to variables list
 */
Variables& getVariables();

/**
 * @brief Makes example code.
 *
 * Each node is created and represents a single instruction.
 * Destination, Source 1 and Source 2 are set if used.
 * Pred and Succ sets (lists) are defined
 * Use and def sets are not defined, as it is left for makeFlowControlGraph()
 */
void createUseDef(LexicalAnalysis& lex);

void createSuccPred(LexicalAnalysis& lex);

void createVariables(LexicalAnalysis& lex);


#endif
