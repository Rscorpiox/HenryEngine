#include "CollisionObject.hpp"

#define _USE_MATH_DEFINES

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\HenryFunctions.hpp"
#include <math.h>


namespace Henry
{ 

	CollisionObject::CollisionObject(btCollisionShape* shape, Vec3f position /* = Vec3f() */, Vec3f orientation /* = Vec3f() */, float mass /* = 1.0f */)
	: m_shape(shape)
{
	btQuaternion quat;
	orientation.x = degree2radians(orientation.x);
	orientation.y = degree2radians(orientation.y);
	orientation.z = degree2radians(orientation.z);
	quat.setEuler(orientation.y, orientation.x, orientation.z);

	btVector3 inertia(0, 0, 0);
	m_shape->calculateLocalInertia(mass, inertia);

	m_motionState = new btDefaultMotionState(btTransform(quat, btVector3(position.x, position.y, position.z)));
	m_constructionInfo = new btRigidBody::btRigidBodyConstructionInfo(mass, m_motionState, m_shape, inertia);
	m_constructionInfo->m_restitution = 0.5f;
	m_constructionInfo->m_friction = 1.5f;
	m_rigidBody = new btRigidBody(*m_constructionInfo);
}


CollisionObject::~CollisionObject()
{
	delete m_shape;
	delete m_constructionInfo;
	delete m_motionState;
	delete m_rigidBody;
}


void CollisionObject::Draw(bool wired /* = true */)
{
	OpenGLRenderer::DEFAULT_MATERIAL->Activate();
	btTransform trans;
	m_motionState->getWorldTransform(trans);
	btQuaternion quat = trans.getRotation();
	Matrix4 rotationMatrix = QuatToMatrix(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
	Matrix4 objectToWorldMatrix;
	objectToWorldMatrix.ApplyTranslation(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	objectToWorldMatrix.ApplyMatrix(&rotationMatrix);

	OpenGLRenderer::DEFAULT_MATERIAL->SendUniform("u_objectToWorldMatrix", objectToWorldMatrix);

	if (wired)
	{
		OpenGLRenderer::LineWidth(2.0f);
		OpenGLRenderer::DrawVertexWithVertexArray(m_vertices, OpenGLRenderer::SHAPE_LINES);
	}
	else
		OpenGLRenderer::DrawVertexWithVertexArray(m_vertices, OpenGLRenderer::SHAPE_TRIANGLES);
}


void CollisionObject::ConstructSphereVertices(float radius)
{
	int rings = (int)(radius * 50);
	int sectors = (int)(radius * 50);

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	m_vertices.resize(rings * sectors * 3);
	//normals.resize(rings * sectors * 3);
	//texcoords.resize(rings * sectors * 2);
	std::vector<Vertex_PosColor>::iterator v = m_vertices.begin();
	//std::vector<GLfloat>::iterator n = normals.begin();
	//std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		Vec3f pos = Vec3f(x, y, z)*radius;
		RGBA color = RGBA(1.0f, 0.0f, 1.0f, 1.0f);

		*v = Vertex_PosColor(pos,color);
		*v++;

//		*t++ = s*S;
//		*t++ = r*R;
//
//		*v++ = x * radius;
//		*v++ = y * radius;
//		*v++ = z * radius;
//
//		*n++ = x;
//		*n++ = y;
//		*n++ = z;
	}

// 	indices.resize(rings * sectors * 4);
// 	std::vector<GLushort>::iterator i = indices.begin();
// 	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
// 		*i++ = r * sectors + s;
// 		*i++ = r * sectors + (s + 1);
// 		*i++ = (r + 1) * sectors + (s + 1);
// 		*i++ = (r + 1) * sectors + s;
// 	}
}


void CollisionObject::ConstructPlaneVertices(Vec3f position, Vec3f orientation)
{
	m_vertices.reserve(6);
	float width = 1000.0f;
	float height = 1000.0f;

	Vertex_PosColor vertices[6];
	vertices[0].position = Vec3f(position.x - width, position.y + height, position.z);
	vertices[1].position = Vec3f(position.x - width, position.y - height, position.z);
	vertices[2].position = Vec3f(position.x + width, position.y - height, position.z);
	vertices[3].position = Vec3f(position.x - width, position.y + height, position.z);
	vertices[4].position = Vec3f(position.x - width, position.y - height, position.z);
	vertices[5].position = Vec3f(position.x + width, position.y + height, position.z);

	vertices[0].color = RGBA(1.f, 0.f, 1.f, 1.0f);
	vertices[1].color = RGBA(1.f, 0.f, 1.f, 1.0f);
	vertices[2].color = RGBA(1.f, 0.f, 1.f, 1.0f);
	vertices[3].color = RGBA(1.f, 0.f, 1.f, 1.0f);
	vertices[4].color = RGBA(1.f, 0.f, 1.f, 1.0f);
	vertices[5].color = RGBA(1.f, 0.f, 1.f, 1.0f);

	for (int index = 0; index < 6; ++index)
		m_vertices.push_back(vertices[index]);
}


void CollisionObject::ConstructBoxVertices(Vec3f halfExtendSize)
{
	m_vertices.reserve(72);

	Vertex_PosColor vertices[72];
	vertices[0].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[1].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[2].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[3].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[4].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[5].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);

	vertices[6].position  = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[7].position  = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[8].position  = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[9].position  = Vec3f(-halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[10].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[11].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);

	vertices[12].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[13].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[14].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[15].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[16].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[17].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);

	vertices[18].position  = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[19].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[20].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[21].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[22].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[23].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);

	vertices[24].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[25].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[26].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[27].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[28].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[29].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);

	vertices[30].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[31].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[32].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[33].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[34].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[35].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);

	vertices[36].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[37].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[38].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[39].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[40].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[41].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);

	vertices[42].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[43].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[44].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[45].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[46].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[47].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);

	vertices[48].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[49].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[50].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[51].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[52].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[53].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);

	vertices[54].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[55].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[56].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, +halfExtendSize.z);
	vertices[57].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[58].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);
	vertices[59].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, +halfExtendSize.z);

	vertices[60].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[61].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[62].position = Vec3f(-halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[63].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[64].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[65].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);

	vertices[66].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[67].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[68].position = Vec3f(+halfExtendSize.x, +halfExtendSize.y, -halfExtendSize.z);
	vertices[69].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[70].position = Vec3f(+halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);
	vertices[71].position = Vec3f(-halfExtendSize.x, -halfExtendSize.y, -halfExtendSize.z);

	for (int index = 0; index < 72; ++index)
	{
		vertices[index].color = RGBA(1.f, 0.f, 1.f, 1.0f);
		m_vertices.push_back(vertices[index]);
	}
}


}