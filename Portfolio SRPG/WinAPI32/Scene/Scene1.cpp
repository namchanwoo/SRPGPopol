#include "stdafx.h"
#include "Scene1.h"
//정적멤버는 전역에서 초기화
int Scene1::LoadCount = 0;

Scene1::Scene1()
{
	   
    bg2 = new Image();
    bg2->init(_T("Dugeon.jpg"));
    bg2->Pos = Vector2(400.0f,300.0f);
    bg2->Scale = Vector2(800.0f,600.0f);
}

Scene1::~Scene1()
{
    SAFE_RELEASE(FadeBuffer);
}

HRESULT Scene1::init()
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

    bg = new TileMap();

    bg->init();

    //init후에 들어가는 이미지 순서로 인덱스가 매겨진다.
	

    bg->AddImage(_T("Tile.png"), 8, 6);

    bg->AddImage(_T("Tile2.png"), 11, 7);

	bg->AddImage(_T("HexTile.png"), 1, 1);




    //에디터에서 쓸 변수 초기값
    ImgIdxX = 0;
    ImgIdxY = 0;
    vecIdx = 0;
    TileState = TILE_NONE;
    Scale.x = 50.0f;
    Scale.y = 50.0f;
    MaxX = 10;
    MaxY = 10;
    LB.x = 0.0f;
    LB.y = 0.0f;

    //타일맵으로 쓸 배경

    

    return S_OK;
}

void Scene1::release()
{
    bg->release();
    SAFE_DELETE(bg);
}
void Scene1::update()
{
	//벡터 순회 버튼
	VecIdxButton();
	//타일맵이미지 버튼
	TileImgButton();
	//타일맵 크기 재조정 버튼
	ResizeButton();
	//세이브 로드 버튼
	TileMapSaveLoad();
    
	//왼쪽아래점 재조정
	if (ImGui::SliderFloat2("LB", LB, -MAXPOSITION, MAXPOSITION))
	{
		bg->GetRefLB() = LB;
	}
	//타일 전체 스케일
    if(ImGui::SliderFloat2("TileScale",Scale,1.0f, MAXSCALE))
    {
		bg->GetRefTileSize() = Scale;
        bg->InitPosition();
    }

	//마우스로 찍을 타일 상태값 재조정
	ImGui::InputInt("TileState", &TileState);

	//마우스가 ImGui 창 위에 없을때
	ImVec2 min = ImGui::GetWindowPos();
	ImVec2 max;
	max.x= min.x + ImGui::GetWindowSize().x;
	max.y= min.y + ImGui::GetWindowSize().y;
	if (!ImGui::IsMouseHoveringRect(min, max))
	{
        //cout << "창위에 있다"<<endl;
		//마우스클릭 이벤트
		POINT idx;
		if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
		{
			if (PtInTile(bg, g_MousePt, idx))
			{
				bg->GetRefTiles()[idx.x][idx.y].vecIdx = vecIdx;
				bg->GetRefTiles()[idx.x][idx.y].ImgIdx.x = ImgIdxX;
				bg->GetRefTiles()[idx.x][idx.y].ImgIdx.y = ImgIdxY;
				bg->GetRefTiles()[idx.x][idx.y].state = TileState;
				
			}
		}
	}
    //위에 쓰이기만할 알파값
    ImGui::SliderFloat("Alpha",&bg->GetRefAlpha(),0.0f,1.0f);
	
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

	bg2->update();
    bg->update();
}

void Scene1::render()
{
    //상수버퍼 업데이트 W값을 WBuffer에 전달
    g_DeviceContext->UpdateSubresource(FadeBuffer, 0, NULL, &Fade, 0, 0);
    //                            연결될레지스터 번호 0
    g_DeviceContext->PSSetConstantBuffers(1, 1, &FadeBuffer);
    bg2->render();
    bg->render();
}

