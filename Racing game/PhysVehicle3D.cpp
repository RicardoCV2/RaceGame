#include "PhysVehicle3D.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body,vec3(0,0,0)), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis.color.Set(0.1f, 0.1f, 0.1f, 1);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	Cube top(info.top_size.x, info.top_size.y, info.top_size.z);
	top.color.Set(0.0f, 0.49f, 1.0f, 1);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&top.transform);
	btQuaternion q1 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 toffset(info.top_offset.x, info.top_offset.y, info.top_offset.z);
	toffset = toffset.rotate(q1.getAxis(), q1.getAngle());

	llight.size = { info.llight_size.x, info.llight_size.y, info.llight_size.z };
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&llight.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 lloffset(info.llight_offset.x, info.llight_offset.y, info.llight_offset.z);
	lloffset = lloffset.rotate(q2.getAxis(), q2.getAngle());

	rlight.size = { info.rlight_size.x, info.rlight_size.y, info.rlight_size.z };
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rlight.transform);
	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 rloffset(info.rlight_offset.x, info.rlight_offset.y, info.rlight_offset.z);
	rloffset = rloffset.rotate(q3.getAxis(), q3.getAngle());
	
	Cube top_rlight(info.top_rlight_size.x, info.top_rlight_size.y, info.top_rlight_size.z);
	top_rlight.color.Set(1.0f, 1.0f, 0.0f, 1);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&top_rlight.transform);
	btQuaternion q4 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 top_rloffset(info.top_rlight_offset.x, info.top_rlight_offset.y, info.top_rlight_offset.z);
	top_rloffset = top_rloffset.rotate(q4.getAxis(), q4.getAngle());

	Cube top_llight(info.top_llight_size.x, info.top_llight_size.y, info.top_llight_size.z);
	top_llight.color.Set(1.0f, 1.0f, 0.0f, 1);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&top_llight.transform);
	btQuaternion q5 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 top_lloffset(info.top_llight_offset.x, info.top_llight_offset.y, info.top_llight_offset.z);
	top_lloffset = top_lloffset.rotate(q5.getAxis(), q5.getAngle());

	break_light.size = { info.break_light_size.x, info.break_light_size.y, info.break_light_size.z };
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&break_light.transform);
	btQuaternion q6 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 bloffset(info.break_light_offset.x, info.break_light_offset.y, info.break_light_offset.z);
	bloffset = bloffset.rotate(q6.getAxis(), q6.getAngle());


	if (aux_light == false)
	{
		llight.color.Set(1.0f, 0.0f, 0.0f, 1);
		rlight.color.Set(1.0f, 0.0f, 0.0f, 1);
		break_light.color.Set(1.0f, 0.5f, 0.0f, 1);
	}
	else
	{
		llight.color.Set(1.0f, 1.0f, 1.0f, 1);
		rlight.color.Set(1.0f, 1.0f, 1.0f, 1);
		break_light.color.Set(1.0f, 0.0f, 0.0f, 1);
	}

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	top.transform.M[12] += toffset.getX();
	top.transform.M[13] += toffset.getY();
	top.transform.M[14] += toffset.getZ();

	llight.transform.M[12] += lloffset.getX();
	llight.transform.M[13] += lloffset.getY();
	llight.transform.M[14] += lloffset.getZ();

	rlight.transform.M[12] += rloffset.getX();
	rlight.transform.M[13] += rloffset.getY();
	rlight.transform.M[14] += rloffset.getZ();

	top_rlight.transform.M[12] += top_rloffset.getX();
	top_rlight.transform.M[13] += top_rloffset.getY();
	top_rlight.transform.M[14] += top_rloffset.getZ();
	
	top_llight.transform.M[12] += top_lloffset.getX();
	top_llight.transform.M[13] += top_lloffset.getY();
	top_llight.transform.M[14] += top_lloffset.getZ();

	break_light.transform.M[12] += bloffset.getX();
	break_light.transform.M[13] += bloffset.getY();
	break_light.transform.M[14] += bloffset.getZ();


	chassis.Render();
	top.Render();
	llight.Render();
	rlight.Render();
	top_rlight.Render();
	top_llight.Render();
	break_light.Render();
	
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}
// ----------------------------------------------------------------------------
void PhysVehicle3D::Stop()
{
	vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
	vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}