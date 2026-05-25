/*
GH0ST: 2047 - Logic Questions Implementation
Parses questions from structured text file and provides random selection
*/

#include "logic_questions.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Helper: trim trailing newline/carriage return
//------------------------------------------------------------------------------------
static void TrimNewline(char* str)
{
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[len - 1] = '\0';
        len--;
    }
}

//------------------------------------------------------------------------------------
// Load questions from structured text file
// Format per question:
//   Line 1: Question text
//   Line 2-5: Options A-D
//   Line 6: Correct answer index (0-3)
//   Line 7: Separator "---"
//------------------------------------------------------------------------------------
void QuestionBank_Load(QuestionBank* bank, const char* filename)
{
    if (!bank || !filename) return;

    memset(bank, 0, sizeof(QuestionBank));
    bank->lastUsedIndex = -1;

    FILE* file = fopen(filename, "r");
    if (!file)
    {
        TraceLog(LOG_WARNING, "QUESTIONS: Failed to load '%s' - hints will always be available", filename);
        bank->loaded = false;
        return;
    }

    char line[256];
    int questionIndex = 0;

    while (questionIndex < MAX_QUESTIONS)
    {
        LogicQuestion* q = &bank->questions[questionIndex];

        // Line 1: Question text
        if (!fgets(line, sizeof(line), file)) break;
        TrimNewline(line);
        if (strlen(line) == 0) continue; // skip blank lines between questions
        strncpy(q->question, line, sizeof(q->question) - 1);

        // Lines 2-5: Options
        bool valid = true;
        for (int i = 0; i < 4; i++)
        {
            if (!fgets(line, sizeof(line), file)) { valid = false; break; }
            TrimNewline(line);
            strncpy(q->options[i], line, sizeof(q->options[i]) - 1);
        }
        if (!valid) break;

        // Line 6: Correct index
        if (!fgets(line, sizeof(line), file)) break;
        TrimNewline(line);
        q->correctIndex = atoi(line);
        if (q->correctIndex < 0 || q->correctIndex > 3) q->correctIndex = 0;

        // Line 7: Separator "---"
        if (!fgets(line, sizeof(line), file)) { questionIndex++; break; }
        // Just consume the separator line

        questionIndex++;
    }

    fclose(file);

    bank->count = questionIndex;
    bank->loaded = (bank->count > 0);

    TraceLog(LOG_INFO, "QUESTIONS: Loaded %d logic questions from '%s'", bank->count, filename);
}

//------------------------------------------------------------------------------------
// Get a random question (avoids repeating the last one)
//------------------------------------------------------------------------------------
LogicQuestion* QuestionBank_GetRandom(QuestionBank* bank)
{
    if (!bank || !bank->loaded || bank->count == 0) return NULL;

    int index;
    if (bank->count == 1)
    {
        index = 0;
    }
    else
    {
        do {
            index = GetRandomValue(0, bank->count - 1);
        } while (index == bank->lastUsedIndex);
    }

    bank->lastUsedIndex = index;
    return &bank->questions[index];
}

//------------------------------------------------------------------------------------
// Cleanup
//------------------------------------------------------------------------------------
void QuestionBank_Unload(QuestionBank* bank)
{
    if (!bank) return;
    memset(bank, 0, sizeof(QuestionBank));
    bank->loaded = false;
}
