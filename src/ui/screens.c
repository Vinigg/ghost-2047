/*
GH0ST: 2047 - Screens Module
Update and Draw functions for each game screen
*/

#include "ui/screens.h"
#include "ui/ui_components.h"
#include "game/game_logic.h"
#include "game/logic_questions.h"
#include "game/analytics.h"
#include "effects/matrix_effect.h"
#include "effects/glitch_effect.h"
#include "audio/music_manager.h"
#include "persistence/session_logger.h"
#include "persistence/history.h"
#include "config.h"
#include "theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//------------------------------------------------------------------------------------
// Module State
//------------------------------------------------------------------------------------
static ScreenContext ctx = {0};
static bool showLogs = true;
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

static void DrawTutorialPanel(int x, int y, int width, int height, const char* title,
                              const char* line1, const char* line2, const char* line3,
                              Color borderColor)
{
    DrawRectangle(x, y, width, height, ColorAlpha(borderColor, 0.05f));
    DrawRectangleLines(x, y, width, height, borderColor);
    DrawRectangleLines(x + 2, y + 2, width - 4, height - 4, ColorAlpha(borderColor, 0.45f));

    DrawText(title, x + 24, y + 22, 20, borderColor);
    DrawText(line1, x + 24, y + 62, 15, COLOR_CYBER_GREEN_DARK);
    DrawText(line2, x + 24, y + 92, 15, COLOR_CYBER_GREEN_DARK);
    DrawText(line3, x + 24, y + 122, 15, COLOR_CYBER_GREEN_DARK);
}

//------------------------------------------------------------------------------------
// Public: Init
//------------------------------------------------------------------------------------
void Screens_Init(ScreenContext context)
{
    ctx = context;
    showLogs = true;
    logTimer = 0.0f;
    currentLog = 0;
}

//------------------------------------------------------------------------------------
// Main Menu Screen
//------------------------------------------------------------------------------------
void Screen_UpdateMainMenu(void)
{
    MusicManager_SwitchToMenu();
    
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
    
    Vector2 mousePos = ctx.getVirtualMousePosition();
    
    // Button: Start Game
    Rectangle btnStart = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_DIFFICULTY_SELECT;
    }
    
    // Button: Tutorial
    Rectangle btnTutorial = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 170, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnTutorial) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_TUTORIAL;
    }

    // Button: Statistics
    Rectangle btnStats = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 240, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStats) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Stats_Calculate(ctx.stats);
        *ctx.currentScreen = SCREEN_STATS;
    }
    
    // Button: Quit
    Rectangle btnQuit = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 310, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnQuit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        History_Save(ctx.stats);
        CloseWindow();
    }
}

