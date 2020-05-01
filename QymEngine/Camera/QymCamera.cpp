#include "Camera/QymCamera.h"

#include "Scene/QymScene.h"
#include "Common/QymEngineInstance.h"
#include "Log/QymLog.h"

using namespace QymEngine;

QymCamera * QymCamera::m_pCurrentCam = NULL;

QymCamera::QymCamera() :
m_mView(mat4::identity()),
m_mProj(mat4::identity()),
m_rViewport(Ratio2D(0.0f, 0.0f), Ratio2D(1.0f, 1.0f))
{
	m_mView = vmath::lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
	m_mProj = vmath::perspective(60.0f, 1.0f, -1.0f, -1000.0f);
	//m_mProj = vmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1000.0f);
}

mat4 QymCamera::GetViewM() {
	/*vec4 newEyePos4 = vEyePos * this->m_mGlobalModel;
	vec4 newTargetPos4 = vTargetPos * this->m_mGlobalModel;
	vec4 newUpDir4 = vUpDir * this->m_mGlobalModel;

	vec3 newEyePos3 = vec3(newEyePos4[0], newEyePos4[1], newEyePos4[2]);
	vec3 newTargetPos3 = vec3(newTargetPos4[0], newTargetPos4[1], newTargetPos4[2]);
	vec3 newUpDir3 = vec3(newUpDir4[0], newUpDir4[1], newUpDir4[2]);*/

	vec3 newEyePos3 = this->m_mGlobalModel * vEyePos;
	vec3 newTargetPos3 = this->m_mGlobalModel * vTargetPos;
	vec3 newUpDir3 = this->m_mGlobalModel * vUpDir;

	m_mView = vmath::lookat(newEyePos3, newTargetPos3, newUpDir3);
	return m_mView;
}

void QymCamera::RenderScene(const QymScene & _scene) {
	this->ActivateCamera();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

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

	glPopAttrib();
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
	mat4 viewm = this->GetViewM();
	mat4 mvpm = this->m_mProj * viewm;

	vec4 v1, v2, v3, v4, v5, v6, v7, v8;

	if (mode == E_INTERSECTION_DETECTION_MODE::AABB) {
		QymAABB aabb = obj.GetAABB();

		v1 = mvpm * vec4(aabb.GetMinX(), aabb.GetMinY(), aabb.GetMinZ(), 1.0f);
		v2 = mvpm * vec4(aabb.GetMinX(), aabb.GetMinY(), aabb.GetMaxZ(), 1.0f);
		v3 = mvpm * vec4(aabb.GetMinX(), aabb.GetMaxY(), aabb.GetMinZ(), 1.0f);
		v4 = mvpm * vec4(aabb.GetMinX(), aabb.GetMaxY(), aabb.GetMaxZ(), 1.0f);
		v5 = mvpm * vec4(aabb.GetMaxX(), aabb.GetMinY(), aabb.GetMinZ(), 1.0f);
		v6 = mvpm * vec4(aabb.GetMaxX(), aabb.GetMinY(), aabb.GetMaxZ(), 1.0f);
		v7 = mvpm * vec4(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMinZ(), 1.0f);
		v8 = mvpm * vec4(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMaxZ(), 1.0f);
	}
	else if (mode == E_INTERSECTION_DETECTION_MODE::OBB)
	{
		QymOBB obb = obj.GetOBB();

		v1 = mvpm * vec4(obb[0], 1.0f);
		v2 = mvpm * vec4(obb[1], 1.0f);
		v3 = mvpm * vec4(obb[2], 1.0f);
		v4 = mvpm * vec4(obb[3], 1.0f);
		v5 = mvpm * vec4(obb[4], 1.0f);
		v6 = mvpm * vec4(obb[5], 1.0f);
		v7 = mvpm * vec4(obb[6], 1.0f);
		v8 = mvpm * vec4(obb[7], 1.0f);
	}

	//vec4 r = mvpm * vec4(0.0f, 0.0f, -50.0f, 1.0f);
	//vec4 r1 = mvpm * vec4(85.0f, 0.0f, -50.0f, 1.0f);
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