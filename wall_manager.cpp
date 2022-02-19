
#include "wall_manager.h"
#include <stdio.h>

wall* wall_manager::pWall[WALL_MAX];

void wall_manager::Initialize() {

	for (int i = 0; i < WALL_MAX; i++) {

		pWall[i] = NULL;

	}
}

void wall_manager::Draw() {

	for (int i = 0; i < WALL_MAX;i++) {

		if (pWall[i] == NULL) continue;

		pWall[i]->Draw();

	}
}

void wall_manager::Update() {

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] == NULL) continue;

		pWall[i]->Update();

	}
}

void wall_manager::SetWall(D3DXVECTOR3 set_pos, float height, float width, int type) {

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] != NULL) continue;

		pWall[i] = new wall(set_pos,height,width,type);

		break;
	}

}

void wall_manager::SetTransparentWall(D3DXVECTOR3 set_pos, float height, float width, int type) {

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] != NULL) continue;

		pWall[i] = new transparent_wall(set_pos, height, width, type);

		break;
	}

}

void wall_manager::SetCubeWall(D3DXVECTOR3 set_pos, float height, float width) {

	int count = 0;
	D3DXVECTOR3 pos;

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] != NULL) continue;

		if (count == 0) {//°
			pWall[i] = new wall(set_pos, height, width, XZ_FIELD);
		}
		else if (count == 1) {//¶–Ê
			pos = set_pos;
			pos.x -= width / 2;
			pos.y += height / 2;
			pWall[i] = new wall(pos, height, width, YZ_FIELD);
		}
		else if (count == 2) {//‰E–Ê
			pos = set_pos;
			pos.x += width / 2;
			pos.y += height / 2;
			pWall[i] = new wall(pos, height, width, YZ_FIELD);
		}
		else if (count == 3) {//‰œ–Ê
			pos = set_pos;
			pos.y += height / 2;
			pos.z += width / 2;
			pWall[i] = new wall(pos, height, width, XY_FIELD);
		}
		else if (count == 4) {//Žè‘O–Ê
			pos = set_pos;
			pos.y += height / 2;
			pos.z -= width / 2;
			pWall[i] = new wall(pos, height, width, XY_FIELD);
		}
		else if (count == 5) {//ã–Ê
			pos = set_pos;
			pos.y += height;
			pWall[i] = new wall(pos, height, width, XZ_FIELD);
		}


		count++;

		if(count >= 6) break;
	}

}

void wall_manager::SetMoveWall(D3DXVECTOR3 set_pos, float height, float width, int type,player* pPlayer) {

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] != NULL) continue;

		pWall[i] = new move_wall(set_pos, height, width, type,pPlayer);

		break;
	}

}

void wall_manager::SetPaintWall(D3DXVECTOR3 set_pos, float height, float width, int type) {

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] != NULL) continue;

		pWall[i] = new paint_wall(set_pos, height, width, type);

		break;
	}

}

void wall_manager::SetBgWall(D3DXVECTOR3 set_pos, float height, float width, int type,int kind) {

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] != NULL) continue;

		switch (kind)
		{
		case GRASS:
			pWall[i] = new grass_wall(set_pos, height, width, type);

			break;

		case SKY:
			pWall[i] = new sky_wall(set_pos, height, width, type);

			break;

		case CLOUD:
			pWall[i] = new cloud_wall(set_pos, height, width, type);

			break;


		}


		break;
	}

}


wall* wall_manager::GetWall(int index) {

	return pWall[index];
}

void wall_manager::AllDestroy() {

	for (int i = 0; i < WALL_MAX;i++) {

		Destroy(i);

	}

}

void wall_manager::Destroy(int index) {

	delete pWall[index];
	pWall[index] = NULL;

}

int wall_manager::CheckPaintColor(int check_color) {

	int cnt = 0;

	for (int i = 0; i < WALL_MAX; i++) {

		if (pWall[i] == NULL) continue;

		if (pWall[i]->GetWallType() == PAINT_WALL) {

			if (pWall[i]->paint_type == check_color) cnt++;

		}

	}

	return cnt;

}