#include "stdafx.h"
#include "Scene2.h"




Scene2::Scene2()
{
	edit = new TileMapEdit();

	//버튼 생성	13개
	for (int i = 0; i < 13; i++)
	{		
		bts.emplace_back(new UIButton());
	}

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



#pragma region UI Initialize

	//버튼에 필요한 이미지 추가
	AddImage(_T("/MapEditUI/UiCanvas1.png"), 60,1);				//버튼캔버스 배경
	AddImage(_T("/MapEditUI/ButtonUI.png"), 5, 5);				//버튼 이미지들
	AddImage(_T("/MapEditUI/ButtonAplha1.png"), 1,1);			//알파값 버튼배경
	AddImage(_T("/MapEditUI/ButtonAplha2.png"), 1,1);			//알파값 버튼


	//uiBts[0] = UICANVAS
	bts[0]->numIdx = 0;
	bts[0]->imgIdx = POINT{ 1,1 };
	bts[0]->Scale = Vector2(WINSIZEX * 0.2f, WINSIZEY);
	bts[0]->Pos = Vector2(WINSIZEX - bts[0]->Scale.x*0.5f, WINSIZEY*0.5f);
	bts[0]->Pos2 = Vector2(WINSIZEX - bts[0]->Scale.x*0.5f, WINSIZEY*0.5f);
	bts[0]->col = CreateColRect(bts[0]->Pos, bts[0]->Scale);
	bts[0]->ButtonInit(edit, &EditManager::TileIndexResize);


	


	//버튼들의 공통적인 스케일 값
	Vector2 btScale = Vector2(bts[0]->Scale.x *0.25f, WINSIZEY * 0.1f);
	float sizeX = btScale.x;


	
	for (int i = 0; i < 4; i++)
	{
		//버튼1,2,3,4 타일인덱스레인지 함수
		bts[i + 1]->parent = bts[0];
		//bts[i + 1]->btfunc = &EditManager::TileIndexResize;
		bts[i + 1]->ButtonInit(edit, &EditManager::TileIndexResize);
		bts[i + 1]->ButtonImageInit(1, POINT{ i,0 });
		bts[i + 1]->Scale = btScale;
		bts[i + 1]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x* i, WINSIZEY*0.8f);				//처음좌표
		bts[i + 1]->Pos2 = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x* i, WINSIZEY*0.8f);			//고정좌표
		bts[i + 1]->col = CreateColRect(bts[i + 1]->Pos, bts[i + 1]->Scale);


		//버튼 5 6 7 8 LB위치 조정 함수
		bts[i + 5]->parent = bts[0];
		bts[i + 5]->ButtonInit(edit, &EditManager::TileInitPosition);
		bts[i + 5]->ButtonImageInit(1, POINT{ i,1 });
		bts[i + 5]->Scale = btScale;
		bts[i + 5]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x* i, WINSIZEY*0.7f);
		bts[i + 5]->Pos2 = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x* i, WINSIZEY*0.7f);
		bts[i + 5]->col = CreateColRect(bts[i + 5]->Pos, bts[i + 5]->Scale);

		//	//버튼 9 10 11 12 타일 사이즈 조정 함수
		//	bts[i + 9]->parent = bts[0];
		//	bts[i + 9]->ButtonInit(edit,&EditManager::TileScaleResize);
		//	bts[i + 9]->ButtonImageInit(1, POINT{ i,2 });
		//	bts[i + 9]->Scale = btScale;
		//	bts[i + 9]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f ) + btScale.x*i, WINSIZEY*0.6f);
		//	bts[i + 9]->Pos2 = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x*i, WINSIZEY*0.6f);
		//	bts[i + 9]->col = CreateColRect(bts[i + 9]->Pos, bts[i + 9]->Scale);
	}

	for (int i = 0; i < 4; i++)
	{
			//버튼 9 10 11 12 타일 사이즈 조정 함수
			bts[i + 9]->parent = bts[0];
			bts[i + 9]->ButtonInit(edit,&EditManager::TileScaleResize);
			bts[i + 9]->ButtonImageInit(1, POINT{ i,2 });
			bts[i + 9]->Scale = btScale;
			bts[i + 9]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f ) + btScale.x*i, WINSIZEY*0.6f);
			bts[i + 9]->Pos2 = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x*i, WINSIZEY*0.6f);
			bts[i + 9]->col = CreateColRect(bts[i + 9]->Pos, bts[i + 9]->Scale);
	}



	
	

