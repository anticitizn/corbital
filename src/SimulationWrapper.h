#pragma once

#include <math.h>
#include <string>
#include <src/PhysicsEngine.h>
#include <src/Camera.h>
#include <external/omar/src/omar.h>

using namespace omar;

class SimulationWrapper
{
public:
	Camera camera;
	SimulationWrapper()
	{
		window.initialize();
		camera.Scale = 10000000;
		CenterCameraOnPos(physen.Entities[0].Pos.x(), physen.Entities[0].Pos.y());
	}
	
	void CenterCameraOnPos(double x, double y)
	{
		camera.Pos = Vector3d(x, y, 0);
	}

	void Tick()
	{
		physen.Tick();
		CalculateTrails();
		//~ CenterCameraOnPos(physen.Entities[3].Pos.x(), physen.Entities[3].Pos.y());
		Draw();
	}

	void Draw()
	{
		for (TrailPoint& tp : Trails)
		{
			int x = round((tp.Pos.x() - camera.Pos.x()) / camera.Scale) + window.getTileDimensions().x / 2;
			int y = round((tp.Pos.y() - camera.Pos.y()) / camera.Scale) + window.getTileDimensions().y / 2;
			
			window.setChar(x, y, '.');
			window.setTextColor(x, y, tp.Color.x(), tp.Color.y(), tp.Color.z());
		}

		for (Entity& e : physen.Entities)
		{
			int x = round((e.Pos.x() - camera.Pos.x()) / camera.Scale) + window.getTileDimensions().x / 2;
			int y = round((e.Pos.y() - camera.Pos.y()) / camera.Scale) + window.getTileDimensions().y / 2;
			
			window.setChar(x, y, e.Symbol);
			window.setTextColor(x, y, e.Color.x(), e.Color.y(), e.Color.z());
			//~ cout << e.Name << ' ' << x << ' ' << y << endl;
		}
		
		window.draw();
		window.clear();
	}

	void SelectEntity(const int& num)
	{
		if (num == 1)
		{
			selectedEntity = Entity("Asteroid", Vector3d(), Vector3d(), 0.150e+12, '@', Vector3d(230, 230, 230));
		}
		else if (num == 2)
		{
			selectedEntity = Entity("Star", Vector3d(4500e+6f, 2500e+6f, 0), Vector3d(0, 0, 0), 1.989e+18, 'O', Vector3d(255, 180, 62));
		}
		else if (num == 3)
		{
			selectedEntity = Entity("Giant star", Vector3d(4500e+6f, 2500e+6f, 0), Vector3d(0, 0, 0), 1.989e+20, '0', Vector3d(60, 60, 230));
		}
		else
		{
			selectedEntity = Entity("Asteroid", Vector3d(), Vector3d(), 0.150e+12, '@');
		}
	}
	
	void ChangeTimestep(double factor)
	{
		physen.Simspeed(factor);
	}

	void OnClick(const int& x, const int& y)
	{
		if (entityPlacementMode == false)
		{
			entityPlacementMode = true;

			newEntity = selectedEntity;
			newEntity.PhysicsEnabled = false;
			newEntity.Velocity = Vector3d();
			
			// replace 10 with window.tileSize
			newEntity.Pos = Vector3d(((x / 10) - window.getTileDimensions().x / 2) * camera.Scale + camera.Pos.x(), ((y / 10) - window.getTileDimensions().y / 2) * camera.Scale + camera.Pos.y(), 0);
			physen.Entities.push_back(newEntity);

			lastClickPosX = x;
			lastClickPosY = y;

			cout << "newEntity: " << newEntity.Pos.x() << ' ' << newEntity.Pos.y() << endl;
		}
		else
		{
			entityPlacementMode = false;
			physen.Entities[physen.Entities.size() - 1].Velocity = Vector3d(x - lastClickPosX, y - lastClickPosY, 0) / 10;
			physen.Entities[physen.Entities.size() - 1].PhysicsEnabled = true;
		}
	}

	void Clear()
	{
		Trails.clear();
	}

private:
	PhysicsEngine physen; // (10.0f, ' ')
	Terminal window = Terminal(10.0f, ' ', (string)"");

	Entity selectedEntity = Entity("Asteroid", Vector3d(), Vector3d(), 0.150e+12, '@');
	Entity newEntity;

	bool entityPlacementMode = false;

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

