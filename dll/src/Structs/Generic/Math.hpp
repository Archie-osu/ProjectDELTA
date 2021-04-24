#pragma once

struct Vector3
{
	double X, Y, Z;

	Vector3() : X(0), Y(0), Z(0) {}

	Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}
};

using matrix4x4 = float[4][4];