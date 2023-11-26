#pragma once

#include <coreinit/time.h>
#include <vpad/input.h>

int GameLoop(VPADStatus status, OSTick deltaTime);

void GameShutdown(void);

void ProfilerStartFrame(void);

void ProfilerLog(char *title);

void ProfilerLogTime(char *title, OSTick time);
