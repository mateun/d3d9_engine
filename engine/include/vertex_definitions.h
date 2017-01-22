#include <d3d9.h>

struct VertexScreenSpaceColor {
	VertexScreenSpaceColor(FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _r, DWORD col) : x(_x), y(_y), z(_z), rhw(_r), color(col) {}
	FLOAT x, y, z, rhw;
	DWORD color;
};

struct VertexPosColor {
	VertexPosColor(FLOAT x, FLOAT y, FLOAT z, DWORD col) : _x(x), _y(y), _z(z), _color(col) {}
	FLOAT _x, _y, _z;
	DWORD _color;
};

struct VertexPosTexNormal {
	VertexPosTexNormal(FLOAT x, FLOAT y, FLOAT z, FLOAT nx, FLOAT ny, FLOAT nz, FLOAT tu, FLOAT tv) : _x(x), _y(y), _z(z), 
		_nx(nx), _ny(ny), _nz(nz), _tu(tu), _tv(tv) {}
	FLOAT _x, _y, _z;
	FLOAT _nx, _ny, _nz;
	FLOAT _tu, _tv;
	
};