void Scene1::VecIdxButton()
{
	int tempIdx = vecIdx;
	//이미지 넘기기
	if (ImGui::InputInt("VecIdx", &tempIdx))
	{
		if ((UINT)tempIdx > bg->GetRefvecTile().size() - 1)
		{
			vecIdx = 0;
		}
		else if (tempIdx < 0)
		{
			vecIdx = bg->GetRefvecTile().size() - 1;
		}
		else
		{
			vecIdx = tempIdx;
		}
	}
}
void Scene1::ResizeButton()
{
	if (ImGui::InputInt("MaxX", &MaxX))
	{
		//늘려야 되고
		if (bg->GetRefTileMax().x < MaxX)
		{
			//타일배열을 추가할수있는 비어있는 포인터
			bg->GetRefTiles().emplace_back();

			bg->GetRefTiles()[MaxX - 1].resize(MaxY);

			bg->GetRefTileMax().x = MaxX;
			bg->InitPosition();
		}
		//줄여야 됨
		if (bg->GetRefTileMax().x > MaxX)
		{
			bg->GetRefTiles()[MaxX].clear();
			bg->GetRefTiles()[MaxX].shrink_to_fit();
			bg->GetRefTiles().pop_back();
			bg->GetRefTileMax().x = MaxX;
		}
	}
	if (ImGui::InputInt("MaxY", &MaxY))
	{
		//늘려야 되고
		if (bg->GetRefTileMax().y < MaxY)
		{
			for (int i = 0; i < MaxX; i++)
			{
				bg->GetRefTiles()[i].emplace_back();
			}
			bg->GetRefTileMax().y = MaxY;
			bg->InitPosition();
		}
		//줄여야 됨
		if (bg->GetRefTileMax().y > MaxY)
		{
			for (int i = 0; i < MaxX; i++)
			{
				bg->GetRefTiles()[i].pop_back();
			}
			bg->GetRefTileMax().y = MaxY;
		}
	}
}
void Scene1::TileImgButton()
{
	//타일 이미지 먼저 받아오기
	Image* img = bg->GetRefvecTile()[vecIdx];

	ImVec2 size;
	size.x = 300.0f / (float)img->MaxFrameX;
	size.y = 300.0f / (float)img->MaxFrameY;
	ImVec2 LT, RB;

	int index = 0;
	for (UINT i = 0; i < img->MaxFrameY; i++)
	{
		for (UINT j = 0; j < img->MaxFrameX; j++)
		{
			if (j != 0)
			{
				ImGui::SameLine();
			}
			LT.x = 1.0f / img->MaxFrameX * j;
			LT.y = 1.0f / img->MaxFrameY * i;
			RB.x = 1.0f / img->MaxFrameX * (j + 1);
			RB.y = 1.0f / img->MaxFrameY * (i + 1);
			ID3D11ShaderResourceView* srv =
				bg->GetRefvecTile()
				[vecIdx]->srv;
			ImGui::PushID(index);
			if (ImGui::ImageButton((void*)srv, size, LT, RB))
			{
				ImgIdxX = j;
				ImgIdxY = i;
			}
			index++;
			ImGui::PopID();
		}
	}

}
void Scene1::TileMapSaveLoad()
{
	if (ImGui::Button("Save"))
	{
		ofstream fout;
		//ios::binary 이진법으로 쓸때
		//파일 열기    읽어줄파일        쓰기용 | 처음부터 
		fout.open("Save/TileMap.txt", ios::out | ios::trunc);
		//열렸니?
		//텍스트로 쓰는방법
		if (fout.is_open())
		{
			//가로세로 크기,갯수,왼쪽아래점
			fout << "TileMax " << MaxX << ' ' << MaxY << '\n';
			fout << "TileScale " << Scale.x << ' ' << Scale.y << '\n';
			fout << "LB " << LB.x << ' ' << LB.y << '\n';

			//타일마다 x,y인덱스 ,이미지 인덱스,상태
			for (int i = 0; i < MaxX; i++)
			{
				for (int j = 0; j < MaxY; j++)
				{
					//현재 타일의 인덱스
					fout << i << ' ' << j << endl;
					//벡터 인덱스
					fout << "vecIdx " << bg->GetRefTiles()[i][j].vecIdx << '\n';

					//분할된 이미지 인덱스
					fout << "State " << bg->GetRefTiles()[i][j].state << '\n';

					//분할된 이미지 인덱스
					fout << "ImgIdx " << bg->GetRefTiles()[i][j].ImgIdx.x <<
						' ' << bg->GetRefTiles()[i][j].ImgIdx.y << '\n';
				}
			}
			//파일 닫기
			fout.flush();
			fout.close();
		}
		else
		{
			assert(0 && "세이브 실패");
		}


	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		ifstream fin;
		//                          읽기전용
		fin.open("Save/TileMap.txt", ios::in);
		//열렸니?
		//텍스트로 읽는방법
		if (fin.is_open())
		{
			//먼저 지우고
			bg->ClearTile();

			//가로세로 크기,갯수,왼쪽아래점
			string temp;
			fin >> temp;
			fin >> MaxX;
			fin >> MaxY;

			bg->GetRefTileMax().x = MaxX;
			bg->GetRefTileMax().y = MaxY;

			fin >> temp;
			fin >> Scale.x;
			fin >> Scale.y;

			bg->GetRefTileSize() = Scale;

			fin >> temp;
			fin >> LB.x;
			fin >> LB.y;

			bg->GetRefLB() = LB;

			//Tile갯수 다시만들기
			bg->ResizeTile();
			//위치 다시잡기
			bg->InitPosition();

			//타일 읽어오기
			for (int i = 0; i < MaxX; i++)
			{
				for (int j = 0; j < MaxY; j++)
				{
					//현재 타일의 인덱스
					int temp2;
					fin >> temp2;
					fin >> temp2;

					fin >> temp;
					fin >> bg->GetRefTiles()[i][j].vecIdx;

					fin >> temp;
					fin >> bg->GetRefTiles()[i][j].state;

					fin >> temp;
					fin >> bg->GetRefTiles()[i][j].ImgIdx.x;
					fin >> bg->GetRefTiles()[i][j].ImgIdx.y;
				}
			}
			//파일 닫기
			fin.clear();
			fin.close();
		}
		else
		{
			assert(0 && "로드 실패");
		}
	}
}