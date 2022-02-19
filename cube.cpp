#include <d3dx9.h>
#include "cube.h"
#include "mydirect3d.h"
#include "camera.h"
#include "texture.h"


#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

static float g_angle = 0.0f;
static float g_value = 0.0f;

static int g_TexId = TEXTURE_INVALID_ID;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

//���_�f�[�^�z��
/*
static const Vertex3D g_CubeVertex[] = {

	//�O�ʁ@��
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(255,0,0,255)},
	{D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(255,0,0,255)},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(255,0,0,255)},
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(255,0,0,255)},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(255,0,0,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(255,0,0,255)},

	//��� ��
	{D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DCOLOR_RGBA(0,255,0,255)},
	{D3DXVECTOR3(0.5f,0.5f,0.5f),D3DCOLOR_RGBA(0,255,0,255)},
	{D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(0,255,0,255)},
	{D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DCOLOR_RGBA(0,255,0,255)},
	{D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(0,255,0,255)},
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(0,255,0,255)},

	//�E�� ��
	{D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(0,0,255,255)},
	{D3DXVECTOR3(0.5f,0.5f,0.5f),D3DCOLOR_RGBA(0,0,255,255)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(0,0,255,255)},
	{D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(0,0,255,255)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(0,0,255,255)},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(0,0,255,255)},

	//���� ���F
	{D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DCOLOR_RGBA(0,255,255,255)},
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DCOLOR_RGBA(0,255,255,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(0,255,255,255)},
	{D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DCOLOR_RGBA(0,255,255,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(0,255,255,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(0,255,255,255)},

	//���� ���F
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(255,255,0,255)},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(255,255,0,255)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(255,255,0,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DCOLOR_RGBA(255,255,0,255)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(255,255,0,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(255,255,0,255)},

	//���� ��
	{D3DXVECTOR3(0.5f,0.5f,0.5f),D3DCOLOR_RGBA(255,0,255,255)},
	{D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DCOLOR_RGBA(255,0,255,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(255,0,255,255)},
	{D3DXVECTOR3(0.5f,0.5f,0.5f),D3DCOLOR_RGBA(255,0,255,255)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(255,0,255,255)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DCOLOR_RGBA(255,0,255,255)},

};*/

//���_�f�[�^�z��
/*
static const Vertex3D g_CubeVertex[] = {

	//�O�ʁ@��
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f)  },
	{D3DXVECTOR3(0.5f,0.5f,-0.5f)  ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255)  ,D3DXVECTOR2(0.25f,0.0f) },
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.25f,0.25f)},
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f)  },
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.25f,0.25f)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.25f) },

	//��� ��
	{D3DXVECTOR3(-0.5f,0.5f,0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.25f,0.0f)   },
	{D3DXVECTOR3(0.5f,0.5f,0.5f)  ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255)  ,D3DXVECTOR2(0.50f,0.0f) },
	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.50f,0.25f) },
	{D3DXVECTOR3(-0.5f,0.5f,0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.25f,0.0f)   },
	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.50f,0.25f) },
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.25f)  },

	//�E�� ��
	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.0f) },
	{D3DXVECTOR3(0.5f,0.5f,0.5f)  ,D3DXVECTOR3(1.0f,0.0f,0.0f),  D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.0f) },
	{D3DXVECTOR3(0.5f,-0.5f,0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.25f)},
	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.0f) },
	{D3DXVECTOR3(0.5f,-0.5f,0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.25f)},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.25f)},

	//���� ���F
	{D3DXVECTOR3(-0.5f,0.5f,0.5f)  ,D3DXVECTOR3(-1.0f,0.0f,0.0f),  D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.0f) },
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f) ,D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.00f,0.0f) },
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.00f,0.25f)},
	{D3DXVECTOR3(-0.5f,0.5f,0.5f)  ,D3DXVECTOR3(-1.0f,0.0f,0.0f),  D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.0f) },
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.00f,0.25f)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.25f)},

	//���� ���F
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.25f)  },
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f) ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.25f) },
	{D3DXVECTOR3(0.5f,-0.5f,0.5f)  ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.50f)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.25f)  },
	{D3DXVECTOR3(0.5f,-0.5f,0.5f)  ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.50f)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.50f) },

	//���� ��
	{D3DXVECTOR3(0.5f,0.5f,0.5f)   ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.25f) },
	{D3DXVECTOR3(-0.5f,0.5f,0.5f)  ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.25f)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.50f)},
	{D3DXVECTOR3(0.5f,0.5f,0.5f)   ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.25f) },
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.50f)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f)  ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.50f) },

};
*/
static const Vertex3D g_CubeVertex[] = {

	//�O�ʁ@��
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f)  },
	{D3DXVECTOR3(0.5f,0.5f,-0.5f)  ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255)  ,D3DXVECTOR2(1.0f,0.0f) },
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1.0f,1.0f)},
//	{D3DXVECTOR3(-0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f)  },
//	{D3DXVECTOR3(0.5f,-0.5f,-0.5f) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.25f,0.25f)},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },

	//��� ��
	{D3DXVECTOR3(-0.5f,0.5f,0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f)   },
	{D3DXVECTOR3(0.5f,0.5f,0.5f)  ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255)  ,D3DXVECTOR2(1.0f,0.0f) },
	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1.0f,1.0f) },
