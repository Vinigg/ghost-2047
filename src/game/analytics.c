/*
GH0ST: 2047 - Analytics Module
Recursive statistical analysis and strategy heuristics
*/

#include "game/analytics.h"
#include "config.h"
#include <stdlib.h>
#include <math.h>

//------------------------------------------------------------------------------------
// Aggregate Statistics (Recursive)
//------------------------------------------------------------------------------------
int Analytics_RecursiveSum(const DetailedSession* sessions, int count, int index)
{
    if (!sessions || index >= count) return 0;
    return sessions[index].totalAttempts + Analytics_RecursiveSum(sessions, count, index + 1);
}

int Analytics_RecursiveMin(const DetailedSession* sessions, int count, int index)
{
    if (!sessions || count <= 0) return 0;
    if (index == count - 1) return sessions[index].totalAttempts;

    int tailMin = Analytics_RecursiveMin(sessions, count, index + 1);
    return sessions[index].totalAttempts < tailMin ? sessions[index].totalAttempts : tailMin;
}

int Analytics_RecursiveMax(const DetailedSession* sessions, int count, int index)
{
    if (!sessions || count <= 0) return 0;
    if (index == count - 1) return sessions[index].totalAttempts;

    int tailMax = Analytics_RecursiveMax(sessions, count, index + 1);
    return sessions[index].totalAttempts > tailMax ? sessions[index].totalAttempts : tailMax;
}

float Analytics_RecursiveSumSquares(const DetailedSession* sessions, int count, float mean, int index)
{
    if (!sessions || index >= count) return 0.0f;

    float deviation = sessions[index].totalAttempts - mean;
    return deviation * deviation + Analytics_RecursiveSumSquares(sessions, count, mean, index + 1);
}

//------------------------------------------------------------------------------------
// Strategy Heuristics
//------------------------------------------------------------------------------------
int Analytics_LongestMonotonicRun(const DetailedSession* session)
{
    if (!session || session->totalAttempts <= 1) return session ? session->totalAttempts : 0;

    int best = 1;
    int current = 1;
    int direction = 0;

    for (int i = 1; i < session->totalAttempts; i++)
    {
        int diff = session->guesses[i] - session->guesses[i - 1];
        int newDirection = (diff > 0) ? 1 : (diff < 0 ? -1 : 0);

        if (newDirection != 0 && newDirection == direction)
        {
            current++;
        }
        else if (newDirection != 0)
        {
            current = 2;
            direction = newDirection;
        }
        else
        {
            current = 1;
            direction = 0;
        }

        if (current > best) best = current;
    }

    return best;
}

bool Analytics_HasRepetitivePattern(const DetailedSession* session)
{
    if (!session || session->totalAttempts < 4) return false;

    int repeatedSteps = 0;
    int uniqueSteps[MAX_HISTORY] = {0};
    int uniqueCount = 0;
    int previousStep = abs(session->guesses[1] - session->guesses[0]);

    uniqueSteps[uniqueCount++] = previousStep;

    for (int i = 2; i < session->totalAttempts; i++)
    {
        int step = abs(session->guesses[i] - session->guesses[i - 1]);
        if (step == previousStep) repeatedSteps++;

        bool knownStep = false;
        for (int j = 0; j < uniqueCount; j++)
        {
            if (uniqueSteps[j] == step)
            {
                knownStep = true;
                break;
            }
        }
        if (!knownStep && uniqueCount < MAX_HISTORY) uniqueSteps[uniqueCount++] = step;

        previousStep = step;
    }

    return repeatedSteps >= 2 || uniqueCount <= 2;
}

float Analytics_BinarySimilarity(const DetailedSession* session)
{
    if (!session || session->totalAttempts <= 0) return 0.0f;

    int low = 1;
    int high = 100;
    int matches = 0;

    for (int i = 0; i < session->totalAttempts; i++)
    {
        float midpoint = (low + high) / 2.0f;
        if (fabsf(session->guesses[i] - midpoint) <= 10.0f) matches++;

        if (session->guesses[i] < session->target)
        {
            low = session->guesses[i] + 1;
        }
        else if (session->guesses[i] > session->target)
        {
            high = session->guesses[i] - 1;
        }
    }

    return (matches * 100.0f) / session->totalAttempts;
}
