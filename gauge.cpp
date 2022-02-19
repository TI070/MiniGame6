
#include "gauge.h"
#include "texture.h"
#include "sprite.h"

static int GaugeTexId;

void Gauge_Initialize(void) {

	GaugeTexId = Texture_SetTextureLoadFile("asset/white.png");

}


void Gauge_Draw(float posx, float posy, float size_x, float size_y, float gauge_max, float current_value) {


	//”w–Ê
	Sprite_SetColor(128,128,128,255);

	Sprite_Draw(GaugeTexId,posx,posy,size_x,size_y);


	//‘O–Ê
	Sprite_SetColor(0, 255, 0, 255);

	Sprite_Draw(GaugeTexId, posx, posy, size_x * (current_value/gauge_max), size_y);


}

void Gauge_Draw(float posx, float posy,float gauge_max, float current_value) {


	//”w–Ê
	Sprite_SetColor(128, 128, 128, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH, GAUGE_HEIGHT);


	//‘O–Ê
	Sprite_SetColor(0, 255, 0, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH * (current_value / gauge_max), GAUGE_HEIGHT);


}

void Gauge_Draw2(float posx, float posy,float gauge_max, float current_value, float charge_max, float charge_value) {


	//”w–Ê
	Sprite_SetColor(128, 128, 128, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH, GAUGE_HEIGHT);


	//‘O–Ê
	Sprite_SetColor(0, 255, 0, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH * (current_value / gauge_max), GAUGE_HEIGHT);

	//ƒ`ƒƒ[ƒW
	Sprite_SetColor(255, 140, 0, 255);

	if (charge_value > charge_max) charge_value = charge_max;

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH * (charge_value / charge_max), GAUGE_HEIGHT);

	Sprite_SetColorDefault();
}