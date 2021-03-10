#include "stdafx.h"
#include "TileMapEdit.h"

Hex TileMapEdit::hex_add(Hex a, Hex b)
{
	return Hex(a.q + b.q, a.r + b.r, a.s + b.s);
}

Hex TileMapEdit::hex_subtract(Hex a, Hex b)
{
	return Hex(a.q - b.q, a.r - b.r, a.s - b.s);
}

//�� ���ϱ�
Hex TileMapEdit::hex_multiply(Hex a, int k)
{
	return Hex(a.q * k, a.r * k, a.s * k);
}

//�ٽ� �Ÿ����ϱ�
int TileMapEdit::hex_length(Hex hex)
{
	return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

//�ٽ� ���� �Ÿ� ���ϱ�
int TileMapEdit::hex_distance(Hex a, Hex b)
{
	return hex_length(hex_subtract(a, b));
}


//������ ������� �Լ�
Hex TileMapEdit::hex_direction(int direction /* 0 to 5 */)
{
	//0���� Ŀ���ϰ� 6���ٴ� �۾ƾ��Ѵ�.
	assert(0 <= direction && direction < 6);
	return hex_directions[direction];
}

//�ٽ��� �̿����ϱ�
Hex TileMapEdit::hex_neighbor(Hex hex, int direction) {
	return hex_add(hex, hex_direction(direction));
}



//��縦 �ȼ�ȭ������
Vector2 TileMapEdit::hex_to_pixel(Layout layout, Hex h)
{
	const Orientation& M = layout.orientation;
	double x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
	double y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
	return Vector2(x + layout.origin.x, y + layout.origin.y);
}

//�ȼ��� �ٻ�ȭ������
FractionalHex TileMapEdit::pixel_to_hex(Layout layout, Vector2 p)
{

	const Orientation& M = layout.orientation;
	Vector2 pt = Vector2((p.x - layout.origin.x)  / layout.size.x, (p.y - layout.origin.y)  / layout.size.y);
	double q = M.b0 * pt.x + M.b1 * pt.y;
	double r = M.b2 * pt.x + M.b3 * pt.y ;
	return FractionalHex(q, r, -q - r);

}

//�ٻ� �𼭸� ���ϱ�
Vector2 TileMapEdit::hex_corner_offset(Layout layout, int corner) {
	Vector2 size = layout.size;
	double angle = 2.0 * M_PI *	(layout.orientation.start_angle + corner) / 6;
	return Vector2(size.x * cos(angle), size.y * sin(angle));
}

//��������Ʈ ��ȯ �ڳʱ����ִ� �Լ�
vector<Vector2> TileMapEdit::polygon_corners(Layout layout, Hex h)
{
	vector<Vector2> corners = {};
	Vector2 center = hex_to_pixel(layout, h);
	for (int i = 0; i < 6; i++)
	{
		Vector2 offset = hex_corner_offset(layout, i);
		corners.push_back(Vector2(center.x + offset.x, center.y + offset.y));
	}
	return corners;
}

//�м��ٻ縦 �ݿø����ִ� �Լ�
Hex TileMapEdit::hex_round(FractionalHex h) {
	int q = int(round(h.q));
	int r = int(round(h.r));
	int s = int(round(h.s));
	double q_diff = abs(q - h.q);
	double r_diff = abs(r - h.r);
	double s_diff = abs(s - h.s);
	if (q_diff > r_diff && q_diff > s_diff)
	{
		q = -r - s;
	}
	else if (r_diff > s_diff)
	{
		r = -q - s;
	}
	else
	{
		s = -q - r;
	}
	return Hex(q, r, s);
}

//�������� �Լ�
float TileMapEdit::lerp(double a, double b, double t)
{
	return a * (1 - t) + b * t;
	/* better for floating point precision than
	   a + (b - a) * t, which is what I usually write */
	   /*�ε� �Ҽ��� ���е��� �� �����ϴ�.
		   a + (b - a) * t, ���� ���� ���� �� */
}

//�ε��Ҽ� �ٻ縦 ���������ϰ� ��ȯ�ϴ� �Լ�
FractionalHex TileMapEdit::hex_lerp(Hex a, Hex b, double t) {
	return FractionalHex(lerp(a.q, b.q, t), lerp(a.r, b.r, t), lerp(a.s, b.s, t));
}

////�ε��Ҽ� �ٻ縦 ���������ϰ� ��ȯ�ϴ� �Լ�
//FractionalHex TileMapEdit::hex_lerp(FractionalHex a, FractionalHex b, double t) {
//	return FractionalHex(lerp(a.q, b.q, t),
//		lerp(a.r, b.r, t),
//		lerp(a.s, b.s, t));
//}

//�ٻ� ���� �׸���
vector<Hex> TileMapEdit::hex_linedraw(Hex a, Hex b) {
	int N = hex_distance(a, b);
	vector<Hex> results = {};
	double step = 1.0 / max(N, 1);
	for (int i = 0; i <= N; i++) {
		results.push_back(hex_round(hex_lerp(a, b, step * i)));
	}
	return results;
}


////�ٻ� ���� �׸��� (������ ������ �����ִ� ��
//vector<Hex> TileMapEdit::hex_linedraw(Hex a, Hex b) {
//	int N = hex_distance(a, b);
//	FractionalHex a_nudge(a.q + 1e-6, a.r + 1e-6, a.s - 2e-6);
//	FractionalHex b_nudge(b.q + 1e-6, b.r + 1e-6, b.s - 2e-6);
//	vector<Hex> results = {};
//	double step = 1.0 / max(N, 1);
//	for (int i = 0; i <= N; i++) {
//		results.push_back(
//			hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
//	}
//	return results;
//}

//����ȸ��
Hex TileMapEdit::hex_rotate_left(Hex a)
{
	return Hex(-a.s, -a.q, -a.r);
}

//������ ȸ��
Hex TileMapEdit::hex_rotate_right(Hex a)
{
	return Hex(-a.r, -a.s, -a.q);
}


//������ ��ǥ�� ��ȯ
OffsetCoord TileMapEdit::cube_to_roffset(int offset, Hex h) {
	assert(offset == EVEN || offset == ODD);
	int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
	int row = h.r;
	return OffsetCoord(col, row);
}

//ť����ǥ�� ��ȯ
Hex TileMapEdit::roffset_to_cube(int offset, OffsetCoord h) {
	assert(offset == EVEN || offset == ODD);
	int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
	int r = h.row;
	int s = -q - r;
	return Hex(q, r, s);
}

//������ ��������ǥ��ȯ
OffsetCoord TileMapEdit::cube_to_oddr(Hex a)
{
	int col = a.q + (a.r - (a.r & 1)) / 2;
	int row = a.r;
	return OffsetCoord(col, row);
}


HRESULT TileMapEdit::Init()
{
	map.Init();

	layout.orientation = layout_pointy;
	layout.size = Vector2(35, 35);
	layout.origin = Vector2(map.TileSize.x*0.5f, map.TileSize.y*0.5f);




	return S_OK;
}

void TileMapEdit::Release()
{
	map.ClearTile();
}

void TileMapEdit::Update()
{	

}

void TileMapEdit::Render()
{
	map.Reneder();
}


void RectangularPointyTopMap::AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs, string ps)
{
	Image* temp = new Image();
	temp->init(file, vs, ps);
	temp->Scale.x = TileSize.x;
	temp->Scale.y = TileSize.y;

	temp->MaxFrameX = MaxFrameX;
	temp->MaxFrameY = MaxFrameY;
	tileImg.emplace_back(temp);
}

