#pragma once
#include <external/Eigen/Core>

using namespace Eigen;
using namespace std;

class TrailPoint
{
public:
	TrailPoint(Vector3d pos, Vector3d color)
	{
		Pos = pos;
		Color = color;
	}
	Vector3d Pos;
	Vector3d Color;
};
