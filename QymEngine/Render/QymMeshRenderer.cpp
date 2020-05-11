#include "Render/QymMeshRenderer.h"
#include <string>

#include "Camera/QymCamera.h"
#include "Log/QymLog.h"

using namespace QymEngine;
using namespace QymEngine::Math;

QymMeshRenderer::QymMeshRenderer(const std::shared_ptr<QymShaderProgram> & _program) :
m_pProgram(_program)
{
}

void QymMeshRenderer::DrawMesh(const QymMesh & _mesh, const Matrix4x4f & _modelm) {
	QymCamera * _camera = QymCamera::GetCurrentCamera();

	Matrix4x4f viewm = _camera->GetViewM();
	Matrix4x4f projm = _camera->GetProjM();
	Matrix4x4f mvpm = projm * viewm * _modelm;
	//Matrix4x4f mvpm = vmath::ortho(-10, 10, -10, 10, 0, -1000) * viewm * _modelm;

	unsigned int progID = this->m_pProgram->GetProgramID();
	GLuint vaoID = _mesh.GetVAO();
	GLsizei indexCount = static_cast<GLsizei>(_mesh.IndexCount());

	glUseProgram(progID);
	glDisable(GL_BLEND);

	this->_bindTextures();

	// Matrix4x4f类有隐式转换操作符
	glUniformMatrix4fv(this->m_pProgram->m_uModel, 1, GL_FALSE, ToPtr(_modelm));
	glUniformMatrix4fv(this->m_pProgram->m_uView, 1, GL_FALSE, ToPtr(viewm));
	glUniformMatrix4fv(this->m_pProgram->m_uProjection, 1, GL_FALSE, ToPtr(projm));
	glUniformMatrix4fv(this->m_pProgram->m_uMvp, 1, GL_FALSE, ToPtr(mvpm));
	//glUniformMatrix4fv(this->m_pProgram->m_uTexm, 1, GL_FALSE, vmath::Matrix4x4f::identity());

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);

	this->_unbindTextures();

	glUseProgram(0);
}

void QymMeshRenderer::_bindTextures() {
	unsigned int progID = this->m_pProgram->GetProgramID();
	for (auto & item : this->m_pTexList) {
		if (glIsTexture(item.second->GetTexID())) {
			glActiveTextureARB(GL_TEXTURE0_ARB + item.first);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, item.second->GetTexID());

			std::string name = "Texm";
			if (item.first > 0) {
				name += std::to_string(item.first);
			}

			int uTexm = glGetUniformLocation(progID, name.c_str());
			if (uTexm >= 0) {
				glUniformMatrix4fv(uTexm, 1, GL_FALSE, ToPtr(item.second->GetTexm()));
			}
		}
	}
}

void QymMeshRenderer::_unbindTextures() {
	for (auto & item : this->m_pTexList) {
		if (glIsTexture(item.second->GetTexID())) {
			glActiveTextureARB(GL_TEXTURE0_ARB + item.first);
			glDisable(GL_TEXTURE_2D);
		}
	}
}