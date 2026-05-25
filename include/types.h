/*
GH0ST: 2047 - Type Definitions
All game data structures and enums
*/

#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include "config.h"
#include <stdbool.h>

//------------------------------------------------------------------------------------
// Enumerations
//------------------------------------------------------------------------------------
typedef enum {
    SCREEN_MAIN_MENU = 0,
    SCREEN_GAME,
    SCREEN_RESULT,
    SCREEN_STATS
} GameScreen;

typedef enum {
    ALERT_STABLE = 0,    // 1-2 attempts (Green)
    ALERT_WARNING,       // 3-4 attempts (Yellow)
    ALERT_DANGER,        // 5-6 attempts (Red)
    ALERT_LOCKED         // 7 attempts (Dark Red)
} AlertLevel;

typedef enum {
    QUESTION_IDLE = 0,       // Player can type guess
    QUESTION_SHOWING,        // Question overlay is displayed
    QUESTION_ANSWERED_CORRECT,  // Brief feedback: hint unlocked
    QUESTION_ANSWERED_WRONG     // Brief feedback: hint denied
} QuestionState;

//------------------------------------------------------------------------------------
// Game Structures
//------------------------------------------------------------------------------------
typedef struct {
    int value;
    char feedback[20]; // "MAIOR", "MENOR", "ACERTO!"
} GuessHistory;

typedef struct {
    int targetNumber;
    int attempts;
    int maxAttempts;
    int minRange;
    int maxRange;
    char inputBuffer[MAX_INPUT_LENGTH + 1];
    GuessHistory history[MAX_HISTORY];
    bool gameOver;
    bool won;
    AlertLevel alertLevel;
    char errorMsg[64];
    float errorTimer;
    // Logic question state
    QuestionState questionState;
    int selectedOption;     // -1 = none selected
    bool hintUnlocked;      // whether current round's hint is active
    float questionFeedbackTimer;
} GameState;

//------------------------------------------------------------------------------------
// Statistics Structures
//------------------------------------------------------------------------------------
typedef struct {
    char date[32];
    int attempts;
    bool won;
} GameSession;

// Detailed session for CSV logging
typedef struct {
    char timestamp[32];
    int target;
    int totalAttempts;
    int lowBiasCount;   // Palpites abaixo do alvo
    int highBiasCount;  // Palpites acima do alvo
    int guesses[MAX_HISTORY];  // Sequência de palpites
    bool won;
} DetailedSession;

typedef struct {
    GameSession sessions[MAX_SESSIONS];
    int sessionCount;
    int bestScore;
    float avgAttempts;
    float winRate;
} Statistics;

//------------------------------------------------------------------------------------
// Visual Effects Structures
//------------------------------------------------------------------------------------
typedef struct {
    Vector2 position;
    float speed;
    char text[16];
    float alpha;
} MatrixParticle;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float maxLifetime;
    Color color;
} GlitchParticle;

#endif // TYPES_H
