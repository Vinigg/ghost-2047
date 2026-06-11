/*
GH0ST: 2047 - Configuration Constants
All game configuration values in one place
*/

#ifndef CONFIG_H
#define CONFIG_H

//------------------------------------------------------------------------------------
// Screen Configuration
//------------------------------------------------------------------------------------
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

//------------------------------------------------------------------------------------
// Game Configuration
//------------------------------------------------------------------------------------
#define MAX_ATTEMPTS 7
#define MAX_HISTORY 7
#define MAX_INPUT_LENGTH 3

//------------------------------------------------------------------------------------
// Logic Questions Configuration
//------------------------------------------------------------------------------------
#define MAX_QUESTIONS 50
#define QUESTIONS_FILE "logic_questions.txt"
#define QUESTIONS_FILE_EASY "logic_questions.txt"
#define QUESTIONS_FILE_HARD "hard_logic_questions.txt"

//------------------------------------------------------------------------------------
// Timer Configuration
//------------------------------------------------------------------------------------
#define GAME_TIME_EASY 120.0f    // 2 minutes
#define GAME_TIME_HARD 120.0f    // 2 minutes (same for now)

//------------------------------------------------------------------------------------
// Statistics Configuration
//------------------------------------------------------------------------------------
#define MAX_SESSIONS 10000
#define HISTORY_FILE "ghost2047_history.dat"
#define SESSIONS_CSV_FILE "ghost2047_sessions.csv"
#define HISTORY_MAGIC 0x47483437 // "GH47"
#define HISTORY_VERSION 1

//------------------------------------------------------------------------------------
// Visual Effects Configuration
//------------------------------------------------------------------------------------
#define MAX_PARTICLES 50
#define MAX_LOGS 8

#endif // CONFIG_H