#pragma endregion

	

	edit->Init();

	//init후에 들어가는 이미지 순서로 인덱스가 매겨진다.
	edit->map.AddImage(_T("HexTile.png"), 2, 1);
	

	//타일맵으로 쓸 배경
	bg = new Image();
	bg->init(_T("Dugeon.jpg"));
	bg->Pos = Vector2(800, 600);
	bg->Scale = Vector2(800.0f, 600.0f);

	return S_OK;
}

void Scene2::release()
{
	edit->Release();
	SAFE_DELETE(edit);

	bg->release();
	SAFE_DELETE(bg);

}
void Scene2::update()
{


	MapArrayResize();		//타일 갯수 조절
	MapOriginMove();		//타일 LB위치 조절
	TileResize();			//타일 스케일 조절

	

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
	}

	
	if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
	{
		if (PtInUi() == false)
		{
			OffsetCoord Coord;
			
			if (PtInHexTile(edit, g_MousePt, Coord))
			{
				if(Coord.col != NULLNUMBER && Coord.row != NULLNUMBER)
					edit->map.Tiles[Coord.col][Coord.row].imgIdx.x = 1;
			}
			
		}
		
	}




	edit->Update();						//에디트 업데이트
	bg->update();						//배경 업데이트
	for (int i = 0; i < 13; i++)		//버튼 업데이트
		bts[i]->Update();
	



	
}

void Scene2::render()
{

	//상수버퍼 업데이트 W값을 WBuffer에 전달
	g_DeviceContext->UpdateSubresource(FadeBuffer, 0, NULL, &Fade, 0, 0);
	//                            연결될레지스터 번호 0
	g_DeviceContext->PSSetConstantBuffers(1, 1, &FadeBuffer);

	bg->render();			//배경이미지 렌더
	edit->Render();			//에디트의 맵 타일 렌더


	for (int i = 0; i < 13; i++)
	{
		int vecindex;
		if (bts[0]->IsActive == true)
		{
			if (i == 0)
			{
				vecindex = bts[i]->numIdx;
				uiImg[vecindex]->Pos = bts[i]->Pos;
				uiImg[vecindex]->Scale = bts[i]->Scale;
				uiImg[vecindex]->update();
				uiImg[vecindex]->PlayAnimation(ANI_LOOP, true, 0.05f);
				uiImg[vecindex]->render();
			}
			else
			{
				vecindex = bts[i]->numIdx;
				uiImg[vecindex]->Pos = bts[i]->Pos;
				uiImg[vecindex]->Scale = bts[i]->Scale;
				uiImg[vecindex]->CurrentFrameX = bts[i]->imgIdx.x;
				uiImg[vecindex]->CurrentFrameY = bts[i]->imgIdx.y;
				uiImg[vecindex]->update();				
				uiImg[vecindex]->render();
			}	
		}
	}

	uiImg[2]->Pos = Vector2(bts[0]->Pos2.x, WINSIZEY*0.1f);
	uiImg[2]->Scale = Vector2(300, 22);
	uiImg[2]->update();
	uiImg[2]->render();

	uiImg[3]->Pos = Vector2(bts[0]->Pos2.x, WINSIZEY*0.1f);
	uiImg[3]->Scale = Vector2(30, 20);
	uiImg[3]->update();
	uiImg[3]->render();

}





bool Scene2::PtInUi()
{
	if (PtinRect(g_MousePt, bts[0]->col))
	{
		return  true;
	}
	return false;
}

void Scene2::MapArrayResize()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		if (PtinRect(g_MousePt, bts[1]->col))
		{
			if (edit->mapManager->width > 0)
			{
				edit->mapManager->width--;				
				bts[1]->ButtonDown();
			}
		}
		else if (PtinRect(g_MousePt, bts[2]->col))
		{
			edit->mapManager->width++;
			bts[2]->ButtonDown();
		}

		else if (PtinRect(g_MousePt, bts[3]->col))
		{
			edit->mapManager->height++;
			bts[3]->ButtonDown();
		}
		else if (PtinRect(g_MousePt, bts[4]->col))
		{
			if (edit->mapManager->height > 0)
			{
				edit->mapManager->height--;
				bts[4]->ButtonDown();
			}
		}
	}
}