void Screen_DrawMainMenu(void)
{
    DrawMatrixEffect();
    
    Vector2 mousePos = ctx.getVirtualMousePosition();
    
    // Title with glow effect
    UI_DrawGlowText("GH0ST: 2047", SCREEN_WIDTH/2 - 220, 100, 64, COLOR_CYBER_GREEN, *ctx.glowPulse);
    
    // Subtitle
    const char* subtitle = "NEURAL GUESSING SYSTEM v3.14";
    int subtitleWidth = MeasureText(subtitle, 14);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 180, 14, COLOR_CYBER_GREEN_DARK);
    
    // Warning message
    const char* warning = "WARNING: 7 TENTATIVAS ANTES DO BLOQUEIO TOTAL";
    int warningWidth = MeasureText(warning, 12);
    float warningAlpha = (sinf(*ctx.glowPulse * 2.0f) + 1.0f) * 0.5f;
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
    UI_DrawCyberButton(">> INICIAR MISSAO", btnStart.x, btnStart.y, btnStart.width, btnStart.height, hoverStart);
    
    Rectangle btnTutorial = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 170, 300, 50};
    bool hoverTutorial = CheckCollisionPointRec(mousePos, btnTutorial);
    UI_DrawCyberButton(">> TUTORIAL", btnTutorial.x, btnTutorial.y, btnTutorial.width, btnTutorial.height, hoverTutorial);

    Rectangle btnStats = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 240, 300, 50};
    bool hoverStats = CheckCollisionPointRec(mousePos, btnStats);
    UI_DrawCyberButton(">> ESTATISTICAS", btnStats.x, btnStats.y, btnStats.width, btnStats.height, hoverStats);

    Rectangle btnQuit = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 310, 300, 50};
    bool hoverQuit = CheckCollisionPointRec(mousePos, btnQuit);
    UI_DrawCyberButton(">> ENCERRAR SISTEMA", btnQuit.x, btnQuit.y, btnQuit.width, btnQuit.height, hoverQuit);
    
    // Fullscreen hint
    DrawText("[F11] TELA CHEIA", SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, 12, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Tutorial Screen
//------------------------------------------------------------------------------------
void Screen_UpdateTutorial(void)
{
    MusicManager_SwitchToMenu();

    Vector2 mousePos = ctx.getVirtualMousePosition();

    Rectangle btnStart = {SCREEN_WIDTH/2 - 320, SCREEN_HEIGHT - 105, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_DIFFICULTY_SELECT;
    }

    Rectangle btnBack = {SCREEN_WIDTH/2 + 20, SCREEN_HEIGHT - 105, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_MAIN_MENU;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        *ctx.currentScreen = SCREEN_DIFFICULTY_SELECT;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        *ctx.currentScreen = SCREEN_MAIN_MENU;
    }
}

void Screen_DrawTutorial(void)
{
    DrawMatrixEffect();

    Vector2 mousePos = ctx.getVirtualMousePosition();

    UI_DrawGlowText("TUTORIAL DE INTRUSAO", SCREEN_WIDTH/2 - 300, 55, 46, COLOR_CYBER_GREEN, *ctx.glowPulse);

    const char* subtitle = "APRENDA O PROTOCOLO ANTES DE TENTAR DESBLOQUEAR A SENHA";
    int subtitleWidth = MeasureText(subtitle, 14);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 118, 14, COLOR_CYBER_GREEN_DARK);

    DrawTutorialPanel(
        90, 165, 560, 150,
        "1. OBJETIVO",
        "Descubra a senha numerica secreta antes do tempo acabar.",
        "A senha esta dentro do intervalo exibido na tela.",
        "Use as pistas MAIOR e MENOR para aproximar seu palpite.",
        COLOR_CYBER_GREEN
    );

    DrawTutorialPanel(
        716, 165, 560, 150,
        "2. COMO JOGAR",
        "Digite apenas numeros e pressione ENTER para confirmar.",
        "BACKSPACE apaga o ultimo digito digitado.",
        "ESC abandona a missao e volta para o menu principal.",
        COLOR_CYBER_GREEN
    );

    DrawTutorialPanel(
        90, 350, 560, 150,
        "3. ESTRATEGIA",
        "Comece pelo meio do intervalo: a sugestao da IA ajuda nisso.",
        "Se aparecer MAIOR, tente acima do valor informado.",
        "Se aparecer MENOR, tente abaixo do valor informado.",
        COLOR_ALERT_YELLOW
    );

    DrawTutorialPanel(
        716, 350, 560, 150,
        "4. ALERTAS E DICAS",
        "Voce tem 7 tentativas antes do bloqueio total.",
        "Depois de alguns erros, um desafio logico pode liberar dica.",
        "Responda com as teclas 1-4 ou clique na opcao correta.",
        COLOR_DANGER_RED
    );

    DrawRectangle(SCREEN_WIDTH/2 - 360, 535, 720, 70, ColorAlpha(COLOR_CYBER_GREEN, 0.06f));
    DrawRectangleLines(SCREEN_WIDTH/2 - 360, 535, 720, 70, COLOR_CYBER_GREEN_DIM);
    DrawText("RESUMO: DIVIDA O INTERVALO, CONFIE NAS PISTAS E ECONOMIZE TENTATIVAS.",
             SCREEN_WIDTH/2 - 315, 562, 16, COLOR_CYBER_GREEN);

    Rectangle btnStart = {SCREEN_WIDTH/2 - 320, SCREEN_HEIGHT - 105, 300, 50};
    bool hoverStart = CheckCollisionPointRec(mousePos, btnStart);
    UI_DrawCyberButton(">> ESCOLHER DIFICULDADE", btnStart.x, btnStart.y, btnStart.width, btnStart.height, hoverStart);

    Rectangle btnBack = {SCREEN_WIDTH/2 + 20, SCREEN_HEIGHT - 105, 300, 50};
    bool hoverBack = CheckCollisionPointRec(mousePos, btnBack);
    UI_DrawCyberButton(">> VOLTAR AO MENU", btnBack.x, btnBack.y, btnBack.width, btnBack.height, hoverBack);

    DrawText("ENTER PARA CONTINUAR | ESC PARA VOLTAR | F11 TELA CHEIA",
             SCREEN_WIDTH/2 - 245, SCREEN_HEIGHT - 30, 12, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Difficulty Selection Screen
//------------------------------------------------------------------------------------
void Screen_UpdateDifficultySelect(void)
{
    MusicManager_SwitchToMenu();
    
    Vector2 mousePos = ctx.getVirtualMousePosition();
    
    // Button: Easy Mode
    Rectangle btnEasy = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 - 20, 320, 60};
    if (CheckCollisionPointRec(mousePos, btnEasy) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentDifficulty = DIFFICULTY_EASY;
        QuestionBank_Unload(ctx.questionBank);
        QuestionBank_Load(ctx.questionBank, QUESTIONS_FILE_EASY);
        Game_StartNew(ctx.gameState, DIFFICULTY_EASY);
        *ctx.currentQuestion = QuestionBank_GetRandom(ctx.questionBank);
        *ctx.currentScreen = SCREEN_GAME;
    }
    
    // Button: Hard Mode
    Rectangle btnHard = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 + 60, 320, 60};
    if (CheckCollisionPointRec(mousePos, btnHard) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentDifficulty = DIFFICULTY_HARD;
        QuestionBank_Unload(ctx.questionBank);
        QuestionBank_Load(ctx.questionBank, QUESTIONS_FILE_HARD);
        Game_StartNew(ctx.gameState, DIFFICULTY_HARD);
        *ctx.currentQuestion = QuestionBank_GetRandom(ctx.questionBank);
        *ctx.currentScreen = SCREEN_GAME;
    }
    
    // ESC to return to menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        *ctx.currentScreen = SCREEN_MAIN_MENU;
    }
}

