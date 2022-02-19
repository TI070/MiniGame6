//ライブラリインクルード(ソースコードから)
/*
#pragma comment(lib,"d3d9.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
*/

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h> //算術系処理をするときに使用
#include "config.h"
#include "mydirect3d.h"
#include "game.h"
#include "debug_font.h"
#include "system_timer.h"
#include <stdio.h>
#include "keyboard.h"
#include "key_manager.h"
#include "scene_manager.h"
#include "sound.h"
#include "debug_primitive.h"


////////////
//定数定義
////////////

#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION "minigame6"



///////////
//プロトタイプ宣言
///////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//ゲームの初期化 戻り値:初期化に失敗したときfalse
static bool Initialize(HWND hWnd);
//ゲームの更新
static int Update();
//ゲームの描画
static void Draw();
//ゲームの終了処理
static void Finalize();

///////////
//グローバル変数宣言
///////////
LPDIRECT3DDEVICE9 g_pDevice;
static int g_FrameCount = 0;
static int g_BaseFrame = 0;
static double g_BaseTime = 0;
static double g_FPS = 0.0;
static double g_ReserveTime = 0.0;


/////////
//構造体
/////////






static LPDIRECT3DTEXTURE9 g_pTexture = NULL;//テクスチャインターフェース テクスチャ１つにつき１つ必要
static LPDIRECT3DTEXTURE9 g_pTexture2 = NULL;//テクスチャインターフェース テクスチャ１つにつき１つ必要
static LPDIRECT3DTEXTURE9 g_pTexture3[6] = {NULL,NULL,NULL,NULL,NULL,NULL};//テクスチャインターフェース テクスチャ１つにつき１つ必要


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//ウインドウ構造体の設定
	WNDCLASS wc = {};//中身にごみが残った状態で宣言しないように、空(0)にしよう
	wc.lpfnWndProc = WndProc; //ウインドウプロシージャの登録
	wc.lpszClassName = CLASS_NAME;//クラス名の設定
	wc.hInstance = hInstance;//インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//マウスカーソルの指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);//ウインドウの背景色を設定 (COLOR_BACKGROUNDは+1した値を入れてください(謎)

	RegisterClass(&wc);//システムにウインドウ構造体を登録

	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	//都合のいいウインドウサイズの算出
	RECT window_rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	AdjustWindowRect(&window_rect,style,FALSE);
	//ウインドウ枠分膨張する
	LONG ex_left = 0 - window_rect.left;
	LONG ex_top = 0 - window_rect.top;
	LONG ex_right = (window_rect.right - window_rect.left) - SCREEN_WIDTH - ex_left;
	LONG ex_bottom = (window_rect.bottom - window_rect.top) - SCREEN_HEIGHT - ex_top;

	RECT window_rect2 = { 0 + ex_left,0 + ex_top,SCREEN_WIDTH - ex_right,SCREEN_HEIGHT - ex_bottom};
	AdjustWindowRect(&window_rect2, WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX), FALSE);

	//表示するウインドウの場所を算出する
	//デスクトップ(クライアント領域)の取得

	//APIを利用したデスクトップの幅、高さ取得
	/*
	RECT desktop_rect;
	GetWindowRect(GetDesktopWindow(),&desktop_rect);
	*/

	//デバイス(システム)からモニター解像度を取得
	//プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//仮想画面の解像度取得
	int virtual_desktop_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int virtual_desktop_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	//仮想画面の解像度取得
	int virtual_desktop_x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int virtual_desktop_y = GetSystemMetrics(SM_YVIRTUALSCREEN);

	//max 値の大きい引数を返す
	int tes = max(-500,0);

	////hWnd ウインドウハンドル
	//HWND hWnd = CreateWindow(
	//	CLASS_NAME,//ウインドウクラスの登録
	//	WINDOW_CAPTION,//タイトルの文字列
	//	style,//ウインドウスタイル (ビットフラグ)
	//	desktop_width/2 - (desktop_width / 2)/2,//サイズと位置 (CW_USEDEFAULT・・・システム任せ)
	//	desktop_height / 2 - (desktop_height / 2) / 2,//ウインドウの左上の位置 x//ウインドウの左上の位置 y
	//	desktop_width / 2,//ウインドウ幅   フレーム含めた全体の幅、高さ
	//	desktop_height / 2,//ウインドウ高さ
	//
	//	NULL,//親ウインドウハンドル
	//	NULL,//メニューハンドル
	//	hInstance,//インスタンスハンドル
	//	NULL//追加のアプリケーションデータ(謎
	//);

	//hWnd ウインドウハンドル
	HWND hWnd = CreateWindow(
		CLASS_NAME,//ウインドウクラスの登録
		WINDOW_CAPTION,//タイトルの文字列
		style,//ウインドウスタイル (ビットフラグ)
		20,//サイズと位置 (CW_USEDEFAULT・・・システム任せ)
		20,//ウインドウの左上の位置 x//ウインドウの左上の位置 y
		SCREEN_WIDTH,//ウインドウ幅   フレーム含めた全体の幅、高さ
		SCREEN_HEIGHT,//ウインドウ高さ

		NULL,//親ウインドウハンドル
		NULL,//メニューハンドル
		hInstance,//インスタンスハンドル
		NULL//追加のアプリケーションデータ(謎
	);

	/*
	HWND btn = CreateWindow(
		TEXT("BUTTON"),//ウインドウクラスの登録
		"ボタン",//タイトルの文字列
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//ウインドウスタイル 
		//サイズと位置 (CW_USEDEFAULT・・・システム任せ)
		20,//ウインドウの左上の位置 x
		20,//ウインドウの左上の位置 y
		100,//ウインドウ幅   フレーム含めた全体の幅、高さ
		100,//ウインドウ高さ

		hWnd,//親ウインドウハンドル
		NULL,//メニューハンドル
		hInstance,//インスタンスハンドル
		NULL//追加のアプリケーションデータ(謎
	);
	*/

	if (hWnd == NULL) {
		//ウインドウハンドルが何らかの理由で生成できなかった
		return 0;
	}

	//ウインドウの表示
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);//ウインドウの更新

	//ゲームの初期化 処理に時間がかかるため、置く場所は考えよう
	if (!Initialize(hWnd)) {
		//メッセージボックスでゲームの初期化が失敗したためアプリケーションが終了したことを伝える
		MessageBox(hWnd, "ダメです", "ゲームの初期化失敗", MB_OKCANCEL | MB_DEFBUTTON2);
		return 0;
	}

	MSG msg = {};//msg.message == WM_NULL
	
	
	Game_Initialize();
	Keyboard_Initialize();


	//タイトル曲
	//PlaySound(SOUND_LABEL_BGMTITLE);

	//頂点カラーとテクスチャのブレンド設定
	//g_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);//透明度ブレンド
				 
	//windowsゲーム用ループ
	while (WM_QUIT != msg.message) {
		
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {//メッセージが送られていたら取得,ない場合0を返す
			TranslateMessage(&msg);//メッセージの翻訳
			DispatchMessage(&msg);//メッセージに対する処理 ウインドウプロシージャで指定した関数が呼ばれる
		}
		else {
			//ここがゲーム処理
			
			double time = SystemTimer_GetTime();
			if ((time - g_ReserveTime) < 1.0 / 60.0f) {
				Sleep(0);
				continue;
			}
			g_ReserveTime = time;
			
			//更新　慣れてきたらupdateに戻り値を入れて終了できるようにしよう
			if (Update() == GAMEEND) break;
			
			//描画
			Draw();
		}
	}

	//ゲームの終了処理
	Finalize();

	DestroyWindow(hWnd);//hWndを破壊
	return (int)msg.wParam;

}

