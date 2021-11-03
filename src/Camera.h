#pragma once
#include <external/Eigen/Core>

using namespace Eigen;
using namespace std;

class Camera
{
public:
	Vector2d Pos; // world pos
	double Scale; // how many km one tile represents
};
