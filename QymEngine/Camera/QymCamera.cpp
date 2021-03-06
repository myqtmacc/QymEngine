#include "Camera/QymCamera.h"

#include "Scene/QymScene.h"
#include "Common/QymEngineInstance.h"
#include "Log/QymLog.h"

using namespace QymEngine;
using namespace QymEngine::Math;

QymCamera * QymCamera::m_pCurrentCam = NULL;

Math::Matrix4x4f m_mView;
Math::Matrix4x4f m_mProj;
Math::Matrix4x4f m_mCustomProj;

bool m_bOrthogonalProj;
Math::Vector2f m_vNearFar;
float m_fOrthoSize;
float m_fFov;
float m_fRatio;
float m_fCustomRatio;

bool m_bCustomProj;
bool m_bCustomRatio;

bool m_bProjDirty;

QymCamera::QymCamera() :
m_mView(Identity<Matrix4x4f>()),
m_mProj(Identity<Matrix4x4f>()),
m_mCustomProj(Identity<Matrix4x4f>()),
m_bOrthogonalProj(false),
m_vNearFar(1.0f, 1000.0f),
m_fOrthoSize(1),
m_fFov(60),
m_fRatio(1.0f),
m_fCustomRatio(1.0f),
m_bCustomProj(false),
m_bCustomRatio(false),
m_bProjDirty(true),
m_rViewport(Ratio2D(0.0f, 0.0f), Ratio2D(1.0f, 1.0f)),
m_ResizeHandler(INVALID_EVENTID)
{
	m_mView = LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f));

	OnResize(QymEngineInstance::GetResolution());
	GetProjM();

	m_ResizeHandler = QymEventUtil<Size2D>::Listen(std::bind(&QymCamera::OnResize, this, std::placeholders::_1));
}

QymCamera::~QymCamera()
{
	QymEventUtil<Size2D>::Remove(m_ResizeHandler);
}

void QymCamera::OnResize(const Size2D resolution)
{
	this->m_fRatio = (float)resolution.X / (float)resolution.Y;
	this->SetProjDirty();
}

Matrix4x4f QymCamera::GetViewM() {

	Vector3f newEyePos3 = this->m_mGlobalModel * vEyePos;
	Vector3f newTargetPos3 = this->m_mGlobalModel * vTargetPos;
	Vector3f newUpDir3 = static_cast<Vector3f>(this->m_mGlobalModel * vUpDir) - newEyePos3;

	m_mView = LookAt(newEyePos3, newTargetPos3, newUpDir3);
	return m_mView;
}

Math::Matrix4x4f QymCamera::GetProjM()
{
	if (m_bProjDirty)
	{
		if (m_bCustomProj)
			m_mProj = m_mCustomProj;
		else
		{
			float ratio = GetRatio();
			if (m_bOrthogonalProj)
				m_mProj = Math::Ortho(-m_fOrthoSize * ratio, m_fOrthoSize * ratio, -m_fOrthoSize, m_fOrthoSize, m_vNearFar[0], m_vNearFar[1]);
			else
				m_mProj = Math::Perspective(m_fFov, ratio, m_vNearFar[0], m_vNearFar[1]);
		}
		m_bProjDirty = false;
	}

	return m_mProj;
}

float QymCamera::GetRatio()
{
	return m_bCustomRatio ? m_fCustomRatio : m_fRatio;
}

void QymCamera::RenderScene(const QymScene & _scene) {
	this->ActivateCamera();

	//glPushAttrib(GL_ALL_ATTRIB_BITS);

	if (!this->m_pRT)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glDrawBuffer(GL_BACK);

		auto resolution = QymEngineInstance::GetResolution();
		GLint _X = 0;
		GLint _Y = 0;
		GLint _W = static_cast<GLint>(resolution.X);
		GLint _H = static_cast<GLint>(resolution.Y);

		glPushMatrix();
		glViewport(_X, _Y, _W, _H);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glScissor(_X, _Y, _W, _H);
		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
		glPopMatrix();
	}
	else {
		GLuint _fbo = this->m_pRT->GetFBO();
		GLint _width = this->m_pRT->GetWidth();
		GLint _height = this->m_pRT->GetHeight();
		GLint _X = static_cast<GLint>(_width * this->m_rViewport.Location.X);
		GLint _Y = static_cast<GLint>(_height * this->m_rViewport.Location.Y);
		GLint _W = static_cast<GLint>(_width * this->m_rViewport.Size.X);
		GLint _H = static_cast<GLint>(_height * this->m_rViewport.Size.Y);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

		glPushMatrix();
		glViewport(_X, _Y, _W, _H);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glScissor(_X, _Y, _W, _H);
		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
		glPopMatrix();
	}

	PGameObjects & objs = const_cast<QymScene &>(_scene).GetGameObjects();

	for (size_t i = 0; i < objs.size(); i++) {
		objs[i]->DrawGameObject();
	}

	//glPopAttrib();
}

void QymCamera::RenderScene(const QymScene & _scene, const std::vector<GLenum> & drawBuffers) {
	this->ActivateCamera();

	GLuint _fbo = this->m_pRT->GetFBO();
	GLint _width = this->m_pRT->GetWidth();
	GLint _height = this->m_pRT->GetHeight();
	GLint _X = static_cast<GLint>(_width * this->m_rViewport.Location.X);
	GLint _Y = static_cast<GLint>(_height * this->m_rViewport.Location.Y);
	GLint _W = static_cast<GLint>(_width * this->m_rViewport.Size.X);
	GLint _H = static_cast<GLint>(_height * this->m_rViewport.Size.Y);

	//glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);

	GLsizei n = static_cast<GLsizei>(drawBuffers.size());
	GLenum * buffers = new GLenum[n];
	for (int i = 0; i < n; i++) {
		buffers[i] = drawBuffers[i];
	}
	glDrawBuffers(n, buffers);

	delete[] buffers;

	glPushMatrix();
	glViewport(_X, _Y, _W, _H);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glScissor(_X, _Y, _W, _H);
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glPopMatrix();

	PGameObjects & objs = const_cast<QymScene &>(_scene).GetGameObjects();

	for (size_t i = 0; i < objs.size(); i++) {
		objs[i]->DrawGameObject();
	}

	//glPopAttrib();
}

bool QymCamera::FrustrumIntersected(QymGameObject & obj, E_INTERSECTION_DETECTION_MODE mode) {

	Matrix4x4f viewm = this->GetViewM();
	Matrix4x4f vpm = this->m_mProj * viewm;

	if (mode == E_INTERSECTION_DETECTION_MODE::AABB) {
		QymAABB aabb = obj.GetAABB();
		return Math::QymIntersection::FrustrumIntersection(vpm, aabb);

	}
	else if (mode == E_INTERSECTION_DETECTION_MODE::OBB)
	{
		QymOBB obb = obj.GetOBB();
		return Math::QymIntersection::FrustrumIntersection(vpm, obb);
	}

	return false;
}
