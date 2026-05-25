/*
GH0ST: 2047 - Logic Questions Module
Loads and manages propositional logic questions from file
*/

#ifndef LOGIC_QUESTIONS_H
#define LOGIC_QUESTIONS_H

#include "config.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// Logic Question Structures
//------------------------------------------------------------------------------------
typedef struct {
    char question[256];
    char options[4][128];
    int correctIndex; // 0-3
} LogicQuestion;

typedef struct {
    LogicQuestion questions[MAX_QUESTIONS];
    int count;
    int lastUsedIndex; // avoid consecutive repeats
    bool loaded;
} QuestionBank;

//------------------------------------------------------------------------------------
// Logic Question Functions
//------------------------------------------------------------------------------------
void QuestionBank_Load(QuestionBank* bank, const char* filename);
LogicQuestion* QuestionBank_GetRandom(QuestionBank* bank);
void QuestionBank_Unload(QuestionBank* bank);

#endif // LOGIC_QUESTIONS_H
