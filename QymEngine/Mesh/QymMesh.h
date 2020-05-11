#pragma once

#include <memory>
#include <vector>
#include "Common/QymCommonHeader.h"
#include "Math/QymMath.h"

namespace QymEngine {

	struct VertexAttribs
	{
		std::vector<Math::Vector3f> position;
		std::vector<Math::Vector4f> color;
		std::vector<Math::Vector2f> uv0;
		std::vector<Math::Vector2f> uv1;
	};

	struct SpherePatchParams
	{
		float startLongitude;
		float longitudeRange;
		float startLatitude;
		float latitudeRange;
	};

	typedef std::vector<GLuint> IndexArray;

	class DLL_EXPORT QymMesh
	{

	public:
		QymMesh();
		QymMesh(const QymMesh & _mesh);
		QymMesh(const VertexAttribs & vectices, const IndexArray & indices);
		QymMesh(VertexAttribs && vectices, IndexArray && indices);
		~QymMesh();

		QymMesh & operator=(const QymMesh & _mesh);

		void ClearMesh();

		bool IsValidMesh() const { return this->m_bValidMesh; }

		GLuint GetVAO() const { return this->m_iVAO; }
		size_t VertexCount() const { return this->m_iVertexCount; }
		size_t IndexCount() const { return this->m_iIndexCount; }

		Math::QymAABB BuildAABB();
		Math::QymOBB BuildOBB(const Math::Matrix4x4f & transform);

	public:
		static std::shared_ptr<QymMesh> BuildGlobe(const float uScale = 1.0f, const float vScale = 1.0f);
		static std::shared_ptr<QymMesh> BuildSpherePatch(SpherePatchParams params, const float uScale = 1.0f, const float vScale = 1.0f);
		static std::shared_ptr<QymMesh> BuildQuad();
		static std::shared_ptr<QymMesh> BuildQuadPatch(SpherePatchParams params);
		static std::shared_ptr<QymMesh> BuildTesselledQuad(const int horizontal, const int vertical);

		static std::shared_ptr<QymMesh> LoadModel(const std::string path);

	private:
		std::vector<Math::Vector3f> m_vfPosition;
		std::vector<Math::Vector4f> m_vfColor;
		std::vector<Math::Vector2f> m_vfUV0;
		std::vector<Math::Vector2f> m_vfUV1;

		std::vector<GLuint> m_vsIndices;

		GLuint m_iVBO;
		GLuint m_iEBO;
		GLuint m_iVAO;

		bool m_bGLInited;

		void _initGLObjects();
		void _clearGLObjects();

		void _buildVAOFromMesh();

		size_t m_iVertexCount;
		size_t m_iIndexCount;

		bool _validVertexData();
		bool m_bValidMesh;
	};
};