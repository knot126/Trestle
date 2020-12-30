#include <iostream>

// THIS IS OKAY FOR NOW
#include "Vector3.hpp"

int main() {
	Vec3 a, b, c;
	
	b.set(1.0f, 2.0f, -2.5f);
	c.set(1.0f, -1.0f, 1.0f);
	a = c.normalised();
	
	std::cout << c.magnitude() << std::endl;
	std::cout << a.x << " " << a.y << " " << a.z << std::endl;
	a = c * b;
	std::cout << a.x << " " << a.y << " " << a.z << std::endl;
	a = c + b;
	std::cout << a.x << " " << a.y << " " << a.z << std::endl;
	a = c - b;
	std::cout << a.x << " " << a.y << " " << a.z << std::endl;
	
	return 0;
}
