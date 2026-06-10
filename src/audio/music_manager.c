/*
GH0ST: 2047 - Music Manager Module
Handles music loading, switching, and playback state
*/

#include "audio/music_manager.h"
#include <stddef.h>

//------------------------------------------------------------------------------------
// Module State
//------------------------------------------------------------------------------------
static Music musicMenu = {0};
static Music musicGame1 = {0};
static Music musicGame2 = {0};
static Music* currentMusic = NULL;
static int currentMusicTrack = 0;  // 0=none, 1=menu, 2=game1, 3=game2

//------------------------------------------------------------------------------------
// Internal Helper
//------------------------------------------------------------------------------------
static void SwitchMusic(Music* newMusic, int trackNumber)
{
    if (currentMusic != NULL && currentMusic != newMusic)
    {
        StopMusicStream(*currentMusic);
    }
    
    if (newMusic != NULL && newMusic->stream.buffer != NULL)
    {
        PlayMusicStream(*newMusic);
        currentMusic = newMusic;
        currentMusicTrack = trackNumber;
    }
}

//------------------------------------------------------------------------------------
// Public Functions
//------------------------------------------------------------------------------------
void MusicManager_Init(void)
{
    musicMenu = LoadMusicStream("menu_music.mp3");
    musicGame1 = LoadMusicStream("game_music_1.mp3");
    musicGame2 = LoadMusicStream("game_music_2.mp3");
    
    if (musicMenu.stream.buffer != NULL) musicMenu.looping = true;
    if (musicGame1.stream.buffer != NULL) musicGame1.looping = true;
    if (musicGame2.stream.buffer != NULL) musicGame2.looping = true;
    
    // Start menu music
    if (musicMenu.stream.buffer != NULL)
    {
        PlayMusicStream(musicMenu);
        currentMusic = &musicMenu;
        currentMusicTrack = 1;
    }
}

void MusicManager_Update(void)
{
    if (currentMusic != NULL)
    {
        UpdateMusicStream(*currentMusic);
    }
}

void MusicManager_Unload(void)
{
    if (currentMusic != NULL) StopMusicStream(*currentMusic);
    UnloadMusicStream(musicMenu);
    UnloadMusicStream(musicGame1);
    UnloadMusicStream(musicGame2);
}

void MusicManager_SwitchToMenu(void)
{
    if (currentMusicTrack != 1)
    {
        SwitchMusic(&musicMenu, 1);
    }
}

void MusicManager_SwitchToGame1(void)
{
    if (currentMusicTrack != 2 && currentMusicTrack != 3)
    {
        SwitchMusic(&musicGame1, 2);
    }
}

void MusicManager_SwitchToGame2(void)
{
    if (currentMusicTrack != 3)
    {
        SwitchMusic(&musicGame2, 3);
    }
}

int MusicManager_GetCurrentTrack(void)
{
    return currentMusicTrack;
}
