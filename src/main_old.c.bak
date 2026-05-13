/*
GH0ST: 2047 - Neural Guessing System
A cyberpunk-themed number guessing game built with Raylib
Based on React prototypes from Figma Make
*/

#include "raylib.h"
#include "resource_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Constants and Theme Colors
//------------------------------------------------------------------------------------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MAX_ATTEMPTS 7
#define HISTORY_FILE "ghost2047_history.dat"
#define HISTORY_MAGIC 0x47483437 // "GH47"
#define MAX_HISTORY 7
#define MAX_SESSIONS 100
#define MAX_INPUT_LENGTH 3
#define MAX_PARTICLES 50
#define MAX_LOGS 8

// Cyberpunk color palette
#define COLOR_CYBER_GREEN (Color){0, 255, 156, 255}
#define COLOR_CYBER_GREEN_DARK (Color){0, 255, 156, 128}
#define COLOR_CYBER_GREEN_DIM (Color){0, 255, 156, 64}
#define COLOR_ALERT_YELLOW (Color){255, 215, 0, 255}
#define COLOR_DANGER_RED (Color){255, 59, 59, 255}
#define COLOR_LOCKED_RED (Color){255, 0, 0, 255}
#define COLOR_BG_BLACK (Color){0, 0, 0, 255}

//------------------------------------------------------------------------------------
// Types and Structures Definition
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

typedef struct {
    int value;
    char feedback[20]; // "MAIOR", "MENOR", "ACERTO!"
} GuessHistory;

typedef struct {
    char date[32];
    int attempts;
    bool won;
} GameSession;

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
} GameState;

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

typedef struct {
    GameSession sessions[MAX_SESSIONS];
    int sessionCount;
    int bestScore;
    float avgAttempts;
    float winRate;
} Statistics;

//------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------
static GameScreen currentScreen = SCREEN_MAIN_MENU;
static GameState gameState = {0};
static Statistics stats = {0};
static MatrixParticle matrixParticles[MAX_PARTICLES] = {0};
static GlitchParticle glitchParticles[MAX_PARTICLES] = {0};
static float glowPulse = 0.0f;
static bool showLogs = false;
static float logTimer = 0.0f;
static int currentLog = 0;

static const char* systemLogs[MAX_LOGS] = {
    "> INITIALIZING GHOST PROTOCOL...",
    "> LOADING NEURAL NETWORK...",
    "> PARSING HISTORY FILE...",
    "> TIMESTAMP: 2047-04-23 03:42:17",
    "> SENHA NUMERICA: GERADA",
    "> TENTATIVAS MAXIMAS: 7",
    "> SISTEMA DE ALERTA: ATIVO",
    "> READY FOR INTRUSION."
};

//------------------------------------------------------------------------------------
// Module Functions Declaration
//------------------------------------------------------------------------------------
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UnloadGame(void);

// Screen specific functions
static void UpdateMainMenu(void);
static void DrawMainMenu(void);
static void UpdateGameScreen(void);
static void DrawGameScreen(void);
static void UpdateResultScreen(void);
static void DrawResultScreen(void);
static void UpdateStatsScreen(void);
static void DrawStatsScreen(void);

// Game logic
static void StartNewGame(void);
static void ProcessGuess(int guess);
static AlertLevel GetAlertLevel(int attempts);
static Color GetAlertColor(AlertLevel level);
static const char* GetAlertText(AlertLevel level);
static void AddSession(int attempts, bool won);
static void CalculateStatistics(void);
static void SaveHistory(void);
static void LoadHistory(void);

// UI and Effects
static void InitMatrixEffect(void);
static void UpdateMatrixEffect(void);
static void DrawMatrixEffect(void);
static void InitGlitchEffect(void);
static void UpdateGlitchEffect(void);
static void DrawGlitchEffect(void);
static void DrawGlowText(const char* text, int x, int y, int fontSize, Color color);
static void DrawCyberButton(const char* text, int x, int y, int width, int height, bool hover);
static void DrawAlertBox(int x, int y, int width, int height);

