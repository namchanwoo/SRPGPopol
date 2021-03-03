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
Point TileMapEdit::hex_to_pixel(Layout layout, Hex h)
{
	const Orientation& M = layout.orientation;
	double x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
	double y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
	return Point(x + layout.origin.x, y + layout.origin.y);
}

//픽셀을 핵사화면으로
FractionalHex TileMapEdit::pixel_to_hex(Layout layout, Point p)
{
	
	const Orientation& M = layout.orientation;
	Point pt = Point((p.x - layout.origin.x) / layout.size.x, (p.y - layout.origin.y) / layout.size.y);
	double q = M.b0 * pt.x + M.b1 * pt.y;
	double r = M.b2 * pt.x + M.b3 * pt.y;
	return FractionalHex(q, r, -q - r);

}

//핵사 모서리 구하기
Point TileMapEdit::hex_corner_offset(Layout layout, int corner) {
	Point size = layout.size;
	double angle = 2.0 * M_PI *	(layout.orientation.start_angle + corner) / 6;
	return Point(size.x * cos(angle), size.y * sin(angle));
}

//벡터포인트 반환 코너구해주는 함수
vector<Point> TileMapEdit::polygon_corners(Layout layout, Hex h)
{
	vector<Point> corners = {};
	Point center = hex_to_pixel(layout, h);
	for (int i = 0; i < 6; i++)
	{
		Point offset = hex_corner_offset(layout, i);
		corners.push_back(Point(center.x + offset.x, center.y + offset.y));
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
	return FractionalHex(lerp(a.q, b.q, t),	lerp(a.r, b.r, t),	lerp(a.s, b.s, t));
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
OffsetCoord TileMapEdit::qoffset_from_cube(int offset, Hex h) {
	assert(offset == EVEN || offset == ODD,"offset은 EVEN(+1) 또는 ODD(-1)이여야 합니다.");
	int col = h.q;
	int row = h.r + int((h.q + offset * (h.q & 1)) / 2);
	return OffsetCoord(col, row);
}

//큐브좌표로 변환
Hex TileMapEdit::qoffset_to_cube(int offset, OffsetCoord h) {
	assert(offset == EVEN || offset == ODD, "offset은 EVEN(+1) 또는 ODD(-1)이여야 합니다.");
	int q = h.col;
	int r = h.row - int((h.col + offset * (h.col & 1)) / 2);
	int s = -q - r;
	return Hex(q, r, s);
}

//오프셋 좌표로 변환
OffsetCoord TileMapEdit::roffset_from_cube(int offset, Hex h)
{	
	assert(offset == EVEN || offset == ODD);
	int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
	int row = h.r;
	return OffsetCoord(col, row);
}

//큐브좌표로 변환
Hex TileMapEdit::roffset_to_cube(int offset, OffsetCoord h)
{	
	assert(offset == EVEN || offset == ODD);
	int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
	int r = h.row;
	int s = -q - r;
	return Hex(q, r, s);
}

void TileMapEdit::Init()
{

	map.width = 10;
	map.height = 10;

	map.LB.x = 0.0f;
	map.LB.y = 0.0f;

	map.Origin = Point{ 0.0,0.0 };
	map.TileSize = Point{ 0.0,0.0 };

	//타일 재조정
	map.ResizeTile();

	//타일들 위치와 
	map.InitPosition();


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
		int r_offset = floor(r / 2); // or r>>1
		for (int q = -r_offset; q < width - r_offset; q++)
		{
			OffsetCoord off(r, q);
			int s = -q - r;
			Hex hex(q, r, s);
			hTiles.insert(make_pair(off,hex));
		}
	}


	Vector2 halfScale;
	halfScale.x= TileSize.x *0.5f;
	halfScale.y = TileSize.x *0.5f;

	/*for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i % 2 == 0)
			{
				Tiles[i][j].offCoord = &OffsetCoord(i, j);
				Tiles[i][j].cubeCoord = &hTiles[*Tiles[i][j].offCoord];
				Tiles[i][j].Pos.x = halfScale.x + (TileSize.x *i);
				Tiles[i][j].Pos.y = halfScale.y + (TileSize.x *j);

			}
			else
			{
				Tiles[i][j].offCoord = &OffsetCoord(i, j);
				Tiles[i][j].cubeCoord = &hTiles[*Tiles[i][j].offCoord];
				Tiles[i][j].Pos.x = halfScale.x + (TileSize.x *i);
				Tiles[i][j].Pos.y = halfScale.y + (TileSize.x *j);
			}
		}
	}*/



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

}

void RectangularPointyTopMap::Reneder()
{
	for (UINT i = 0; i < width; i++)
	{
		for (UINT j = 0; j < height; j++)
		{
			UINT vecindex = Tiles[i][j].vecIdx;
			tileImg[vecindex]->Pos = Tiles[i][j].Pos + LB;
			tileImg[vecindex]->update();
			tileImg[vecindex]->CurrentFrameX =	Tiles[i][j].imgIdx.x;
			tileImg[vecindex]->CurrentFrameY =	Tiles[i][j].imgIdx.y;
			tileImg[vecindex]->render();
		}
	}
}
