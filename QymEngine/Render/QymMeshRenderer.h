#pragma once

#include <map>
#include <memory>
#include "Texture/QymTexture.h"
#include "Shader/QymShaderProgram.h"
#include "Math/QymMath.h"

namespace QymEngine {

	class DLL_EXPORT QymMeshRenderer
	{
	public:
		QymMeshRenderer() = default;
		QymMeshRenderer(const std::shared_ptr<QymShaderProgram> & _program);
		QymMeshRenderer(const QymMeshRenderer &) = delete;
		~QymMeshRenderer() = default;

		void SetShaderProgram(const std::shared_ptr<QymShaderProgram> & _program) { this->m_pProgram = _program; }
		std::shared_ptr<QymShaderProgram> GetShaderProgram() const { return this->m_pProgram; }

		void AddTexture(int bindID, const std::shared_ptr<QymTexture> & tex) {
			this->m_pTexList[bindID] = tex;
		}
		void RemoveTexture(int bindID) {
			this->m_pTexList.erase(bindID);
		}
		std::shared_ptr<QymTexture> GetTexture(int bindID) {
			return this->m_pTexList[bindID];
		}

		void SetTextureMap(const std::map<int, std::shared_ptr<QymTexture> > & _texs) { this->m_pTexList = _texs; }
		void ClearTextureMap() { this->m_pTexList.clear(); }

		void DrawMesh(const QymMesh & _mesh, const Math::Matrix4x4f & _modelm);

	private:
		std::shared_ptr<QymShaderProgram> m_pProgram;

		std::map<int, std::shared_ptr<QymTexture> > m_pTexList;

		void _bindTextures();
		void _unbindTextures();
	};
}