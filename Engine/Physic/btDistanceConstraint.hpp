#pragma once
#include "Engine\Physic\Bullet3\btBulletDynamicsCommon.h"

#ifndef BTDISTANCECONSTRAINT_HPP
#define BTDISTANCECONSTRAINT_HPP

namespace Henry
{ 

class btDistanceConstraint : public btPoint2PointConstraint
{
public:
	btDistanceConstraint(btRigidBody& rbA, btRigidBody& rbB, const btVector3& pivotInA, const btVector3& pivotInB, btScalar dist);
	~btDistanceConstraint();
	void getInfo1(btConstraintInfo1* info);
	void getInfo2(btConstraintInfo2* info);

private:
	btScalar m_distance;
};

}

#endif