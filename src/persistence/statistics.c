/*
GH0ST: 2047 - Statistics Implementation
*/

#include "statistics.h"
#include "history.h"
#include "config.h"
#include <time.h>
#include <string.h>

//------------------------------------------------------------------------------------
// Statistics Functions Implementation
//------------------------------------------------------------------------------------
void Stats_Init(Statistics* stats)
{
    if (!stats) return;
    
    memset(stats, 0, sizeof(Statistics));
    stats->bestScore = MAX_ATTEMPTS + 1; // Start with worst possible score
}

void Stats_AddSession(Statistics* stats, int attempts, bool won)
{
    if (!stats || stats->sessionCount >= MAX_SESSIONS) return;
    
    stats->sessions[stats->sessionCount].attempts = attempts;
    stats->sessions[stats->sessionCount].won = won;
    
    // Real timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(stats->sessions[stats->sessionCount].date,
             sizeof(stats->sessions[stats->sessionCount].date),
             "%Y-%m-%d %H:%M:%S", t);
    
    stats->sessionCount++;
    History_Save(stats);
}

void Stats_Calculate(Statistics* stats)
{
    if (!stats || stats->sessionCount == 0) return;
    
    int wonCount = 0;
    int totalAttempts = 0;
    stats->bestScore = MAX_ATTEMPTS + 1;
    
    for (int i = 0; i < stats->sessionCount; i++)
    {
        if (stats->sessions[i].won)
        {
            wonCount++;
            totalAttempts += stats->sessions[i].attempts;
            if (stats->sessions[i].attempts < stats->bestScore)
            {
                stats->bestScore = stats->sessions[i].attempts;
            }
        }
    }
    
    stats->winRate = (wonCount * 100.0f) / stats->sessionCount;
    stats->avgAttempts = wonCount > 0 ? (float)totalAttempts / wonCount : 0.0f;
}