//	{D3DXVECTOR3(-0.5f,0.5f,0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.25f,0.0f)   },
//	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.50f,0.25f) },
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)  },

	//�E�� ��
	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{D3DXVECTOR3(0.5f,0.5f,0.5f)  ,D3DXVECTOR3(1.0f,0.0f,0.0f),  D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{D3DXVECTOR3(0.5f,-0.5f,0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
//	{D3DXVECTOR3(0.5f,0.5f,-0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.0f) },
//	{D3DXVECTOR3(0.5f,-0.5f,0.5f) ,D3DXVECTOR3(1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.25f)},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)},

	//���� ���F
	{D3DXVECTOR3(-0.5f,0.5f,0.5f)  ,D3DXVECTOR3(-1.0f,0.0f,0.0f),  D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f) ,D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
//	{D3DXVECTOR3(-0.5f,0.5f,0.5f)  ,D3DXVECTOR3(-1.0f,0.0f,0.0f),  D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.75f,0.0f) },
//	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.00f,0.25f)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)},

	//���� ���F
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f)  },
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f) ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{D3DXVECTOR3(0.5f,-0.5f,0.5f)  ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
//	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.25f)  },
//	{D3DXVECTOR3(0.5f,-0.5f,0.5f)  ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.50f)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },

	//���� ��
	{D3DXVECTOR3(0.5f,0.5f,0.5f)   ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{D3DXVECTOR3(-0.5f,0.5f,0.5f)  ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f)},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
//	{D3DXVECTOR3(0.5f,0.5f,0.5f)   ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.25f,0.25f) },
//	{D3DXVECTOR3(-0.5f,-0.5f,0.5f) ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.50f,0.50f)},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f)  ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },

};

void Cube_Initialize() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	g_TexId = Texture_SetTextureLoadFile("asset/obj.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s", "�G���[", MB_OK);
	}


	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);


	Vertex3D* pV;

	g_pVertexBuffer->Lock(0,0,(void**)&pV,0);
	memcpy(pV,g_CubeVertex,sizeof(g_CubeVertex));
	g_pVertexBuffer->Unlock();

	WORD* pIndex;
	g_pIndexBuffer->Lock(0,0,(void**)&pIndex,0);

	//index����
	for (int i = 0; i < 6; i++) {
		pIndex[0 + i * 6] = 0 + i * 4;
		pIndex[1 + i * 6] = 1 + i * 4;
		pIndex[2 + i * 6] = 2 + i * 4;
		pIndex[3 + i * 6] = 0 + i * 4;
		pIndex[4 + i * 6] = 2 + i * 4;
		pIndex[5 + i * 6] = 3 + i * 4;
	}


	g_pIndexBuffer->Unlock();
}


