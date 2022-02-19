#pragma once
#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#define COUNTDOWN_END (-1 * 60)


#define TOMENU_POSX ((SCREEN_WIDTH / 2) - (32 * 8 / 2))
#define TOMENU_POSY (400)

class player;

class game_system {
private:
	game_system();

	static char message[64];
	static bool messageIsUse;

public:

	static void Initiaize();
	static void PlayerWin(player* pP, int i);
	static void Draw();
	static void SetMessage(char mes[64]);
	static void ExplainToMenu();

	static bool StartCountdown();

	static int start_counter;
	static int win_player;

};
#endif