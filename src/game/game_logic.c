/*
GH0ST: 2047 - Game Logic Implementation
*/

#include "game_logic.h"
#include "config.h"
#include "theme.h"
#include "persistence/statistics.h"
#include "persistence/session_logger.h"
#include "effects/glitch_effect.h"
#include "raylib.h"
#include <string.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Game Logic Functions Implementation
//------------------------------------------------------------------------------------
void Game_StartNew(GameState* state)
{
    if (!state) return;
    
    state->targetNumber = GetRandomValue(1, 100);
    state->attempts = 0;
    state->maxAttempts = MAX_ATTEMPTS;
    state->minRange = 1;
    state->maxRange = 100;
    state->inputBuffer[0] = '\0';
    state->gameOver = false;
    state->won = false;
    state->alertLevel = ALERT_STABLE;
    state->errorMsg[0] = '\0';
    state->errorTimer = 0.0f;
    
    memset(state->history, 0, sizeof(state->history));
    
    InitGlitchEffect(false);
}

void Game_ProcessGuess(GameState* state, Statistics* stats, int guess, GameScreen* currentScreen)
{
    if (!state || !currentScreen) return;
    
    // Validation
    if (guess < 1 || guess > 100)
    {
        strcpy(state->errorMsg, "INPUT INVALIDO: RANGE 1-100");
        state->errorTimer = 2.0f;
        return;
    }
    
    if (guess < state->minRange || guess > state->maxRange)
    {
        strcpy(state->errorMsg, "NUMERO FORA DO RANGE ATUAL!");
        state->errorTimer = 2.0f;
        return;
    }
    
    // Process guess
    state->attempts++;
    state->alertLevel = Game_GetAlertLevel(state->attempts);
    
    if (guess == state->targetNumber)
    {
        // WIN!
        strcpy(state->history[state->attempts - 1].feedback, "ACERTO!");
        state->history[state->attempts - 1].value = guess;
        state->won = true;
        state->gameOver = true;
        Stats_AddSession(stats, state->attempts, true);
        
        // Log detailed session to CSV
        DetailedSession detailedSession = Game_CreateDetailedSession(state);
        SessionLogger_Append(&detailedSession);
        
        InitGlitchEffect(true);
        *currentScreen = SCREEN_RESULT;
    }
    else if (state->attempts >= state->maxAttempts)
    {
        // GAME OVER
        strcpy(state->history[state->attempts - 1].feedback, guess < state->targetNumber ? "MAIOR" : "MENOR");
        state->history[state->attempts - 1].value = guess;
        state->gameOver = true;
        state->won = false;
        Stats_AddSession(stats, state->attempts, false);
        
        // Log detailed session to CSV
        DetailedSession detailedSession = Game_CreateDetailedSession(state);
        SessionLogger_Append(&detailedSession);
        
        InitGlitchEffect(false);
        *currentScreen = SCREEN_RESULT;
    }
    else
    {
        // Update range and continue
        if (guess < state->targetNumber)
        {
            strcpy(state->history[state->attempts - 1].feedback, "MAIOR");
            state->minRange = guess + 1;
        }
        else
        {
            strcpy(state->history[state->attempts - 1].feedback, "MENOR");
            state->maxRange = guess - 1;
        }
        state->history[state->attempts - 1].value = guess;
    }
}

AlertLevel Game_GetAlertLevel(int attempts)
{
    if (attempts <= 2) return ALERT_STABLE;
    if (attempts <= 4) return ALERT_WARNING;
    if (attempts <= 6) return ALERT_DANGER;
    return ALERT_LOCKED;
}

Color Game_GetAlertColor(AlertLevel level)
{
    switch (level)
    {
        case ALERT_STABLE: return COLOR_CYBER_GREEN;
        case ALERT_WARNING: return COLOR_ALERT_YELLOW;
        case ALERT_DANGER: return COLOR_DANGER_RED;
        case ALERT_LOCKED: return COLOR_LOCKED_RED;
        default: return COLOR_CYBER_GREEN;
    }
}

const char* Game_GetAlertText(AlertLevel level)
{
    switch (level)
    {
        case ALERT_STABLE: return "SISTEMA ESTAVEL";
        case ALERT_WARNING: return "ALERTA DETECTADO";
        case ALERT_DANGER: return "RASTREAMENTO ATIVO";
        case ALERT_LOCKED: return "BLOQUEADO";
        default: return "UNKNOWN";
    }
}

DetailedSession Game_CreateDetailedSession(const GameState* state)
{
    DetailedSession session = {0};
    
    if (!state) return session;
    
    // Timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(session.timestamp, sizeof(session.timestamp), "%Y-%m-%d %H:%M:%S", t);
    
    // Game data
    session.target = state->targetNumber;
    session.totalAttempts = state->attempts;
    session.won = state->won;
    
    // Calculate bias
    session.lowBiasCount = 0;
    session.highBiasCount = 0;
    
    for (int i = 0; i < state->attempts; i++) {
        session.guesses[i] = state->history[i].value;
        
        if (state->history[i].value < state->targetNumber) {
            session.lowBiasCount++;
        } else if (state->history[i].value > state->targetNumber) {
            session.highBiasCount++;
        }
    }
    
    return session;
}