void Cube_Finalize() {

	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if (g_pIndexBuffer) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}


}


void Cube_Draw(const D3DXMATRIX* pMtxWorld) {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	//���[���h���W�ϊ��s��
	//D3DXMATRIX mtxWorld,mtxS,mtxR,mtxT;
	//D3DXMATRIX mtx1,mtx2;

	//D3DXMatrixRotationY(&mtxR,g_angle);
	//D3DXMatrixTranslation(&mtxT,4.5f,0.5f,0.5f);
	//D3DXMatrixScaling(&mtxS,1.0f,3.0f * sinf(g_angle),1.0f);
	
	//mtxWorld = mtxR * mtxT * mtxR;
	//�P�ʍs��̍쐬
	//D3DXMatrixIdentity(&mtxWorld);
	//��]�s��̍쐬
	//D3DXVECTOR3 v_axis(1.0f,1.0f,0.0f);
	//D3DXMatrixRotationYawPitchRoll(&mtxWorld, g_angle,0.5f,2.0f);
	//D3DXMatrixRotationAxis(&mtxWorld,&v_axis,g_angle);
	//D3DXMatrixRotationY(&mtxWorld,g_angle);
	//D3DXMatrixRotationZ(&mtxWorld, g_angle);
	//D3DXMatrixRotationAxis();(���� �A�h���X,����\���x�N�g���̃A�h���X,��]�p�x)
	//D3DXMatrixRotationYawPitchRoll();�w�肵���p�x�ɉ�]������(���� �A�h���X,yaw:y�� pitch:x�� roll:z��)

	//�g��s��
	//D3DXMatrixScaling(&mtxWorld,1.0f,2.0f,3.0f);


	//�s��̍���
	//D3DXMatrixMultiply();
	//D3DXMATRIX�^��*�ō����ł���

	//���s�ړ��s��
	//D3DXMatrixTranslation(&mtxWorld,1.0f,2.0f,3.0f);

	//���C�g��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���C�g(���s����)�̐ݒ���쐬
	D3DLIGHT9 light = {};
	//memset(&light,0,sizeof(light));��Q�����ŏ�����
	//ZeroMemory(&light, sizeof(light));0�ŏ�����
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-2.0f,-2.0f,1.0f);
	//D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	//light.Direction = vecDirLight;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	
	// �A���r�G���g�͌ʂ̃��C�g���̂ɂ��ݒ�ł��A���̏ꍇ�ɂ͑S�̂̃A���r�G���g�Ƃ��̃A���r�G���g�𑫂������̂ɂȂ�炵��
	light.Ambient.r = 0.1f;
	light.Ambient.g = 0.1f;
	light.Ambient.b = 0.1f;
	light.Ambient.a = 1.0f;
	

	//�f�B�t���[�Y�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);

	//�A���r�G���g���C�g�̐ݒ�
	//pDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA(192,96,128,255));
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(64, 32, 64, 255));


	//�A���r�G���g�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//�f�o�C�X�Ƀ��C�g��ݒ�
	pDevice->SetLight(0,&light);
	pDevice->LightEnable(0,TRUE);


	//�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD,pMtxWorld);

	//�f�o�C�X��FVF�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX3D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(g_TexId));

	//�|���S���̕`��
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,2*6,&g_CubeVertex,sizeof(Vertex3D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0,g_pVertexBuffer,0,sizeof(Vertex3D));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,24,0,12);

	//mtxWorld = mtxR;

	//pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2 * 6, &g_CubeVertex, sizeof(Vertex3D));


}


void Cube_Update(){

	//g_angle += 0.01f;
	//g_value = 5 * sinf(g_angle);

}
