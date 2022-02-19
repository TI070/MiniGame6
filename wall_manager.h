#pragma once
#ifndef WALL_MANAGER_H
#define WALL_MANAGER_H

#include <d3dx9.h>
#include "wall.h"

#define WALL_MAX (1024)

class wall_manager {
private:
	wall_manager();
	static wall* pWall[WALL_MAX];
public:

	
	static void Initialize();
	static void Draw();
	static void Update();
	static void SetWall(D3DXVECTOR3 set_pos, float height, float width, int type);
	static void SetTransparentWall(D3DXVECTOR3 set_pos, float height, float width, int type);
	static void SetCubeWall(D3DXVECTOR3 set_pos, float height, float width);
	static void SetMoveWall(D3DXVECTOR3 set_pos, float height, float width,int type,player* pPlayer);
	static void SetPaintWall(D3DXVECTOR3 set_pos, float height, float width, int type);
	static void SetBgWall(D3DXVECTOR3 set_pos, float height, float width, int type, int kind);
	static wall* GetWall(int index);
	static void AllDestroy();
	static void Destroy(int index);
	static int CheckPaintColor(int check_color);

};
#endif