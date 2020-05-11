#include "Camera/QymCamera.h"

#include "Scene/QymScene.h"
#include "Common/QymEngineInstance.h"
#include "Log/QymLog.h"

using namespace QymEngine;
using namespace QymEngine::Math;

QymCamera * QymCamera::m_pCurrentCam = NULL;

QymCamera::QymCamera() :
m_mView(Identity<Matrix4x4f>()),
m_mProj(Identity<Matrix4x4f>()),
m_rViewport(Ratio2D(0.0f, 0.0f), Ratio2D(1.0f, 1.0f))
{
	m_mView = LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f));
	m_mProj = Perspective(60.0f, 1.0f, 1.0f, 1000.0f);
}

Matrix4x4f QymCamera::GetViewM() {

	Vector3f newEyePos3 = this->m_mGlobalModel * vEyePos;
	Vector3f newTargetPos3 = this->m_mGlobalModel * vTargetPos;
	Vector3f newUpDir3 = static_cast<Vector3f>(this->m_mGlobalModel * vUpDir) - newEyePos3;

	m_mView = LookAt(newEyePos3, newTargetPos3, newUpDir3);
	return m_mView;
}

void QymCamera::RenderScene(const QymScene & _scene) {
	this->ActivateCamera();

	//glPushAttrib(GL_ALL_ATTRIB_BITS);

	if (!this->m_pRT)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glDrawBuffer(GL_BACK);

		auto resolution = QymEngineInstance::Resolution();
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

		glDisable(GL_DEPTH_TEST);
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

	bool valid = true;
	Matrix4x4f viewm = this->GetViewM();
	Matrix4x4f mvpm = this->m_mProj * viewm;

	Vector4f v1, v2, v3, v4, v5, v6, v7, v8;

	if (mode == E_INTERSECTION_DETECTION_MODE::AABB) {
		QymAABB aabb = obj.GetAABB();

		v1 = mvpm * Vector4f(aabb.GetMinX(), aabb.GetMinY(), aabb.GetMinZ(), 1.0f);
		v2 = mvpm * Vector4f(aabb.GetMinX(), aabb.GetMinY(), aabb.GetMaxZ(), 1.0f);
		v3 = mvpm * Vector4f(aabb.GetMinX(), aabb.GetMaxY(), aabb.GetMinZ(), 1.0f);
		v4 = mvpm * Vector4f(aabb.GetMinX(), aabb.GetMaxY(), aabb.GetMaxZ(), 1.0f);
		v5 = mvpm * Vector4f(aabb.GetMaxX(), aabb.GetMinY(), aabb.GetMinZ(), 1.0f);
		v6 = mvpm * Vector4f(aabb.GetMaxX(), aabb.GetMinY(), aabb.GetMaxZ(), 1.0f);
		v7 = mvpm * Vector4f(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMinZ(), 1.0f);
		v8 = mvpm * Vector4f(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMaxZ(), 1.0f);
	}
	else if (mode == E_INTERSECTION_DETECTION_MODE::OBB)
	{
		QymOBB obb = obj.GetOBB();

		v1 = mvpm * Vector4f(obb[0], 1.0f);
		v2 = mvpm * Vector4f(obb[1], 1.0f);
		v3 = mvpm * Vector4f(obb[2], 1.0f);
		v4 = mvpm * Vector4f(obb[3], 1.0f);
		v5 = mvpm * Vector4f(obb[4], 1.0f);
		v6 = mvpm * Vector4f(obb[5], 1.0f);
		v7 = mvpm * Vector4f(obb[6], 1.0f);
		v8 = mvpm * Vector4f(obb[7], 1.0f);
	}

	//Vector4f r = mvpm * Vector4f(0.0f, 0.0f, -50.0f, 1.0f);
	//Vector4f r1 = mvpm * Vector4f(85.0f, 0.0f, -50.0f, 1.0f);
	for (int i = 0; i < 3; i++) {

		bool b1 = false, b2 = false;
		b1 = v1[i] < -v1[3] && v2[i] < -v2[3] && v3[i] < -v3[3] && v4[i] < -v4[3] &&
			v5[i] < -v5[3] && v6[i] < -v6[3] && v7[i] < -v7[3] && v8[i] < -v8[3];

		b2 = v1[i] > v1[3] && v2[i] > v2[3] && v3[i] > v3[3] && v4[i] > v4[3] &&
			v5[i] > v5[3] && v6[i] > v6[3] && v7[i] > v7[3] && v8[i] > v8[3];

		if (b1 || b2) {
			valid = false;
			break;
		}
	}

	return valid;
}