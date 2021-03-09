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

//헥스 곱하기
Hex TileMapEdit::hex_multiply(Hex a, int k)
{
	return Hex(a.q * k, a.r * k, a.s * k);
}

//핵스 거리구하기
int TileMapEdit::hex_length(Hex hex)
{
	return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

//핵스 사이 거리 구하기
int TileMapEdit::hex_distance(Hex a, Hex b)
{
	return hex_length(hex_subtract(a, b));
}


//방향을 갖고오는 함수
Hex TileMapEdit::hex_direction(int direction /* 0 to 5 */)
{
	//0보다 커야하고 6보다는 작아야한다.
	assert(0 <= direction && direction < 6);
	return hex_directions[direction];
}

//핵스의 이웃구하기
Hex TileMapEdit::hex_neighbor(Hex hex, int direction) {
	return hex_add(hex, hex_direction(direction));
}



//헥사를 픽셀화면으로
Vector2 TileMapEdit::hex_to_pixel(Layout layout, Hex h)
{
	const Orientation& M = layout.orientation;
	double x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
	double y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
	return Vector2(x + layout.origin.x, y + layout.origin.y);
}

//픽셀을 핵사화면으로
FractionalHex TileMapEdit::pixel_to_hex(Layout layout, Vector2 p)
{

	const Orientation& M = layout.orientation;
	Vector2 pt = Vector2((p.x - layout.origin.x)  / layout.size.x, (p.y - layout.origin.y)  / layout.size.y);
	double q = M.b0 * pt.x + M.b1 * pt.y;
	double r = M.b2 * pt.x + M.b3 * pt.y ;
	return FractionalHex(q, r, -q - r);

}

//핵사 모서리 구하기
Vector2 TileMapEdit::hex_corner_offset(Layout layout, int corner) {
	Vector2 size = layout.size;
	double angle = 2.0 * M_PI *	(layout.orientation.start_angle + corner) / 6;
	return Vector2(size.x * cos(angle), size.y * sin(angle));
}

//벡터포인트 반환 코너구해주는 함수
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

//분수핵사를 반올림해주는 함수
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

//선형보간 함수
float TileMapEdit::lerp(double a, double b, double t)
{
	return a * (1 - t) + b * t;
	/* better for floating point precision than
	   a + (b - a) * t, which is what I usually write */
	   /*부동 소수점 정밀도가 더 좋습니다.
		   a + (b - a) * t, 내가 보통 쓰는 것 */
}

//부동소수 핵사를 선형보간하고 반환하는 함수
FractionalHex TileMapEdit::hex_lerp(Hex a, Hex b, double t) {
	return FractionalHex(lerp(a.q, b.q, t), lerp(a.r, b.r, t), lerp(a.s, b.s, t));
}

////부동소수 핵사를 선형보간하고 반환하는 함수
//FractionalHex TileMapEdit::hex_lerp(FractionalHex a, FractionalHex b, double t) {
//	return FractionalHex(lerp(a.q, b.q, t),
//		lerp(a.r, b.r, t),
//		lerp(a.s, b.s, t));
//}

//핵사 라인 그리기
vector<Hex> TileMapEdit::hex_linedraw(Hex a, Hex b) {
	int N = hex_distance(a, b);
	vector<Hex> results = {};
	double step = 1.0 / max(N, 1);
	for (int i = 0; i <= N; i++) {
		results.push_back(hex_round(hex_lerp(a, b, step * i)));
	}
	return results;
}


////핵사 라인 그리기 (러프의 오류를 막아주는 것
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

//왼쪽회전
Hex TileMapEdit::hex_rotate_left(Hex a)
{
	return Hex(-a.s, -a.q, -a.r);
}

//오른쪽 회전
Hex TileMapEdit::hex_rotate_right(Hex a)
{
	return Hex(-a.r, -a.s, -a.q);
}


//오프셋 좌표로 변환
OffsetCoord TileMapEdit::cube_to_roffset(int offset, Hex h) {
	assert(offset == EVEN || offset == ODD);
	int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
	int row = h.r;
	return OffsetCoord(col, row);
}

//큐브좌표로 변환
Hex TileMapEdit::roffset_to_cube(int offset, OffsetCoord h) {
	assert(offset == EVEN || offset == ODD);
	int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
	int r = h.row;
	int s = -q - r;
	return Hex(q, r, s);
}

//검증된 오프셋좌표변환
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

	//타일 재조정
	ResizeTile();

	//타일들 위치와 
	InitPosition();

}

void RectangularPointyTopMap::Update()
{




}

void RectangularPointyTopMap::ResizeTile()
{
	//벡터크기를 맥스 사이즈만큼 잡는다
	Tiles.resize(width);
	for (LONG i = 0; i < width; i++)
	{
		Tiles[i].resize(height);
	}

}

void RectangularPointyTopMap::InitPosition()
{

	//오프셋과 핵사좌표를 잡아주기
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
	//수평거리
	float w = sqrt(3)*size;
	//수직거리
	float h = 2 * size;

	//전체적으로 타일 위치 다시잡기
	for (LONG i = 0; i < width; i++)
	{
		for (LONG j = 0; j < height; j++)
		{
			//홀수 행 일때 포지션 잡아주기
			if (j % 2 != 0)
			{
				Tiles[i][j].Pos.x = 0 + (w * i)+ LB.x;
				Tiles[i][j].Pos.y = size + (h * 3 / 4 * j)+ LB.y;
				Tiles[i][j].offCoord = OffsetCoord(i, j);
				Tiles[i][j].cubeCoord = hTiles[Tiles[i][j].offCoord];
			}
			//짝수 행 일때 포지션 잡아주기
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

	//벡터크기를 맥스 사이즈만큼 지운다
	for (LONG i = 0; i < width; i++)
	{
		Tiles[i].clear();
		Tiles[i].shrink_to_fit();
	}
	Tiles.clear();
	Tiles.shrink_to_fit();


	//벡터에 추가한 이미지 지우기
	for (UINT i = 0; i < tileImg.size(); i++)
	{
		SAFE_DELETE(tileImg[i]);
	}
	//clear 했을때 내부버퍼(capacity)가 안지워짐
	tileImg.clear();
	//그걸지워주는함수
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





#pragma region 일단 보류중인 함수들
//
/// 오프셋 좌표로 변환
//OffsetCoord TileMapEdit::roffset_from_cube(int offset, Hex h)
//{
//	assert(offset == EVEN || offset == ODD);
//	int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
//	int row = h.r;
//	return OffsetCoord(col, row);
//}
//
////큐브좌표로 변환
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


#pragma region  연산자 재정의

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
//		//늘려야 되고
//		if (map->width < width)
//		{					
//			map->Tiles.emplace_back();
//			map->Tiles[width - 1].resize(height);
//			map->width = width;
//			map->InitPosition();
//		}
//
//		//줄여야 됨
//		if (map->width > width)
//		{
//			map->Tiles[width].clear();
//			map->Tiles[width].shrink_to_fit();
//			map->Tiles.pop_back();
//			map->width = width;
//		}
//
//		//늘려야 되고
//		if (map->height < height)
//		{
//			for (int i = 0; i < width; i++)
//			{
//				map->Tiles[i].emplace_back();				
//			}
//			map->height = height;
//			map->InitPosition();
//		}
//		//줄여야 됨
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
//	//늘려야 되고
//	if (map->TileSize != TileSize)
//	{	
//		map->TileSize = TileSize;
//		map->InitPosition();
//	}
//}

