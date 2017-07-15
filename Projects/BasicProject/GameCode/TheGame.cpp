#include "TheGame.hpp"
#include "TheApplication.hpp"
#include "GameCommon.hpp"

#include "Engine\Memory\MemoryAllocatePool.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\Time.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include <fstream>
#include <sstream>


namespace Henry
{

extern TheApplication* g_theApplication;
//extern MemoryAllocatePool* _memoryAllocatePool;

TheGame::TheGame(void)
{
	m_isQuitting = false;
	m_gameState = STARTING_UP;
	Initialize();
}


TheGame::~TheGame(void)
{
}


void TheGame::Input()
{
	if(m_input->PressedOnce(Inputs::INPUT_ESCAPE))
		SetGameState(QUITTING);

	switch (m_gameState)
	{
	case STARTING_UP:
		break;
	case PLAYING:
		if (m_input->PressedOnce(Inputs::INPUT_ADD))
			m_intMemoryTest = new int[40000];
		if (m_input->PressedOnce(Inputs::INPUT_SUBTRACT) && m_intMemoryTest)
		{
			delete m_intMemoryTest;
			m_intMemoryTest = nullptr;
		}

		if (m_input->PressedOnce(Inputs::INPUT_SPACE))
		{
			for (int index = 0; index < 100; ++index)
			{
				m_floatMemoryTest.push_back(new float[10000]);
			}
			
			for(size_t index = 0; index < m_floatMemoryTest.size(); ++index)
			{
				delete m_floatMemoryTest.back();
				m_floatMemoryTest.pop_back();
			}
		}
		break;
	};
}


void TheGame::Render()
{
	SetUpPerspectiveProjection();
	ApplyCameraTransform( m_camera );

	OpenGLRenderer::BindFrameBuffer(m_fbo.ID);
	OpenGLRenderer::ClearScreen(0.0f,0.0f,0.0f);

	switch(m_gameState)
	{
	case STARTING_UP:
		break;
	case PLAYING:
		RenderPlaying();
		break;
	};

	OpenGLRenderer::BindFrameBuffer(0);
	OpenGLRenderer::ClearScreen(0.f, 0.f, 0.f);
	DrawFrameBuffer( m_fbo, m_material );
}


void TheGame::Update( float deltaSeconds )
{
	switch(m_gameState)
	{
	case STARTING_UP:
		break;
	case PLAYING:
		break;
	case QUITTING:
		m_isQuitting = true;
		break;
	};

	UpdateCameraFromMouseAndKeyboard( m_camera, deltaSeconds );
	m_clock->AdvanceTime(deltaSeconds);
}


void TheGame::RenderAxes()
{
	OpenGLRenderer::LineWidth(2.0f);
	float axisLength = 2.0f;
	float axisStart = 0.0f;
	Vertex_PosColor vertices[6];
	vertices[0].color = RGBA(1.0f,0.0f,0.0f,1.0f);
	vertices[0].position = Vec3f(axisStart,0.0f,0.0f);
	vertices[1].color = RGBA(1.0f,0.0f,0.0f,1.0f);
	vertices[1].position = Vec3f(axisLength,0.0f,0.0f);

	vertices[2].color = RGBA(0.0f,1.0f,0.0f,1.0f);
	vertices[2].position = Vec3f(0.0f,axisStart,0.0f);
	vertices[3].color = RGBA(0.0f,1.0f,0.0f,1.0f);
	vertices[3].position = Vec3f(0.0f,axisLength,0.0f);

	vertices[4].color = RGBA(0.0f,0.0f,1.0f,1.0f);
	vertices[4].position = Vec3f(0.0f,0.0f,axisStart);
	vertices[5].color = RGBA(0.0f,0.0f,1.0f,1.0f);
	vertices[5].position = Vec3f(0.0f,0.0f,axisLength);

	OpenGLRenderer::DrawVertexWithVertexArray(vertices,6,OpenGLRenderer::SHAPE_LINES);
}


void TheGame::DrawFrameBuffer( FBO& toDraw, Material* fboMaterial )
{
	fboMaterial->Activate();
	OpenGLRenderer::SetOrtho(0.0 , 1600 , 0.0 , 900 , 0 , 1 );
	OpenGLRenderer::SendBasicTwoFBOTextures(toDraw.depthTextureID, toDraw.colorTextureID);
	Vertex_PCT quad[4];
	quad[0].position = Vec3f( 0.0f										, 0.0f										, 0.0f );
	quad[1].position = Vec3f( (float)g_theApplication->m_windowSize.x	, 0.0f										, 0.0f );
	quad[2].position = Vec3f( (float)g_theApplication->m_windowSize.x	, (float)g_theApplication->m_windowSize.y	, 0.0f );
	quad[3].position = Vec3f( 0.0f										, (float)g_theApplication->m_windowSize.y	, 0.0f );

	quad[0].texCoords = Vec2f( 0.0f	, 0.0f );
	quad[1].texCoords = Vec2f( 1.0f	, 0.0f );
	quad[2].texCoords = Vec2f( 1.0f	, 1.0f );
	quad[3].texCoords = Vec2f( 0.0f	, 1.0f );

	OpenGLRenderer::DrawVertexWithVertexArray2D(quad,4,OpenGLRenderer::SHAPE_QUADS,(float)g_theApplication->m_windowSize.x,(float)g_theApplication->m_windowSize.y);
}


void TheGame::SetUpPerspectiveProjection()
{
	float aspect = (16.f / 9.f); // VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT;
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1000.f;

	OpenGLRenderer::LoadIdentity();
	OpenGLRenderer::Perspective( fovY, aspect, zNear, zFar ); // Note: Absent in OpenGL ES 2.0*/
}


void TheGame::ApplyCameraTransform( const Camera3D& camera )
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	// Put us in our preferred coordinate system: +X is east (forward), +Y is north, +Z is up
	OpenGLRenderer::Rotatef( -90.f, 1.f, 0.f, 0.f ); // lean "forward" 90 degrees, to put +Z up (was +Y)
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( 90.f, 0.f, 0.f, 1.f ); // spin "left" 90 degrees, to put +X forward (was +Y)
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	// Orient the view per the camera's orientation
	OpenGLRenderer::Rotatef( -camera.m_orientation.rollDegreesAboutX,	1.f, 0.f, 0.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( -camera.m_orientation.pitchDegreesAboutY,	0.f, 1.f, 0.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( -camera.m_orientation.yawDegreesAboutZ,	0.f, 0.f, 1.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	// Position the view per the camera's position
	OpenGLRenderer::Translatef( -camera.m_position.x, -camera.m_position.y, -camera.m_position.z );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
}


Vec2f TheGame::GetMouseMovementSinceLastChecked()
{
	POINT cursorPos;
	GetCursorPos( &cursorPos );

	if(m_input->isFocus)
		SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	else
		return Vec2f(0.0f,0.0f);

	SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	Vec2i mouseDeltaInts( (int)(cursorPos.x - m_centerCursorPos.x), (int)(cursorPos.y - m_centerCursorPos.y) );
	Vec2f mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}


void TheGame::UpdateCameraFromMouseAndKeyboard( Camera3D& camera, float deltaSeconds )
{
	// Update camera orientation (yaw and pitch only) from mouse x,y movement
	const float degreesPerMouseDelta = 0.04f;
	float moveSpeed = 1.0f;
	Vec2f mouseDeltas = GetMouseMovementSinceLastChecked();
	camera.m_orientation.yawDegreesAboutZ		-= (degreesPerMouseDelta * mouseDeltas.x);
	camera.m_orientation.pitchDegreesAboutY		+= (degreesPerMouseDelta * mouseDeltas.y);

	// Update camera position based on which (if any) movement keys are down
	float cameraYawRadians = degree2radians(camera.m_orientation.yawDegreesAboutZ);
	float cameraPitchRadians = degree2radians(camera.m_orientation.pitchDegreesAboutY);
	Vec3f cameraForwardVector = Vec3f( cos(cameraYawRadians) * cos(cameraPitchRadians) , sin(cameraYawRadians) * cos(cameraPitchRadians) , -sin(cameraPitchRadians));
	camera.m_forwardVector = cameraForwardVector;

	if(m_input->m_keyState[Inputs::INPUT_SHIFT].isHold)
		moveSpeed = 4.0f;

	Vec3f movementVector( 0.f, 0.f, 0.f );
	if( m_input->m_keyState[ 'W' ].isHold )
		movementVector += camera.m_forwardVector * moveSpeed;
	if( m_input->m_keyState[ 'S' ].isHold )
		movementVector -= camera.m_forwardVector * moveSpeed;
	if( m_input->m_keyState[ 'A' ].isHold )
		movementVector -= camera.m_forwardVector.crossProductWith(Vec3f(0,0,1)) * moveSpeed;
	if( m_input->m_keyState[ 'D' ].isHold )
		movementVector += camera.m_forwardVector.crossProductWith(Vec3f(0,0,1)) * moveSpeed;

	if( m_input->m_keyState[Inputs::INPUT_SPACE].isHold )
		movementVector = Vec3f(0.f, 0.f, 1.f);

	if( m_input->m_keyState[ 'Z' ].isHold )
		movementVector = Vec3f(0.f, 0.f, -1.f);
	//...and so on, for A,S,D moving left, back, right - and for E,C moving up, down.

	camera.m_position += (movementVector * deltaSeconds);
}


void TheGame::ToggleCursorDisplay(bool show)
{
	if(!show)
	{
		while(ShowCursor(false)>=0);
		SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	}
	else
		ShowCursor(true);
}


GameState TheGame::GetGameState()
{
	return m_gameState;
}


void TheGame::SetGameState(GameState state)
{
	m_gameState = state;
}


void TheGame::Initialize()
{
	m_centerCursorPos = Vec2f(400,300);
	SetCursorPos(400,300);
	ShowCursor(false);
	m_isQuitting = false;
	m_timeOfLastUpdate = GetCurrentTimeSeconds();
	bool frag_success = true;
	bool vert_success = true;

	ShaderProgram sp(OpenGLRenderer::CreateProgram(), new OpenGLShader("./Data/Shaders/doNothing_fragment.glsl", OpenGLRenderer::FRAGMENT_SHADER, "./Data.zip", &frag_success), new OpenGLShader("./Data/Shaders/doNothing_vertex.glsl", OpenGLRenderer::VERTEX_SHADER, "./Data.zip", &vert_success));
	m_material = new Material(sp);

	m_clock = new Clock();

	OpenGLRenderer::InitializeFrameBuffer(m_fbo);
	m_input = g_theApplication->m_input;
	m_font = g_theApplication->m_font;
	m_intMemoryTest = nullptr;

	SetGameState(PLAYING);
}


void TheGame::RenderPlaying()
{
	_memoryAllocatePool->Render();

	RenderAxes();
	m_font->Draw(m_clock->m_name + " : %.2lf", Vec2f(50, 800), 50, RGBA(), OpenGLRenderer::WINDOW_SIZE, m_clock->GetTime());
	m_font->Draw("Total Memory Size (Byte) : %d", Vec2f(50, 700), 30, RGBA(), OpenGLRenderer::WINDOW_SIZE, _memoryAllocatePool->m_memorySize);
	m_font->Draw("Average Memory Allocated (Byte) : %.2lf", Vec2f(50, 650), 30, RGBA(), OpenGLRenderer::WINDOW_SIZE, _memoryAllocatePool->m_allocatedSize / (float)(_memoryAllocatePool->m_numberOfMemoryBlocks));
	m_font->Draw("Size Of Block Allocated (Byte) : %d", Vec2f(50, 600), 30, RGBA(), OpenGLRenderer::WINDOW_SIZE, _memoryAllocatePool->m_allocatedSize);
	m_font->Draw("Number Of Allocated Blocks : %d", Vec2f(50, 550), 30, RGBA(), OpenGLRenderer::WINDOW_SIZE, _memoryAllocatePool->m_numberOfMemoryBlocks);
	m_font->Draw("Largest Size Of Memory Allocated (Byte) : %d", Vec2f(50, 500), 30, RGBA(), OpenGLRenderer::WINDOW_SIZE, _memoryAllocatePool->m_largestSizeOfMemoryAllocated);
	m_font->Draw("Percent Of Allocated Space : %.6f %%", Vec2f(50, 450), 30, RGBA(), OpenGLRenderer::WINDOW_SIZE, _memoryAllocatePool->m_allocatedSize / (float)_memoryAllocatePool->m_memorySize);
}


};