#include "stdafx.h"
#include "Scene2.h"


int UIButton::btsCount = 0;

Scene2::Scene2()
{
	edit = new TileMapEdit();

	//��ư ����	14��
	for (int i = 0; i < 14; i++)
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
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &FadeBuffer);
		assert(SUCCEEDED(hr));
	}

	edit->Init();

	//init�Ŀ� ���� �̹��� ������ �ε����� �Ű�����.
	edit->map.AddImage(_T("HexTile.png"), 2, 1);

	width = edit->map.width;
	height = edit->map.height;
	TileSize = edit->map.TileSize;




	//Ÿ�ϸ����� �� ���
	bg = new Image();
	bg->init(_T("Dugeon.jpg"));
	bg->Pos = Vector2(800, 600);
	bg->Scale = Vector2(800.0f, 600.0f);


#pragma region UI Initialize

	//��ư�� �ʿ��� �̹��� �߰�
	AddImage(_T("/MapEditUI/UiCanvas1.png"), 60,1);				//��ưĵ���� ���
	AddImage(_T("/MapEditUI/ButtonUI.png"), 5, 5);				//��ư �̹�����
	AddImage(_T("/MapEditUI/ButtonAplha1.png"), 1,1);			//���İ� ��ư���
	AddImage(_T("/MapEditUI/ButtonAplha2.png"), 1,1);			//���İ� ��ư


	///////////////////////////////////////////////////////////////UI �̹��� �ʱ�ȭ////////////////////////////////////////////////////////////////

	//uiBts[0] = UICANVAS
	bts[0]->numIdx = 0;
	bts[0]->imgIdx = POINT{ 1,1 };
	bts[0]->Scale = Vector2(WINSIZEX * 0.2f, WINSIZEY);
	bts[0]->Pos = Vector2(WINSIZEX - bts[0]->Scale.x*0.5f, WINSIZEY*0.5f);
	bts[0]->ScreenPos = Vector2(WINSIZEX - bts[0]->Scale.x*0.5f, WINSIZEY*0.5f);
	bts[0]->col = CreateColRect(bts[0]->Pos, bts[0]->Scale);


	//���İ� �����̴� ��� �̹���
	uiImg[2]->Pos = Vector2(bts[0]->ScreenPos.x, WINSIZEY*0.1f);
	AlphaUiPos = uiImg[2]->Pos;
	uiImg[2]->Scale = Vector2(WINSIZEX * 0.2f, 22);





	///////////////////////////////////////////////////////////////��ư UI �ʱ�ȭ//////////////////////////////////////////////////////////////


	


	//��ư���� �������� ������ ��
	Vector2 btScale = Vector2(bts[0]->Scale.x *0.25f, WINSIZEY * 0.1f);
	float sizeX = btScale.x;

	for (int i = 0; i < 4; i++)
	{
		//��ư1,2,3,4 Ÿ���ε��������� �Լ�
		bts[i + 1]->parent = bts[0];
		bts[i + 1]->ButtonImageInit(1, POINT{ i,0 });
		bts[i + 1]->Scale = btScale;
		bts[i + 1]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x* i, WINSIZEY*0.8f);				//ó����ǥ
		bts[i + 1]->ScreenPos = bts[i + 1]->Pos;			//������ǥ
		bts[i + 1]->col = CreateColRect(bts[i + 1]->Pos, bts[i + 1]->Scale);


		//��ư 5 6 7 8 LB��ġ ���� �Լ�
		bts[i + 5]->parent = bts[0];	
		bts[i + 5]->ButtonImageInit(1, POINT{ i,1 });
		bts[i + 5]->Scale = btScale;
		bts[i + 5]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x* i, WINSIZEY*0.7f);
		bts[i + 5]->ScreenPos = bts[i + 5]->Pos;
		bts[i + 5]->col = CreateColRect(bts[i + 5]->Pos, bts[i + 5]->Scale);

		//��ư 9 10 11 12 Ÿ�� ������ ���� �Լ�
		bts[i + 9]->parent = bts[0];
		bts[i + 9]->ButtonImageInit(1, POINT{ i,2 });
		bts[i + 9]->Scale = btScale;
		bts[i + 9]->Pos = Vector2((bts[0]->col.left + btScale.x * 0.5f) + btScale.x*i, WINSIZEY*0.6f);
		bts[i + 9]->ScreenPos = bts[i + 9]->Pos;
		bts[i + 9]->col = CreateColRect(bts[i + 9]->Pos, bts[i + 9]->Scale);
	}

	//���� �� �����ϴ� ��ư
	bts[13]->parent = bts[0];
	bts[13]->ButtonImageInit(3, POINT{ 1,1 });
	bts[13]->Scale = Vector2(50,40);
	bts[13]->Pos = Vector2((bts[0]->Pos.x) , WINSIZEY * 0.1f);
	bts[13]->ScreenPos = bts[13]->Pos;
	bts[13]->col = CreateColRect(bts[13]->Pos, bts[13]->Scale);

	

