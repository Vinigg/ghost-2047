/*
GH0ST: 2047 - History Implementation
*/

#include "history.h"
#include "statistics.h"
#include "config.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// History Functions Implementation
//------------------------------------------------------------------------------------
bool History_Save(const Statistics* stats)
{
    if (!stats) return false;
    
    FILE* f = fopen(HISTORY_FILE, "wb");
    if (!f) return false;

    unsigned int magic = HISTORY_MAGIC;
    int version = HISTORY_VERSION;
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version, sizeof(version), 1, f);

    fwrite(&stats->sessionCount, sizeof(stats->sessionCount), 1, f);

    for (int i = 0; i < stats->sessionCount; i++)
    {
        fwrite(&stats->sessions[i], sizeof(GameSession), 1, f);
    }

    fclose(f);
    return true;
}

bool History_Load(Statistics* stats)
{
    if (!stats) return false;
    
    FILE* f = fopen(HISTORY_FILE, "rb");
    if (!f) return false;

    unsigned int magic = 0;
    int version = 0;
    if (fread(&magic, sizeof(magic), 1, f) != 1 || magic != HISTORY_MAGIC)
    {
        fclose(f);
        return false;
    }
    if (fread(&version, sizeof(version), 1, f) != 1 || version != HISTORY_VERSION)
    {
        fclose(f);
        return false;
    }

    int count = 0;
    if (fread(&count, sizeof(count), 1, f) != 1 || count < 0 || count > MAX_SESSIONS)
    {
        fclose(f);
        return false;
    }
    stats->sessionCount = count;

    for (int i = 0; i < stats->sessionCount; i++)
    {
        if (fread(&stats->sessions[i], sizeof(GameSession), 1, f) != 1)
        {
            stats->sessionCount = i;
            break;
        }
    }

    fclose(f);
    Stats_Calculate(stats);
    return true;
}
