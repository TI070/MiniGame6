#ifndef EFFECT_H
#define EFFECT_H

#include <d3dx9.h>

#define EXPLOSION_SIZE (2.0f)

enum EFFECT_TYPE {

	ACCEL_EFFECT

};


class effect {
protected:

	int EffectTexId;
	D3DXVECTOR3 pos;
	float size;

public:
	effect();
	effect(D3DXVECTOR3 set_pos, float set_size);
	~effect();

	int life;

	void Initialize();
	virtual void Draw();
	virtual void Update();
	
};

class accel_effect:public effect{
private:

	int c_num;

public:

	accel_effect(D3DXVECTOR3 set_pos, float set_size, int chara_num);

	void Initialize();
	void Draw();
	void Update();

};

class explosion_effect :public effect {
private:

	int anime_frame;

public:

	explosion_effect(D3DXVECTOR3 set_pos, float set_size,int set_aframe);

	void Initialize();
	void Draw();
	void Update();

};

#endif