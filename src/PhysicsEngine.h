#pragma once

#include <math.h>
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
					bool collided = CalculateCollision(e1, e2, i, j);
					if (!collided)
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

			Vector3d direction = e2.Pos - e1.Pos;
			direction.normalize();

			double distance = abs(sqrt( pow(e1.Pos.x() - e2.Pos.x(), 2) + pow(e1.Pos.y() - e2.Pos.y(), 2) + pow(e1.Pos.z() - e2.Pos.z(), 2) ));
			double fGrav = (GRAV * e2.Mass) / sqrt(pow(distance, 4) + pow(a, 2));

			Vector3d acceleration(direction.x(), direction.y(), direction.z());
			acceleration *= fGrav;

			e1.Velocity += acceleration;
		}
	}
	
	bool CalculateCollision(Entity& e1, Entity& e2, int index_e1, int index_e2)
	{
		double distance = abs(sqrt( pow(e1.Pos.x() - e2.Pos.x(), 2) + pow(e1.Pos.y() - e2.Pos.y(), 2) + pow(e1.Pos.z() - e2.Pos.z(), 2) ));
		if (distance < 10000)
		{
			cout << e1.Mass << ' ' << e2.Mass << endl;
			float diff =  abs((float)(e1.Mass / e2.Mass));
			
			cout << "Collision detected!" << endl;
			cout << "diff: " << diff << endl;
			
			if (diff > 0.3)
			{
				Vector3d startPos = e1.Pos.head<3>();
				Vector3d direction = e2.Pos - e1.Pos;
				direction.normalize();
				
				AngleAxisd rot1(0.5235988, Vector3d::UnitZ());
				AngleAxisd rot2(-0.5235988, Vector3d::UnitZ());
				
				Vector3d rotVecLeft = rot1.toRotationMatrix() * direction;
				rotVecLeft.normalize();
				
				Vector3d rotVecRight = rot2.toRotationMatrix() * direction;
				rotVecRight.normalize();
				
				int extraDistance = 100000;
				
				Vector3d leftPiecePos = startPos + extraDistance * rotVecLeft;
				Vector3d centerPiecePos = startPos + extraDistance * direction;
				Vector3d rightPiecePos = startPos + extraDistance * rotVecRight;
				
				cout << "Angles: " << rotVecLeft(0) << ' ' << rotVecLeft(1) << " | " << rotVecRight(0) << ' ' << rotVecRight(1) << endl;
				cout << "Positions: " << leftPiecePos(0) << ' ' << leftPiecePos(1) << " | " << centerPiecePos(0) << ' ' << centerPiecePos(1) << " | " << rightPiecePos(0) << ' ' << rightPiecePos(1) << endl;
				
				Vector3d newVelocity = (e1.Velocity + e2.Velocity) / 2;
				
				Vector3d leftPieceVel = rot1.toRotationMatrix() * newVelocity;
				Vector3d centerPieceVel = rot1.toRotationMatrix() * rot2.toRotationMatrix() * newVelocity;
				Vector3d rightPieceVel = rot2.toRotationMatrix() * newVelocity;
				
				unsigned long long pieceMass = e1.Mass / 3 + e2.Mass / 3;
				cout << "piecemass: " << pieceMass << endl;
				cout << "result: " << e1.Mass / 3 << endl;
				
				Entity leftPiece("Debris", leftPiecePos, leftPieceVel, pieceMass, '@', Vector3d(230, 230, 230));
				Entity centerPiece("Debris", centerPiecePos, centerPieceVel, pieceMass, '@', Vector3d(230, 230, 230));
				Entity rightPiece("Debris", rightPiecePos, rightPieceVel, pieceMass, '@', Vector3d(230, 230, 230));
				
				Entities.push_back(leftPiece);
				Entities.push_back(centerPiece);
				Entities.push_back(rightPiece);
				
				if (index_e1 > index_e2)
				{
					Entities.erase(Entities.begin() + index_e1);
					Entities.erase(Entities.begin() + index_e2);
				}
				else
				{
					Entities.erase(Entities.begin() + index_e2);
					Entities.erase(Entities.begin() + index_e1);
				}
				
			}
			else
			{
				cout << "Old mass:      " << e1.Mass << endl;
				e1.Mass += e2.Mass;
				cout << "Combined mass: " << e1.Mass << endl;
				Entities.erase(Entities.begin() + index_e2);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	void CalculatePositions()
	{
		for (Entity& e : Entities)
		{
			if (e.PhysicsEnabled)
			{
				e.Pos += Vector3d(e.Velocity.x(), e.Velocity.y(), e.Velocity.z()) * pow(10, Simspeed());
			}
		}
	}

public:
	double massThreshold = 0;
	vector<Entity> Entities;
	
	PhysicsEngine()
	{
		Simspeed(4.0);

		Entity Sun("Sun", Vector3d(4500e+6f, 2500e+6f, 0), Vector3d(0, 0, 0), 1.989e+18, 'O', Vector3d(247, 204, 62));
		Entities.push_back(Sun);

		Entity Mercury("Mercury", Sun.Pos + Vector3d(0, 46e+6f, 0), Vector3d(58.98f, 0, 0), 0.330e+12, 'm', Vector3d(255, 142, 43));
		Entities.push_back(Mercury);

		Entity Venus("Venus", Sun.Pos + Vector3d(108.94e+6f, 0, 0), Vector3d(0, 34.79f, 0), 4.868e+12, 'v', Vector3d(222, 232, 125));
		Entities.push_back(Venus);

		Entity Earth("Earth", Sun.Pos + Vector3d(147.10e+6f, 0, 0), Vector3d(0, 30.29f, 0), 5.972e+12, 'e', Vector3d(0, 119, 217));
		
		Entity Moon("Moon", Earth.Pos + Vector3d(0, 405500, 0), Vector3d(0.970f + Earth.Velocity.x(), Earth.Velocity.y(), Earth.Velocity.z()), 7.35e+9, '@', Vector3d(150, 150, 150));
		Entities.push_back(Moon);
		Entities.push_back(Earth);

		Entity Mars("Mars", Sun.Pos + Vector3d(249.10e+6f, 0, 0), Vector3d(0, 21.97f, 0), 0.641e+12, 'M', Vector3d(255, 80, 33));
		Entities.push_back(Mars);

		Entity Jupiter("Jupiter", Sun.Pos + Vector3d(816e+6f, 0, 0), Vector3d(0, 12.44f, 0), 1898e+12, 'j', Vector3d(199, 162, 42));
		Entities.push_back(Jupiter);

		Entity Saturn("Saturn", Sun.Pos + Vector3d(1514e+6f, 0, 0), Vector3d(0, 9.09f, 0), 568e+12, 's', Vector3d(214, 187, 101));
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