//------------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------------
int main(void)
{
    // If window doesn't appear, comment out FLAG_WINDOW_HIGHDPI and FLAG_MSAA_4X_HINT
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GH0ST: 2047 - Neural Guessing System");
    
    printf("DEBUG: Window created successfully\n");
    
    SearchAndSetResourceDir("resources");
    
    SetTargetFPS(60);
    
    srand(time(NULL));
    
    InitGame();
    
    printf("DEBUG: Entering main game loop\n");
    
    // Main game loop
    int frameCount = 0;
    while (!WindowShouldClose())
    {
        if (frameCount < 5) {
            printf("DEBUG: Frame %d\n", frameCount);
        }
        UpdateGame();
        DrawGame();
        frameCount++;
    }
    
    printf("DEBUG: Exiting game loop\n");
    
    UnloadGame();
    CloseWindow();
    
    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions
//------------------------------------------------------------------------------------

static void InitGame(void)
{
    InitMatrixEffect();
    stats.bestScore = MAX_ATTEMPTS + 1; // Start with worst possible score
    currentScreen = SCREEN_MAIN_MENU;
    showLogs = true;
    logTimer = 0.0f;
    currentLog = 0;
    LoadHistory();
}

static void UpdateGame(void)
{
    glowPulse += GetFrameTime();
    UpdateMatrixEffect();
    UpdateGlitchEffect();
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:
            UpdateMainMenu();
            break;
        case SCREEN_GAME:
            UpdateGameScreen();
            break;
        case SCREEN_RESULT:
            UpdateResultScreen();
            break;
        case SCREEN_STATS:
            UpdateStatsScreen();
            break;
    }
}

static void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(COLOR_BG_BLACK);
    
    switch (currentScreen)
    {
        case SCREEN_MAIN_MENU:
            DrawMainMenu();
            break;
        case SCREEN_GAME:
            DrawGameScreen();
            break;
        case SCREEN_RESULT:
            DrawResultScreen();
            break;
        case SCREEN_STATS:
            DrawStatsScreen();
            break;
    }
    
    EndDrawing();
}

static void UnloadGame(void)
{
    SaveHistory();
}

//------------------------------------------------------------------------------------
// Main Menu Screen
//------------------------------------------------------------------------------------

static void UpdateMainMenu(void)
{
    // Animate system logs
    if (showLogs && currentLog < MAX_LOGS)
    {
        logTimer += GetFrameTime();
        if (logTimer >= 0.3f)
        {
            currentLog++;
            logTimer = 0.0f;
            if (currentLog >= MAX_LOGS)
            {
                showLogs = false;
            }
        }
    }
    
    Vector2 mousePos = GetMousePosition();
    
    // Button: Start Game
    Rectangle btnStart = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        StartNewGame();
        currentScreen = SCREEN_GAME;
    }
    
    // Button: Statistics
    Rectangle btnStats = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 170, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStats) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        CalculateStatistics();
        currentScreen = SCREEN_STATS;
    }
    Rectangle btnQuit = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 240, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnQuit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
{
    SaveHistory();
    CloseWindow();
}
}

static void DrawMainMenu(void)
{
    DrawMatrixEffect();
    
    Vector2 mousePos = GetMousePosition();
    
    // Title with glow effect
    DrawGlowText("GH0ST: 2047", SCREEN_WIDTH/2 - 220, 100, 64, COLOR_CYBER_GREEN);
    
    // Subtitle
    const char* subtitle = "NEURAL GUESSING SYSTEM v3.14";
    int subtitleWidth = MeasureText(subtitle, 14);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 180, 14, COLOR_CYBER_GREEN_DARK);
    
    // Warning message
    const char* warning = "WARNING: 7 TENTATIVAS ANTES DO BLOQUEIO TOTAL";
    int warningWidth = MeasureText(warning, 12);
    float warningAlpha = (sinf(glowPulse * 2.0f) + 1.0f) * 0.5f;
    Color warningColor = ColorAlpha(COLOR_DANGER_RED, warningAlpha);
    DrawText(warning, SCREEN_WIDTH/2 - warningWidth/2, 210, 12, warningColor);
    
    // System logs box
    int logBoxY = 260;
    DrawRectangle(SCREEN_WIDTH/2 - 400, logBoxY, 800, 240, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 400, logBoxY, 800, 240, COLOR_CYBER_GREEN_DIM);
    
    DrawText("SYSTEM_LOG.TXT", SCREEN_WIDTH/2 - 380, logBoxY + 15, 12, COLOR_CYBER_GREEN);
    
    for (int i = 0; i < currentLog && i < MAX_LOGS; i++)
    {
        DrawText(systemLogs[i], SCREEN_WIDTH/2 - 380, logBoxY + 50 + i * 20, 12, COLOR_CYBER_GREEN);
    }
    
    // Buttons
    Rectangle btnStart = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 50};
    bool hoverStart = CheckCollisionPointRec(mousePos, btnStart);
    DrawCyberButton(">> INICIAR MISSAO", btnStart.x, btnStart.y, btnStart.width, btnStart.height, hoverStart);
    
    Rectangle btnStats = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 170, 300, 50};
    bool hoverStats = CheckCollisionPointRec(mousePos, btnStats);
    DrawCyberButton(">> ESTATISTICAS", btnStats.x, btnStats.y, btnStats.width, btnStats.height, hoverStats);

    // ADICIONADO após o DrawCyberButton de Estatísticas:
    Rectangle btnQuit = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 240, 300, 50};
    bool hoverQuit = CheckCollisionPointRec(mousePos, btnQuit);
    DrawCyberButton(">> ENCERRAR SISTEMA", btnQuit.x, btnQuit.y, btnQuit.width, btnQuit.height, hoverQuit);
}

