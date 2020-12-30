class Vec3 {
public:
	/*
	 * This is a basic implementation of a 3D vector. This uses single-percision
	 * floating point numbers.
	 * 
	 * TODO: Other operations, and a version that uses doubles/int32/int64
	 */
	float x, y, z;
	
	float magnitude();
	Vec3 normalised();
	Vec3 scaled(float);
	Vec3 negated();
	void set(float, float, float);
	Vec3 operator+(Vec3);
	Vec3 operator-(Vec3);
	Vec3 operator*(Vec3);
};

Vec3 getZeroVec3();
