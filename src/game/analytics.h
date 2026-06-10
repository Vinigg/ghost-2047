/*
GH0ST: 2047 - Analytics Module
Recursive statistical analysis and strategy heuristics
*/

#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "types.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// Aggregate Statistics (Recursive)
//------------------------------------------------------------------------------------
int Analytics_RecursiveSum(const DetailedSession* sessions, int count, int index);
int Analytics_RecursiveMin(const DetailedSession* sessions, int count, int index);
int Analytics_RecursiveMax(const DetailedSession* sessions, int count, int index);
float Analytics_RecursiveSumSquares(const DetailedSession* sessions, int count, float mean, int index);

//------------------------------------------------------------------------------------
// Strategy Heuristics
//------------------------------------------------------------------------------------
int Analytics_LongestMonotonicRun(const DetailedSession* session);
bool Analytics_HasRepetitivePattern(const DetailedSession* session);
float Analytics_BinarySimilarity(const DetailedSession* session);

#endif // ANALYTICS_H