#pragma endregion


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

	ButtonIsKeyDown();
	ButtonStayKeyDown();


	{
		//ī�޶��̵�
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

	
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		//���콺�� UI�� ���� �ʴٸ�
		if (PtInUi() == false)
		{
			//��ȯ�� �ε���
			OffsetCoord Coord;
			if (PtInHexTile(edit, g_MousePt, Coord))
			{
				if (edit->map.Tiles[Coord.col][Coord.row].check == true)
				{
					curTiles.push(&edit->map.Tiles[Coord.col][Coord.row]);
				}
				
			}			
		}		
	}




	edit->Update();						//����Ʈ ������Ʈ
	bg->update();						//��� ������Ʈ
	for (int i = 0; i < UIButton::btsCount; i++)		//��ư ������Ʈ
		bts[i]->Update();
	

	
}

void Scene2::render()
{

	//������� ������Ʈ W���� WBuffer�� ����
	g_DeviceContext->UpdateSubresource(FadeBuffer, 0, NULL, &Fade, 0, 0);
	//                            ����ɷ������� ��ȣ 0
	g_DeviceContext->PSSetConstantBuffers(1, 1, &FadeBuffer);

	bg->render();			//����̹��� ����
	edit->Render();			//����Ʈ�� �� Ÿ�� ����
	
	

	//UICanvas ����
	int vecindex = bts[0]->numIdx;
	uiImg[vecindex]->Pos = bts[0]->Pos;
	uiImg[vecindex]->Scale = bts[0]->Scale;
	uiImg[vecindex]->update();
	uiImg[vecindex]->PlayAnimation(ANI_LOOP, true, 0.05f);
	uiImg[vecindex]->render();
	
	//���� �� �����̴� �̹��� ��׶���
	uiImg[2]->Pos = SettingPos(AlphaUiPos);
	uiImg[2]->update();
	uiImg[2]->render();

	//��ư ����
	for (int i = 1; i < UIButton::btsCount; i++)
	{
		int vecindex;
		if (bts[0]->IsActive == true)
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


	//D3DXVec2Lerp
	
		

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

void Scene2::TileIndexResize()
{
	//�÷��� �ǰ�
	if (edit->map.width < width)
	{
		edit->map.Tiles.emplace_back();
		edit->map.Tiles[width - 1].resize(height);
		edit->map.width = width;
		edit->map.InitPosition();
	}
	//�ٿ��� ��
	if (edit->map.width > width)
	{
		edit->map.Tiles[width].clear();
		edit->map.Tiles[width].shrink_to_fit();
		edit->map.Tiles.pop_back();
		edit->map.width = width;
		edit->map.InitPosition();
	}
	//�÷��� �ǰ�
	if (edit->map.height < height)
	{
		for (int i = 0; i < width; i++)
		{
			edit->map.Tiles[i].emplace_back();
		}
		edit->map.height = height;
		edit->map.InitPosition();
	}
	//�ٿ��� ��
	if (edit->map.height > height)
	{
		for (int i = 0; i < width; i++)
		{
			edit->map.Tiles[i].pop_back();
		}
		edit->map.height = height;
	}

}

void Scene2::TileInitPosition()
{
	edit->map.InitPosition();
}

void Scene2::TileScaleResize()
{
	//�÷��� �ǰ�
	if (edit->map.TileSize != TileSize)
	{
		edit->map.TileSize = TileSize;
		edit->map.InitPosition();
	}
}

Vector2 Scene2::SettingPos(Vector2 ScreenPos)
{
	return ScreenPos + MAINCAM->Pos;

}

bool Scene2::PtInUi()
{
	if (PtinRect(g_MousePt, bts[0]->col))
	{
		return  true;
	}
	return false;
}

void Scene2::ButtonIsKeyDown()
{
	if (KEYMANAGER->isKeyDown(VK_LBUTTON))
	{
		//1����ư
		if (PtinRect(g_MousePt, bts[1]->col))
		{
			if (width > 0)
			{
				width--;
				TileIndexResize();
			}
		}
		//2�� ��ư
		else if (PtinRect(g_MousePt, bts[2]->col))
		{
			width++;
			TileIndexResize();
		}
		//3�� ��ư
		else if (PtinRect(g_MousePt, bts[3]->col))
		{
			height++;
			TileIndexResize();
		}
		//4����ư
		else if (PtinRect(g_MousePt, bts[4]->col))
		{
			if (height > 0)
			{
				height--;
				TileIndexResize();
			}
		}
		//11�� ��ư
		else if (PtinRect(g_MousePt, bts[11]->col))
		{
			while (!curTiles.empty())
			{
				if (curTiles.front()->check == true)
				{
					curTiles.front()->CheckSwiching();
					curTiles.front()->tileState = TILESTATE::TILE_NONE;
				}				
				curTiles.pop();
			}
		}
		//12�� ��ư
		else if (PtinRect(g_MousePt, bts[12]->col))
		{
			while (!curTiles.empty())
			{
				if (curTiles.front()->check == true)
				{
					curTiles.front()->CheckSwiching();
					curTiles.front()->tileState = TILESTATE::TILE_WALL;
				}
				curTiles.pop();
			}
		}


	}
}

void Scene2::ButtonStayKeyDown()
{
	if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
	{
		if (PtinRect(g_MousePt, bts[5]->col))
		{
			//����
			edit->map.LB.x -= uiMovespeed;
			TileInitPosition();

		}
		else if (PtinRect(g_MousePt, bts[6]->col))
		{
			//������
			edit->map.LB.x += uiMovespeed;
			TileInitPosition();
		}

		else if (PtinRect(g_MousePt, bts[7]->col))
		{
			//��
			edit->map.LB.y += uiMovespeed;
			TileInitPosition();
		}
		else if (PtinRect(g_MousePt, bts[8]->col))
		{
			//�Ʒ�		
			edit->map.LB.y -= uiMovespeed;
			TileInitPosition();
		}


		else if (PtinRect(g_MousePt, bts[9]->col))
		{
			//����
			TileSize *= 0.99f;
			edit->layout.size *= 0.99f;
			edit->layout.origin.x = TileSize.x * 0.5f;
			edit->layout.origin.x = TileSize.y * 0.5f;
			TileScaleResize();
		}
		else if (PtinRect(g_MousePt, bts[10]->col))
		{
			//����
			TileSize *= 1.01f;
			edit->layout.size *= 1.01f;
			edit->layout.origin.x = TileSize.x * 0.5f;
			edit->layout.origin.x = TileSize.y * 0.5f;
			TileScaleResize();
		}


		else if (PtinRect(g_MousePt, bts[13]->col))
		{
			
			float fromX = bts[0]->col.left + bts[13]->Scale.x *0.5f;	
			float toX = bts[0]->col.right - bts[13]->Scale.x *0.5f;

			if (g_MousePt.x >= fromX && g_MousePt.x <= toX)
			{
				bts[13]->ScreenPos.x = Saturate(g_MousePt.x, fromX, toX);
			}

			alphaColor = InverseLerp(fromX, toX, bts[13]->ScreenPos.x);

			edit->map.Alpha = alphaColor;
			
		}
	}
}





#pragma region ButtonUI �Լ���

void UIButton::ButtonImageInit(int numIdx_, POINT imgIdx_)
{
	numIdx = numIdx_;
	imgIdx = imgIdx_;
}


void UIButton::Update()
{	
	Pos.x = ScreenPos.x + MAINCAM->Pos.x;		//����ķ�� �̵��ص� ������ǥ
	Pos.y = ScreenPos.y + MAINCAM->Pos.y;		//����ķ�� �̵��ص� ������ǥ
	col = CreateColRect(Pos, Scale);
}

#pragma endregion

