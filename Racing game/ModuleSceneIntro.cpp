#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//We don't need sensors yet, morover this code is dirty

	//s.size = vec3(5, 3, 1);
	//s.SetPos(0, 2.5f, 20);

	//sensor = App->physics->AddBody(s, 0.0f);
	//sensor->SetAsSensor(true);
	//sensor->collision_listeners.add(this);

	

	p2List<SpeedwayPieceDef> speedway_pieces_def;

	SpeedwayPieceDef piece1(vec3(10, 5, 100), vec3(0, 5, 40));
	speedway_pieces_def.add(piece1);

	SpeedwayPieceDef piece2(vec3(10, 5, 100), vec3(0, 5, 80), -20, vec3(1, 0, 0));
	speedway_pieces_def.add(piece2);

	SpeedwayPieceDef piece3(vec3(10, 5, 50), vec3(0, 5, 195));
	speedway_pieces_def.add(piece3);

	SpeedwayPieceDef piece4(vec3(10, 5, 50), vec3(-20, 5, 230), -50, vec3 (0,1,0));
	speedway_pieces_def.add(piece4);

	SpeedwayPieceDef piece5(vec3(10, 5, 100), vec3(-37, -10, 300), 10, vec3(1,0,0));
	speedway_pieces_def.add(piece5);

	SpeedwayPieceDef piece6(vec3(20, 5, 200), vec3(-37, -20, 450));
	speedway_pieces_def.add(piece6);

	for (int i = 0; i < speedway_pieces_def.count(); i++)
	{
		Cube s;
		SpeedwayPieceDef piece_def;
		speedway_pieces_def.at(i, piece_def);

		s.size = piece_def.size;
		s.SetPos(piece_def.position.x, piece_def.position.y, piece_def.position.z);
		if (piece_def.angle != 0)
		{ 
			s.SetRotation(piece_def.angle, piece_def.rotation_axis);
		}
		speedway.add(App->physics->AddBody(s, 0.0f));
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//We don't need sensors nor planes yet, morover this code is dirty

	//sensor->GetTransform(&s.transform);
	//s.Render();

	//Plane p(0, 1, 0, 0);
	//p.axis = true;
	//p.Render();

	//Blit all 
	for (int i = 0; i < speedway.count(); i++)
	{
		Cube runder;
		PhysBody3D* body;
		speedway.at(i, body);
		body->GetTransform(&runder.transform);
		runder.size = body->size;
		runder.Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");
}