void RectangularPointyTopMap::Init()
{
	width = 5;
	height = 5;

	LB = Vector2(0.0f, 0.0f);
	TileSize = Vector2(60.5f, 70.0f);

	
	for (int r = 0; r < 30; r++) {
		int r_offset = floor(r / 2); // or r>>1
		for (int q = -r_offset; q < 30 - r_offset; q++) 
		{
			
			Hex cHex = Hex(q, r, -q - r);
			OffsetCoord cofset = OffsetCoord{ r,q + (r >> 1) };
			map.insert(make_pair(cofset, cHex));
		}
	}


	

	//Ÿ�� ������
	ResizeTile();

	//Ÿ�ϵ� ��ġ
	TilePositoinInit();

}



void RectangularPointyTopMap::ResizeTile()
{
	//����ũ�⸦ �ƽ� �����ŭ ��´�
	for (int r = 0; r < height; r++)
	{
		tiles.emplace_back(width);
	}

	/*for (auto it = map.begin(); it != map.end(); it++)
	{
		int q = (*it).q;
		int r = (*it).r;
		tiles[r][q + (r >> 1)] = *it;
	}*/



}

void RectangularPointyTopMap::TilePositoinInit()
{
	   
	float size = TileSize.y *0.5f;
	//����Ÿ�
	float w = sqrt(3)*size;
	//�����Ÿ�
	float h = 2 * size;

	

	//��ü������ Ÿ�� ��ġ �ٽ����
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//Ȧ�� ��
			if (i % 2 != 0)
			{				
				tiles[i][j] = map[OffsetCoord(i, j)];
				tiles[i][j].Pos.x = TileSize.x + (w * j) + LB.x;
				tiles[i][j].Pos.y = size + (h * 3 / 4 * i) + LB.y;
			}
			//¦�� ��
			else
			{
				tiles[i][j] = map[OffsetCoord(i, j)];
				tiles[i][j].Pos.x = TileSize.x *0.5f + (w * j) + LB.x;
				tiles[i][j].Pos.y = size + (h * 3 / 4 *i) + LB.y;				
			}
		}
	}

	

}

