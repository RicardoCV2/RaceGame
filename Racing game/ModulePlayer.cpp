#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 5);
	car.chassis_offset.Set(0, 1, 0);

	car.top_size.Set(2, 1, 2);
	car.top_offset.Set(0, 2, -0.5);

	car.llight_size.Set(0.3f, 0.3f, 0.3f);
	car.llight_offset.Set(0.7, 1.2, -2.5);

	car.rlight_size.Set(0.3f, 0.3f, 0.3f);
	car.rlight_offset.Set(-0.7, 1.2, -2.5);

	car.top_rlight_size.Set(0.3f, 0.3f, 0.3f);
	car.top_rlight_offset.Set(-0.7, 1.2, 2.5);

	car.top_llight_size.Set(0.3f, 0.3f, 0.3f);
	car.top_llight_offset.Set(0.7, 1.2, 2.5);

	car.break_light_size.Set(1.0f, 0.1f, 0.1f);
	car.break_light_offset.Set(0, 2.2, -1.5);

	car.mass = 500.0f;
	car.suspensionStiffness = 8.88f;
	car.suspensionCompression = 5.0f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 12, 0);
	

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	z = vehicle->GetPos().z;
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		App->audio->PlayFx(1);
	}


	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->SetVolume(MIX_MAX_VOLUME / 16);

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->SetVolume(-(MIX_MAX_VOLUME / 16));

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->audio->SetFxVolume(MIX_MAX_VOLUME / 16);

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		App->audio->SetFxVolume(-(MIX_MAX_VOLUME / 16));

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		vec3 velocity_respect_player = World_to_Player(vec3(vehicle->vehicle->getRigidBody()->getLinearVelocity().getX(), vehicle->vehicle->getRigidBody()->getLinearVelocity().getY(), vehicle->vehicle->getRigidBody()->getLinearVelocity().getZ()),false);
		
		if(velocity_respect_player.z > -1)
			acceleration = MAX_ACCELERATION;
		else
			brake = BRAKE_POWER;

		Mix_Resume(1);
		if (!driving)
			App->audio->PlayFx(2, 0, 1);
		driving = true;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		vec3 velocity_respect_player = World_to_Player(vec3(vehicle->vehicle->getRigidBody()->getLinearVelocity().getX(), vehicle->vehicle->getRigidBody()->getLinearVelocity().getY(), vehicle->vehicle->getRigidBody()->getLinearVelocity().getZ()),false);

		if (velocity_respect_player.z < 1)
			acceleration = -MAX_ACCELERATION;
		else
			brake = BRAKE_POWER;

		Mix_Resume(1);
		if (!driving)
			App->audio->PlayFx(2, 0, 1);
		driving = true;

	
		vehicle->aux_light = true;
		
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		vehicle->aux_light = false;
	}
	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN
		|| App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) && vehicle->GetKmh() > 70)
		App->audio->PlayFx(3);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT)
	{
		driving = false;
		Mix_Pause(1);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		//Set vehicle transform to 0
		float initial_transform[16];

		for (int i = 0; i < 16; i++)
		{
			initial_transform[i] = 0;
		}
		initial_transform[0] = 1; initial_transform[5] = 1;	 initial_transform[10] = 1;
		vehicle->SetTransform(initial_transform);
		vehicle->SetPos(0, 12, 0);
		//Set vehicle speed to 0
		vehicle->Stop();
	}

	//Reset player if he falls from the circuit
	if (vehicle->GetPos().y < -100)
	{
		Player_reset();
	}
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

		char title[150];
		sprintf_s(title, "Speed: %.1f Km/h | Laps: %d | Current lap time %.2fs | Last lap time: %.2fs | Best lap time: %.2fs", vehicle->GetKmh(),
			App->scene_intro->laps, App->scene_intro->current_lap_time, App->scene_intro->last_lap_time, App->scene_intro->best_lap_time);
		App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::Player_reset()
{
	//Set vehicle transform to 0
	float initial_transform[16];

	for (int i = 0; i < 16; i++)
	{
		initial_transform[i] = 0;
	}
	initial_transform[0] = 1; initial_transform[5] = 1;	 initial_transform[10] = 1;
	vehicle->SetTransform(initial_transform);
	vehicle->SetPos(0, 12, 0);
	//Set vehicle speed to 0
	vehicle->Stop();
	App->scene_intro->started = false;
}

vec3 ModulePlayer::Player_to_World(vec3 vector_to_change, bool translate)
{
	vec3 player_pos = vehicle->GetPos();
	mat3x3 rotation = vehicle->GetRotation();

	////We need to put it in the world frame, so we need: Origin of the player respect the world (player_pos) and Rotation matrix of the player respect the world frame(rotation).
	vec3 ret(0, 0, 0);
	if (translate)
		ret = rotation*vector_to_change + player_pos;
	else
		ret = rotation*vector_to_change;

	return ret;

}

vec3 ModulePlayer::World_to_Player(vec3 vector_to_change, bool translate)
{
	vec3 player_pos = vehicle->GetPos();
	mat3x3 rotation = vehicle->GetRotation();

	////We need to put it in the player frame, so we need: Origin of the world respect the player (-player_pos) and Rotation matrix of the world frame respect the player(rotation').
	rotation = App->physics->translate_3x3mat(rotation);
	player_pos = -player_pos;

	vec3 ret(0, 0, 0);
	if (translate)
		ret = rotation*vector_to_change + player_pos;
	else
		ret = rotation*vector_to_change;

	return ret;

}

//vec3 ModulePlayer::btVector3_to_vec3(btVector3 vector)
//{
//	return vec3(vector.getX(), vector.getY(), vector.getZ());
//}