//引数:対象のウインドウハンドル,メッセージ,追加パラメータ１,追加パラメータ2
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) {//エスケープキーが押されたとき
				SendMessage(hWnd, WM_CLOSE, 0, 0);//自分にWM_CLOSEを送る
			}
		case WM_ACTIVATEAPP:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard_ProcessMessage(uMsg, wParam, lParam);
			break;


		case WM_CLOSE://ウインドウ×ボタン等
			if (MessageBox(hWnd, "本当に終了してもよろしいですか?", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
				DestroyWindow(hWnd);//hWndを破壊

			}
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);//WM_QUITメッセージの送信
			return 0;
	}
	
	//通常のメッセージ処理はこの関数に任せる 良きに計らえ
	//終了処理だけは任せてはいけない(プロセスが終了しないため)
	return DefWindowProc(hWnd, uMsg, wParam,lParam);
}

bool Initialize(HWND hWnd) {

	Keyboard_Initialize();

	KeyManager_Initilalize();

	InitSound(hWnd);

	MYDirect3D_Initialize(hWnd);

	if (!Game_Initialize) {
		MessageBox(NULL,"sippai","error",MB_OK);
	}

	g_pDevice = MYDirect3D_GetDevice();
	HRESULT hr;


	g_ReserveTime = g_BaseTime = 0;//SystemTimer_GetTime();
	g_FrameCount = g_BaseFrame = 0;
	g_FPS = 0.0;
	/*
	char buf[64];
	sprintf(buf, "Btime = %.2f", g_BaseTime);
	OutputDebugString(buf);
	*/
	SystemTimer_Initialize();

	DebugFont_Initialize();
	DebugPrimitive_Initialize();

	return true;
}


int Update() {

	KeyManager_Update();


	//計測
	double time = SystemTimer_GetTime();
	if (time - g_BaseTime > 1.0) {
		g_FPS = (g_FrameCount - g_BaseFrame) / (time - g_BaseTime);
		g_BaseTime = time;
		g_BaseFrame = g_FrameCount;

	}
	g_FrameCount++;

	return Game_Update();

}

void Draw() {


	//DirectXはポリゴンをカメラに対して右回りに用意しないと表面がこっちを向かないため、表示されない
	//OpenGLは逆


	//画面のクリア　引数:矩形の数、矩形アドレス,バッファフラグ,バッファ(色,深度,ステンシル)
	//begin～endの中に書いてはいけない
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 200, 200, 255), 1.0f, 0);
	//g_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 200, 200, 255), 1.0f, 0);


	//描画パッチ命令の開始
	  //仕事をためる
	g_pDevice->BeginScene();


	//デバッグ描画処理の受付開始
	DebugPrimitive_BatchBegin();


	////この下に描画命令を書く
	Game_Draw();

	DebugPrimitive_BatchDrawCircle(200, 200, 50);
	//FPS表示
	
	//char buf[64];
	//sprintf(buf,"F = %.2f",g_FPS);
	//DebugFont_Draw(0,0,buf);

	//デバイスのＦＶＦ設定
	g_pDevice->SetFVF(FVF_VERTEX2D);
	
	//デバッグ描画処理の実行
	DebugPrimitive_BatchRun();

	//描画パッチ命令の終了
	g_pDevice->EndScene();



	//バックバッファをフリップ(タイミングはD3DPRESENT_PARAMETERSの設定による)
	 //貯めた仕事を実行　１フレームに１回
	 //画面の更新（環境準拠）まで止まる
	g_pDevice->Present(NULL, NULL, NULL, NULL);

}


void Finalize() {

	if (g_pTexture) {
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	DebugFont_Finalize();
	DebugPrimitive_Finalize();

	MYDirect3D_Finalize();

	KeyManager_Finalize();

	//UninitSound();
}