//------------------------------------------------------------------------------------
// Game Screen
//------------------------------------------------------------------------------------

static void UpdateGameScreen(void)
{
    if (gameState.gameOver) return;
    
    // Update error timer
    if (gameState.errorTimer > 0.0f)
    {
        gameState.errorTimer -= GetFrameTime();
    }
    
    // Handle text input for numbers
    int key = GetCharPressed();
    while (key > 0)
    {
        if ((key >= '0' && key <= '9') && strlen(gameState.inputBuffer) < MAX_INPUT_LENGTH)
        {
            int len = strlen(gameState.inputBuffer);
            gameState.inputBuffer[len] = (char)key;
            gameState.inputBuffer[len + 1] = '\0';
        }
        key = GetCharPressed();
    }
    
    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        int len = strlen(gameState.inputBuffer);
        if (len > 0)
        {
            gameState.inputBuffer[len - 1] = '\0';
        }
    }
    
    // Handle Enter key to submit guess
    if (IsKeyPressed(KEY_ENTER) && strlen(gameState.inputBuffer) > 0)
    {
        int guess = atoi(gameState.inputBuffer);
        ProcessGuess(guess);
        gameState.inputBuffer[0] = '\0';
    }
    
    // ESC to return to menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawGameScreen(void)
{
    DrawMatrixEffect();
    
    // Header
    DrawGlowText("GH0ST: 2047", 50, 30, 32, COLOR_CYBER_GREEN);
    
    // Alert status
    Color alertColor = GetAlertColor(gameState.alertLevel);
    const char* alertText = GetAlertText(gameState.alertLevel);
    int alertWidth = MeasureText(alertText, 20);
    DrawRectangle(SCREEN_WIDTH - alertWidth - 80, 30, alertWidth + 60, 40, ColorAlpha(alertColor, 0.1f));
    DrawRectangleLines(SCREEN_WIDTH - alertWidth - 80, 30, alertWidth + 60, 40, alertColor);
    DrawText(alertText, SCREEN_WIDTH - alertWidth - 50, 42, 20, alertColor);
    
    // Attempts counter
    char attemptsText[32];
    sprintf(attemptsText, "TENTATIVAS: %d/%d", gameState.attempts, gameState.maxAttempts);
    DrawText(attemptsText, 50, 90, 20, COLOR_CYBER_GREEN);
    
    // Range info
    char rangeText[64];
    sprintf(rangeText, "RANGE: %d - %d", gameState.minRange, gameState.maxRange);
    DrawText(rangeText, 50, 120, 20, COLOR_CYBER_GREEN_DARK);
    
    // Suggested guess (binary search)
    int suggested = (gameState.minRange + gameState.maxRange) / 2;
    char suggestedText[64];
    sprintf(suggestedText, "SUGESTAO IA: %d", suggested);
    DrawText(suggestedText, 50, 150, 16, COLOR_CYBER_GREEN_DIM);
    
    // Input box
    int inputBoxY = SCREEN_HEIGHT/2 - 50;
    DrawRectangle(SCREEN_WIDTH/2 - 200, inputBoxY, 400, 80, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 200, inputBoxY, 400, 80, COLOR_CYBER_GREEN);
    
    DrawText("DIGITE O NUMERO:", SCREEN_WIDTH/2 - 180, inputBoxY + 15, 16, COLOR_CYBER_GREEN_DARK);
    
    // Input display with cursor
    char displayText[16];
    sprintf(displayText, "%s_", gameState.inputBuffer);
    int textWidth = MeasureText(displayText, 32);
    DrawText(displayText, SCREEN_WIDTH/2 - textWidth/2, inputBoxY + 40, 32, COLOR_CYBER_GREEN);
    
    // Error message
    if (gameState.errorTimer > 0.0f)
    {
        int errorWidth = MeasureText(gameState.errorMsg, 16);
        DrawText(gameState.errorMsg, SCREEN_WIDTH/2 - errorWidth/2, inputBoxY + 90, 16, COLOR_DANGER_RED);
    }
    
    // History panel
    DrawAlertBox(50, SCREEN_HEIGHT - 320, 400, 280);
    
    DrawText("HISTORICO DE TENTATIVAS:", 70, SCREEN_HEIGHT - 300, 16, COLOR_CYBER_GREEN);
    
    for (int i = 0; i < gameState.attempts && i < MAX_HISTORY; i++)
    {
        char historyText[64];
        sprintf(historyText, "%d. VALOR: %d -> %s", 
                i + 1, 
                gameState.history[i].value, 
                gameState.history[i].feedback);
        
        Color historyColor = (strcmp(gameState.history[i].feedback, "ACERTO!") == 0) 
                             ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK;
        
        DrawText(historyText, 70, SCREEN_HEIGHT - 270 + i * 30, 14, historyColor);
    }
    
    // Alert legend
    DrawAlertBox(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 320, 400, 280);
    DrawText("SISTEMA DE ALERTAS", SCREEN_WIDTH - 430, SCREEN_HEIGHT - 300, 16, COLOR_CYBER_GREEN);
    
    const char* alertLevels[] = {
        "1-2: SISTEMA ESTAVEL",
        "3-4: ALERTA DETECTADO",
        "5-6: RASTREAMENTO ATIVO",
        "  7: BLOQUEADO"
    };
    
    Color alertColors[] = {COLOR_CYBER_GREEN, COLOR_ALERT_YELLOW, COLOR_DANGER_RED, COLOR_LOCKED_RED};
    
    for (int i = 0; i < 4; i++)
    {
        DrawRectangle(SCREEN_WIDTH - 430, SCREEN_HEIGHT - 260 + i * 50, 360, 40, ColorAlpha(alertColors[i], 0.1f));
        DrawRectangleLines(SCREEN_WIDTH - 430, SCREEN_HEIGHT - 260 + i * 50, 360, 40, alertColors[i]);
        DrawText(alertLevels[i], SCREEN_WIDTH - 410, SCREEN_HEIGHT - 245 + i * 50, 14, alertColors[i]);
    }
    
    // Instructions
    DrawText("PRESSIONE ENTER PARA CONFIRMAR | ESC PARA SAIR", 
             SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT - 30, 14, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Result Screen
//------------------------------------------------------------------------------------

static void UpdateResultScreen(void)
{
    Vector2 mousePos = GetMousePosition();
    
    // Button: Play Again
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnPlayAgain) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        StartNewGame();
        currentScreen = SCREEN_GAME;
    }
    
    // Button: Menu
    Rectangle btnMenu = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 220, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnMenu) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawResultScreen(void)
{
    DrawGlitchEffect();
    
    Vector2 mousePos = GetMousePosition();
    
    // Main result box
    Color borderColor = gameState.won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
    DrawRectangle(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, borderColor);
    DrawRectangleLines(SCREEN_WIDTH/2 - 397, SCREEN_HEIGHT/2 - 297, 794, 494, borderColor);
    
    // Result icon and title
    const char* resultTitle = gameState.won ? "SENHA DESBLOQUEADA" : "SISTEMA BLOQUEADO";
    const char* resultIcon = gameState.won ? "[OK]" : "[X]";
    
    int titleWidth = MeasureText(resultTitle, 48);
    DrawGlowText(resultTitle, SCREEN_WIDTH/2 - titleWidth/2, SCREEN_HEIGHT/2 - 230, 48, borderColor);
    
    // Attempts info
    DrawRectangle(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 100, 600, 100, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
    DrawRectangleLines(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 100, 600, 100, COLOR_CYBER_GREEN_DIM);
    
    DrawText("TENTATIVAS UTILIZADAS:", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 80, 14, COLOR_CYBER_GREEN_DARK);
    
    char attemptsText[8];
    sprintf(attemptsText, "%d", gameState.attempts);
    int attemptsWidth = MeasureText(attemptsText, 48);
    DrawText(attemptsText, SCREEN_WIDTH/2 - attemptsWidth/2, SCREEN_HEIGHT/2 - 50, 48, COLOR_CYBER_GREEN);
    
    // High score indicator
    if (gameState.won && gameState.attempts <= stats.bestScore)
    {
        const char* newRecord = "*** NOVO RECORDE ***";
        int recordWidth = MeasureText(newRecord, 20);
        float recordAlpha = (sinf(glowPulse * 4.0f) + 1.0f) * 0.5f;
        DrawText(newRecord, SCREEN_WIDTH/2 - recordWidth/2, SCREEN_HEIGHT/2 + 20, 20, 
                 ColorAlpha(COLOR_ALERT_YELLOW, recordAlpha));
    }
    
    // Target number reveal
    char targetText[32];
    sprintf(targetText, "SENHA: %d", gameState.targetNumber);
    int targetWidth = MeasureText(targetText, 24);
    DrawText(targetText, SCREEN_WIDTH/2 - targetWidth/2, SCREEN_HEIGHT/2 + 60, 24, COLOR_CYBER_GREEN_DARK);
    
    // Buttons
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    bool hoverPlayAgain = CheckCollisionPointRec(mousePos, btnPlayAgain);
    DrawCyberButton(">> TENTAR NOVAMENTE", btnPlayAgain.x, btnPlayAgain.y, 
                    btnPlayAgain.width, btnPlayAgain.height, hoverPlayAgain);
    
    Rectangle btnMenu = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 220, 300, 50};
    bool hoverMenu = CheckCollisionPointRec(mousePos, btnMenu);
    DrawCyberButton(">> MENU PRINCIPAL", btnMenu.x, btnMenu.y, btnMenu.width, btnMenu.height, hoverMenu);
}

