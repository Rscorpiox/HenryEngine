#include "PhysicSystem.hpp"

#include "Engine\Core\HenryFunctions.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"


namespace Henry
{ 

PhysicSystem::PhysicSystem()
{
	m_debugDraw = true;
	m_drawWired = true;
	
	Initialize();
}


PhysicSystem::~PhysicSystem()
{
	delete m_dynamicsWorld;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_overlappingPairCache;
	delete m_solver;
}


void PhysicSystem::Update(double deltaSeconds, int steps)
{
	m_dynamicsWorld->stepSimulation((btScalar)deltaSeconds, steps);
}


void PhysicSystem::Render()
{
	if (m_debugDraw)
	{
		for (int index = 0; index < m_dynamicsWorld->getCollisionObjectArray().size(); ++index)
		{
			btCollisionObject* c = m_dynamicsWorld->getCollisionObjectArray()[index];
			m_dynamicsWorld->debugDrawObject(c->getWorldTransform(), c->getCollisionShape(), btVector3(1.f, 0.f, 1.f));
		}

//		for (size_t index = 0; index < m_constrains.size(); ++index)
//		{
//			m_dynamicsWorld->debugDrawConstraint(m_constrains[index]);
//		}

		OpenGLRenderer::DEFAULT_MATERIAL->Activate();
 		Matrix4 objectToWorldMatrix;
 		OpenGLRenderer::DEFAULT_MATERIAL->SendUniform("u_objectToWorldMatrix", objectToWorldMatrix);
 		std::vector<Vertex_PosColor> vertices;
 		vertices.reserve(m_constraints.size() * 2);
 		for (size_t index = 0; index < m_constraints.size(); ++index)
 		{
 			Vertex_PosColor v[2];
 			btVector3 posA = m_constraints[index]->getRigidBodyA().getWorldTransform().getOrigin();
 			btVector3 posB = m_constraints[index]->getRigidBodyB().getWorldTransform().getOrigin();
 			v[0].position = Vec3f(m_constrainOffsetA[index].x + posA.getX(), m_constrainOffsetA[index].y + posA.getY(), m_constrainOffsetA[index].z + posA.getZ());
 			v[1].position = Vec3f(m_constrainOffsetB[index].x + posB.getX(), m_constrainOffsetB[index].y + posB.getY(), m_constrainOffsetB[index].z + posB.getZ());
 			v[0].color = RGBA();
 			v[1].color = RGBA();
 			vertices.push_back(v[0]);
 			vertices.push_back(v[1]);
 		}
 
 		OpenGLRenderer::DrawVertexWithVertexArray(vertices, OpenGLRenderer::SHAPE_LINES);
	}
}


void PhysicSystem::Initialize()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_overlappingPairCache = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setDebugDrawer(&m_debugDrawer);
}


btVector3 PhysicSystem::GetGravity()
{
	return m_dynamicsWorld->getGravity();
}


void PhysicSystem::SetGravity(float x, float y, float z)
{
	m_dynamicsWorld->setGravity(btVector3(x, y, z));
}


int PhysicSystem::AddSphere(Vec3f pos, Vec3f orient, float radius /* = 1.0f */, float mass /* = 1.0f */, int simulateState /* = ACTIVE_TAG */)
{
	btCollisionShape* shape = new btSphereShape(radius);
	CollisionObject* c = new CollisionObject(shape, pos, orient, mass);
	c->ConstructSphereVertices(radius);
	c->m_rigidBody->forceActivationState(simulateState);
	m_dynamicsWorld->addRigidBody(c->m_rigidBody);

	return m_dynamicsWorld->getCollisionObjectArray().size() - 1;
}


int PhysicSystem::AddPlane(Vec3f pos, Vec3f orient, Vec3f normal, float planeConstant /* = 1.0f */, int simulateState /* = ACTIVE_TAG */)
{
	btCollisionShape* shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), planeConstant);
	CollisionObject* c = new CollisionObject(shape, pos, orient, 0);
	c->ConstructPlaneVertices(pos, orient);
	c->m_rigidBody->forceActivationState(simulateState);
	m_dynamicsWorld->addRigidBody(c->m_rigidBody);

	return m_dynamicsWorld->getCollisionObjectArray().size() - 1;
}


int PhysicSystem::AddBox(Vec3f pos, Vec3f orient, Vec3f halfExtendSize, int simulateState /* = ACTIVE_TAG */)
{
	btCollisionShape* shape = new btBoxShape(btVector3(halfExtendSize.x, halfExtendSize.y, halfExtendSize.z));
	CollisionObject* c = new CollisionObject(shape, pos, orient);
	c->ConstructBoxVertices(halfExtendSize);
	c->m_rigidBody->forceActivationState(simulateState);
	m_dynamicsWorld->addRigidBody(c->m_rigidBody);

	return m_dynamicsWorld->getCollisionObjectArray().size() - 1;
}


ClosestRaycastResult PhysicSystem::ClosestRaycast(Vec3f start, Vec3f end)
{
	btVector3 _start = btVector3(start.x, start.y, start.z);
	btVector3 _end = btVector3(end.x, end.y, end.z);
	ClosestRaycastResult rayCallback(_start, _end);
	m_dynamicsWorld->rayTest(_start, _end, rayCallback);
	return rayCallback;
}


int PhysicSystem::AddConstraint(btTypedConstraint* constraint, bool ignoreCollisionBetweetLinkedBodies /* = false */)
{
	constraint->setDbgDrawSize(1.0f);
	m_dynamicsWorld->addConstraint(constraint, ignoreCollisionBetweetLinkedBodies);
	m_constraints.push_back(constraint);

	int index = m_constraints.size() - 1;
	btPoint2PointConstraint* c = ((btPoint2PointConstraint*)m_constraints[index]);
	btVector3 _offsetA = c->getPivotInA() - c->getRigidBodyA().getWorldTransform().getOrigin();
	btVector3 _offsetB = c->getPivotInB() - c->getRigidBodyB().getWorldTransform().getOrigin();
	Vec3f offsetA = Vec3f(_offsetA.getX(),_offsetA.getY(),_offsetA.getZ());
	Vec3f offsetB = Vec3f(_offsetB.getX(),_offsetB.getY(),_offsetB.getZ());
	m_constrainOffsetA.push_back(offsetA);
	m_constrainOffsetB.push_back(offsetB);

	return index;
}


btCollisionObject* PhysicSystem::GetCollisionObjectById(int index)
{
	return m_dynamicsWorld->getCollisionObjectArray()[index];
}


}