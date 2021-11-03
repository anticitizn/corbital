#pragma once

#include <string>
#include <vector>
#include <external/Eigen/Core>
#include <src/TrailPoint.h>

using namespace std;
using namespace Eigen;

class Entity
{
public:
	Entity(string name, Vector2d pos, Vector2d velocity, unsigned long long mass, char symbol)
	{
		Name = name;
		Pos = pos;
		Velocity = velocity;
		Mass = mass;
		Symbol = symbol;
		Color = Vector3d(255, 255, 255);
		PhysicsEnabled = true;
	}

	Entity(string name, Vector2d pos, Vector2d velocity, unsigned long long mass, char symbol, Vector3d color)
	{
		Name = name;
		Pos = pos;
		Velocity = velocity;
		Mass = mass;
		Symbol = symbol;
		Color = color;
		PhysicsEnabled = true;
	}

	Entity(string name, Vector2d pos, Vector2d velocity, unsigned long long mass, char symbol, bool physicsEnabled)
	{
		Name = name;
		Pos = pos;
		Velocity = velocity;
		Mass = mass;
		Symbol = symbol;
		PhysicsEnabled = physicsEnabled;
	}

	Entity()
	{
		//Name = "Empty entity";
		//Pos = Vector4d(0);
		//Velocity = Vector4d(0);
		//Mass = 0;
		//Symbol = '_';
		//Color = Vector4d(255, 0, 255, 1);
		//PhysicsEnabled = false;
	}
	
	string Name;
	Vector2d Pos;
	Vector2d Velocity;
	unsigned long long Mass;
	Vector3d Color;
	char Symbol;
	bool PhysicsEnabled;
	bool pullsOtherBodies = true;
	
};
