#pragma once
#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <d3dx9.h>
#include "game_player.h"

enum PLAYER_TYPE {

	KUMA


};


#define PLAYER_MAX (4)

class player_manager {
private:
	player_manager();
	static player* pPlayers[PLAYER_MAX];
public:


	static void Initialize();
	static void Draw();
	static void Update();
	static void SetPlayer(int type);
	static player* GetPlayer(int index);
	static void GravitySwOn(int index);
	static void GravitySwOff(int index);

};
#endif