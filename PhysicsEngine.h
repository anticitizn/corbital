#pragma once
#include <math.h>
#include <vector>
#include <iostream>
#include "Entity.h"

using namespace std;

string printVec(Vector4d vec)
{
	return to_string(vec.x()) + ' ' + to_string(vec.y()) + ' ' + to_string(vec.z()) + ' ' + to_string(vec.w());
}

class PhysicsEngine
{
private:
	double GRAV = 6.67430 * pow(10, -8 + simspeedFactor);
	double simspeedFactor = 5.0;

	void Simspeed(double factor)
	{
		simspeedFactor = factor;
		GRAV = 6.67430 * pow(10, -8 + simspeedFactor);
	}

	double Simspeed()
	{
		return simspeedFactor;
	}

	void CalculateVelocity()
	{
		for (Entity& e1 : Entities)
		{
			for (Entity& e2 : Entities)
			{
				if (&e1 != &e2 && e1.PhysicsEnabled && e2.PhysicsEnabled && e2.Mass > massThreshold && e2.pullsOtherBodies)
				{
					// give it a more sensible name
					int a = 5; // fGrav hard limit, to prevent singularities

					Vector4d direction = e2.Pos - e1.Pos;
					direction.normalize();

					double distance = abs(sqrt( pow(e1.Pos.x() - e2.Pos.x(), 2) + pow(e1.Pos.y() - e2.Pos.y(), 2) + pow(e1.Pos.z() - e2.Pos.z(), 2) ));
					double fGrav = (GRAV * e2.Mass) / sqrt(pow(distance, 4) + pow(a, 2));

					Vector4d acceleration(direction.x(), direction.y(), direction.z(), 1.0f);
					acceleration *= fGrav;

					e1.Velocity += acceleration;

					/*cout << "############### DEBUG CALC ################" << endl << e1.Name << ' ' << e2.Name << endl << "Direction: " << printVec(direction) << endl << "Acceleration: " << printVec(acceleration) << endl << "Position: " << printVec(e1.Pos) << endl;
					cout << "############### DEBUG END  ################" << endl;*/
				}
			}
		}
	}

	void CalculatePositions()
	{
		for (Entity& e : Entities)
		{
			if (e.PhysicsEnabled)
			{
				e.Pos += Vector4d(e.Velocity.x(), e.Velocity.y(), e.Velocity.z(), 1.0) * pow(10, Simspeed());
			}
		}
	}

public:
	double massThreshold = 0;
	vector<Entity> Entities;
	
	PhysicsEngine()
	{
		Simspeed(5.0);

		Entity Sun("Sun", Vector4d(4500e+6f, 2500e+6f, 0, 1), Vector4d(0, 0, 0, 0), 1.989e+18, 'O', Vector3d(247, 204, 62));
		Entities.push_back(Sun);

		Entity Mercury("Mercury", Sun.Pos + Vector4d(0, 46e+6f, 0, 0), Vector4d(58.98f, 0, 0, 0), 0.330e+12, 'm', Vector3d(255, 142, 43));
		Entities.push_back(Mercury);

		Entity Venus("Venus", Sun.Pos + Vector4d(108.94e+6f, 0, 0, 0), Vector4d(0, 34.79f, 0, 0), 4.868e+12, 'v', Vector3d(222, 232, 125));
		Entities.push_back(Venus);

		Entity Earth("Earth", Sun.Pos + Vector4d(147.10e+6f, 0, 0, 0), Vector4d(0, 30.29f, 0, 0), 5.972e+12, 'e', Vector3d(0, 119, 217));
		Entities.push_back(Earth);

		Entity Mars("Mars", Sun.Pos + Vector4d(249.10e+6f, 0, 0, 0), Vector4d(0, 21.97f, 0, 0), 0.641e+12, 'M', Vector3d(255, 80, 33));
		Entities.push_back(Mars);

		Entity Jupiter("Jupiter", Sun.Pos + Vector4d(816e+6f, 0, 0, 0), Vector4d(0, 12.44f, 0, 0), 1898e+12, 'j', Vector3d(199, 162, 42));
		Entities.push_back(Jupiter);

		//~ Entity Saturn("Saturn", Sun.Pos + Vector4d(1514e+6f, 0, 0, 0), Vector4d(0, 9.09f, 0, 0), 568e+12, 's', Vector4d(214, 187, 101));
		//~ Entities.push_back(Saturn);
		
		//~ for (int i = 0; i < 192; i++)
		//~ {
			//~ Entity Test("Test", Vector4d(100 * i + 1, 0, 0, 0), Vector4d(0, 0, 0, 0), 100, 'a', Vector3d(230, 100, 100));
			//~ Test.PhysicsEnabled = false;
			//~ Entities.push_back(Test);
		//~ }
		
		//~ Entity Test1("Test1", Vector4d(26875000 * 300, 0, 0, 0), Vector4d(0, 0, 0, 0), 100, 'O', Vector3d(255, 255, 255));
		//~ Test1.PhysicsEnabled = false;
		//~ Entities.push_back(Test1);
		
		//~ Entity Test2("Test2", Vector4d(10000000, 0, 0, 0), Vector4d(0, 0, 0, 0), 100, '2', Vector3d(255, 100, 100));
		//~ Test2.PhysicsEnabled = false;
		//~ Entities.push_back(Test2);

	}

	void Tick()
	{
		CalculateVelocity();
		CalculatePositions();
		//Debug();
	}

	void Debug()
	{
		cout << "###############################" << endl;
		for (Entity e : Entities)
		{
			cout << e.Name << endl << printVec(e.Pos) << endl << printVec(e.Velocity) << endl << endl;
		}
	}
};

