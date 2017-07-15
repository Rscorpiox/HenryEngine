#include "PhysicDebugDrawer.hpp"


namespace Henry
{

PhysicDebugDrawer::PhysicDebugDrawer()
{
	m_debugMode = DBG_MAX_DEBUG_DRAW_MODE;
	m_lineWidth = 2.0f;
}


PhysicDebugDrawer::~PhysicDebugDrawer()
{
}


void PhysicDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	OpenGLRenderer::DEFAULT_MATERIAL->Activate();
	OpenGLRenderer::LineWidth(m_lineWidth);

	Matrix4 objectToWorldMatrix;
	OpenGLRenderer::DEFAULT_MATERIAL->SendUniform("u_objectToWorldMatrix",objectToWorldMatrix);

	Vertex_PosColor vertices[2];
	RGBA _color = RGBA(color.getX(), color.getY(), color.getZ(), 1.0f);
	vertices[0].position = Vec3f(from.getX(), from.getY(), from.getZ());
	vertices[1].position = Vec3f(to.getX(), to.getY(), to.getZ());

	vertices[0].color = _color;
	vertices[1].color = _color;

	OpenGLRenderer::DrawVertexWithVertexArray(vertices, 2, OpenGLRenderer::SHAPE_LINES);
}


void PhysicDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}


void PhysicDebugDrawer::reportErrorWarning(const char* warningString)
{

}


void PhysicDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}


void PhysicDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}


int PhysicDebugDrawer::getDebugMode() const
{
	return m_debugMode;
}

}