//------------------------------------------------------------------------------------
// Statistics Screen
//------------------------------------------------------------------------------------

static void UpdateStatsScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentScreen = SCREEN_MAIN_MENU;
    }
}

static void DrawStatsScreen(void)
{
    DrawMatrixEffect();
    
    // Header
    DrawGlowText("ANALISE ESTATISTICA", SCREEN_WIDTH/2 - 260, 50, 48, COLOR_CYBER_GREEN);
    
    char sessionsText[64];
    sprintf(sessionsText, "PROCESSANDO DADOS DE %d SESSOES...", stats.sessionCount);
    int sessionsWidth = MeasureText(sessionsText, 14);
    DrawText(sessionsText, SCREEN_WIDTH/2 - sessionsWidth/2, 120, 14, COLOR_CYBER_GREEN_DARK);
    
    if (stats.sessionCount == 0)
    {
        const char* noData = "NENHUM DADO DISPONIVEL.";
        const char* startMsg = "INICIE UMA SESSAO PARA COLETAR ESTATISTICAS.";
        
        int noDataWidth = MeasureText(noData, 24);
        int startWidth = MeasureText(startMsg, 16);
        
        DrawRectangle(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 100, 800, 200, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 100, 800, 200, COLOR_CYBER_GREEN_DIM);
        
        DrawText(noData, SCREEN_WIDTH/2 - noDataWidth/2, SCREEN_HEIGHT/2 - 20, 24, COLOR_CYBER_GREEN_DARK);
        DrawText(startMsg, SCREEN_WIDTH/2 - startWidth/2, SCREEN_HEIGHT/2 + 20, 16, COLOR_CYBER_GREEN_DIM);
    }
    else
    {
        // Stats cards
        int cardY = 180;
        int cardWidth = 250;
        int cardHeight = 120;
        int spacing = 30;
        int startX = (SCREEN_WIDTH - (cardWidth * 3 + spacing * 2)) / 2;
        
        // Card 1: Total Sessions
        DrawRectangle(startX, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(startX, cardY, cardWidth, cardHeight, COLOR_CYBER_GREEN);
        DrawText("TOTAL SESSOES", startX + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char totalText[16];
        sprintf(totalText, "%d", stats.sessionCount);
        DrawText(totalText, startX + 20, cardY + 50, 42, COLOR_CYBER_GREEN);
        
        // Card 2: Win Rate
        DrawRectangle(startX + cardWidth + spacing, cardY, cardWidth, cardHeight, 
                     ColorAlpha(COLOR_ALERT_YELLOW, 0.05f));
        DrawRectangleLines(startX + cardWidth + spacing, cardY, cardWidth, cardHeight, COLOR_ALERT_YELLOW);
        DrawText("TAXA VITORIA", startX + cardWidth + spacing + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char winRateText[16];
        sprintf(winRateText, "%.1f%%", stats.winRate);
        DrawText(winRateText, startX + cardWidth + spacing + 20, cardY + 50, 42, COLOR_ALERT_YELLOW);
        
        // Card 3: Best Score
        DrawRectangle(startX + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, 
                     ColorAlpha(COLOR_DANGER_RED, 0.05f));
        DrawRectangleLines(startX + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, COLOR_DANGER_RED);
        DrawText("MELHOR SCORE", startX + (cardWidth + spacing) * 2 + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char bestText[16];
        sprintf(bestText, "%d", stats.bestScore <= MAX_ATTEMPTS ? stats.bestScore : 0);
        DrawText(bestText, startX + (cardWidth + spacing) * 2 + 20, cardY + 50, 42, COLOR_DANGER_RED);
        
        // Additional stats
        cardY += 170;
        DrawRectangle(startX, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(startX, cardY, cardWidth, cardHeight, COLOR_CYBER_GREEN_DIM);
        DrawText("MEDIA TENTATIVAS", startX + 20, cardY + 20, 14, COLOR_CYBER_GREEN_DARK);
        char avgText[16];
        sprintf(avgText, "%.2f", stats.avgAttempts);
        DrawText(avgText, startX + 20, cardY + 50, 42, COLOR_CYBER_GREEN);
        
        // Recent sessions history
        int historyY = cardY + 170;
        DrawText("ULTIMAS SESSOES:", startX, historyY, 18, COLOR_CYBER_GREEN);
        
        int displayCount = (stats.sessionCount < 10) ? stats.sessionCount : 10;
        for (int i = 0; i < displayCount; i++)
        {
            int idx = stats.sessionCount - 1 - i;
            GameSession* session = &stats.sessions[idx];
            
            char sessionText[128];
            sprintf(sessionText, "#%d [%s] - %d tent. - %s", 
                    idx + 1,
                    session->date,
                    session->attempts,
                    session->won ? "VITORIA" : "DERROTA");
            
            Color sessionColor = session->won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
            DrawText(sessionText, startX, historyY + 40 + i * 30, 14, sessionColor);
        }
    }
    
    // Instructions
    DrawText("PRESSIONE ESC OU CLIQUE PARA VOLTAR", 
             SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT - 40, 14, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Game Logic Functions
//------------------------------------------------------------------------------------

static void StartNewGame(void)
{
    gameState.targetNumber = GetRandomValue(1, 100);
    gameState.attempts = 0;
    gameState.maxAttempts = MAX_ATTEMPTS;
    gameState.minRange = 1;
    gameState.maxRange = 100;
    gameState.inputBuffer[0] = '\0';
    gameState.gameOver = false;
    gameState.won = false;
    gameState.alertLevel = ALERT_STABLE;
    gameState.errorMsg[0] = '\0';
    gameState.errorTimer = 0.0f;
    
    memset(gameState.history, 0, sizeof(gameState.history));
    
    InitGlitchEffect();
}

static void ProcessGuess(int guess)
{
    // Validation
    if (guess < 1 || guess > 100)
    {
        strcpy(gameState.errorMsg, "INPUT INVALIDO: RANGE 1-100");
        gameState.errorTimer = 2.0f;
        return;
    }
    
    if (guess < gameState.minRange || guess > gameState.maxRange)
    {
        strcpy(gameState.errorMsg, "NUMERO FORA DO RANGE ATUAL!");
        gameState.errorTimer = 2.0f;
        return;
    }
    
    // Process guess
    gameState.attempts++;
    gameState.alertLevel = GetAlertLevel(gameState.attempts);
    
    if (guess == gameState.targetNumber)
    {
        // WIN!
        strcpy(gameState.history[gameState.attempts - 1].feedback, "ACERTO!");
        gameState.history[gameState.attempts - 1].value = guess;
        gameState.won = true;
        gameState.gameOver = true;
        AddSession(gameState.attempts, true);
        currentScreen = SCREEN_RESULT;
    }
    else if (gameState.attempts >= gameState.maxAttempts)
    {
        // GAME OVER
        strcpy(gameState.history[gameState.attempts - 1].feedback, guess < gameState.targetNumber ? "MAIOR" : "MENOR");
        gameState.history[gameState.attempts - 1].value = guess;
        gameState.gameOver = true;
        gameState.won = false;
        AddSession(gameState.attempts, false);
        currentScreen = SCREEN_RESULT;
    }
    else
    {
        // Update range and continue
        if (guess < gameState.targetNumber)
        {
            strcpy(gameState.history[gameState.attempts - 1].feedback, "MAIOR");
            gameState.minRange = guess + 1;
        }
        else
        {
            strcpy(gameState.history[gameState.attempts - 1].feedback, "MENOR");
            gameState.maxRange = guess - 1;
        }
        gameState.history[gameState.attempts - 1].value = guess;
    }
}

static AlertLevel GetAlertLevel(int attempts)
{
    if (attempts <= 2) return ALERT_STABLE;
    if (attempts <= 4) return ALERT_WARNING;
    if (attempts <= 6) return ALERT_DANGER;
    return ALERT_LOCKED;
}

static Color GetAlertColor(AlertLevel level)
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

static const char* GetAlertText(AlertLevel level)
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

static void AddSession(int attempts, bool won)
{
    if (stats.sessionCount < MAX_SESSIONS)
    {
        stats.sessions[stats.sessionCount].attempts = attempts;
        stats.sessions[stats.sessionCount].won = won;
        
        // Real timestamp
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        strftime(stats.sessions[stats.sessionCount].date,
                 sizeof(stats.sessions[stats.sessionCount].date),
                 "%Y-%m-%d %H:%M:%S", t);
        
        stats.sessionCount++;
        SaveHistory();
    }
}

static void CalculateStatistics(void)
{
    if (stats.sessionCount == 0) return;
    
    int wonCount = 0;
    int totalAttempts = 0;
    stats.bestScore = MAX_ATTEMPTS + 1;
    
    for (int i = 0; i < stats.sessionCount; i++)
    {
        if (stats.sessions[i].won)
        {
            wonCount++;
            totalAttempts += stats.sessions[i].attempts;
            if (stats.sessions[i].attempts < stats.bestScore)
            {
                stats.bestScore = stats.sessions[i].attempts;
            }
        }
    }
    
    stats.winRate = (wonCount * 100.0f) / stats.sessionCount;
    stats.avgAttempts = wonCount > 0 ? (float)totalAttempts / wonCount : 0.0f;
}

static void SaveHistory(void)
{
    FILE* f = fopen(HISTORY_FILE, "wb");
    if (!f) return;

    unsigned int magic = HISTORY_MAGIC;
    int version = 1;
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version, sizeof(version), 1, f);

    fwrite(&stats.sessionCount, sizeof(stats.sessionCount), 1, f);

    for (int i = 0; i < stats.sessionCount; i++)
    {
        fwrite(&stats.sessions[i], sizeof(GameSession), 1, f);
    }

    fclose(f);
}

static void LoadHistory(void)
{
    FILE* f = fopen(HISTORY_FILE, "rb");
    if (!f) return;

    unsigned int magic = 0;
    int version = 0;
    if (fread(&magic, sizeof(magic), 1, f) != 1 || magic != HISTORY_MAGIC)
    {
        fclose(f);
        return;
    }
    if (fread(&version, sizeof(version), 1, f) != 1 || version != 1)
    {
        fclose(f);
        return;
    }

    int count = 0;
    if (fread(&count, sizeof(count), 1, f) != 1 || count < 0 || count > MAX_SESSIONS)
    {
        fclose(f);
        return;
    }
    stats.sessionCount = count;

    for (int i = 0; i < stats.sessionCount; i++)
    {
        if (fread(&stats.sessions[i], sizeof(GameSession), 1, f) != 1)
        {
            stats.sessionCount = i;
            break;
        }
    }

    fclose(f);
    CalculateStatistics();
}

//------------------------------------------------------------------------------------
// UI and Visual Effects Functions
//------------------------------------------------------------------------------------

static void InitMatrixEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        matrixParticles[i].position.x = GetRandomValue(0, SCREEN_WIDTH);
        matrixParticles[i].position.y = GetRandomValue(-SCREEN_HEIGHT, 0);
        matrixParticles[i].speed = GetRandomValue(100, 300);
        matrixParticles[i].alpha = GetRandomValue(50, 200) / 255.0f;
        
        // Random binary digits
        int randNum = GetRandomValue(0, 255);
        sprintf(matrixParticles[i].text, "%d", randNum % 2);
    }
}

static void UpdateMatrixEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        matrixParticles[i].position.y += matrixParticles[i].speed * GetFrameTime();
        
        if (matrixParticles[i].position.y > SCREEN_HEIGHT)
        {
            matrixParticles[i].position.x = GetRandomValue(0, SCREEN_WIDTH);
            matrixParticles[i].position.y = -20;
            matrixParticles[i].speed = GetRandomValue(100, 300);
        }
    }
}

static void DrawMatrixEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Color color = ColorAlpha(COLOR_CYBER_GREEN, matrixParticles[i].alpha * 0.3f);
        DrawText(matrixParticles[i].text, 
                 matrixParticles[i].position.x, 
                 matrixParticles[i].position.y, 
                 10, color);
    }
}

