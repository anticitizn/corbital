#pragma once
#include "external/Eigen/Core"

using namespace Eigen;
using namespace std;

class TrailPoint
{
public:
	TrailPoint(Vector4d pos, Vector3d color)
	{
		Pos = pos;
		Color = color;
	}
	Vector4d Pos;
	Vector3d Color;
};