void Screen_DrawDifficultySelect(void)
{
    DrawMatrixEffect();
    
    Vector2 mousePos = ctx.getVirtualMousePosition();
    
    // Title
    UI_DrawGlowText("SELECIONE A DIFICULDADE", SCREEN_WIDTH/2 - 320, 100, 48, COLOR_CYBER_GREEN, *ctx.glowPulse);
    
    // Subtitle
    const char* subtitle = "ESCOLHA O NIVEL DE DESAFIO PARA SUA MISSAO";
    int subtitleWidth = MeasureText(subtitle, 14);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 170, 14, COLOR_CYBER_GREEN_DARK);
    
    // Easy Mode Button
    Rectangle btnEasy = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 - 20, 320, 60};
    bool hoverEasy = CheckCollisionPointRec(mousePos, btnEasy);
    
    Color easyBorderColor = hoverEasy ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DIM;
    DrawRectangle(btnEasy.x, btnEasy.y, btnEasy.width, btnEasy.height, ColorAlpha(COLOR_CYBER_GREEN, hoverEasy ? 0.15f : 0.05f));
    DrawRectangleLines(btnEasy.x, btnEasy.y, btnEasy.width, btnEasy.height, easyBorderColor);
    DrawRectangleLines(btnEasy.x + 2, btnEasy.y + 2, btnEasy.width - 4, btnEasy.height - 4, ColorAlpha(easyBorderColor, 0.5f));
    
    const char* easyLabel = ">> MODO FACIL";
    int easyLabelWidth = MeasureText(easyLabel, 24);
    DrawText(easyLabel, SCREEN_WIDTH/2 - easyLabelWidth/2, btnEasy.y + 18, 24, COLOR_CYBER_GREEN);
    
    // Easy mode description
    const char* easyDesc = "Questoes de logica basicas | 2 minutos";
    int easyDescWidth = MeasureText(easyDesc, 12);
    DrawText(easyDesc, SCREEN_WIDTH/2 - easyDescWidth/2, btnEasy.y + btnEasy.height + 10, 12, COLOR_CYBER_GREEN_DIM);
    
    // Hard Mode Button
    Rectangle btnHard = {SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 + 60, 320, 60};
    bool hoverHard = CheckCollisionPointRec(mousePos, btnHard);
    
    Color hardBorderColor = hoverHard ? COLOR_DANGER_RED : COLOR_ALERT_YELLOW;
    DrawRectangle(btnHard.x, btnHard.y, btnHard.width, btnHard.height, ColorAlpha(COLOR_DANGER_RED, hoverHard ? 0.15f : 0.05f));
    DrawRectangleLines(btnHard.x, btnHard.y, btnHard.width, btnHard.height, hardBorderColor);
    DrawRectangleLines(btnHard.x + 2, btnHard.y + 2, btnHard.width - 4, btnHard.height - 4, ColorAlpha(hardBorderColor, 0.5f));
    
    const char* hardLabel = ">> MODO DIFICIL";
    int hardLabelWidth = MeasureText(hardLabel, 24);
    DrawText(hardLabel, SCREEN_WIDTH/2 - hardLabelWidth/2, btnHard.y + 18, 24, COLOR_ALERT_YELLOW);
    
    // Hard mode description
    const char* hardDesc = "Questoes de logica avancadas | 2 minutos";
    int hardDescWidth = MeasureText(hardDesc, 12);
    DrawText(hardDesc, SCREEN_WIDTH/2 - hardDescWidth/2, btnHard.y + btnHard.height + 10, 12, COLOR_CYBER_GREEN_DIM);
    
    // Instructions
    DrawText("PRESSIONE ESC PARA VOLTAR", SCREEN_WIDTH/2 - 130, SCREEN_HEIGHT - 50, 14, COLOR_CYBER_GREEN_DIM);
    
    // Fullscreen hint
    DrawText("[F11] TELA CHEIA", SCREEN_WIDTH - 150, SCREEN_HEIGHT - 30, 12, COLOR_CYBER_GREEN_DIM);
}

