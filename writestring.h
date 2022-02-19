#pragma once
#ifndef WRITESTRING_H
#define WRITESTRING_H


class writestring {//staticƒNƒ‰ƒX

private:
	writestring();

	static int JapaneseFont_TexId;
	static int AlphabetFont_TexId;


public:

	static void Initialize();
	static void JapaneseFont_Draw(float dx, float dy, const char* pString);
	static void JapaneseFont_Draw(float dx, float dy, const char* pString, float sizex, float sizey);
	static void JapaneseFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey);
	static void AlphabetFont_Draw(float dx, float dy, const char* pString);
	static void AlphabetFont_Draw(float dx, float dy, const char* pString,float sizex, float sizey);
	static void AlphabetFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey);

};


#endif