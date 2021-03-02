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
Point TileMapEdit::hex_to_pixel(Layout layout, Hex h)
{
	const Orientation& M = layout.orientation;
	double x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
	double y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
	return Point(x + layout.origin.x, y + layout.origin.y);
}

//�ȼ��� �ٻ�ȭ������
FractionalHex TileMapEdit::pixel_to_hex(Layout layout, Point p)
{
	
	const Orientation& M = layout.orientation;
	Point pt = Point((p.x - layout.origin.x) / layout.size.x, (p.y - layout.origin.y) / layout.size.y);
	double q = M.b0 * pt.x + M.b1 * pt.y;
	double r = M.b2 * pt.x + M.b3 * pt.y;
	return FractionalHex(q, r, -q - r);

}

//�ٻ� �𼭸� ���ϱ�
Point TileMapEdit::hex_corner_offset(Layout layout, int corner) {
	Point size = layout.size;
	double angle = 2.0 * M_PI *	(layout.orientation.start_angle + corner) / 6;
	return Point(size.x * cos(angle), size.y * sin(angle));
}

//��������Ʈ ��ȯ �ڳʱ����ִ� �Լ�
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
	return FractionalHex(lerp(a.q, b.q, t),	lerp(a.r, b.r, t),	lerp(a.s, b.s, t));
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

void TileMapEdit::MapCreaterHexagons()
{
	/*for (int q = -map_radius; q <= map_radius; q++) {
		int r1 = max(-map_radius, -q - map_radius);
		int r2 = min(map_radius, -q + map_radius);
		for (int r = r1; r <= r2; r++) {
			map.insert(Hex(q, r, -q - r));
		}
	}*/
}

void TileMapEdit::MapCreaterRectangles()
{
	//�ٻ� �� ����� ���簢��
	//for (int r = 0; r < map_height; r++) {
	//	int r_offset = floor(r / 2); // or r>>1
	//	for (int q = -r_offset; q < map_width - r_offset; q++) {
	//		map.insert(Hex(q, r, -q - r));
	//	}
	//}


	/*for (int r = 0; r < height; r++) {
		int r_offset = floor(r / 2);
		for (int q = -r_offset; q < width - r_offset; q++) {
			map.insert(Hex(q, r, -q - r));
		}
	}*/
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
OffsetCoord TileMapEdit::qoffset_from_cube(int offset, Hex h) {
	assert(offset == EVEN || offset == ODD,"offset�� EVEN(+1) �Ǵ� ODD(-1)�̿��� �մϴ�.");
	int col = h.q;
	int row = h.r + int((h.q + offset * (h.q & 1)) / 2);
	return OffsetCoord(col, row);
}

//ť����ǥ�� ��ȯ
Hex TileMapEdit::qoffset_to_cube(int offset, OffsetCoord h) {
	assert(offset == EVEN || offset == ODD, "offset�� EVEN(+1) �Ǵ� ODD(-1)�̿��� �մϴ�.");
	int q = h.col;
	int r = h.row - int((h.col + offset * (h.col & 1)) / 2);
	int s = -q - r;
	return Hex(q, r, s);
}
