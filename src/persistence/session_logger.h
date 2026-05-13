/*
GH0ST: 2047 - Session Logger Module
Detailed CSV logging for game sessions
*/

#ifndef SESSION_LOGGER_H
#define SESSION_LOGGER_H

#include "types.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// Session Logger Functions
//------------------------------------------------------------------------------------
bool SessionLogger_Init(void);
bool SessionLogger_Append(const DetailedSession* session);
bool SessionLogger_Load(DetailedSession* sessions, int* count, int maxSessions);

#endif // SESSION_LOGGER_H
