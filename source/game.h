int GameLoop(VPADStatus status, OSTick deltaTime);

void GameShutdown();

void ProfilerStartFrame();

void ProfilerLog(char *title);

void ProfilerLogTime(char *title, OSTick time);