static void InitGlitchEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        glitchParticles[i].position.x = SCREEN_WIDTH / 2;
        glitchParticles[i].position.y = SCREEN_HEIGHT / 2;
        glitchParticles[i].velocity.x = GetRandomValue(-200, 200);
        glitchParticles[i].velocity.y = GetRandomValue(-200, 200);
        glitchParticles[i].lifetime = 0.0f;
        glitchParticles[i].maxLifetime = GetRandomValue(5, 15) / 10.0f;
        glitchParticles[i].color = gameState.won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
    }
}

static void UpdateGlitchEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        glitchParticles[i].lifetime += GetFrameTime();
        
        if (glitchParticles[i].lifetime < glitchParticles[i].maxLifetime)
        {
            glitchParticles[i].position.x += glitchParticles[i].velocity.x * GetFrameTime();
            glitchParticles[i].position.y += glitchParticles[i].velocity.y * GetFrameTime();
        }
    }
}

static void DrawGlitchEffect(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (glitchParticles[i].lifetime < glitchParticles[i].maxLifetime)
        {
            float progress = glitchParticles[i].lifetime / glitchParticles[i].maxLifetime;
            float alpha = 1.0f - progress;
            
            Color color = ColorAlpha(glitchParticles[i].color, alpha * 0.6f);
            DrawCircle(glitchParticles[i].position.x, glitchParticles[i].position.y, 3, color);
        }
    }
}