//------------------------------------------------------------------------------------
// Game Screen
//------------------------------------------------------------------------------------
void Screen_UpdateGame(void)
{
    GameState* gs = ctx.gameState;
    
    if (gs->gameOver) return;
    
    // Update game timer
    gs->gameTimer -= GetFrameTime();
    if (gs->gameTimer <= 0.0f)
    {
        gs->gameTimer = 0.0f;
        gs->timedOut = true;
        gs->gameOver = true;
        gs->won = false;
        
        Stats_AddSession(ctx.stats, gs->attempts, false);
        DetailedSession detailedSession = Game_CreateDetailedSession(gs);
        SessionLogger_Append(&detailedSession);
        
        InitGlitchEffect(false);
        *ctx.currentScreen = SCREEN_RESULT;
        return;
    }
    
    // Dynamic music switching
    if (!gs->gameOver)
    {
        float elapsedTime = gs->maxGameTime - gs->gameTimer;
        
        if ((elapsedTime >= 60.0f || gs->attempts >= 5) && MusicManager_GetCurrentTrack() != 3)
        {
            MusicManager_SwitchToGame2();
        }
        else if (MusicManager_GetCurrentTrack() != 2 && MusicManager_GetCurrentTrack() != 3)
        {
            MusicManager_SwitchToGame1();
        }
    }
    
    // Update error timer
    if (gs->errorTimer > 0.0f)
    {
        gs->errorTimer -= GetFrameTime();
    }
    
    // Handle question feedback timer
    if (gs->questionState == QUESTION_ANSWERED_CORRECT || 
        gs->questionState == QUESTION_ANSWERED_WRONG)
    {
        gs->questionFeedbackTimer -= GetFrameTime();
        if (gs->questionFeedbackTimer <= 0.0f)
        {
            gs->questionState = QUESTION_IDLE;
        }
        return;
    }
    
    // Handle question showing state
    if (gs->questionState == QUESTION_SHOWING)
    {
        if (*ctx.currentQuestion == NULL && ctx.questionBank->loaded)
        {
            *ctx.currentQuestion = QuestionBank_GetRandom(ctx.questionBank);
        }
        
        if (!ctx.questionBank->loaded)
        {
            gs->hintUnlocked = true;
            gs->questionState = QUESTION_IDLE;
            return;
        }
        
        // Handle keyboard input (1-4 keys)
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1))
        {
            Game_AnswerQuestion(gs, 0, *ctx.currentQuestion);
            *ctx.currentQuestion = NULL;
        }
        else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2))
        {
            Game_AnswerQuestion(gs, 1, *ctx.currentQuestion);
            *ctx.currentQuestion = NULL;
        }
        else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3))
        {
            Game_AnswerQuestion(gs, 2, *ctx.currentQuestion);
            *ctx.currentQuestion = NULL;
        }
        else if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4))
        {
            Game_AnswerQuestion(gs, 3, *ctx.currentQuestion);
            *ctx.currentQuestion = NULL;
        }
        
        // Handle mouse clicks on option buttons
        Vector2 mousePos = ctx.getVirtualMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && *ctx.currentQuestion != NULL)
        {
            int overlayX = SCREEN_WIDTH/2 - 350;
            int overlayY = SCREEN_HEIGHT/2 - 200;
            
            for (int i = 0; i < 4; i++)
            {
                Rectangle optBtn = {overlayX + 30, overlayY + 120 + i * 60, 640, 45};
                if (CheckCollisionPointRec(mousePos, optBtn))
                {
                    Game_AnswerQuestion(gs, i, *ctx.currentQuestion);
                    *ctx.currentQuestion = NULL;
                    break;
                }
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE))
        {
            *ctx.currentQuestion = NULL;
            *ctx.currentScreen = SCREEN_MAIN_MENU;
        }
        return;
    }
    
    // QUESTION_IDLE: Normal game input
    int key = GetCharPressed();
    while (key > 0)
    {
        if ((key >= '0' && key <= '9') && strlen(gs->inputBuffer) < MAX_INPUT_LENGTH)
        {
            int len = strlen(gs->inputBuffer);
            gs->inputBuffer[len] = (char)key;
            gs->inputBuffer[len + 1] = '\0';
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        int len = strlen(gs->inputBuffer);
        if (len > 0)
        {
            gs->inputBuffer[len - 1] = '\0';
        }
    }
    
    if (IsKeyPressed(KEY_ENTER) && strlen(gs->inputBuffer) > 0)
    {
        int guess = atoi(gs->inputBuffer);
        Game_ProcessGuess(gs, ctx.stats, guess, ctx.currentScreen);
        gs->inputBuffer[0] = '\0';
    }
    
    if (IsKeyPressed(KEY_ESCAPE))
    {
        *ctx.currentScreen = SCREEN_MAIN_MENU;
    }
}

