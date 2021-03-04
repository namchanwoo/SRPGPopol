#include "stdafx.h"
#include "Scene1.h"
//��������� �������� �ʱ�ȭ
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
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &FadeBuffer);
        assert(SUCCEEDED(hr));
    }

    bg = new TileMap();

    bg->init();

    //init�Ŀ� ���� �̹��� ������ �ε����� �Ű�����.
	

    bg->AddImage(_T("Tile.png"), 8, 6);

    bg->AddImage(_T("Tile2.png"), 11, 7);

	bg->AddImage(_T("HexTile.png"), 1, 1);




    //�����Ϳ��� �� ���� �ʱⰪ
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

    //Ÿ�ϸ����� �� ���

    

    return S_OK;
}

void Scene1::release()
{
    bg->release();
    SAFE_DELETE(bg);
}
void Scene1::update()
{
	//���� ��ȸ ��ư
	VecIdxButton();
	//Ÿ�ϸ��̹��� ��ư
	TileImgButton();
	//Ÿ�ϸ� ũ�� ������ ��ư
	ResizeButton();
	//���̺� �ε� ��ư
	TileMapSaveLoad();
    
	//���ʾƷ��� ������
	if (ImGui::SliderFloat2("LB", LB, -MAXPOSITION, MAXPOSITION))
	{
		bg->GetRefLB() = LB;
	}
	//Ÿ�� ��ü ������
    if(ImGui::SliderFloat2("TileScale",Scale,1.0f, MAXSCALE))
    {
		bg->GetRefTileSize() = Scale;
        bg->InitPosition();
    }

	//���콺�� ���� Ÿ�� ���°� ������
	ImGui::InputInt("TileState", &TileState);

	//���콺�� ImGui â ���� ������
	ImVec2 min = ImGui::GetWindowPos();
	ImVec2 max;
	max.x= min.x + ImGui::GetWindowSize().x;
	max.y= min.y + ImGui::GetWindowSize().y;
	if (!ImGui::IsMouseHoveringRect(min, max))
	{
        //cout << "â���� �ִ�"<<endl;
		//���콺Ŭ�� �̺�Ʈ
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
    //���� ���̱⸸�� ���İ�
    ImGui::SliderFloat("Alpha",&bg->GetRefAlpha(),0.0f,1.0f);
	
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

	bg2->update();
    bg->update();
}

void Scene1::render()
{
    //������� ������Ʈ W���� WBuffer�� ����
    g_DeviceContext->UpdateSubresource(FadeBuffer, 0, NULL, &Fade, 0, 0);
    //                            ����ɷ������� ��ȣ 0
    g_DeviceContext->PSSetConstantBuffers(1, 1, &FadeBuffer);
    bg2->render();
    bg->render();
}

void Scene1::VecIdxButton()
{
	int tempIdx = vecIdx;
	//�̹��� �ѱ��
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
		//�÷��� �ǰ�
		if (bg->GetRefTileMax().x < MaxX)
		{
			//Ÿ�Ϲ迭�� �߰��Ҽ��ִ� ����ִ� ������
			bg->GetRefTiles().emplace_back();

			bg->GetRefTiles()[MaxX - 1].resize(MaxY);

			bg->GetRefTileMax().x = MaxX;
			bg->InitPosition();
		}
		//�ٿ��� ��
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
		//�÷��� �ǰ�
		if (bg->GetRefTileMax().y < MaxY)
		{
			for (int i = 0; i < MaxX; i++)
			{
				bg->GetRefTiles()[i].emplace_back();
			}
			bg->GetRefTileMax().y = MaxY;
			bg->InitPosition();
		}
		//�ٿ��� ��
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
	//Ÿ�� �̹��� ���� �޾ƿ���
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
		//ios::binary ���������� ����
		//���� ����    �о�������        ����� | ó������ 
		fout.open("Save/TileMap.txt", ios::out | ios::trunc);
		//���ȴ�?
		//�ؽ�Ʈ�� ���¹��
		if (fout.is_open())
		{
			//���μ��� ũ��,����,���ʾƷ���
			fout << "TileMax " << MaxX << ' ' << MaxY << '\n';
			fout << "TileScale " << Scale.x << ' ' << Scale.y << '\n';
			fout << "LB " << LB.x << ' ' << LB.y << '\n';

			//Ÿ�ϸ��� x,y�ε��� ,�̹��� �ε���,����
			for (int i = 0; i < MaxX; i++)
			{
				for (int j = 0; j < MaxY; j++)
				{
					//���� Ÿ���� �ε���
					fout << i << ' ' << j << endl;
					//���� �ε���
					fout << "vecIdx " << bg->GetRefTiles()[i][j].vecIdx << '\n';

					//���ҵ� �̹��� �ε���
					fout << "State " << bg->GetRefTiles()[i][j].state << '\n';

					//���ҵ� �̹��� �ε���
					fout << "ImgIdx " << bg->GetRefTiles()[i][j].ImgIdx.x <<
						' ' << bg->GetRefTiles()[i][j].ImgIdx.y << '\n';
				}
			}
			//���� �ݱ�
			fout.flush();
			fout.close();
		}
		else
		{
			assert(0 && "���̺� ����");
		}


	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		ifstream fin;
		//                          �б�����
		fin.open("Save/TileMap.txt", ios::in);
		//���ȴ�?
		//�ؽ�Ʈ�� �д¹��
		if (fin.is_open())
		{
			//���� �����
			bg->ClearTile();

			//���μ��� ũ��,����,���ʾƷ���
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

			//Tile���� �ٽø����
			bg->ResizeTile();
			//��ġ �ٽ����
			bg->InitPosition();

			//Ÿ�� �о����
			for (int i = 0; i < MaxX; i++)
			{
				for (int j = 0; j < MaxY; j++)
				{
					//���� Ÿ���� �ε���
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
			//���� �ݱ�
			fin.clear();
			fin.close();
		}
		else
		{
			assert(0 && "�ε� ����");
		}
	}
}