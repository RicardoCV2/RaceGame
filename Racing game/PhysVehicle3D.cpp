#include "PhysVehicle3D.h"
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
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	Cube front(info.front_size.x, info.front_size.y, info.front_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front.transform);
	btQuaternion q1 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 foffset(info.front_offset.x, info.front_offset.y, info.front_offset.z);
	foffset = foffset.rotate(q1.getAxis(), q1.getAngle());

	Cube pale(info.pale_size.x, info.pale_size.y, info.pale_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&pale.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 poffset(info.pale_offset.x, info.pale_offset.y, info.pale_offset.z);
	poffset = poffset.rotate(q2.getAxis(), q2.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	front.transform.M[12] += foffset.getX();
	front.transform.M[13] += foffset.getY();
	front.transform.M[14] += foffset.getZ();

	pale.transform.M[12] += poffset.getX();
	pale.transform.M[13] += poffset.getY();
	pale.transform.M[14] += poffset.getZ();


	chassis.Render();
	front.Render();
	pale.Render();
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