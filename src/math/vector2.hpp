#include <cinttypes>

struct DgVec2 {
	float x, y;
};

struct DgVec2D {
	double x, y;
}

struct DgVec2I {
	int32_t x, y;
};

struct DgVec2U {
	union {
		uint32_t x;
		uint32_t height;
	};
	
	union {
		uint32_t y;
		uint32_t width;
	};
};

struct DgVec2I64 {
	int64_t x, y;
};

template <typename T>
struct DgVec2T {
	T x, y;
};
