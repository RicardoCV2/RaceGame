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

	Cube s;

	s.size = vec3(10, 5, 100);
	s.SetPos(0, 5, 40);
	speedway.add(App->physics->AddBody(s, 0.0f));

	s.size = vec3(20, 2, 100);
	s.SetPos(0, 5, 80);
	s.SetRotation(-30, vec3(1, 0, 0));
	speedway.add(App->physics->AddBody(s, 0.0f));

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
	//We don't need sensors yet, morover this code is dirty

	//sensor->GetTransform(&s.transform);
	//s.Render();



	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

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

