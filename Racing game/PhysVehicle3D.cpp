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

	Cube llight(info.llight_size.x, info.llight_size.y, info.llight_size.z);
	llight.color.Set(1.0f, 0.0f, 0.0f, 1);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&llight.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 lloffset(info.llight_offset.x, info.llight_offset.y, info.llight_offset.z);
	lloffset = lloffset.rotate(q2.getAxis(), q2.getAngle());

	Cube rlight(info.rlight_size.x, info.rlight_size.y, info.rlight_size.z);
	rlight.color.Set(1.0f, 0.0f, 0.0f, 1);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rlight.transform);
	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 rloffset(info.rlight_offset.x, info.rlight_offset.y, info.rlight_offset.z);
	rloffset = rloffset.rotate(q3.getAxis(), q3.getAngle());
	
	
	

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


	chassis.Render();
	top.Render();
	llight.Render();
	rlight.Render();
	
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