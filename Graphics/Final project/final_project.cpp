#include <Windows.h>
#include <iostream>
#include <cmath>
#include <windows.graphics.h>
#include <tchar.h>
#include <stack>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#define MAXENTRIES 600
#define TableSize 600
using namespace std;
//DEFINING THE OPTIONS
#define IDM_BGCOLOR_WHITE 1
#define IDM_BGCOLOR_GRAY 2
#define IDM_COLOR_RED 3
#define IDM_COLOR_GREEN 4
#define IDM_COLOR_BLUE 5
#define IDM_COLOR_BLCK 6
#define IDM_LINE_DDA 7
#define IDM_LINE_MID 8
#define IDM_LINE_PARAM 9
#define IDM_CIRC_DIRECT 10
#define IDM_CIRC_POLAR 11
#define IDM_CIRC_ITRVPOLAR 12
#define IDM_CIRC_MID 13
#define IDM_CIRC_MODMID 14
#define IDM_QRTR_FIRST_LINE 15
#define IDM_QRTR_SECOND_LINE 16
#define IDM_QRTR_THIRD_LINE 17
#define IDM_QRTR_FOURTH_LINE 18
#define IDM_QRTR_FIRST_CIRC 19
#define IDM_QRTR_SECOND_CIRC 20
#define IDM_QRTR_THIRD_CIRC 21
#define IDM_QRTR_FOURTH_CIRC 22
#define IDM_CONVEX_POLYGON_FILL 23
#define IDM_NON_POLYGON_FILL 24
#define IDM_RCRSV_FF 25
#define IDM_NON_RCRSV_FF 26
#define IDM_CLP_REC_PNT 27
#define IDM_CLP_REC_LINE 28
#define IDM_CLP_REC_POLY 29
#define IDM_CLP_SQR_PNT 30
#define IDM_CLP_SQR_LINE 31
#define IDM_CLP_CIRC_PNT 32
#define IDM_CLP_CIRC_LINE 33
#define IDM_ELPS_DIRECT 34
#define IDM_ELPS_POLAR 35
#define IDM_ELPS_MID 36
#define IDM_CRV_HRMT 37
#define IDM_CRV_BZR 38
#define IDM_SPLINE 39
#define IDM_LOAD 49
#define IDM_SAVE 50
#define CLEAR 100
double const PI = 3.14159265358979323846;;
//SETTING COLOR INITIALLY TO BLACK
COLORREF color = RGB(0, 0, 0);
//////////////////////LINE ALGORITHMS//////////////////////
// 1. DDA LINE
void Swap(int& x1, int& y1) {
	int temp = x1;
	x1 = y1;
	y1 = temp;
}
void DDALine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	SetPixel(hdc, x1, y1, color);
	if (abs(dx) >= abs(dy)) {
		if (x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
		}
		int x = x1;
		double y = y1;
		double m = (double)dy / dx;
		while (x2 > x) {
			x++;
			y += m;
			SetPixel(hdc, x, round(y), color);
		}
	}
	else {
		if (y1 > y2) {
			swap(x1, x2);
			swap(y1, y2);
		}
		double x = x1;
		int y = y1;
		double mi = (double)dx / dy;
		while (y2 > y) {
			y++;
			x += mi;
			SetPixel(hdc, round(x), y, color);
		}
	}
}
// 2. MidPoint
void MidpointLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = x2 - x1;
	int dy = y2 - y1;

	//Working on X:
	if (abs(dx) >= abs(dy)) {
		//CASE 1: dy>0 , dx>0 ,dx>=dy
		if (x1 > x2) {
			Swap(x1, x2);
			Swap(y1, y2);
		}
		if (dy >= 0) {
			int x = x1;
			int y = y1;
			int Initial_d = dx - (2 * dy);
			int change1 = -2 * dy;
			int change2 = (2 * dx) - (2 * dy);
			SetPixel(hdc, x1, y1, c);
			while (x < x2) {
				if (Initial_d <= 0) { ///added equal.
					x++;
					y++;
					//Change 2
					Initial_d += change2;
				}
				else {
					x++;
					//Change 1
					Initial_d += change1;
				}
				SetPixel(hdc, x, y, c);
			}
		}
		else {
			//CASE 2: dy<0 , dx>0 ,dx>=abs(dy)
			int x = x1;
			int y = y1;
			int Initial_d = dx - (2 * abs(dy));
			int change1 = -2 * abs(dy);
			int change2 = (2 * dx) - (2 * abs(dy));
			SetPixel(hdc, x1, y1, c);
			while (x < x2) {
				if (Initial_d <= 0) { ///added equal.
					x++;
					y--;
					//Change 2
					Initial_d += change2;
				}
				else {
					x++;
					//Change 1
					Initial_d += change1;
				}
				SetPixel(hdc, x, y, c);
			}

		}
	}
	else {//working on y.
		//CASE 3: dx>0, dy>0 , dx<dy
		if (y1 > y2) {
			Swap(y1, y2);
			Swap(x1, x2);
			dx = -dx; dy = -dy;
		}
		if (dx >= 0) {
			int x = x1;
			int y = y1;
			int Initial_d = dy - (2 * dx);
			int change1 = -2 * dx;
			int change2 = (2 * dy) - (2 * dx);
			SetPixel(hdc, x1, y1, c);
			while (y < y2) {
				if (Initial_d <= 0) {
					x++;
					y++;
					//Change 2
					Initial_d += change2;
				}
				else {
					y++;
					//Change 1
					Initial_d += change1;
				}
				SetPixel(hdc, x, y, c);
			}
		}
		else {
			//CASE 4: dx<0, dy>0 , dx<dy
			int x = x1;
			int y = y1;
			int Initial_d = dy - (2 * abs(dx));
			int change1 = -2 * abs(dx);
			int change2 = (2 * dy) - (2 * abs(dx));
			while (y < y2) {
				if (Initial_d <= 0) {
					x--;
					y++;
					//Change 2
					Initial_d += change2;
				}
				else {
					y++;
					//Change 1
					Initial_d += change1;
				}
				SetPixel(hdc, x, y, c);
			}
		}
	}
}
// 3. Parametric
void ParametricLine(HDC hdc, double x1, double y1, double x2, double y2, COLORREF c) {
	//Initial values of x and y.
	double x = x1;
	double y = y1;
	//Calculating delta x and delta y.
	double dx = x2 - x1;
	double dy = y2 - y1;
	//Drawing first point.
	SetPixel(hdc, x1, y1, c);
	//Looping on t to draw the line using Parametric equations of line.
	for (double t = 0; t < 1; t += 0.001) {
		x = x1 + t * dx;
		y = y1 + t * dy;
		SetPixel(hdc, x, y, c);
	}
}
//////////////////////CIRCLE ALGORITHMS//////////////////////
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc + y, yc - x, c);
};
// 1. Direct
void Direct(HDC hdc, int xc, int yc, double r, COLORREF c) {
	int x = 0;
	double y = r;
	Draw8Points(hdc, xc, yc, 0, r, c);
	while (x < y) {
		x++;
		y = (double)sqrt((pow(r, 2) - pow(x, 2)));
		Draw8Points(hdc, xc, yc, x, round(y), c);
	}
}
// 2. Polar
void Polar(HDC hdc, int xc, int yc, int r, COLORREF c)
{
	double dtheta = 1.0 / r;
	for (double theta = 0; theta < 6.28; theta += dtheta)
	{
		double x = xc + r * cos(theta);
		double y = yc + r * sin(theta);
		SetPixel(hdc, x, y, c);
	}
}
// 3. Iterative Polar
void IterativePolar(HDC hdc, int xc, int yc, int r, COLORREF c) //iterative polar function
{
	double dtheta = 1.0 / r;
	double cost = cos(dtheta), sint = sin(dtheta);
	double x = r, y = 0;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x > y) {
		double Xnew = x * cost - y * sint;
		double Ynew = y * cost + x * sint;
		x = Xnew, y = Ynew;
		Draw8Points(hdc, xc, yc, round(y), round(x), c);
	}
}
// 4. MidPoint 
void MidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF c)
{
	int x = 0;
	int y = r;
	int d = 1 - r;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x <= y)
	{
		x++;
		if (d < 0)
		{
			d += 2 * x + 1;
		}
		else
		{
			y--;
			d += 2 * (x - y) + 1;
		}
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}
// 5. Modified MidPoint
void ModifiedMidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF c)
{
	int x = 0, y = r;
	int d = 1 - r;
	int d1 = 3;
	int d2 = -2 * r + 5;
	while (y >= x)
	{
		if (d < 0)
		{
			d += d1;
			d1 += 2;
			d2 += 2;
		}
		else
		{
			d += d2;
			d1 += 2;
			d2 += 4;
			y--;
		}
		x++;
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}
///////////////Filling Quarters by lines//////////////////
void FillQuarter1(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	double dtheta = 1.0 / r;
	const double PI = 3.14159265358979323846;
	for (double theta = (PI * 3) / 2; theta <= 2 * PI; theta += dtheta) {
		double x = xc + (r * cos(theta));
		double y = yc + (r * sin(theta));
		DDALine(hdc, xc, yc, x, y, color);
	}
}
void FillQuarter2(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	double dtheta = 1.0 / r;
	const double PI = 3.14159265358979323846;
	for (double theta = PI; theta <= (PI * 3) / 2; theta += dtheta) {
		double x = xc + (r * cos(theta));
		double y = yc + (r * sin(theta));
		DDALine(hdc, xc, yc, x, y, color);
	}
}
void FillQuarter3(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	double dtheta = 1.0 / r;
	const double PI = 3.14159265358979323846;
	for (double theta = (PI / 2); theta <= PI; theta += dtheta) {
		double x = xc + (r * cos(theta));
		double y = yc + (r * sin(theta));
		DDALine(hdc, xc, yc, x, y, color);
	}
}
void FillQuarter4(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	double dtheta = 1.0 / r;
	const double PI = 3.14159265358979323846;
	for (double theta = 0.0; theta <= (PI / 2); theta += dtheta) {
		double x = xc + (r * cos(theta));
		double y = yc + (r * sin(theta));
		DDALine(hdc, xc, yc, x, y, color);
	}
}
///////////////Filling Quarters by CIRCLES//////////////////
void FillQuarter1circ(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	const double PI = 3.14159265358979323846;
	double dtheta = 1.0 / r;
	for (double rad = 0; rad <= r; rad += 4) {
		for (double theta = (3 * PI) / 2; theta < 2 * PI; theta += dtheta)
		{
			double x = xc + rad * cos(theta);
			double y = yc + rad * sin(theta);
			SetPixel(hdc, x, y, color);
		}
	}
}
void FillQuarter2circ(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	const double PI = 3.14159265358979323846;
	double dtheta = 1.0 / r;
	for (double rad = 0; rad <= r; rad += 4) {
		for (double theta = PI; theta < (3 * PI) / 2; theta += dtheta)
		{
			double x = xc + rad * cos(theta);
			double y = yc + rad * sin(theta);
			SetPixel(hdc, x, y, color);
		}
	}
}
void FillQuarter3circ(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	const double PI = 3.14159265358979323846;
	double dtheta = 1.0 / r;
	for (double rad = 0; rad <= r; rad += 4) {
		for (double theta = PI / 2; theta < PI; theta += dtheta)
		{
			double x = xc + rad * cos(theta);
			double y = yc + rad * sin(theta);
			SetPixel(hdc, x, y, color);
		}
	}
}
void FillQuarter4circ(HDC hdc, int xc, int yc, int r, COLORREF color)
{
	const double PI = 3.14159265358979323846;
	double dtheta = 1.0 / r;
	for (double rad = 0; rad <= r; rad += 4) {
		for (double theta = 0; theta < PI / 2; theta += dtheta)
		{
			double x = xc + rad * cos(theta);
			double y = yc + rad * sin(theta);
			SetPixel(hdc, x, y, color);
		}
	}
}
///////////////CONVEX & NONCONVEX FILLING ALGORITHM//////////////////
void DrawPolygon(HDC hdc, POINT poly[], int size, COLORREF c) {
	for (int i = 0; i < size; i++) {
		int x1 = poly[i].x;
		int y1 = poly[i].y;
		int x2 = poly[(i + 1) % size].x;
		int y2 = poly[(i + 1) % size].y;
		DDALine(hdc, x1, y1, x2, y2, c);
	}
}
// 1. Convex Filling
struct Table {
	int xleft, xright;
};
void InitTable(Table arr[]) {
	for (int i = 0; i < MAXENTRIES; i++) {
		arr[i].xleft = INT_MAX;
		arr[i].xright = -INT_MAX;
	}
}
void EdgeToTableDDAAlogrithm(POINT p1, POINT p2, Table arr[]) {
	//Means horizontal line.
	//If it is a horizontal line then it is an edge and the 
	//edge is not included in the filling algorithm.
	if (p1.y == p2.y)
		return;
	if (p1.y > p2.y)
		swap(p1, p2);

	double x = p1.x;
	int y = p1.y;

	//Calculating the inverse of the slope.
	double ml = (double)(p2.x - p1.x) / (p2.y - p1.y);
	while (y < p2.y) {
		if (x < arr[y].xleft)
		{
			//take ceiling for left point, to get correct polygon.
			arr[y].xleft = (int)ceil(x);
		}

		if (x > arr[y].xright)
		{
			//pixels will be out of the edge, to get correct polygon.
			arr[y].xright = (int)floor(x);
		}
		y++;
		x += ml;
	}
}
void DrawTableToScreen(HDC hdc, Table arr[], COLORREF c) {
	//Looping on the table of intersection points to draw it on screen.
	for (int i = 0; i < MAXENTRIES; i++) {
		//If the left element is less the the right element then the is intersection.
		if (arr[i].xleft < arr[i].xright)
			//Draw horizontal line from left point to right point.
			for (int x = arr[i].xleft; x <= arr[i].xright; x++)
				SetPixel(hdc, x, i, c);
	}
}
void ConvexFilling(HDC hdc, POINT poly[], int n, COLORREF c) {
	//Declaring table to hold intersection points.
	Table* table = new Table[MAXENTRIES];
	//Initialize the table.
	InitTable(table);
	//Start with last point.
	POINT vertix1 = poly[n - 1];
	//Looping on the polygon to get the points on the edge using the starting point
	//and ending point of the edge.
	for (int i = 0; i < n; i++) {
		POINT v2 = poly[i];
		EdgeToTableDDAAlogrithm(vertix1, v2, table);
		vertix1 = poly[i];
	}
	//Drawing lines to fill the polygon.
	DrawTableToScreen(hdc, table, c);
	//Deleting the table from memory.
	delete table;
}
// 2. Non Convex Filling
class Node {
public:
	double x;
	double ml;//inverse of the slope.
	int yMax;
	Node() {}
	Node(int xx, double mll, int ymax) {
		x = xx;
		ml = mll;
		yMax = ymax;
	}
	//Overloading operator < to use it in sort function.
	bool operator< (Node N) {
		return x < N.x;
	}
};
typedef list<Node>dynamicTable;
Node InitEdgeNodes(POINT& p1, POINT& p2) {
	if (p1.y > p2.y)
		swap(p1, p2);
	//Calculating the inverse of the slope.
	double inverseM = (double)(p2.x - p1.x) / (p2.y - p1.y);
	//Construct a node.
	Node N;
	N.x = p1.x;
	N.ml = inverseM;
	N.yMax = p2.y;
	return N;
}
void InitTable(POINT* poly, int siz, dynamicTable table[])
{
	//Starting from the last Point.
	POINT vertix1 = poly[siz - 1];
	//Looping on the polygon to construct the edge table.
	for (int i = 0; i < siz; i++) {
		POINT vertix2 = poly[i];
		//Means horizontal line.
	   //If it is a horizontal line then it is an edge and the edge is not included in the filling algorithm.
		if (vertix1.y == vertix2.y) {
			vertix1 = vertix2;
			continue;
		}
		//Constructing a node.
		Node node = InitEdgeNodes(vertix1, vertix2);
		//Append node to table.
		table[vertix1.y].push_back(node);
		vertix1 = poly[i];
	}
}
void NonConvexFilling(HDC hdc, POINT* poly, int siz, COLORREF c) {
	//Dynamic Table:
	dynamicTable* Table = new dynamicTable[TableSize];
	//Constructing the table.
	InitTable(poly, siz, Table);
	//Index
	int y = 0;
	//Increment Y as long as there is no node in the list.
	while (y < TableSize && Table[y].size() == 0)
	{
		y++;
	}
	//End the algorithm if Y index reached the table size
	if (y == TableSize)return;

	//Create Active List to store the first node in the table.
	dynamicTable ActiveList = Table[y];

	while (ActiveList.size() > 0) {
		//Sorting active list before beginning filling
		ActiveList.sort();
		for (dynamicTable::iterator dt = ActiveList.begin(); dt != ActiveList.end(); dt++) {
			int x1 = (int)ceil(dt->x);
			dt++;
			int x2 = (int)floor(dt->x);
			//Looping to draw a line from the start to end.
			for (int i = x1; i <= x2; i++) {
				SetPixel(hdc, i, y, c);
			}
		}
		y++;
		//Iterator on ActiveList to check each node in it.
		dynamicTable::iterator dt = ActiveList.begin();
		while (dt != ActiveList.end()) {
			//If the index y reached the yMax value of the node delete this node.
			if (y == dt->yMax) {
				dt = ActiveList.erase(dt);
			}
			else {
				dt++;
			}
		}
		//Iterator on ActiveList to update the x value of each node.
		for (dynamicTable::iterator dt = ActiveList.begin(); dt != ActiveList.end(); dt++) {
			dt->x += dt->ml;
		}
		//Append the Node to ActiveList.
		//Appending at the end of the Active list.
		ActiveList.insert(ActiveList.end(), Table[y].begin(), Table[y].end());
	}
	//Deleting the dynamic memory.
	delete[] Table;
}
///////////RESCURSIVE & NONRESCURSIVE FLOODFILL ALGORITHM/////////////////
// 1. Recursive floodfill
void RecursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
	//Getting the color of the point.
	COLORREF c = GetPixel(hdc, x, y);
	//Testing C.
	if (c == bc || c == fc)
		return;
	SetPixel(hdc, x, y, fc);
	//Filling Right.
	RecursiveFloodFill(hdc, x + 1, y, bc, fc);
	//Filling Left.
	RecursiveFloodFill(hdc, x - 1, y, bc, fc);
	//Filling UP.
	RecursiveFloodFill(hdc, x, y + 1, bc, fc);
	//Filling Down.
	RecursiveFloodFill(hdc, x, y - 1, bc, fc);
}
// 2. non Recursive floodfill
class point {
public:
	int x; int y;
	point() {}
	point(int xx, int yy)
	{
		x = xx;
		y = yy;
	}
};
void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
	stack <point> s;
	//Pushing the first point in the stack.
	s.push(point(x, y));
	while (!s.empty()) {
		point po;
		//Soring the point on top of the stack.
		po = s.top();
		//Deleting it from the stack.
		s.pop();
		//Getting the color of the point.
		COLORREF c = GetPixel(hdc, po.x, po.y);
		//Testing C.
		if (c == bc || c == fc)
			continue;
		SetPixel(hdc, po.x, po.y, fc);
		//Filling Right.
		s.push(point(po.x + 1, po.y));
		//Filling Left.
		s.push(point(po.x - 1, po.y));
		//Filling UP.
		s.push(point(po.x, po.y + 1));
		//Filling Down.
		s.push(point(po.x, po.y - 1));
	}
}
///////////////// CLIPPING ALGORITHMS/////////////////////
void MyRectangle(HDC hdc, int left, int top, int right, int bottom, COLORREF c)
{
	// Draw top side
	DDALine(hdc, left, top, right, top, c);
	// Draw right side
	DDALine(hdc, right, top, right, bottom, c);
	// Draw bottom side
	DDALine(hdc, right, bottom, left, bottom, c);
	// Draw left side
	DDALine(hdc, left, bottom, left, top, c);
}
void MySquare(HDC hdc, int left, int top, int size, COLORREF c)
{
	int right = left + size;
	int bottom = top + size;
	DDALine(hdc, left, top, right, top, c);
	DDALine(hdc, right, top, right, bottom, c);
	DDALine(hdc, right, bottom, left, bottom, c);
	DDALine(hdc, left, bottom, left, top, c);
}
union OutCode
{
	unsigned All : 4;
	struct
	{
		unsigned left : 1, top : 1, right : 1, bottom : 1;
	};
};
OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom)
{
	OutCode out;
	out.All = 0;
	if (x < xleft)
		out.left = 1;
	else if (x > xright)
		out.right = 1;
	if (y < ytop)
		out.top = 1;
	else if (y > ybottom)
		out.bottom = 1;
	return out;
}
void VIntersect(double x1, double y1, double x2, double y2, double X, double* xi, double* yi)
{
	*xi = X;
	*yi = y1 + (X - x1) * (y2 - y1) / (x2 - x1);
}
void HIntersect(double x1, double y1, double x2, double y2, double Y, double* xi, double* yi)
{
	*yi = Y;
	*xi = x1 + (Y - y1) * (x2 - x1) / (y2 - y1);
}
// 1. point clipping
void ClipPnt(HDC hdc, int px, int py, double left, double top, double right, double bottom, COLORREF c)
{
	if (px >= left && px <= right && py >= top && py <= bottom) {
		SetPixel(hdc, px, py, c);
	}
}
// 2. Line clipping
void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom, COLORREF c)
{
	double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
	OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
	OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
	while ((out1.All || out2.All) && !(out1.All & out2.All))
	{
		double xi, yi;
		if (out1.All)
		{
			if (out1.left)
				VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
			else if (out1.top)
				HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
			else if (out1.right)
				VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
			else
				HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
			x1 = xi;
			y1 = yi;
			out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
		}
		else
		{
			if (out2.left)
				VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
			else if (out2.top)
				HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
			else if (out2.right)
				VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
			else
				HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
			x2 = xi;
			y2 = yi;
			out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
		}
	}
	if (!out1.All && !out2.All)
	{
		DDALine(hdc, x1, y1, x2, y2, c);
	}
}
// 3. polygon clipping
struct Point
{
	double x, y;
	Point(int x1 = 0, int y1 = 0)
	{
		x = x1;
		y = y1;
	}
};
typedef vector<Point> PointList;
typedef bool (*IsInFunc)(Point& v, int edge);
typedef Point(*IntersectFunc)(Point& v1, Point& v2, int edge);
void DrawPolygon(HDC hdc, Point poly[], int siz, COLORREF c) {
	for (int i = 0; i < siz; i++) {
		int x1 = poly[i].x;
		int y1 = poly[i].y;
		int x2 = poly[(i + 1) % siz].x;
		int y2 = poly[(i + 1) % siz].y;
		DDALine(hdc, x1, y1, x2, y2, c);
	}
}
bool InLeft(Point& v, int edge)
{
	return v.x >= edge;
}
bool InRight(Point& v, int edge)
{
	return v.x <= edge;
}
bool InTop(Point& v, int edge)
{
	return v.y >= edge;
}
bool InBottom(Point& v, int edge)
{
	return v.y <= edge;
}
PointList ClipWithEdge(PointList p, int edge, IsInFunc In, IntersectFunc Intersect)
{
	PointList OutList;
	Point v1 = p[p.size() - 1];
	bool v1_in = In(v1, edge);
	for (int i = 0; i < (int)p.size(); i++)
	{
		Point v2 = p[i];
		bool v2_in = In(v2, edge);
		if (!v1_in && v2_in)
		{
			OutList.push_back(Intersect(v1, v2, edge));
			OutList.push_back(v2);
		}
		else if (v1_in && v2_in) OutList.push_back(v2);
		else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
		v1 = v2;
		v1_in = v2_in;
	}
	return OutList;
}
Point VIntersect(Point& v1, Point& v2, int xedge)
{
	Point res;
	res.x = xedge;
	res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
	return res;
}
Point HIntersect(Point& v1, Point& v2, int yedge)
{
	Point res;
	res.y = yedge;
	res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
	return res;
}
void PolygonClip(HDC hdc, Point* p, int n, int xleft, int ytop, int xright, int ybottom, COLORREF c)
{
	PointList vlist;
	for (int i = 0; i < n; i++)vlist.push_back(Point(p[i].x, p[i].y));
	vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
	vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
	vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
	vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
	Point v1 = vlist[vlist.size() - 1];
	int siz = (int)vlist.size();
	for (int i = 0; i < (int)vlist.size(); i++)
	{
		Point v2 = vlist[i];
		DDALine(hdc, v1.x, v1.y, v2.x, v2.y, c);
		v1 = v2;
	}
}
/////////////////CIRCLE CLIPPING ALGORITHMS/////////////////////
// 1. point clipping
void circClipPnt(HDC hdc, double px, double py, double xc, double yc, double r, COLORREF c)
{
	double distance = sqrt((px - xc) * (px - xc) + (py - yc) * (py - yc));
	if (distance <= r) {
		SetPixel(hdc, px, py, c);
	}
}
// 2. line clipping
// Function to calculate the intersection points between a circle and a line
void findIntersection(HDC hdc, double xs, double ys, double xe, double ye, double xc, double yc, double r, double m, double c1, COLORREF C) {
	// Calculate the discriminant of the quadratic formula
	double discriminant = pow(r, 2) * (pow(m, 2) + 1) - pow(yc - m * xc - c1, 2);
	double p1x = xs, p1y = ys, p2x = xe, p2y = ye;
	// Calculate the distances between the line endpoints and the circle center
	double d1 = sqrt((p1x - xc) * (p1x - xc) + (p1y - yc) * (p1y - yc));
	double d2 = sqrt((p2x - xc) * (p2x - xc) + (p2y - yc) * (p2y - yc));
	if (d1 <= r && d2 <= r) {
		DDALine(hdc, xs, ys, xe, ye, C);
	}
	else if (d1 <= r)
	{
		if (discriminant < 0) {
			return;
		}
		// If the discriminant is zero, there is only one intersection point
		else if (discriminant == 0) {
			xe = xc + (yc - c1) * m;
			ye = m * xe + c1;
			DDALine(hdc, xs, ys, xe, ye, C);

		}
	}
	else if (d2 <= r)
	{
		if (discriminant < 0) {
			return;
		}
		// If the discriminant is zero, there is only one intersection point
		else if (discriminant == 0) {
			xs = xc + (yc - c1) * m;
			ys = m * xs + c1;
			DDALine(hdc, xs, ys, xe, ye, C);
		}
	}
	else if (d2 > r && d2 > r)
	{
		if (discriminant < 0) {
			return;
		} // If the discriminant is positive, there are two intersection points
		else {
			double xs = (xc + (yc - c1) * m + sqrt(discriminant)) / (pow(m, 2) + 1);
			double ys = m * xs + c1;
			double xe = (xc + (yc - c1) * m - sqrt(discriminant)) / (pow(m, 2) + 1);
			double ye = m * xe + c1;
			DDALine(hdc, xs, ys, xe, ye, C);
		}
	}
}
void ClipLineCircle(HDC hdc, double xs, double ys, double xe, double ye, double xc, double yc, double r, COLORREF c)
{
	double dx = xe - xs;
	double dy = ye - ys;
	double m = dy / dx;
	double c1 = ys - m * xs;
	findIntersection(hdc, xs, ys, xe, ye, xc, yc, r, m, c1, c);
}
/////////////////ELLIPSE ALGORITHMS/////////////////////
void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc + x, yc - y, c);
};
// 1. Direct ellipse
void cartesianEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF color) {
	double a2 = a * a;
	double b2 = b * b;
	/// from left to right
	for (int x = -a; x <= a; x++) {
		double y = round((double)b / a * sqrt(a2 - x * x));
		Draw4Points(hdc, xc, yc, x, round(y), color);
	}
	///from down to up
	for (int y = -b; y <= b; y++) {
		double x = round((double)a / b * sqrt(b2 - y * y));
		Draw4Points(hdc, xc, yc, round(x), y, color);
	}
}
// 2. Polar ellipse
void PolarEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF c)
{
	int x = A;
	int y = 0;
	double dtheta = 1.0 / max(A, B);
	double theta = 0;
	Draw4Points(hdc, xc, yc, x, y, c);
	while (theta < PI / 2) {
		x = A * cos(theta);
		y = B * sin(theta);
		theta += dtheta;
		Draw4Points(hdc, xc, yc, x, y, c);
	}
}
// 3. Midpoint ellipse
void MidpointEllipse(HDC hdc, int cx, int cy, int a, int b, COLORREF color) {
	int a2 = a * a;
	int b2 = b * b;
	int x = 0;
	int y = b;
	int d = b2 - a2 * b + a2 / 4;
	Draw4Points(hdc, cx, cy, x, y, color);
	while (b2 * x <= a2 * y) {
		if (d < 0) {
			x++;
			d += 2 * b2 * x + b2;
		}
		else {
			x++;
			y--;
			d += 2 * b2 * x - 2 * a2 * y + b2;
		}
		Draw4Points(hdc, cx, cy, x, y, color);
	}
	d = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
	while (y > 0) {
		if (d < 0) {
			x++;
			y--;
			d += 2 * b2 * x - 2 * a2 * y + a2;
		}
		else {
			y--;
			d -= 2 * a2 * y - a2;
		}
		Draw4Points(hdc, cx, cy, x, y, color);
	}
}
///////////////CURVES ALGORITHMS//////////////////
struct Vector {
	double v[2];
	Vector(double x = 0, double y = 0)
	{
		v[0] = x; v[1] = y;
	}
	double& operator[](int i) {
		return v[i];
	}
};
void DrawHermiteCurve(HDC hdc, POINT p1, POINT T1, POINT p2, POINT T2, COLORREF c)
{
	double a0 = p1.x, a1 = T1.x,
		a2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
		a3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
	double b0 = p1.y, b1 = T1.y,
		b2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
		b3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t * t, t3 = t2 * t;
		double x = a0 + a1 * t + a2 * t2 + a3 * t3;
		double y = b0 + b1 * t + b2 * t2 + b3 * t3;
		SetPixel(hdc, round(x), round(y), c);
	}
}
void DrawBezierCurve(HDC hdc, POINT p1, POINT p2, POINT p3, POINT p4, COLORREF c)
{
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t * t, t3 = t2 * t;
		double x = (1 - t) * (1 - t) * (1 - t) * p1.x + 3 * t * (1 - t) * (1 - t) * p2.x + 3 * t2 * (1 - t) * p3.x + t3 * p4.x;
		double y = (1 - t) * (1 - t) * (1 - t) * p1.y + 3 * t * (1 - t) * (1 - t) * p2.y + 3 * t2 * (1 - t) * p3.y + t3 * p4.y;
		SetPixel(hdc, round(x), round(y), c);
	}
}
// 1. Fill with Hermit
void hermitFill(HDC hdc, int sqrX, int sqrY, int size, COLORREF c) {
	POINT p1, p2;
	POINT T1 = { 300, 600 };
	POINT T2 = { -250, -70 }; // Set the control points and tangents
	p1.x = sqrX;
	p1.y = sqrY;
	p2.x = sqrX;
	p2.y = sqrY + size;
	for (int i = sqrX; i < sqrX + size; i += 4) {
		DrawHermiteCurve(hdc, p1, T1, p2, T2, c);
		p1.x = i;
		p2.x = i;
	}
}
// 2. Fill with Bezier
void bezierFill(HDC hdc, int left, int top, int right, int bottom, COLORREF c) {
	// Determine the control points for the Bezier curve
	POINT points[4];
	points[0] = { left, top };
	points[1] = { (left + right) / 2, top };
	points[2] = { (left + right) / 2, bottom };
	points[3] = { right, top };

	for (int i = top; i < bottom; i += 4) {
		DrawBezierCurve(hdc, points[0], points[1], points[2], points[3], c);
		points[0].y = i;
		points[3].y = i;
	}
}
// 3. Cardinal Splines
double tension = 0.5;
void drawsplinecurve(HDC hdc, POINT points[], COLORREF c) {
	// Calculate the tangent vectors using the Cardinal Spline Curve equation
	POINT tangents[10];
	int splinecount = 10;
	tangents[0].x = (int)(tension * (points[1].x - points[0].x));
	tangents[0].y = (int)(tension * (points[1].y - points[0].y));
	tangents[splinecount - 1].x = (int)(tension * (points[splinecount - 1].x - points[splinecount - 2].x));
	tangents[splinecount - 1].y = (int)(tension * (points[splinecount - 1].y - points[splinecount - 2].y));
	for (int i = 1; i < splinecount - 1; i++) {
		tangents[i] = { (int)(tension * (points[i + 1].x - points[i - 1].x)), (int)(tension * (points[i + 1].y - points[i - 1].y)) };
	}

	// Draw the curve using Hermitian interpolation
	for (int i = 0; i < splinecount - 1; i++) {
		DrawHermiteCurve(hdc, points[i], tangents[i], points[i + 1], tangents[i + 1], c);
	}
}
////////////////////////////////////////////////
void writeToFile(const std::vector<COLORREF>& colors, const std::string& filename) {

	std::ofstream file(filename, std::ios::out | std::ios::trunc);
	if (file.is_open()) {
		for (const auto& color : colors) {
			int red = GetRValue(color);
			int green = GetGValue(color);
			int blue = GetBValue(color);
			file << red << "," << green << "," << blue << "\n";
		}
		file.close();
		std::cout << "Colors successfully written to " << filename << std::endl;
	}
	else {
		std::cerr << "Error opening file " << filename << std::endl;
	}
	cout << "Done!" << endl;
}

