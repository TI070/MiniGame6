#pragma once
#ifndef WALL_H
#define WALL_H

#include <d3dx9.h>
#include "meshfield.h"

enum WALL_KIND {

	NORMAL_WALL,
	TRANSPARENT_WALL,
	MOVE_WALL,
	PAINT_WALL,
	BACKGROUND_WALL
};

enum PAINTCOLOR_TYPE {

	NONE_PAINT,
	PLAYER1_PAINTCOLOR,
	PLAYER2_PAINTCOLOR,
	PLAYER3_PAINTCOLOR,
	PLAYER4_PAINTCOLOR

};

enum BG_WALLKIND {

	GRASS,
	SKY,
	CLOUD
};


class player;

class wall {

protected:
	int WallTexId;
	D3DXVECTOR3 pos;

	meshfield* pMeshField;

public:
	wall();
	wall(D3DXVECTOR3 set_pos,float height,float width,int type);

	int walltype;
	float height;
	float width;

	void Initialize();
	virtual void Draw();
	virtual void Update();
	D3DXVECTOR3 GetPos();
	virtual int GetWallType();
	virtual void Action(player* pP);

	int paint_type;
};

class transparent_wall:public wall{
private:

	
	
public:

	transparent_wall(D3DXVECTOR3 set_pos, int height, int width, int type);
	void Draw();
	int GetWallType();
};

class move_wall :public wall {
private:

	player* pPlayer;

	float posx_max;
	float posz_max;


public:

	move_wall(D3DXVECTOR3 set_pos, int height, int width, int type,player* pP);
	//void Draw();
	void Update();
	void RightMove();
	void LeftMove();
	int GetWallType();
};


class paint_wall :public wall {
private:

	
	int PaintWallTexId[5];


public:

	paint_wall(D3DXVECTOR3 set_pos, int height, int width, int type);
	void Draw();
	int GetWallType();
	void Action(player* pP);
};


class grass_wall :public wall {
private:

public:

	grass_wall(D3DXVECTOR3 set_pos, int height, int width, int type);
	int GetWallType();

};

class sky_wall :public wall {
private:

public:

	sky_wall(D3DXVECTOR3 set_pos, int height, int width, int type);
	void Draw();
	int GetWallType();

};

class cloud_wall :public wall {
private:

public:

	cloud_wall(D3DXVECTOR3 set_pos, int height, int width, int type);
	void Draw();
	void Update();
	int GetWallType();

};


#endif