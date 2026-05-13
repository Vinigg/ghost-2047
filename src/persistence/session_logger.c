/*
GH0ST: 2047 - Session Logger Implementation
CSV-based detailed session logging
*/

#include "session_logger.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Session Logger Functions Implementation
//------------------------------------------------------------------------------------

bool SessionLogger_Init(void)
{
    FILE* f = fopen(SESSIONS_CSV_FILE, "r");
    if (f) {
        // File exists
        fclose(f);
        return true;
    }
    
    // Create new file with header
    f = fopen(SESSIONS_CSV_FILE, "w");
    if (!f) return false;
    
    fprintf(f, "timestamp;alvo;tentativas;baixos;altos;sequencia;resultado\n");
    fclose(f);
    return true;
}

bool SessionLogger_Append(const DetailedSession* session)
{
    if (!session) return false;
    
    FILE* f = fopen(SESSIONS_CSV_FILE, "a");
    if (!f) return false;
    
    // timestamp;alvo;tentativas;baixos;altos;sequencia;resultado
    fprintf(f, "%s;%d;%d;%d;%d;",
            session->timestamp,
            session->target,
            session->totalAttempts,
            session->lowBiasCount,
            session->highBiasCount);
    
    // Write guess sequence
    for (int i = 0; i < session->totalAttempts; i++) {
        fprintf(f, "%d", session->guesses[i]);
        if (i < session->totalAttempts - 1) {
            fprintf(f, ",");
        }
    }
    
    // Write result
    fprintf(f, ";%s\n", session->won ? "VITORIA" : "DERROTA");
    
    fclose(f);
    return true;
}

bool SessionLogger_Load(DetailedSession* sessions, int* count, int maxSessions)
{
    if (!sessions || !count) return false;
    
    FILE* f = fopen(SESSIONS_CSV_FILE, "r");
    if (!f) {
        *count = 0;
        return false;
    }
    
    char line[1024];
    *count = 0;
    
    // Skip header
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return false;
    }
    
    // Read sessions
    while (fgets(line, sizeof(line), f) && *count < maxSessions) {
        DetailedSession* session = &sessions[*count];
        
        // Parse CSV line
        char guessSequence[256];
        char result[16];
        
        int parsed = sscanf(line, "%31[^;];%d;%d;%d;%d;%255[^;];%15s",
                           session->timestamp,
                           &session->target,
                           &session->totalAttempts,
                           &session->lowBiasCount,
                           &session->highBiasCount,
                           guessSequence,
                           result);
        
        if (parsed == 7) {
            // Parse guess sequence (comma-separated)
            char* token = strtok(guessSequence, ",");
            int guessIndex = 0;
            while (token && guessIndex < MAX_HISTORY) {
                session->guesses[guessIndex++] = atoi(token);
                token = strtok(NULL, ",");
            }
            
            session->won = (strcmp(result, "VITORIA") == 0);
            (*count)++;
        }
    }
    
    fclose(f);
    return true;
}