void Scene2::MapOriginMove()
{
	if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
	{
		if (PtinRect(g_MousePt, bts[5]->col))
		{
			//왼쪽
			edit->mapManager->map->LB.x -= uiMovespeed;
			bts[5]->ButtonDown();
			//edit->mapManager->map->InitPosition();

		}
		else if (PtinRect(g_MousePt, bts[6]->col))
		{
			//오른족
			edit->mapManager->map->LB.x += uiMovespeed;
			bts[6]->ButtonDown();
			//edit->mapManager->map->InitPosition();
		}

		else if (PtinRect(g_MousePt, bts[7]->col))
		{
			//위
			edit->mapManager->map->LB.y += uiMovespeed;
			bts[7]->ButtonDown();
			//edit->mapManager->map->InitPosition();

		}
		else if (PtinRect(g_MousePt, bts[8]->col))
		{
			//아래		
			edit->mapManager->map->LB.y -= uiMovespeed;
			bts[8]->ButtonDown();
			//edit->mapManager->map->InitPosition();
		}
	}
}

void Scene2::TileResize()
{
	if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
	{
		if (PtinRect(g_MousePt, bts[9]->col))
		{
			//왼쪽
			edit->mapManager->TileSize *= 0.98f;
			edit->layout.size *= 0.98f;
			edit->layout.origin.x = edit->mapManager->TileSize.x * 0.5f;
			edit->layout.origin.x = edit->mapManager->TileSize.y * 0.5f;
			bts[9]->ButtonDown();
		

		}
		else if (PtinRect(g_MousePt, bts[10]->col))
		{
			//오른족
			//왼쪽
			edit->mapManager->TileSize *= 1.02f;
			edit->layout.size *= 1.02f;
			edit->layout.origin.x = edit->mapManager->TileSize.x * 0.5f;
			edit->layout.origin.x = edit->mapManager->TileSize.y * 0.5f;
			bts[10]->ButtonDown();
		}

	}

}

void Scene2::AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs, string ps)
{
	Image* temp = new Image();
	temp->init(file, vs, ps);
	//temp->Scale.x = TileSize.x;
	//temp->Scale.y = TileSize.y;

	temp->MaxFrameX = MaxFrameX;
	temp->MaxFrameY = MaxFrameY;
	uiImg.emplace_back(temp);
}






#pragma region ButtonUI 함수들


void UIButton::ButtonInit(TileMapEdit* edit_,ept otherFunc)
{
	edit = edit_;
	btfunc =  otherFunc;
}

void UIButton::ButtonImageInit(int numIdx_, POINT imgIdx_)
{

	numIdx = numIdx_;
	imgIdx = imgIdx_;
}

void UIButton::PosSettling()
{

	Pos.x = Pos2.x + MAINCAM->Pos.x;
	Pos.y = Pos2.y + MAINCAM->Pos.y;
}

void UIButton::Update()
{	
	PosSettling();
	col = CreateColRect(Pos, Scale);
	ButtonDown();
}

void UIButton::ButtonDown()
{		
	if (btfunc != nullptr)
	{
		(*edit->mapManager.*btfunc)();
	}	
}

#pragma endregion


#pragma region 실패한 코드들


//if (KEYMANAGER->isKeyDown(VK_LBUTTON))
//{
//	if (PtinRect(g_MousePt, bts[1]->col))
//	{
//		if (edit->mapManager->width > 0)
//		{
//			edit->mapManager->width--;
//			//bts[1]->ButtonDown();
//		}
//	}
//	else if (PtinRect(g_MousePt, bts[2]->col))
//	{
//		edit->mapManager->width++;
//		//(*edit->mapManager.*bts[1]->btfunc)();
//	}

//	else if (PtinRect(g_MousePt, bts[3]->col))
//	{
//		edit->mapManager->height++;
//		//(*edit->mapManager.*bts[1]->btfunc)();

//	}
//	else if (PtinRect(g_MousePt, bts[4]->col))
//	{
//		if (edit->mapManager->height > 0)
//		{
//			edit->mapManager->height--;
//			//(*edit->mapManager.*bts[1]->btfunc)();

//		}
//	}
//}