void Screen_DrawGame(void)
{
    GameState* gs = ctx.gameState;
    float gp = *ctx.glowPulse;
    
    DrawMatrixEffect();
    
    // Header
    UI_DrawGlowText("GH0ST: 2047", 50, 30, 32, COLOR_CYBER_GREEN, gp);
    
    // Alert status
    Color alertColor = Game_GetAlertColor(gs->alertLevel);
    const char* alertText = Game_GetAlertText(gs->alertLevel);
    int alertWidth = MeasureText(alertText, 20);
    DrawRectangle(SCREEN_WIDTH - alertWidth - 80, 30, alertWidth + 60, 40, ColorAlpha(alertColor, 0.1f));
    DrawRectangleLines(SCREEN_WIDTH - alertWidth - 80, 30, alertWidth + 60, 40, alertColor);
    DrawText(alertText, SCREEN_WIDTH - alertWidth - 50, 42, 20, alertColor);
    
    // Timer display
    int timerMinutes = (int)gs->gameTimer / 60;
    int timerSeconds = (int)gs->gameTimer % 60;
    char timerText[16];
    sprintf(timerText, "TEMPO: %02d:%02d", timerMinutes, timerSeconds);
    
    float timeRatio = gs->gameTimer / gs->maxGameTime;
    Color timerColor;
    if (timeRatio > 0.5f) timerColor = COLOR_CYBER_GREEN;
    else if (timeRatio > 0.25f) timerColor = COLOR_ALERT_YELLOW;
    else {
        float pulse = (sinf(gp * 8.0f) + 1.0f) * 0.5f;
        timerColor = ColorAlpha(COLOR_DANGER_RED, 0.5f + pulse * 0.5f);
    }
    
    int timerWidth = MeasureText(timerText, 20);
    DrawRectangle(SCREEN_WIDTH/2 - timerWidth/2 - 20, 30, timerWidth + 40, 40, ColorAlpha(timerColor, 0.1f));
    DrawRectangleLines(SCREEN_WIDTH/2 - timerWidth/2 - 20, 30, timerWidth + 40, 40, timerColor);
    DrawText(timerText, SCREEN_WIDTH/2 - timerWidth/2, 42, 20, timerColor);
    
    // Attempts counter
    char attemptsText[32];
    sprintf(attemptsText, "TENTATIVAS: %d/%d", gs->attempts, gs->maxAttempts);
    DrawText(attemptsText, 50, 90, 20, COLOR_CYBER_GREEN);
    
    // Range info
    char rangeText[64];
    sprintf(rangeText, "RANGE: %d - %d", gs->minRange, gs->maxRange);
    DrawText(rangeText, 50, 120, 20, COLOR_CYBER_GREEN_DARK);
    
    // Suggested guess (binary search)
    int suggested = (gs->minRange + gs->maxRange) / 2;
    char suggestedText[64];
    sprintf(suggestedText, "SUGESTAO IA: %d", suggested);
    DrawText(suggestedText, 50, 150, 16, COLOR_CYBER_GREEN_DIM);
    
    // Input box
    int inputBoxY = SCREEN_HEIGHT/2 - 50;
    DrawRectangle(SCREEN_WIDTH/2 - 200, inputBoxY, 400, 80, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 200, inputBoxY, 400, 80, COLOR_CYBER_GREEN);
    
    DrawText("DIGITE O NUMERO:", SCREEN_WIDTH/2 - 180, inputBoxY + 15, 16, COLOR_CYBER_GREEN_DARK);
    
    char displayText[16];
    sprintf(displayText, "%s_", gs->inputBuffer);
    int textWidth = MeasureText(displayText, 32);
    DrawText(displayText, SCREEN_WIDTH/2 - textWidth/2, inputBoxY + 40, 32, COLOR_CYBER_GREEN);
    
    // Error message
    if (gs->errorTimer > 0.0f)
    {
        int errorWidth = MeasureText(gs->errorMsg, 16);
        DrawText(gs->errorMsg, SCREEN_WIDTH/2 - errorWidth/2, inputBoxY + 90, 16, COLOR_DANGER_RED);
    }
    
    // History panel
    UI_DrawAlertBox(50, SCREEN_HEIGHT - 320, 400, 280);
    
    DrawText("HISTORICO DE TENTATIVAS:", 70, SCREEN_HEIGHT - 300, 16, COLOR_CYBER_GREEN);
    
    for (int i = 0; i < gs->attempts && i < MAX_HISTORY; i++)
    {
        char historyText[64];
        sprintf(historyText, "%d. VALOR: %d -> %s", 
                i + 1, 
                gs->history[i].value, 
                gs->history[i].feedback);
        
        Color historyColor = (strcmp(gs->history[i].feedback, "ACERTO!") == 0) 
                             ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK;
        
        DrawText(historyText, 70, SCREEN_HEIGHT - 270 + i * 30, 14, historyColor);
    }
    
    // Alert legend
    UI_DrawAlertBox(SCREEN_WIDTH - 450, SCREEN_HEIGHT - 320, 400, 280);
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
    if (gs->questionState == QUESTION_IDLE)
    {
        DrawText("PRESSIONE ENTER PARA CONFIRMAR | ESC PARA SAIR", 
                 SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT - 30, 14, COLOR_CYBER_GREEN_DIM);
    }
    
    // Question overlay
    if (gs->questionState == QUESTION_SHOWING && *ctx.currentQuestion != NULL)
    {
        LogicQuestion* q = *ctx.currentQuestion;
        
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.75f));
        
        int overlayX = SCREEN_WIDTH/2 - 350;
        int overlayY = SCREEN_HEIGHT/2 - 200;
        int overlayW = 700;
        int overlayH = 400;
        
        DrawRectangle(overlayX, overlayY, overlayW, overlayH, COLOR_BG_BLACK);
        DrawRectangleLines(overlayX, overlayY, overlayW, overlayH, COLOR_CYBER_GREEN);
        DrawRectangleLines(overlayX + 2, overlayY + 2, overlayW - 4, overlayH - 4, COLOR_CYBER_GREEN_DIM);
        
        DrawText(">> DESAFIO LOGICO <<", overlayX + 30, overlayY + 20, 20, COLOR_CYBER_GREEN);
        DrawText("Responda para desbloquear a dica:", overlayX + 30, overlayY + 50, 14, COLOR_CYBER_GREEN_DARK);
        
        DrawText(q->question, overlayX + 30, overlayY + 85, 18, COLOR_CYBER_GREEN);
        
        Vector2 mousePos = ctx.getVirtualMousePosition();
        const char* labels[] = {"[1]", "[2]", "[3]", "[4]"};
        
        for (int i = 0; i < 4; i++)
        {
            Rectangle optBtn = {overlayX + 30, overlayY + 120 + i * 60, 640, 45};
            bool hover = CheckCollisionPointRec(mousePos, optBtn);
            
            Color btnBg = hover ? ColorAlpha(COLOR_CYBER_GREEN, 0.15f) : ColorAlpha(COLOR_CYBER_GREEN, 0.05f);
            Color btnBorder = hover ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DIM;
            
            DrawRectangle(optBtn.x, optBtn.y, optBtn.width, optBtn.height, btnBg);
            DrawRectangleLines(optBtn.x, optBtn.y, optBtn.width, optBtn.height, btnBorder);
            
            char optText[160];
            sprintf(optText, "%s %s", labels[i], q->options[i]);
            DrawText(optText, optBtn.x + 15, optBtn.y + 14, 16, hover ? COLOR_CYBER_GREEN : COLOR_CYBER_GREEN_DARK);
        }
        
        DrawText("USE TECLAS 1-4 OU CLIQUE NA OPCAO", overlayX + 30, overlayY + overlayH - 30, 12, COLOR_CYBER_GREEN_DIM);
    }
    
    // Question feedback overlay
    if (gs->questionState == QUESTION_ANSWERED_CORRECT)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));
        const char* msg = ">> DICA DESBLOQUEADA <<";
        int msgW = MeasureText(msg, 36);
        float alpha = (sinf(gp * 6.0f) + 1.0f) * 0.3f + 0.7f;
        DrawText(msg, SCREEN_WIDTH/2 - msgW/2, SCREEN_HEIGHT/2 - 18, 36, ColorAlpha(COLOR_CYBER_GREEN, alpha));
    }
    else if (gs->questionState == QUESTION_ANSWERED_WRONG)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));
        const char* msg = ">> DICA NEGADA <<";
        int msgW = MeasureText(msg, 36);
        float alpha = (sinf(gp * 6.0f) + 1.0f) * 0.3f + 0.7f;
        DrawText(msg, SCREEN_WIDTH/2 - msgW/2, SCREEN_HEIGHT/2 - 18, 36, ColorAlpha(COLOR_DANGER_RED, alpha));
    }
}

