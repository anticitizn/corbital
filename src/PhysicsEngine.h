#pragma once

#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <external/Eigen/Geometry>
#include <src/Entity.h>

using namespace std;

class PhysicsEngine
{
private:
	double GRAV;
	double simspeedFactor;

	void DoEntityInteractions()
	{
		for (long unsigned int i = 0; i < Entities.size(); i++)
		{
			for (long unsigned int j = 0; j < Entities.size(); j++)
			{
				Entity& e1 = Entities[i];
				Entity& e2 = Entities[j];
				if (&e1 != &e2 && e1.PhysicsEnabled && e2.PhysicsEnabled)
				{
					CalculateVelocity(e1, e2);
				}
			}
		}
	}
	
	void CalculateVelocity(Entity& e1, Entity& e2)
	{
		if (e2.Mass > massThreshold && e2.pullsOtherBodies)
		{
			// give it a more sensible name
			int a = 5; // fGrav hard limit, to prevent singularities

			Vector2d direction = e2.Pos - e1.Pos;
			direction.normalize();

			double distance = abs(sqrt( pow(e1.Pos.x() - e2.Pos.x(), 2) + pow(e1.Pos.y() - e2.Pos.y(), 2)));
			double fGrav = (GRAV * e2.Mass) / sqrt(pow(distance, 4) + pow(a, 2));

			Vector2d acceleration(direction.x(), direction.y());
			acceleration *= fGrav;

			e1.Velocity += acceleration;
		}
	}
	

	void CalculatePositions()
	{
		for (Entity& e : Entities)
		{
			if (e.PhysicsEnabled)
			{
				e.Pos += Vector2d(e.Velocity.x(), e.Velocity.y()) * pow(10, Simspeed());
			}
		}
	}

public:
	double massThreshold = 0;
	vector<Entity> Entities;
	
	PhysicsEngine()
	{
		Simspeed(4.0);

		Entity Sun("Sun", Vector2d(4500e+6f, 2500e+6f), Vector2d(0, 0), 1.989e+18, 'O', Vector3d(247, 204, 62));
		Entities.push_back(Sun);

		Entity Mercury("Mercury", Sun.Pos + Vector2d(0, 46e+6f), Vector2d(58.98f, 0), 0.330e+12, 'm', Vector3d(255, 142, 43));
		Entities.push_back(Mercury);

		Entity Venus("Venus", Sun.Pos + Vector2d(108.94e+6f, 0), Vector2d(0, 34.79f), 4.868e+12, 'v', Vector3d(222, 232, 125));
		Entities.push_back(Venus);

		Entity Earth("Earth", Sun.Pos + Vector2d(147.10e+6f, 0), Vector2d(0, 30.29f), 5.972e+12, 'e', Vector3d(0, 119, 217));
		
		Entity Moon("Moon", Earth.Pos + Vector2d(0, 405500), Vector2d(0.970f + Earth.Velocity.x(), Earth.Velocity.y()), 7.35e+9, '@', Vector3d(150, 150, 150));
		Entities.push_back(Moon);
		Entities.push_back(Earth);

		Entity Mars("Mars", Sun.Pos + Vector2d(249.10e+6f, 0), Vector2d(0, 21.97f), 0.641e+12, 'M', Vector3d(255, 80, 33));
		Entities.push_back(Mars);

		Entity Jupiter("Jupiter", Sun.Pos + Vector2d(816e+6f, 0), Vector2d(0, 12.44f), 1898e+12, 'j', Vector3d(199, 162, 42));
		Entities.push_back(Jupiter);

		Entity Saturn("Saturn", Sun.Pos + Vector2d(1514e+6f, 0), Vector2d(0, 9.09f), 568e+12, 's', Vector3d(214, 187, 101));
		Entities.push_back(Saturn);

	}

	void Tick()
	{
		DoEntityInteractions();
		CalculatePositions();
	}
	
	double Simspeed()
	{
		return simspeedFactor;
	}
	
	void Simspeed(double factor)
	{
		simspeedFactor = factor;
		GRAV = 6.67430 * pow(10, simspeedFactor - 8);
	}
};