std::vector<std::vector<int>> readColorsFromFile(const std::string& filename) {
	cout << "Start reading from file..." << endl;
	std::vector<std::vector<int>> colors;

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return colors;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string token;
		std::vector<int> color;
		while (std::getline(iss, token, ',')) {
			color.push_back(std::stoi(token));
		}
		colors.push_back(color);
	}

	cout << "Done!" << endl;

	file.close();
	return colors;
}
//////////////////////////////////////////////

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	static std::vector<COLORREF> colors;
	//RECT is a structure that specifies the dimensions of the client window area.
	//will use it the calculate the dimensions of rectangle.
	RECT rect;
	COLORREF bounaryColor = RGB(0, 0, 0);
	COLORREF fillColor = RGB(255, 255, 255);
	static bool drawing = false;
	static int x1, y1, x2, y2;
	static int mode = 0;
	static int counter = 0;
	static int count = 0;
	static POINT convxpolygon[5];
	static POINT nonconvxpolygon[6];
	static POINT rcrsvpolygon[5];
	static POINT nonrcrsvpolygon[5];
	HDC hdc;
	static double  r = 300;
	static HBRUSH hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	static double px, py;
	static double  rcirc = 100;
	static double windowWidth, windowHeight;
	// Set background color initially to gray
	static HBRUSH hwhite = CreateSolidBrush(RGB(255, 255, 255));
	static HBRUSH hgray = CreateSolidBrush(RGB(200, 200, 200));
	static HBRUSH hcurrent = hgray;
	static int lx1, lx2, ly1, ly2;
	int left = 200, top = 100, right = 700, bottom = 400;
	static Point poly[5];
	static int n = 5;
	static int cnt = 0;
	static double xs, ys, xe, ye;
	static int x = -1, y = -1, A = -1;
	static int splinecnt = 0;
	static POINT spline[10];
	switch (mcode)
	{
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case IDM_BGCOLOR_WHITE:
			// Change background color to white
			hcurrent = hwhite;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case IDM_BGCOLOR_GRAY:
			// Change background color back to gray
			hcurrent = hgray;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case CLEAR:
			// Clears everything on the screen
			hcurrent = hwhite;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case IDM_COLOR_RED:
			// Select red color
			color = RGB(255, 0, 0);
			break;
		case IDM_COLOR_GREEN:
			// Select green color
			color = RGB(0, 255, 0);
			break;
		case IDM_COLOR_BLUE:
			// Select blue color
			color = RGB(0, 0, 255);
			break;
		case IDM_COLOR_BLCK:
			// Select black color
			color = RGB(0, 0, 0);
			break;
		case IDM_LINE_DDA:
			mode = IDM_LINE_DDA;
			drawing = true;
			break;
		case IDM_LINE_MID:
			mode = IDM_LINE_MID;
			drawing = true;
			break;
		case IDM_LINE_PARAM:
			mode = IDM_LINE_PARAM;
			drawing = true;
			break;
		case IDM_CIRC_DIRECT:
			mode = IDM_CIRC_DIRECT;
			drawing = true;
			break;
		case IDM_CIRC_POLAR:
			mode = IDM_CIRC_POLAR;
			drawing = true;
			break;
		case IDM_CIRC_ITRVPOLAR:
			mode = IDM_CIRC_ITRVPOLAR;
			drawing = true;
			break;
		case IDM_CIRC_MID:
			mode = IDM_CIRC_MID;
			drawing = true;
			break;
		case IDM_CIRC_MODMID:
			mode = IDM_CIRC_MODMID;
			drawing = true;
			break;
		case IDM_QRTR_FIRST_LINE:
			mode = IDM_QRTR_FIRST_LINE;
			drawing = true;
			break;
		case IDM_QRTR_SECOND_LINE:
			mode = IDM_QRTR_SECOND_LINE;
			drawing = true;
			break;
		case IDM_QRTR_THIRD_LINE:
			mode = IDM_QRTR_THIRD_LINE;
			drawing = true;
			break;
		case IDM_QRTR_FOURTH_LINE:
			mode = IDM_QRTR_FOURTH_LINE;
			drawing = true;
			break;
		case IDM_QRTR_FIRST_CIRC:
			mode = IDM_QRTR_FIRST_CIRC;
			drawing = true;
			break;
		case IDM_QRTR_SECOND_CIRC:
			mode = IDM_QRTR_SECOND_CIRC;
			drawing = true;
			break;
		case IDM_QRTR_THIRD_CIRC:
			mode = IDM_QRTR_THIRD_CIRC;
			drawing = true;
			break;
		case IDM_QRTR_FOURTH_CIRC:
			mode = IDM_QRTR_FOURTH_CIRC;
			drawing = true;
			break;
		case IDM_NON_POLYGON_FILL:
			mode = IDM_NON_POLYGON_FILL;
			drawing = true;
			break;
		case IDM_CONVEX_POLYGON_FILL:
			mode = IDM_CONVEX_POLYGON_FILL;
			drawing = true;
			break;
		case IDM_RCRSV_FF:
			mode = IDM_RCRSV_FF;
			drawing = true;
			break;
		case IDM_NON_RCRSV_FF:
			mode = IDM_NON_RCRSV_FF;
			drawing = true;
			break;
		case IDM_CLP_REC_PNT:
			mode = IDM_CLP_REC_PNT;
			drawing = true;
			break;
		case IDM_CLP_REC_LINE:
			mode = IDM_CLP_REC_LINE;
			drawing = true;
			break;
		case IDM_CLP_REC_POLY:
			mode = IDM_CLP_REC_POLY;
			drawing = true;
			break;
		case IDM_CLP_SQR_PNT:
			mode = IDM_CLP_SQR_PNT;
			drawing = true;
			break;
		case IDM_CLP_SQR_LINE:
			mode = IDM_CLP_SQR_LINE;
			drawing = true;
			break;
		case IDM_CLP_CIRC_PNT:
			mode = IDM_CLP_CIRC_PNT;
			drawing = true;
			break;
		case IDM_CLP_CIRC_LINE:
			mode = IDM_CLP_CIRC_LINE;
			drawing = true;
			break;
		case IDM_ELPS_DIRECT:
			mode = IDM_ELPS_DIRECT;
			drawing = true;
			break;
		case IDM_ELPS_POLAR:
			mode = IDM_ELPS_POLAR;
			drawing = true;
			break;
		case IDM_ELPS_MID:
			mode = IDM_ELPS_MID;
			drawing = true;
			break;
		case IDM_CRV_HRMT:
			mode = IDM_CRV_HRMT;
			drawing = true;
			break;
		case IDM_CRV_BZR:
			mode = IDM_CRV_BZR;
			drawing = true;
			break;
		case IDM_SPLINE:
			mode = IDM_SPLINE;
			drawing = true;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (mode == IDM_LINE_DDA || mode == IDM_LINE_MID || mode == IDM_LINE_PARAM)
		{
			x1 = LOWORD(lp);
			y1 = HIWORD(lp);
		}
		else if (mode == IDM_CIRC_DIRECT || mode == IDM_CIRC_POLAR || mode == IDM_CIRC_ITRVPOLAR || mode == IDM_CIRC_MID || mode == IDM_CIRC_MODMID)
		{
			x1 = LOWORD(lp);
			y1 = HIWORD(lp);
		}
		else if (mode == IDM_QRTR_FIRST_LINE || mode == IDM_QRTR_SECOND_LINE || mode == IDM_QRTR_THIRD_LINE || mode == IDM_QRTR_FOURTH_LINE) {
			x1 = LOWORD(lp);
			y1 = HIWORD(lp);
		}
		else if (mode == IDM_QRTR_FIRST_CIRC || mode == IDM_QRTR_SECOND_CIRC || mode == IDM_QRTR_THIRD_CIRC || mode == IDM_QRTR_FOURTH_CIRC) {
			x1 = LOWORD(lp);
			y1 = HIWORD(lp);
		}
		else if (mode == IDM_CONVEX_POLYGON_FILL) {
			hdc = GetDC(hwnd);
			if (counter <= 6) {
				convxpolygon[counter].x = LOWORD(lp);
				convxpolygon[counter].y = HIWORD(lp);
				counter++;
				if (counter == 5) {
					DrawPolygon(hdc, convxpolygon, 5, color);
				}
				if (counter == 6) {
					ConvexFilling(hdc, convxpolygon, 5, color);
					ReleaseDC(hwnd, hdc);
					counter = 0;
				}
			}
			ReleaseDC(hwnd, hdc);
		}
		else if (mode == IDM_NON_POLYGON_FILL) {
			hdc = GetDC(hwnd);
			if (counter <= 7) {
				nonconvxpolygon[counter].x = LOWORD(lp);
				nonconvxpolygon[counter].y = HIWORD(lp);
				counter++;
				if (counter == 6) {
					DrawPolygon(hdc, nonconvxpolygon, 6, color);
				}
				if (counter == 7) {
					NonConvexFilling(hdc, nonconvxpolygon, 6, color);
					ReleaseDC(hwnd, hdc);
					counter = 0;
				}
			}
			else {
				ReleaseDC(hwnd, hdc);
			}
		}
		else if (mode == IDM_NON_RCRSV_FF) {
			hdc = GetDC(hwnd);
			//This Function returns the client area of window.
			GetClientRect(hwnd, &rect);
			//Calculating the coordinates of the center of client window area.
		//Dividing by 2 will give us the x and y coordinates of the center.
			x = rect.right / 2;
			y = rect.bottom / 2;
			fillColor = color;
			Rectangle(hdc, x + 10, y + 50, x - 10, y - 50);
			RecursiveFloodFill(hdc, x, y, bounaryColor, fillColor);
			ReleaseDC(hwnd, hdc);

		}
		else if (mode == IDM_RCRSV_FF) {
			hdc = GetDC(hwnd);
			//This Function returns the client area of window.
			GetClientRect(hwnd, &rect);
			//Calculating the coordinates of the center of client window area.
		//Dividing by 2 will give us the x and y coordinates of the center.
			x = rect.right / 2;
			y = rect.bottom / 2;
			fillColor = color;
			Rectangle(hdc, x + 10, y + 50, x - 10, y - 50);
			NonRecursiveFloodFill(hdc, x, y, bounaryColor, fillColor);
			ReleaseDC(hwnd, hdc);
		}
		else if (mode == IDM_RCRSV_FF) {
			x = LOWORD(lp);
			y = HIWORD(lp);
		}
		else if (mode == IDM_CLP_REC_PNT || mode == IDM_CLP_SQR_PNT) {
			x = LOWORD(lp);
			y = HIWORD(lp);
		}
		else if (mode == IDM_CLP_CIRC_PNT) {
			px = LOWORD(lp);
			py = HIWORD(lp);
		}
		else if (mode == IDM_CLP_CIRC_LINE) {
			xs = LOWORD(lp);
			ys = HIWORD(lp);
		}
		else if (mode == IDM_ELPS_DIRECT || mode == IDM_ELPS_POLAR || mode == IDM_ELPS_MID)
		{
			x1 = LOWORD(lp);
			y1 = HIWORD(lp);
		}
		else if (mode == IDM_CRV_HRMT) {
			hdc = GetDC(hwnd);
			int left = 200, top = 100, size = 300;
			MySquare(hdc, left, top, size, RGB(0, 0, 0));
			hermitFill(hdc, left, top, size, color);
			ReleaseDC(hwnd, hdc);
		}
		else if (mode == IDM_CRV_BZR) {
			hdc = GetDC(hwnd);
			MyRectangle(hdc, left, top, right, bottom, RGB(0, 0, 0));
			bezierFill(hdc, left, top, right, bottom, color);
			ReleaseDC(hwnd, hdc);
		}
		else if (mode == IDM_SPLINE) {
			hdc = GetDC(hwnd);
			if (splinecnt <= 10) {
				spline[splinecnt].x = LOWORD(lp);
				spline[splinecnt].y = HIWORD(lp);
				splinecnt++;
				if (splinecnt == 10) {
					drawsplinecurve(hdc, spline, color);
					ReleaseDC(hwnd, hdc);
					splinecnt = 0;
				}
			}
			else {
				ReleaseDC(hwnd, hdc);
			}
		}
		break;
	case WM_LBUTTONUP:
		if (drawing)
		{
			if (mode == IDM_LINE_DDA) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				DDALine(hdc, x1, y1, x2, y2, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_LINE_MID) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				MidpointLine(hdc, x1, y1, x2, y2, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_LINE_PARAM) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				ParametricLine(hdc, x1, y1, x2, y2, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CIRC_DIRECT) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				Direct(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CIRC_POLAR) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				Polar(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CIRC_ITRVPOLAR) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				IterativePolar(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CIRC_MID) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				MidpointCircle(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CIRC_MODMID) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_FIRST_LINE) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter1(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_SECOND_LINE) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter2(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_THIRD_LINE) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter3(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_FOURTH_LINE) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter4(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_FIRST_CIRC) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter1circ(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_SECOND_CIRC) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter2circ(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_THIRD_CIRC) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter3circ(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_QRTR_FOURTH_CIRC) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				r = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				ModifiedMidpointCircle(hdc, x1, y1, r, color);
				FillQuarter4circ(hdc, x1, y1, r, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CLP_REC_LINE || mode == IDM_CLP_SQR_LINE) {
				lx1 = LOWORD(lp);
				ly1 = HIWORD(lp);
			}
			else if (mode == IDM_CLP_REC_POLY) {
				hdc = GetDC(hwnd);
				if (cnt <= 5) {
					poly[cnt].x = LOWORD(lp);
					poly[cnt].y = HIWORD(lp);
					cnt++;
					if (cnt == 5) {
						//DrawPolygon(hdc, poly, 5, color);
						PolygonClip(hdc, poly, n, left, top, right, bottom, color);
					}
				}
			}

			else if (mode == IDM_ELPS_MID) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				A = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				MidpointEllipse(hdc, x1, y1, A, 70, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_ELPS_POLAR) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				A = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				PolarEllipse(hdc, x1, y1, A, 70, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_ELPS_DIRECT) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				A = sqrt(pow((x1 - x2), 2) - pow((y1 - y2), 2));
				cartesianEllipse(hdc, x1, y1, A, 35, color);
				ReleaseDC(hwnd, hdc);
			}
		}
		break;
	case WM_RBUTTONUP:
		if (drawing) {
			if (mode == IDM_CLP_REC_PNT) {
				hdc = GetDC(hwnd);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				double left = 200, top = 100, right = 700, bottom = 400;
				MyRectangle(hdc, left, top, right, bottom, RGB(0, 0, 0));
				SelectObject(hdc, hbrOld); // restore old brush
				ClipPnt(hdc, x, y, left, top, right, bottom, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CLP_REC_LINE) {
				lx2 = LOWORD(lp);
				ly2 = HIWORD(lp);
				hdc = GetDC(hwnd);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				int left = 200, top = 100, right = 700, bottom = 400;
				MyRectangle(hdc, left, top, right, bottom, RGB(0, 0, 0));
				SelectObject(hdc, hbrOld); // restore old brush
				CohenSuth(hdc, lx1, ly1, lx2, ly2, left, top, right, bottom, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CLP_SQR_PNT) {
				hdc = GetDC(hwnd);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				int left = 200, top = 100, size = 300;
				MySquare(hdc, left, top, size, RGB(0, 0, 0));
				SelectObject(hdc, hbrOld); // restore old brush
				ClipPnt(hdc, x, y, left, top, left + size, top + size, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CLP_SQR_LINE) {
				lx2 = LOWORD(lp);
				ly2 = HIWORD(lp);
				hdc = GetDC(hwnd);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				int left = 200, top = 100, size = 300;
				MySquare(hdc, left, top, size, RGB(0, 0, 0));
				SelectObject(hdc, hbrOld); // restore old brush
				CohenSuth(hdc, lx1, ly1, lx2, ly2, left, top, left + size, top + size, color);
				ReleaseDC(hwnd, hdc);
			}
			else if (mode == IDM_CLP_CIRC_PNT) {
				hdc = GetDC(hwnd);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				PAINTSTRUCT ps;
				ModifiedMidpointCircle(hdc, 600, 300, r, color);

				circClipPnt(hdc, px, py, 600, 300, r, color);
				SelectObject(hdc, hbrOld); // restore old brush
			}
			else if (mode == IDM_CLP_CIRC_LINE) {
				xe = LOWORD(lp);
				ye = HIWORD(lp);
				hdc = GetDC(hwnd);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				PAINTSTRUCT ps;
				ModifiedMidpointCircle(hdc, 600, 300, r, color);
				ClipLineCircle(hdc, xs, ys, xe, ye, 600, 300, r, color);
				SelectObject(hdc, hbrOld); // restore old brush
				break;
			}
			else if (mode == IDM_CLP_REC_POLY) {
				hdc = GetDC(hwnd);
				x2 = LOWORD(lp);
				y2 = HIWORD(lp);
				HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbrBackground); // select new brush
				MyRectangle(hdc, left, top, right, bottom, color);
				SelectObject(hdc, hbrOld); // restore old brush
				ReleaseDC(hwnd, hdc);
			}
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// Fill the window with the current color
		FillRect(hdc, &ps.rcPaint, hcurrent);
		EndPaint(hwnd, &ps);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteObject(hcurrent);
		DeleteObject(hgray);
		DeleteObject(hwhite);
		break;
	case WM_CREATE:
		if (mode == IDM_CLP_CIRC_PNT || mode == IDM_CLP_CIRC_LINE) {
			RECT rect;
			GetClientRect(hwnd, &rect);
			windowWidth = rect.right - rect.left;
			windowHeight = rect.bottom - rect.top;
			break;

		}
	default:
		return DefWindowProc(hwnd, mcode, wp, lp);
		return 0;
	}
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
	wc.hbrBackground = hBrush;
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"Graphics project 2023", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
	//CREATING THE MENU
	HMENU hMenuBar = CreateMenu();
	HMENU BGColorMenu = CreatePopupMenu();
	HMENU colorsMenu = CreatePopupMenu();
	HMENU linesMenu = CreatePopupMenu();
	HMENU circMenu = CreatePopupMenu();
	HMENU quartrsLineMenu = CreatePopupMenu();
	HMENU quartrsCircMenu = CreatePopupMenu();
	HMENU fillingMenu = CreatePopupMenu();
	HMENU floodfilMenu = CreatePopupMenu();
	HMENU clippingSqrMenu = CreatePopupMenu();
	HMENU clippingRecMenu = CreatePopupMenu();
	HMENU clippingCircMenu = CreatePopupMenu();
	HMENU elipseMenu = CreatePopupMenu();
	HMENU curves = CreatePopupMenu();
	HMENU files = CreatePopupMenu();
	HMENU Clear = CreatePopupMenu();

	//APPINDING OPTIONS
	AppendMenu(BGColorMenu, MF_STRING, IDM_BGCOLOR_WHITE, L"&Change Color to White");
	AppendMenu(BGColorMenu, MF_STRING, IDM_BGCOLOR_GRAY, L"&Change Color to Gray");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)BGColorMenu, L"&Background Color");

	AppendMenu(colorsMenu, MF_STRING, IDM_COLOR_RED, L"&Red color");
	AppendMenu(colorsMenu, MF_STRING, IDM_COLOR_GREEN, L"&Green color");
	AppendMenu(colorsMenu, MF_STRING, IDM_COLOR_BLUE, L"&Blue color");
	AppendMenu(colorsMenu, MF_STRING, IDM_COLOR_BLCK, L"&Black color");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)colorsMenu, L"&Colors");

	AppendMenu(linesMenu, MF_STRING, IDM_LINE_DDA, L"&DDA line");
	AppendMenu(linesMenu, MF_STRING, IDM_LINE_MID, L"&Midpoint Line");
	AppendMenu(linesMenu, MF_STRING, IDM_LINE_PARAM, L"&Parametric Line");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)linesMenu, L"&Lines");

	AppendMenu(circMenu, MF_STRING, IDM_CIRC_DIRECT, L"&Direct Circle");
	AppendMenu(circMenu, MF_STRING, IDM_CIRC_POLAR, L"&Polar Circle");
	AppendMenu(circMenu, MF_STRING, IDM_CIRC_ITRVPOLAR, L"&Iterative Polar circle");
	AppendMenu(circMenu, MF_STRING, IDM_CIRC_MID, L"&MidPoint Circle");
	AppendMenu(circMenu, MF_STRING, IDM_CIRC_MODMID, L"&Modified MidPoint Circle");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)circMenu, L"&Circles");

	AppendMenu(quartrsLineMenu, MF_STRING, IDM_QRTR_FIRST_LINE, L"&First Quarter line filling");
	AppendMenu(quartrsLineMenu, MF_STRING, IDM_QRTR_SECOND_LINE, L"&Second Quarter line filling");
	AppendMenu(quartrsLineMenu, MF_STRING, IDM_QRTR_THIRD_LINE, L"&Third Quarter line filling");
	AppendMenu(quartrsLineMenu, MF_STRING, IDM_QRTR_FOURTH_LINE, L"&Fourth Quarter line filling");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)quartrsLineMenu, L"&Line Filling Quarters");

	AppendMenu(quartrsCircMenu, MF_STRING, IDM_QRTR_FIRST_CIRC, L"&First Quarter circle filling");
	AppendMenu(quartrsCircMenu, MF_STRING, IDM_QRTR_SECOND_CIRC, L"&Second Quarter circle filling");
	AppendMenu(quartrsCircMenu, MF_STRING, IDM_QRTR_THIRD_CIRC, L"&Third Quarter circle filling");
	AppendMenu(quartrsCircMenu, MF_STRING, IDM_QRTR_FOURTH_CIRC, L"&Fourth Quarter circle filling");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)quartrsCircMenu, L"&Circle Filling Quarters");

	AppendMenu(fillingMenu, MF_STRING, IDM_CONVEX_POLYGON_FILL, L"&Convex Polygon filling");
	AppendMenu(fillingMenu, MF_STRING, IDM_NON_POLYGON_FILL, L"&Non Convex Polygon filling");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)fillingMenu, L"&Polygon Filling");

	AppendMenu(floodfilMenu, MF_STRING, IDM_RCRSV_FF, L"&Recursive FloodFill");
	AppendMenu(floodfilMenu, MF_STRING, IDM_NON_RCRSV_FF, L"&non recursive FloodFill");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)floodfilMenu, L"&FloodFill");

	AppendMenu(clippingRecMenu, MF_STRING, IDM_CLP_REC_PNT, L"&Clipping Point");
	AppendMenu(clippingRecMenu, MF_STRING, IDM_CLP_REC_LINE, L"&Clipping Line");
	AppendMenu(clippingRecMenu, MF_STRING, IDM_CLP_REC_POLY, L"&Clipping Polygon");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)clippingRecMenu, L"&Rectangle as Clipping Window");

	AppendMenu(clippingSqrMenu, MF_STRING, IDM_CLP_SQR_PNT, L"&Clipping Point");
	AppendMenu(clippingSqrMenu, MF_STRING, IDM_CLP_SQR_LINE, L"&Clipping Line");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)clippingSqrMenu, L"&Square as Clipping Window");

	AppendMenu(clippingCircMenu, MF_STRING, IDM_CLP_CIRC_PNT, L"&Clipping Point");
	AppendMenu(clippingCircMenu, MF_STRING, IDM_CLP_CIRC_LINE, L"&Clipping Line");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)clippingCircMenu, L"&Circle as Clipping Window");

	AppendMenu(elipseMenu, MF_STRING, IDM_ELPS_DIRECT, L"&Direct Ellipse");
	AppendMenu(elipseMenu, MF_STRING, IDM_ELPS_POLAR, L"&polar Ellipse");
	AppendMenu(elipseMenu, MF_STRING, IDM_ELPS_MID, L"&Midpoint Ellipse");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)elipseMenu, L"&Ellipse");

	AppendMenu(curves, MF_STRING, IDM_CRV_HRMT, L"&Fill sqare with Hermit Curve");
	AppendMenu(curves, MF_STRING, IDM_CRV_BZR, L"&Fill rectangle with Bezier Curve");
	AppendMenu(curves, MF_STRING, IDM_SPLINE, L"&Cardinal spline Curve");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)curves, L"&Curves");

	AppendMenu(files, MF_STRING, IDM_LOAD, L"&load");
	AppendMenu(files, MF_STRING, IDM_SAVE, L"&save");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)files, L"&files");
	AppendMenu(Clear, MF_STRING, CLEAR, L"&Clear Shapes");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)Clear, L"&Clear");
	//SETTING MWNU TO WINDOW
	SetMenu(hwnd, hMenuBar);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