//------------------------------------------------------------------------------------
// Result Screen
//------------------------------------------------------------------------------------
void Screen_UpdateResult(void)
{
    MusicManager_SwitchToGame2();
    
    Vector2 mousePos = ctx.getVirtualMousePosition();
    
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnPlayAgain) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_DIFFICULTY_SELECT;
    }
    
    Rectangle btnMenu = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 220, 300, 50};
    if (CheckCollisionPointRec(mousePos, btnMenu) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_MAIN_MENU;
    }
}

void Screen_DrawResult(void)
{
    GameState* gs = ctx.gameState;
    float gp = *ctx.glowPulse;
    
    DrawGlitchEffect();
    
    Vector2 mousePos = ctx.getVirtualMousePosition();
    
    Color borderColor = gs->won ? COLOR_CYBER_GREEN : COLOR_DANGER_RED;
    DrawRectangle(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, COLOR_BG_BLACK);
    DrawRectangleLines(SCREEN_WIDTH/2 - 400, SCREEN_HEIGHT/2 - 300, 800, 500, borderColor);
    DrawRectangleLines(SCREEN_WIDTH/2 - 397, SCREEN_HEIGHT/2 - 297, 794, 494, borderColor);
    
    const char* resultTitle;
    if (gs->won) {
        resultTitle = "SENHA DESBLOQUEADA";
    } else if (gs->timedOut) {
        resultTitle = "TEMPO ESGOTADO";
    } else {
        resultTitle = "SISTEMA BLOQUEADO";
    }
    
    int titleWidth = MeasureText(resultTitle, 48);
    UI_DrawGlowText(resultTitle, SCREEN_WIDTH/2 - titleWidth/2, SCREEN_HEIGHT/2 - 230, 48, borderColor, gp);
    
    DrawRectangle(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 100, 600, 100, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
    DrawRectangleLines(SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 100, 600, 100, COLOR_CYBER_GREEN_DIM);
    
    DrawText("TENTATIVAS UTILIZADAS:", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 80, 14, COLOR_CYBER_GREEN_DARK);
    
    char attemptsText[8];
    sprintf(attemptsText, "%d", gs->attempts);
    int attemptsWidth = MeasureText(attemptsText, 48);
    DrawText(attemptsText, SCREEN_WIDTH/2 - attemptsWidth/2, SCREEN_HEIGHT/2 - 50, 48, COLOR_CYBER_GREEN);
    
    if (gs->won && gs->attempts <= ctx.stats->bestScore)
    {
        const char* newRecord = "*** NOVO RECORDE ***";
        int recordWidth = MeasureText(newRecord, 20);
        float recordAlpha = (sinf(gp * 4.0f) + 1.0f) * 0.5f;
        DrawText(newRecord, SCREEN_WIDTH/2 - recordWidth/2, SCREEN_HEIGHT/2 + 20, 20, 
                 ColorAlpha(COLOR_ALERT_YELLOW, recordAlpha));
    }
    
    char targetText[32];
    sprintf(targetText, "SENHA: %d", gs->targetNumber);
    int targetWidth = MeasureText(targetText, 24);
    DrawText(targetText, SCREEN_WIDTH/2 - targetWidth/2, SCREEN_HEIGHT/2 + 60, 24, COLOR_CYBER_GREEN_DARK);
    
    Rectangle btnPlayAgain = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 150, 300, 50};
    bool hoverPlayAgain = CheckCollisionPointRec(mousePos, btnPlayAgain);
    UI_DrawCyberButton(">> TENTAR NOVAMENTE", btnPlayAgain.x, btnPlayAgain.y, 
                    btnPlayAgain.width, btnPlayAgain.height, hoverPlayAgain);
    
    Rectangle btnMenu = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 220, 300, 50};
    bool hoverMenu = CheckCollisionPointRec(mousePos, btnMenu);
    UI_DrawCyberButton(">> MENU PRINCIPAL", btnMenu.x, btnMenu.y, btnMenu.width, btnMenu.height, hoverMenu);
}

