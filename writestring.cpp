#include "writestring.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "camera.h"

#define FONT_WIDTH (64)
#define FONT_HEIGHT (64)

#define FONT_H_NUM (16) //横の文字数

int writestring::JapaneseFont_TexId;
int writestring::AlphabetFont_TexId;

void writestring::Initialize() {

	JapaneseFont_TexId = Texture_SetTextureLoadFile("asset/font/japanese.png");
	AlphabetFont_TexId = Texture_SetTextureLoadFile("asset/font/alphabet.png");


	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}


}

void writestring::JapaneseFont_Draw(float dx, float dy, const char* pString) {


	for (int i = 0; i < strlen(pString); i = i + 2) {

		unsigned int dummy_int;
		char dummy_string[3] = "ぁ";
		char dummy_string2[3] = "　";
		char dummy_string3[3] = "ー";
		char dummy_string4[3] = "ェ";

		//例外処理
		if (pString[i] * 10 + pString[i + 1] == dummy_string4[0] * 10 + dummy_string4[1]) {
			Sprite_DebugFontDraw(JapaneseFont_TexId, dx + i / 2 * FONT_WIDTH, dy, 64, 64, 64 * 8, 64 * 6, FONT_WIDTH, FONT_HEIGHT);

			continue;
		}
		if (pString[i] == ' ') {
			i--;
			continue;
		}
		if (pString[i] * 10 + pString[i + 1] == dummy_string2[0] * 10 + dummy_string2[1]) {

			continue;
		}
		if (pString[i] * 10 + pString[i + 1] == dummy_string3[0] * 10 + dummy_string3[1]) {

			Sprite_DebugFontDraw(JapaneseFont_TexId, dx + i / 2 * FONT_WIDTH, dy, 64, 64, 64, 0, FONT_WIDTH, FONT_HEIGHT);

			continue;
		}


		//いったん数値に直す
		dummy_int = pString[i] * 0 + pString[i + 1];
		dummy_int -= (dummy_string[0] * 0 + dummy_string[1]);
		dummy_int += 1;

		if(dummy_int > 160) dummy_int -= 65;
		if (dummy_int > 224) dummy_int -= 1;

		int tcx = (dummy_int % FONT_H_NUM) * FONT_WIDTH;
		int tcy = (dummy_int / FONT_H_NUM) * FONT_HEIGHT;

		Sprite_DebugFontDraw(JapaneseFont_TexId, dx + i/2 * FONT_WIDTH, dy, 64, 64, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

void writestring::JapaneseFont_Draw(float dx, float dy, const char* pString, float sizex, float sizey) {

	for (int i = 0; i < strlen(pString); i = i + 2) {

		unsigned int dummy_int;
		char dummy_string[3] = "ぁ";
		char dummy_string2[3] = "　";
		char dummy_string3[3] = "ー";
		char dummy_string4[3] = "ェ";

		//例外処理
		if (pString[i] * 10 + pString[i + 1] == dummy_string4[0] * 10 + dummy_string4[1]) {
			Sprite_DebugFontDraw(JapaneseFont_TexId, dx + i / 2 * sizex, dy, sizex, sizey, 64 * 8, 64 * 6, FONT_WIDTH, FONT_HEIGHT);

			continue;
		}
		if (pString[i] == ' ') {
			i--;
			continue;
		}

		if (pString[i] * 10 + pString[i + 1] == dummy_string2[0] * 10 + dummy_string2[1]) {
			
			continue;
		}
		if (pString[i] * 10 + pString[i + 1] == dummy_string3[0] * 10 + dummy_string3[1]) {

			Sprite_DebugFontDraw(JapaneseFont_TexId, dx + i / 2 * sizex, dy, sizex, sizey, 64, 0, FONT_WIDTH, FONT_HEIGHT);

			continue;
		}


		//いったん数値に直す
		dummy_int = pString[i] * 0 + pString[i + 1];
		dummy_int -= (dummy_string[0] * 0 + dummy_string[1]);
		dummy_int += 1;

		if (dummy_int > 160) dummy_int -= 65;
		if (dummy_int > 224) dummy_int -= 1;
		//if (dummy_int > 200) dummy_int -= 321;

		int tcx = (dummy_int % FONT_H_NUM) * FONT_WIDTH;
		int tcy = (dummy_int / FONT_H_NUM) * FONT_HEIGHT;

		Sprite_DebugFontDraw(JapaneseFont_TexId, dx + i / 2 * sizex, dy, sizex, sizey, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

//void writestring::JapaneseFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey) {
//
//	dx += camera::GetCameraPos().x;
//	dy += camera::GetCameraPos().y;
//
//	JapaneseFont_Draw(dx, dy, pString, sizex, sizey);
//
//}
//
void writestring::AlphabetFont_Draw(float dx, float dy, const char* pString) {


	for (int i = 0; i < strlen(pString); i++) {


		int tcx = ((pString[i] % 16) % FONT_H_NUM) * FONT_WIDTH;
		int tcy = ((pString[i] - ' ') / FONT_H_NUM) * FONT_HEIGHT;

		Sprite_DebugFontDraw(AlphabetFont_TexId, dx + i * FONT_WIDTH, dy, 64, 64, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

void writestring::AlphabetFont_Draw(float dx, float dy, const char* pString, float sizex, float sizey) {


	for (int i = 0; i < strlen(pString); i++) {


		if (pString[i] == ' ') continue;

		int tcx = ((pString[i] % 16) % FONT_H_NUM) * FONT_WIDTH;
		int tcy = ((pString[i] - ' ') / FONT_H_NUM) * FONT_HEIGHT;

		Sprite_DebugFontDraw(AlphabetFont_TexId, dx + i * sizex, dy, sizex, sizey, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

//void writestring::AlphabetFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey) {
//
//	dx += camera::GetCameraPos().x;
//	dy += camera::GetCameraPos().y;
//
//	AlphabetFont_Draw(dx, dy, pString, sizex, sizey);
//
//}