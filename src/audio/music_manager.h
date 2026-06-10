/*
GH0ST: 2047 - Music Manager Module
Handles music loading, switching, and playback state
*/

#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "raylib.h"

//------------------------------------------------------------------------------------
// Music Manager Functions
//------------------------------------------------------------------------------------
void MusicManager_Init(void);
void MusicManager_Update(void);
void MusicManager_Unload(void);
void MusicManager_SwitchToMenu(void);
void MusicManager_SwitchToGame1(void);
void MusicManager_SwitchToGame2(void);
int MusicManager_GetCurrentTrack(void);

#endif // MUSIC_MANAGER_H