//------------------------------------------------------------------------------------
// Statistics Screen
//------------------------------------------------------------------------------------
void Screen_UpdateStats(void)
{
    if (IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *ctx.currentScreen = SCREEN_MAIN_MENU;
    }
}

void Screen_DrawStats(void)
{
    float gp = *ctx.glowPulse;
    
    DrawMatrixEffect();
    
    UI_DrawGlowText("ANALISE ESTATISTICA", SCREEN_WIDTH/2 - 260, 50, 48, COLOR_CYBER_GREEN, gp);
    
    char sessionsText[64];
    sprintf(sessionsText, "PROCESSANDO DADOS DE %d SESSOES...", ctx.stats->sessionCount);
    int sessionsWidth = MeasureText(sessionsText, 14);
    DrawText(sessionsText, SCREEN_WIDTH/2 - sessionsWidth/2, 120, 14, COLOR_CYBER_GREEN_DARK);
    
    if (ctx.stats->sessionCount == 0)
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
        DetailedSession* detailedSessions = (DetailedSession*)calloc(MAX_SESSIONS, sizeof(DetailedSession));
        if (!detailedSessions) return;
        int detailedCount = 0;
        bool hasDetailedData = SessionLogger_Load(detailedSessions, &detailedCount, MAX_SESSIONS);

        if (!hasDetailedData || detailedCount == 0)
        {
            detailedCount = ctx.stats->sessionCount;
            for (int i = 0; i < detailedCount; i++)
            {
                strncpy(detailedSessions[i].timestamp, ctx.stats->sessions[i].date,
                        sizeof(detailedSessions[i].timestamp) - 1);
                detailedSessions[i].totalAttempts = ctx.stats->sessions[i].attempts;
                detailedSessions[i].won = ctx.stats->sessions[i].won;
            }
        }

        int totalAttempts = Analytics_RecursiveSum(detailedSessions, detailedCount, 0);
        float meanAttempts = detailedCount > 0 ? (float)totalAttempts / detailedCount : 0.0f;
        int bestAttempts = Analytics_RecursiveMin(detailedSessions, detailedCount, 0);
        int worstAttempts = Analytics_RecursiveMax(detailedSessions, detailedCount, 0);
        float variance = detailedCount > 0 ?
            Analytics_RecursiveSumSquares(detailedSessions, detailedCount, meanAttempts, 0) / detailedCount : 0.0f;
        float standardDeviation = sqrtf(variance);

        int bestIndex = 0;
        int worstIndex = 0;
        int totalLow = 0;
        int totalHigh = 0;
        int extremeStarts = 0;
        int monotonicLong = 0;
        int repetitivePatterns = 0;
        float binarySimilarityTotal = 0.0f;

        for (int i = 0; i < detailedCount; i++)
        {
            if (detailedSessions[i].totalAttempts == bestAttempts) bestIndex = i;
            if (detailedSessions[i].totalAttempts == worstAttempts) worstIndex = i;

            totalLow += detailedSessions[i].lowBiasCount;
            totalHigh += detailedSessions[i].highBiasCount;

            if (detailedSessions[i].totalAttempts > 0)
            {
                int firstGuess = detailedSessions[i].guesses[0];
                if (firstGuess <= 20 || firstGuess >= 80) extremeStarts++;
            }

            if (Analytics_LongestMonotonicRun(&detailedSessions[i]) >= 4) monotonicLong++;
            if (Analytics_HasRepetitivePattern(&detailedSessions[i])) repetitivePatterns++;
            binarySimilarityTotal += Analytics_BinarySimilarity(&detailedSessions[i]);
        }

        float avgLow = detailedCount > 0 ? (float)totalLow / detailedCount : 0.0f;
        float avgHigh = detailedCount > 0 ? (float)totalHigh / detailedCount : 0.0f;
        float bias = avgLow - avgHigh;
        float extremeRate = detailedCount > 0 ? (extremeStarts * 100.0f) / detailedCount : 0.0f;
        float monotonicRate = detailedCount > 0 ? (monotonicLong * 100.0f) / detailedCount : 0.0f;
        float repetitiveRate = detailedCount > 0 ? (repetitivePatterns * 100.0f) / detailedCount : 0.0f;
        float binarySimilarity = detailedCount > 0 ? binarySimilarityTotal / detailedCount : 0.0f;

        int cardY = 160;
        int cardWidth = 260;
        int cardHeight = 95;
        int spacing = 35;
        int startX = (SCREEN_WIDTH - (cardWidth * 3 + spacing * 2)) / 2;

        DrawRectangle(startX, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(startX, cardY, cardWidth, cardHeight, COLOR_CYBER_GREEN);
        DrawText("MEDIA POR SESSAO", startX + 20, cardY + 18, 14, COLOR_CYBER_GREEN_DARK);
        char meanText[16];
        sprintf(meanText, "%.2f", meanAttempts);
        DrawText(meanText, startX + 20, cardY + 42, 34, COLOR_CYBER_GREEN);

        DrawRectangle(startX + cardWidth + spacing, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_ALERT_YELLOW, 0.05f));
        DrawRectangleLines(startX + cardWidth + spacing, cardY, cardWidth, cardHeight, COLOR_ALERT_YELLOW);
        DrawText("DESVIO PADRAO", startX + cardWidth + spacing + 20, cardY + 18, 14, COLOR_CYBER_GREEN_DARK);
        char deviationText[16];
        sprintf(deviationText, "%.2f", standardDeviation);
        DrawText(deviationText, startX + cardWidth + spacing + 20, cardY + 42, 34, COLOR_ALERT_YELLOW);

        DrawRectangle(startX + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, ColorAlpha(COLOR_DANGER_RED, 0.05f));
        DrawRectangleLines(startX + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, COLOR_DANGER_RED);
        DrawText("TAXA VITORIA", startX + (cardWidth + spacing) * 2 + 20, cardY + 18, 14, COLOR_CYBER_GREEN_DARK);
        char winRateText[16];
        sprintf(winRateText, "%.1f%%", ctx.stats->winRate);
        DrawText(winRateText, startX + (cardWidth + spacing) * 2 + 20, cardY + 42, 34, COLOR_DANGER_RED);

        int panelY = 290;
        int panelWidth = 570;
        int panelHeight = 350;
        int leftX = 90;
        int rightX = SCREEN_WIDTH - leftX - panelWidth;

        DrawRectangle(leftX, panelY, panelWidth, panelHeight, ColorAlpha(COLOR_CYBER_GREEN, 0.05f));
        DrawRectangleLines(leftX, panelY, panelWidth, panelHeight, COLOR_CYBER_GREEN_DIM);
        DrawText("METRICAS AGREGADAS", leftX + 24, panelY + 22, 20, COLOR_CYBER_GREEN);

        char line[160];
        sprintf(line, "- Sessoes analisadas: %d", detailedCount);
        DrawText(line, leftX + 24, panelY + 64, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Melhor sessao: #%d [%s] - %d tent.", bestIndex + 1,
                detailedSessions[bestIndex].timestamp, bestAttempts);
        DrawText(line, leftX + 24, panelY + 94, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Pior sessao: #%d [%s] - %d tent.", worstIndex + 1,
                detailedSessions[worstIndex].timestamp, worstAttempts);
        DrawText(line, leftX + 24, panelY + 124, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Vies medio: %.2f baixo / %.2f alto", avgLow, avgHigh);
        DrawText(line, leftX + 24, panelY + 154, 16, COLOR_CYBER_GREEN_DARK);
        DrawText("- Formula: sigma = sqrt( soma((xi - media)^2) / n )",
                 leftX + 24, panelY + 184, 16, COLOR_CYBER_GREEN_DARK);
        DrawText("- Recursao aplicada em soma, minimo, maximo e soma quadratica",
                 leftX + 24, panelY + 214, 16, COLOR_CYBER_GREEN_DARK);

        DrawRectangle(rightX, panelY, panelWidth, panelHeight, ColorAlpha(COLOR_ALERT_YELLOW, 0.05f));
        DrawRectangleLines(rightX, panelY, panelWidth, panelHeight, COLOR_ALERT_YELLOW);
        DrawText("HEURISTICAS DE ESTRATEGIA", rightX + 24, panelY + 22, 20, COLOR_ALERT_YELLOW);

        const char* biasText = "equilibrio entre baixos e altos";
        if (bias > 1.0f) biasText = "tendencia a chutar baixo";
        else if (bias < -1.0f) biasText = "tendencia a chutar alto";

        sprintf(line, "- Vies dominante: %s", biasText);
        DrawText(line, rightX + 24, panelY + 64, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Comeco muito baixo/alto: %.0f%% das sessoes", extremeRate);
        DrawText(line, rightX + 24, panelY + 94, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Sequencias monotonicas longas: %.0f%%", monotonicRate);
        DrawText(line, rightX + 24, panelY + 124, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Padrao repetitivo pouco eficiente: %.0f%%", repetitiveRate);
        DrawText(line, rightX + 24, panelY + 154, 16, COLOR_CYBER_GREEN_DARK);
        sprintf(line, "- Aproximacao de busca binaria: %.0f%%", binarySimilarity);
        DrawText(line, rightX + 24, panelY + 184, 16, COLOR_CYBER_GREEN_DARK);

        if (binarySimilarity >= 70.0f)
        {
            DrawText("- Diagnostico: estrategia consistente e proxima do ideal.",
                     rightX + 24, panelY + 228, 16, COLOR_CYBER_GREEN);
        }
        else if (monotonicRate >= 30.0f || repetitiveRate >= 30.0f)
        {
            DrawText("- Diagnostico: reduza sequencias lineares e divida melhor o intervalo.",
                     rightX + 24, panelY + 228, 16, COLOR_ALERT_YELLOW);
        }
        else
        {
            DrawText("- Diagnostico: use mais o meio do intervalo apos cada pista.",
                     rightX + 24, panelY + 228, 16, COLOR_ALERT_YELLOW);
        }
        free(detailedSessions);
    }
    
    DrawText("PRESSIONE ESC OU CLIQUE PARA VOLTAR", 
             SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT - 40, 14, COLOR_CYBER_GREEN_DIM);
}
