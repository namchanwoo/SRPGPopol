#include "stdafx.h"
#include "Scene2.h"


Scene2::Scene2()
{
	edit = new TileMapEdit();
	
}

Scene2::~Scene2()
{
	SAFE_RELEASE(FadeBuffer);
}

HRESULT Scene2::init()
{
	Fade = Color(1.0f, 1.0f, 1.0f, 1.0f);

	//CreateConstantBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Color);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &FadeBuffer);
		assert(SUCCEEDED(hr));
	}


	edit->Init();

	//init후에 들어가는 이미지 순서로 인덱스가 매겨진다.
	edit->map.AddImage(_T("HexTile.png"), 1, 1);


	//타일맵으로 쓸 배경
	bg1 = new Image();
	bg1->init(_T("Dugeon.jpg"));
	bg1->Pos = Vector2(400.0f, 300.0f);
	bg1->Scale = Vector2(800.0f, 600.0f);

	return S_OK;
}

void Scene2::release()
{
	edit->Release();
	SAFE_DELETE(edit);

	bg1->release();
	SAFE_DELETE(bg1);

}
void Scene2::update()
{
	

	//카메라이동
	if (KEYMANAGER->StayKeyDown('W'))
	{
		MAINCAM->Pos.y += 100.0f*DeltaTime;

	}
	if (KEYMANAGER->StayKeyDown('S'))
	{
		MAINCAM->Pos.y -= 100.0f*DeltaTime;

	}
	if (KEYMANAGER->StayKeyDown('A'))
	{
		MAINCAM->Pos.x -= 100.0f*DeltaTime;

	}
	if (KEYMANAGER->StayKeyDown('D'))
	{

		MAINCAM->Pos.x += 100.0f*DeltaTime;
	}


	edit->Update();

	bg1->update();

}

void Scene2::render()
{
	//상수버퍼 업데이트 W값을 WBuffer에 전달
	g_DeviceContext->UpdateSubresource(FadeBuffer, 0, NULL, &Fade, 0, 0);
	//                            연결될레지스터 번호 0
	g_DeviceContext->PSSetConstantBuffers(1, 1, &FadeBuffer);
	bg1->render();
	edit->Render();
}
