#pragma once
#include "PhysicsEngine.h"
#include "external/omar/omar.h"
#include <math.h>

class SimulationWrapper
{
public:
	SimulationWrapper()
	{
		window.initialize(10.0f, ' ');
	}
	
	SimulationWrapper(PhysicsEngine Physen, Terminal Window)
	{
		physen = Physen;
		window = Window;
	}

	void Tick()
	{
		physen.Tick();
		CalculateTrails();
		Draw();
	}

	void Draw()
	{
		for (TrailPoint& tp : Trails)
		{
			window.setChar(round(tp.Pos.x() / scale) - xoffset, round(tp.Pos.y() / scale) - yoffset, '.');
			window.setColor(round(tp.Pos.x() / scale) - xoffset, round(tp.Pos.y() / scale) - yoffset, tp.Color.x(), tp.Color.y(), tp.Color.z());
		}

		for (Entity& e : physen.Entities)
		{
			// TO-DO: automate x and y offsets so that the camera always starts with the sun in the center
			window.setChar(round(e.Pos.x() / scale) - xoffset, round(e.Pos.y() / scale) - yoffset, e.Symbol);
			window.setColor(round(e.Pos.x() / scale) - xoffset, round(e.Pos.y() / scale) - yoffset, e.Color.x(), e.Color.y(), e.Color.z());
			// cout << e.Name << ' ' << round(e.Pos.x() / scale) - xoffset << ' ' << round(e.Pos.y() / scale) - yoffset << endl;
		}
		
		window.draw();
		window.clear();
	}

	void SelectEntity(const int& num)
	{
		if (num == 1)
		{
			selectedEntity = Entity("Asteroid", Vector4d(), Vector4d(), 0.150e+12, '@', Vector3d(230, 230, 230));
		}
		else if (num == 2)
		{
			selectedEntity = Entity("Star", Vector4d(4500e+6f, 2500e+6f, 0, 1), Vector4d(0, 0, 0, 0), 1.989e+18, 'O', Vector3d(255, 180, 62));
		}
		else if (num == 3)
		{
			selectedEntity = Entity("Giant star", Vector4d(4500e+6f, 2500e+6f, 0, 1), Vector4d(0, 0, 0, 0), 1.989e+20, '0', Vector3d(60, 60, 230));
		}
		else
		{
			selectedEntity = Entity("Asteroid", Vector4d(), Vector4d(), 0.150e+12, '@');
		}
	}

	void OnClick(const int& x, const int& y)
	{
		//cout << "OnClick: " << x << ' ' << y << endl;
		if (entityPlacementMode == false)
		{
			entityPlacementMode = true;

			newEntity = selectedEntity;
			newEntity.PhysicsEnabled = false;
			newEntity.Velocity = Vector4d();
			newEntity.Pos = Vector4d(((long long)x / 10 + xoffset) * (long long)scale, ((long long)y / 10 + yoffset)* (long long)scale, 0, 0);
			physen.Entities.push_back(newEntity);

			lastClickPosX = x;
			lastClickPosY = y;

			// cout << "newEntity: " << newEntity.Pos.x() << ' ' << newEntity.Pos.y() << endl;
		}
		else
		{
			entityPlacementMode = false;
			physen.Entities[physen.Entities.size() - 1].Velocity = Vector4d(x - lastClickPosX, y - lastClickPosY, 0, 0) / 10;
			physen.Entities[physen.Entities.size() - 1].PhysicsEnabled = true;
		}
	}

	void Clear()
	{
		for (Entity& e : physen.Entities)
		{
			Trails.clear();
		}
	}

private:
	PhysicsEngine physen;
	Terminal window;

	Entity selectedEntity = Entity("Asteroid", Vector4d(), Vector4d(), 0.150e+12, '@');
	Entity newEntity;

	bool entityPlacementMode = false;

	int scale = 10000000;
	int xoffset = 360;
	int yoffset = 200;

	int lastClickPosX = 0;
	int lastClickPosY = 0;

	vector<TrailPoint> Trails;

	void CalculateTrails()
	{
		for (Entity& e : physen.Entities)
		{
			if (Trails.size() > 100 * physen.Entities.size())
			{
				Trails.erase(Trails.begin(), Trails.begin() + physen.Entities.size());
			}

			Trails.push_back(TrailPoint(e.Pos, e.Color));
		}
	}
};

