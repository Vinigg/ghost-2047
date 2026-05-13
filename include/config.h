/*
GH0ST: 2047 - Configuration Constants
All game configuration values in one place
*/

#ifndef CONFIG_H
#define CONFIG_H

//------------------------------------------------------------------------------------
// Screen Configuration
//------------------------------------------------------------------------------------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//------------------------------------------------------------------------------------
// Game Configuration
//------------------------------------------------------------------------------------
#define MAX_ATTEMPTS 7
#define MAX_HISTORY 7
#define MAX_INPUT_LENGTH 3

//------------------------------------------------------------------------------------
// Statistics Configuration
//------------------------------------------------------------------------------------
#define MAX_SESSIONS 100
#define HISTORY_FILE "ghost2047_history.dat"
#define HISTORY_MAGIC 0x47483437 // "GH47"
#define HISTORY_VERSION 1

//------------------------------------------------------------------------------------
// Visual Effects Configuration
//------------------------------------------------------------------------------------
#define MAX_PARTICLES 50
#define MAX_LOGS 8

#endif // CONFIG_H
