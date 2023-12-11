#pragma once

#include <coreinit/time.h>
#include <vpad/input.h>

// This is the main game loop. It should be called every frame.
int GameLoop(VPADStatus status, OSTick deltaTime);

// This cleans up the game's malloc-ed memory. It should be called when the game
// quits.
void GameShutdown(void);

// This resets the profiler. It should be called at the start of every frame.
void ProfilerStartFrame(void);

// This logs a profiler entry using the time since the the last call to this
// function this frame or the start of the frame.
void ProfilerLog(char *title);

// This logs a profiler entry using the given time.
void ProfilerLogTime(char *title, OSTick time);
