#include "Render/QymMeshRenderer.h"
#include <string>

#include "Camera/QymCamera.h"
#include "Log/QymLog.h"


using namespace QymEngine;

QymMeshRenderer::QymMeshRenderer(const std::shared_ptr<QymShaderProgram> & _program) :
m_pProgram(_program)
{
}

void QymMeshRenderer::DrawMesh(const QymMesh & _mesh, const mat4 & _modelm) {
	QymCamera * _camera = QymCamera::GetCurrentCamera();

	mat4 viewm = _camera->GetViewM();
	mat4 projm = _camera->GetProjM();
	mat4 mvpm = projm * viewm * _modelm;
	//mat4 mvpm = vmath::ortho(-10, 10, -10, 10, 0, -1000) * viewm * _modelm;

	unsigned int progID = this->m_pProgram->GetProgramID();
	GLuint vaoID = _mesh.GetVAO();
	GLsizei indexCount = static_cast<GLsizei>(_mesh.IndexCount());

	glUseProgram(progID);
	glDisable(GL_BLEND);

	this->_bindTextures();

	// mat4类有隐式转换操作符
	glUniformMatrix4fv(this->m_pProgram->m_uModel, 1, GL_FALSE, _modelm);
	glUniformMatrix4fv(this->m_pProgram->m_uView, 1, GL_FALSE, viewm);
	glUniformMatrix4fv(this->m_pProgram->m_uProjection, 1, GL_FALSE, projm);
	glUniformMatrix4fv(this->m_pProgram->m_uMvp, 1, GL_FALSE, mvpm);
	//glUniformMatrix4fv(this->m_pProgram->m_uTexm, 1, GL_FALSE, vmath::mat4::identity());

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);

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
				glUniformMatrix4fv(uTexm, 1, GL_FALSE, item.second->GetTexm());
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