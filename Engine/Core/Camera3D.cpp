#include "Camera3D.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include <cmath>
#include "..\Renderer\OpenGLRenderer.hpp"

#define UNUSED(x) (void)(x);

namespace Henry
{

Camera3D::Camera3D(InputSystem* input) : m_input(input)
{
	m_position = Vec3f(3.f,3.f,3.f);
	m_orientation.yawDegreesAboutZ = 225.f;
	m_orientation.pitchDegreesAboutY = 30.f;	
}


Camera3D::~Camera3D(void)
{
}


void Camera3D::SetPosition(const Vec3f pos)
{
	m_position = pos;
}


void Camera3D::Update(float deltaSeconds)
{
	UpdateCameraFromMouseAndKeyboard();
	SetUpPerspectiveProjection();

	m_position += (m_movementVector * deltaSeconds);
	m_movementVector = Vec3f( 0.f, 0.f, 0.f );

	ApplyCameraTransform();
}


void Camera3D::ApplyCameraTransform()
{
	// Put us in our preferred coordinate system: +X is east (forward), +Y is north, +Z is up
	OpenGLRenderer::Rotatef(-90.f, 1.f, 0.f, 0.f); // lean "forward" 90 degrees, to put +Z up (was +Y)
	OpenGLRenderer::Rotatef(90.f, 0.f, 0.f, 1.f); // spin "left" 90 degrees, to put +X forward (was +Y)
	// Orient the view per the camera's orientation
	OpenGLRenderer::Rotatef(-m_orientation.rollDegreesAboutX, 1.f, 0.f, 0.f);
	OpenGLRenderer::Rotatef(-m_orientation.pitchDegreesAboutY, 0.f, 1.f, 0.f);
	OpenGLRenderer::Rotatef(-m_orientation.yawDegreesAboutZ, 0.f, 0.f, 1.f);
	// Position the view per the camera's position
	OpenGLRenderer::Translatef(-m_position.x, -m_position.y, -m_position.z);
}


void Camera3D::SetUpPerspectiveProjection()
{
	float aspect = ((float)OpenGLRenderer::WINDOW_SIZE.x / (float)OpenGLRenderer::WINDOW_SIZE.y); // VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT;
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1200.f;

	OpenGLRenderer::LoadIdentity();
	OpenGLRenderer::Perspective(fovY, aspect, zNear, zFar); // Note: Absent in OpenGL ES 2.0*/
}


void Camera3D::UpdateCameraFromMouseAndKeyboard()
{
	// Update camera orientation (yaw and pitch only) from mouse x,y movement
	const float degreesPerMouseDelta = 0.04f;
	Vec2i mouseDeltas = m_input->GetMouseMovementFromLastFrame();
	m_orientation.yawDegreesAboutZ -= (degreesPerMouseDelta * mouseDeltas.x);
	m_orientation.pitchDegreesAboutY += (degreesPerMouseDelta * mouseDeltas.y);
	float moveSpeed = 8.0f;

	// Update camera position based on which (if any) movement keys are down
	float cameraYawRadians = degree2radians(m_orientation.yawDegreesAboutZ);
	float cameraPitchRadians = degree2radians(m_orientation.pitchDegreesAboutY);
	Vec3f cameraForwardVector = Vec3f(cos(cameraYawRadians) * cos(cameraPitchRadians), sin(cameraYawRadians) * cos(cameraPitchRadians), -sin(cameraPitchRadians));
	m_forwardVector = cameraForwardVector;
	m_rightVector = m_forwardVector.crossProductWith(Vec3f(0, 0, 1));
	m_upVector = m_rightVector.crossProductWith(m_forwardVector);

	if (m_input->m_keyState[Inputs::INPUT_SHIFT].isHold)
		moveSpeed = 100.0f;

	//Vec3f movementVector( 0.f, 0.f, 0.f );
	//m_camera.m_movementVector = Vec3f( 0.f, 0.f, 0.f );
	if (m_input->m_keyState['W'].isHold)
		m_movementVector += m_forwardVector * moveSpeed;
	if (m_input->m_keyState['S'].isHold)
		m_movementVector -= m_forwardVector * moveSpeed;
	if (m_input->m_keyState['A'].isHold)
		m_movementVector -= m_rightVector * moveSpeed;
	if (m_input->m_keyState['D'].isHold)
		m_movementVector += m_rightVector * moveSpeed;

	if (m_input->m_keyState[Inputs::INPUT_SPACE].isHold)
		m_movementVector = Vec3f(0.f, 0.f, 0.25f) * moveSpeed;

	if (m_input->m_keyState['Z'].isHold)
		m_movementVector = Vec3f(0.f, 0.f, -0.25f) * moveSpeed;
	//...and so on, for A,S,D moving left, back, right - and for E,C moving up, down.
}


};