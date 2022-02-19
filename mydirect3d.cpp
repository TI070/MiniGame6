#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h> //算術系処理をするときに使用
#include "config.h"

///////////
//プロトタイプ宣言
///////////





///////////
//グローバル変数宣言
///////////
//インターフェース　関数への窓口
static LPDIRECT3D9 g_pD3D = NULL;//Direct3Dインターフェース
static LPDIRECT3DDEVICE9 g_pDevice = NULL;//Direct3Dデバイスインターフェース


//Direct3D9の初期化 戻り値:失敗したらfalse
bool MYDirect3D_Initialize(HWND hWnd) {

	//Direct3Dインターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		//Direct3Dインターフェースの取得に失敗
		MessageBox(NULL, "失敗しました", "エラー", MB_OK);
		return false;
	}


	////Direct3Dデバイスの取得 

	//デバイスの取得に必要な情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;//バックバッファの横幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;//バックバッファのフォーマット　色らしい　UNKNOWNにしておけばデスクトップ準拠になるらしい
	d3dpp.BackBufferCount = 1;//バックバッファ数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//スワップ方法の設定　バッファをどういう感じで切り替えるか　変える必要はないらしい
	d3dpp.Windowed = TRUE;//ウインドウ or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;//深度バッファ・ステンシルバッファの使用
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//深度バッファ・ステンシルバッファのフォーマット
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//フルスクリーン時のリフレッシュレートの指定　ウインドウモードはデスクトップ準拠
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//リフレッシュレートとPresent処理の関係

	////Direct3Dデバイスの取得
	//引数(使用するアダプター,3D計算をソフトでやるかハードでやるか,,頂点をソフトでやるかハードでやるか,取得する構造体,取得した情報を入れる)
	HRESULT hr;//成功失敗とか失敗の理由とか
	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);

	//HRESULT型は成功や失敗が値として入っているので,失敗や成功だけ判定する場合はマクロを利用する
	//FAILED(hr) SUCCEEDE(hr)
	if (FAILED(hr)) {
		//デバイスの取得に失敗
		MessageBox(NULL, "Direct3Dデバイスの取得に失敗", "エラー", MB_OK);
		return false;
	}


	g_pDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);




	//アルファブレンド

	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//αブレンドを有効にする
	//半透明...描画色 = 今から描画するRGB * 今から描画するa + 画面のRGB * (1 - 今から描画するa);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//
	
	//法線ベクトルは必ず正規化して使う設定をする
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//UVアドレッシングモードの設定 デフォルト：ダブルラップ 繰り返し CLAMP:最後のピクセルを伸ばす MIRROR:鏡像
	//g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	//g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	//g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(128,128,255,0));

	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);

	//初期化完了
	return true;

}


//Direct3D9の終了
void MYDirect3D_Finalize() {

	if (g_pDevice) {
		g_pDevice->Release();
		g_pDevice = NULL;
	}

	if (g_pD3D) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

LPDIRECT3DDEVICE9 MYDirect3D_GetDevice(void) {

	return g_pDevice;
}