//버튼들의 공통적인 스케일 값
	//Vector2 btScale = Vector2(uiBts[0]->Scale.x *0.25f, WINSIZEY * 0.1f);

	////UI캔버스 만들기
	//uiBts[0]->numIdx = 0;														//쓸 이미지 인덱스
	//uiBts[0]->imgIdx = POINT{ 1,1 };											//이미지 current 인덱스
	//uiBts[0]->Scale = Vector2(WINSIZEX * 0.15f, WINSIZEY);						//캔버스 크기
	//uiBts[0]->Pos = Vector2(WINSIZEX - uiBts[0]->Scale.x*0.5f, WINSIZEY*0.5f);	//캔버스 위치
	//uiBts[0]->col = CreateColRect(uiBts[0]->Pos, uiBts[0]->Scale);				//충돌체 만들기



	//
	//float sizeX = uiBts[0]->Scale.x * 0.5f;

	//uiBts[1]->numIdx = 1;
	//uiBts[1]->imgIdx = POINT{ 0,0 };
	//uiBts[1]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[1]->Pos = Vector2(uiBts[0]->col.left + sizeX*0.5f, WINSIZEY*0.85f);
	//uiBts[1]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기


	//uiBts[2]->numIdx = 1;
	//uiBts[2]->imgIdx = POINT{ 1,0 };
	//uiBts[2]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[2]->Pos = Vector2(uiBts[1]->Pos.x + sizeX, WINSIZEY*0.85f);
	//uiBts[2]->col = CreateColRect(uiBts[2]->Pos, uiBts[2]->Scale);				//충돌체 만들기


	//uiBts[3]->numIdx = 1;
	//uiBts[3]->imgIdx = POINT{ 2,0 };
	//uiBts[3]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[3]->Pos = Vector2(uiBts[2]->Pos.x + sizeX, WINSIZEY*0.85f);
	//uiBts[3]->col = CreateColRect(uiBts[3]->Pos, uiBts[3]->Scale);				//충돌체 만들기


	//uiBts[4]->numIdx = 1;
	//uiBts[4]->imgIdx = POINT{ 3,0 };
	//uiBts[4]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[4]->Pos = Vector2(uiBts[3]->Pos.x + sizeX, WINSIZEY*0.85f);
	//uiBts[4]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기

	////////////////////////////////////////////////////////////////////////////////////////
	//uiBts[5]->numIdx = 1;
	//uiBts[5]->imgIdx = POINT{ 0,1 };
	//uiBts[5]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[5]->Pos = Vector2(uiBts[0]->col.left + sizeX * 0.5f, WINSIZEY*0.7f);
	//uiBts[5]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기

	//uiBts[6]->numIdx = 1;
	//uiBts[6]->imgIdx = POINT{ 1,1 };
	//uiBts[6]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[6]->Pos = Vector2(uiBts[1]->Pos.x + sizeX, WINSIZEY*0.7f);
	//uiBts[6]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기


	//uiBts[7]->numIdx = 1;
	//uiBts[7]->imgIdx = POINT{ 2,1 };
	//uiBts[7]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[7]->Pos = Vector2(uiBts[2]->Pos.x + sizeX, WINSIZEY*0.7f);
	//uiBts[7]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기


	//uiBts[8]->numIdx = 1;
	//uiBts[8]->imgIdx = POINT{ 3,1 };
	//uiBts[8]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[8]->Pos = Vector2(uiBts[3]->Pos.x + sizeX, WINSIZEY*0.7f);
	//uiBts[8]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기

	////////////////////////////////////////////////////////////////////////////////////////
	//uiBts[9]->numIdx = 1;
	//uiBts[9]->imgIdx = POINT{ 0,2 };
	//uiBts[9]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[9]->Pos = Vector2(uiBts[0]->col.left + sizeX * 0.5f, WINSIZEY*0.6f);
	//uiBts[9]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기

	//uiBts[10]->numIdx = 1;
	//uiBts[10]->imgIdx = POINT{ 1,2 };
	//uiBts[10]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[10]->Pos = Vector2(uiBts[1]->Pos.x + sizeX, WINSIZEY*0.6f);
	//uiBts[10]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기


	//uiBts[11]->numIdx = 1;
	//uiBts[11]->imgIdx = POINT{ 2,2 };
	//uiBts[11]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[11]->Pos = Vector2(uiBts[2]->Pos.x + sizeX, WINSIZEY*0.6f);
	//uiBts[11]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기


	//uiBts[12]->numIdx = 1;
	//uiBts[12]->imgIdx = POINT{ 3,2 };
	//uiBts[12]->Scale = Vector2(uiBts[0]->Scale.x * 0.25f, WINSIZEY * 0.1f);
	//uiBts[12]->Pos = Vector2(uiBts[3]->Pos.x + sizeX, WINSIZEY*0.6f);
	//uiBts[12]->col = CreateColRect(uiBts[1]->Pos, uiBts[1]->Scale);				//충돌체 만들기
#pragma endregion