static void DrawGlowText(const char* text, int x, int y, int fontSize, Color color)
{
    // Draw shadow/glow layers
    float glow = (sinf(glowPulse * 2.0f) + 1.0f) * 0.5f;
    
    for (int i = 3; i > 0; i--)
    {
        Color glowColor = ColorAlpha(color, glow * 0.2f * i);
        DrawText(text, x - i, y, fontSize, glowColor);
        DrawText(text, x + i, y, fontSize, glowColor);
        DrawText(text, x, y - i, fontSize, glowColor);
        DrawText(text, x, y + i, fontSize, glowColor);
    }
    
    // Draw main text
    DrawText(text, x, y, fontSize, color);
}

static void DrawCyberButton(const char* text, int x, int y, int width, int height, bool hover)
{
    Color btnColor = hover ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK;
    Color bgColor = hover ? ColorAlpha(COLOR_CYBER_GREEN, 0.2f) : ColorAlpha(COLOR_CYBER_GREEN, 0.05f);
    
    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangleLines(x, y, width, height, btnColor);
    
    if (hover)
    {
        DrawRectangleLines(x - 2, y - 2, width + 4, height + 4, btnColor);
    }
    
    int textWidth = MeasureText(text, 18);
    DrawText(text, x + (width - textWidth) / 2, y + (height - 18) / 2, 18, btnColor);
}

static void DrawAlertBox(int x, int y, int width, int height)
{
    DrawRectangle(x, y, width, height, COLOR_BG_BLACK);
    DrawRectangleLines(x, y, width, height, COLOR_CYBER_GREEN_DIM);
}