void RectangularPointyTopMap::ClearTile()
{

	//����ũ�⸦ �ƽ� �����ŭ �����
	for (int i = 0; i < height; i++)
	{
		tiles[i].clear();
		tiles[i].shrink_to_fit();
	}
	tiles.clear();
	tiles.shrink_to_fit();

	//���Ϳ� �߰��� �̹��� �����
	for (UINT i = 0; i < tileImg.size(); i++)
	{
		SAFE_DELETE(tileImg[i]);
	}
	//clear ������ ���ι���(capacity)�� ��������
	tileImg.clear();
	//�װ������ִ��Լ�
	tileImg.shrink_to_fit();
}

void RectangularPointyTopMap::Reneder()
{
	for (UINT i = 0; i < height; i++)
	{
		for (UINT j = 0; j < width; j++)
		{
			UINT vecindex = 0;
			tileImg[vecindex]->Pos = tiles[i][j].Pos;
			tileImg[vecindex]->Scale.x = TileSize.x;
			tileImg[vecindex]->Scale.y = TileSize.y;
			tileImg[vecindex]->update();			
			

			if (tiles[i][j].check == true)
			{
				tileImg[vecindex]->color = Color(0.9f,0.15f,0.11f, Alpha);
			}
			else
			{
				tileImg[vecindex]->color = Color(1, 1, 1, Alpha);
			}

			if (tiles[i][j].tileState == TILESTATE::TILE_WALL)
			{
				tileImg[vecindex]->CurrentFrameX = 1;
				tileImg[vecindex]->CurrentFrameY = tiles[i][j].imgIdx.y;
			}
			else
			{				
				tileImg[vecindex]->CurrentFrameX = tiles[i][j].imgIdx.x;
				tileImg[vecindex]->CurrentFrameY = tiles[i][j].imgIdx.y;
			}    

			tileImg[vecindex]->render();

		}
	}
}




//���� ���Ÿ�� �ȿ� �ִ°�?
bool TileMapEdit::PtInHexTile(Vector2 Pt, OUT OffsetCoord& coord)//�ۿ� ������ �ٲ�
{

	COL_RECT col;
	col.left = 0;
	col.bottom = 0;
	col.right = col.left + map.width * (map.TileSize.x + map.LB.x) + map.TileSize.x;
	col.top = col.bottom + map.height * (map.TileSize.y + map.LB.y) + map.TileSize.y;
	


	//�������� �̵��Ѹ�ŭ ���ֱ�
	Pt -= map.LB;


	/*Vector2 testPos = hex_to_pixel(layout, Hex(1, 1, -2));
	Vector2 resultPos = map.tiles[1][1].Pos;

	Vector2 testPos2 = hex_to_pixel(layout, Hex(1, 2, -3));
	Vector2 resultPos2 = map.tiles[2][2].Pos;

	Vector2 testPos3 = hex_to_pixel(layout, Hex(4, 8, -12));
	Vector2 resultPos3 = map.tiles[8][8].Pos;

	Vector2 testPos4 = hex_to_pixel(layout, Hex(7, 14, -21));
	Vector2 resultPos4 = map.tiles[14][14].Pos;*/

	//���콺�� ����Ʈ�� �ٻ���ǥ�� ��ȯ (��ȯ �������� doubleŸ���� ���� ��ȯ)
	FractionalHex curHex = pixel_to_hex(layout, Pt);
	//�÷�Ÿ���� ��縦 ��ƮŸ���� ���� ��ȯ
	Hex curHex2 = hex_round(curHex);

	//coord = edit->cube_to_roffset(+1, curHex2);

	//�ش� ť����ǥ�� ��������ǥ ��ȯ �� �Է�
	OffsetCoord curCoord = cube_to_roffset(-1, curHex2);

	//�ο찡 �� ���� ��

	if (curCoord.col > map.height - 1 || curCoord.row > map.width - 1)
	{
		coord = OffsetCoord();
	}
	else
	{
		coord = curCoord;
		map.tiles[coord.col][coord.row].CheckSwiching();
		if (map.tiles[coord.col][coord.row].check == true)
		{
			selectTiles.insert(make_pair(coord, &map.tiles[coord.col][coord.row]));
		}
		else
		{
			//�޽��ε� ���� �����Ѵٸ�
			if (selectTiles.find(coord) != selectTiles.end())
			{
				selectTiles.erase(coord);
			}
		}
	}

	if (!PtinRect(Pt, col))
	{
		//Ÿ�� �ۿ� �ִ°Ŵ�.
		return false;
	}
	//q = x , r=z ,s =y
	return true;
}



#pragma region  ������ ������

bool operator==(const Hex & a, const Hex & b)
{
	return a.q == b.q && a.r == b.r && a.s == b.s;
}

bool operator!=(const Hex & a, const Hex & b)
{
	return !(a == b);
}

bool operator==(const OffsetCoord& p1, const OffsetCoord& p2) {
	return p1.col == p2.col && p1.row == p2.row;
}

#pragma endregion

