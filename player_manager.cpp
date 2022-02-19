
#include "player_manager.h"

player* player_manager::pPlayers[PLAYER_MAX];

void player_manager::Initialize() {

	for (int i = 0; i < PLAYER_MAX; i++) {

		pPlayers[i] = NULL;

	}

}

void player_manager::Draw() {

	for (int i = 0; i < PLAYER_MAX; i++) {

		if (pPlayers[i] == NULL) continue;

		pPlayers[i]->Draw();

	}

}

void player_manager::Update() {

	for (int i = 0; i < PLAYER_MAX; i++) {

		if (pPlayers[i] == NULL) continue;

		pPlayers[i]->Update();

	}

}


void player_manager::SetPlayer(int type) {

	for (int i = 0; i < PLAYER_MAX; i++) {

		if (pPlayers[i] != NULL) continue;


		switch (type)
		{
		case KUMA:
			pPlayers[i] = new player(i);
			break;
		}

		

		break;
	}

}

player* player_manager::GetPlayer(int index) {

	return pPlayers[index];
}

void player_manager::GravitySwOn(int index) {

	pPlayers[index]->gravity_sw = true;
}

void player_manager::GravitySwOff(int index) {

	pPlayers[index]->gravity_sw = false;
}