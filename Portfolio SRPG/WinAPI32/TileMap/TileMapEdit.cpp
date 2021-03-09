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
	layout.size = Vector2(35.0f, 35.0f);
	layout.origin = Vector2(map.TileSize.x*0.5f,map.TileSize.y*0.5f);

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
	width = 10;
	height = 7;

	LB = Vector2(0.0f, 0.0f);
	TileSize = Vector2(60.5f, 70.0f);

	//Ÿ�� ������
	ResizeTile();

	//Ÿ�ϵ� ��ġ�� 
	InitPosition();

}

void RectangularPointyTopMap::Update()
{




}

void RectangularPointyTopMap::ResizeTile()
{
	//����ũ�⸦ �ƽ� �����ŭ ��´�
	Tiles.resize(width);
	for (LONG i = 0; i < width; i++)
	{
		Tiles[i].resize(height);
	}

}

void RectangularPointyTopMap::InitPosition()
{

	//�����°� �ٻ���ǥ�� ����ֱ�
	for (int r = 0; r < height; r++) {
		int r_offset = floor((r) / 2); // or r>>1
		for (int q = -r_offset; q < width - r_offset; q++)
		{
			OffsetCoord off(q + (r >> 1), r);
			int s = -q - r;
			Hex hex(q, r, s);
			hTiles.insert(make_pair(off, hex));
		}
	}

	float size = TileSize.y *0.5f;
	//����Ÿ�
	float w = sqrt(3)*size;
	//�����Ÿ�
	float h = 2 * size;

	//��ü������ Ÿ�� ��ġ �ٽ����
	for (LONG i = 0; i < width; i++)
	{
		for (LONG j = 0; j < height; j++)
		{
			//Ȧ�� �� �϶� ������ ����ֱ�
			if (j % 2 != 0)
			{
				Tiles[i][j].Pos.x = 0 + (w * i)+ LB.x;
				Tiles[i][j].Pos.y = size + (h * 3 / 4 * j)+ LB.y;
				Tiles[i][j].offCoord = OffsetCoord(i, j);
				Tiles[i][j].cubeCoord = hTiles[Tiles[i][j].offCoord];
			}
			//¦�� �� �϶� ������ ����ֱ�
			else
			{
				Tiles[i][j].Pos.x = TileSize.x*0.5f + (w * i)+LB.x;
				Tiles[i][j].Pos.y = size + (h * 3 / 4 * j)+ LB.y;
				Tiles[i][j].offCoord = OffsetCoord(i, j);
				Tiles[i][j].cubeCoord = hTiles[Tiles[i][j].offCoord];
			}
		}
	}

}

void RectangularPointyTopMap::ClearTile()
{

	//����ũ�⸦ �ƽ� �����ŭ �����
	for (LONG i = 0; i < width; i++)
	{
		Tiles[i].clear();
		Tiles[i].shrink_to_fit();
	}
	Tiles.clear();
	Tiles.shrink_to_fit();


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
	for (UINT i = 0; i < width; i++)
	{
		for (UINT j = 0; j < height; j++)
		{
			UINT vecindex = Tiles[i][j].vecIdx;
			tileImg[vecindex]->Pos = Tiles[i][j].Pos;
			tileImg[vecindex]->Scale.x = TileSize.x;
			tileImg[vecindex]->Scale.y = TileSize.y;
			tileImg[vecindex]->update();
			
			if (Tiles[i][j].check == true)
			{
				tileImg[vecindex]->color = Color(0.9f,0.15f,0.11f, Alpha);
			}
			else
			{
				tileImg[vecindex]->color = Color(1, 1, 1, Alpha);
			}

			if (Tiles[i][j].tileState == TILESTATE::TILE_WALL)
			{
				tileImg[vecindex]->CurrentFrameX = 1;
				tileImg[vecindex]->CurrentFrameY = Tiles[i][j].imgIdx.y;
			}
			else
			{				
				tileImg[vecindex]->CurrentFrameX = Tiles[i][j].imgIdx.x;
				tileImg[vecindex]->CurrentFrameY = Tiles[i][j].imgIdx.y;
			}    

			tileImg[vecindex]->render();
		}
	}
}





#pragma region �ϴ� �������� �Լ���
//
/// ������ ��ǥ�� ��ȯ
//OffsetCoord TileMapEdit::roffset_from_cube(int offset, Hex h)
//{
//	assert(offset == EVEN || offset == ODD);
//	int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
//	int row = h.r;
//	return OffsetCoord(col, row);
//}
//
////ť����ǥ�� ��ȯ
//Hex TileMapEdit::roffset_to_cube(int offset, OffsetCoord h)
//{
//	assert(offset == EVEN || offset == ODD);
//	int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
//	int r = h.row;
//	int s = -q - r;
//	return Hex(q, r, s);
////}
//namespace std {
//	template <> struct hash<Hex> {
//		size_t operator()(const Hex& h) const {
//			hash<int> int_hash;
//			size_t hq = int_hash(h.q);
//			size_t hr = int_hash(h.r);
//			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
//		}
//	};
//}
#pragma endregion


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


//
//void EditManager::TileIndexResize()
//{
//
//		//�÷��� �ǰ�
//		if (map->width < width)
//		{					
//			map->Tiles.emplace_back();
//			map->Tiles[width - 1].resize(height);
//			map->width = width;
//			map->InitPosition();
//		}
//
//		//�ٿ��� ��
//		if (map->width > width)
//		{
//			map->Tiles[width].clear();
//			map->Tiles[width].shrink_to_fit();
//			map->Tiles.pop_back();
//			map->width = width;
//		}
//
//		//�÷��� �ǰ�
//		if (map->height < height)
//		{
//			for (int i = 0; i < width; i++)
//			{
//				map->Tiles[i].emplace_back();				
//			}
//			map->height = height;
//			map->InitPosition();
//		}
//		//�ٿ��� ��
//		if (map->height > height)
//		{
//			for (int i = 0; i < width; i++)
//			{
//				map->Tiles[i].pop_back();
//			}
//			map->height = height;
//		}
//}
//
//void EditManager::TileInitPosition()
//{
//	map->InitPosition();
//}
//
//
//
//void EditManager::TileScaleResize()
//{
//	//�÷��� �ǰ�
//	if (map->TileSize != TileSize)
//	{	
//		map->TileSize = TileSize;
//		map->InitPosition();
//	}
//}

