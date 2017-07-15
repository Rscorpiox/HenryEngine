#pragma once

#ifndef COLLISIONOBJECT_HPP 
#define COLLISIONOBJECT_HPP

#include "Engine\Physic\Bullet3\btBulletDynamicsCommon.h"
#include "Engine\Core\VertexStruct.hpp"

#include <vector>


namespace Henry
{

class CollisionObject
{
public:
	CollisionObject(btCollisionShape* shape, Vec3f position = Vec3f(), Vec3f orientation = Vec3f(), float mass = 1.0f);
	~CollisionObject();
	void Draw(bool wired = true);
	void ConstructSphereVertices(float radius);
	void ConstructPlaneVertices(Vec3f position, Vec3f orientation);
	void ConstructBoxVertices(Vec3f halfExtendSize);

public:
	btRigidBody* m_rigidBody;
	btCollisionShape* m_shape;
	btRigidBody::btRigidBodyConstructionInfo* m_constructionInfo;
	btDefaultMotionState* m_motionState;

private:
	std::vector<Vertex_PosColor> m_vertices;
};

}

#endif