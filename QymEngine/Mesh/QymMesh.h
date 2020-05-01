#pragma once

#include <memory>
#include <vector>
#include <GL/glew.h>
#include "Math/QymMath.h"

namespace QymEngine {

	using namespace vmath;

	struct VertexAttribs
	{
		std::vector<vec3> position;
		std::vector<vec4> color;
		std::vector<vec2> uv0;
		std::vector<vec2> uv1;
	};

	struct SpherePatchParams
	{
		float startLongitude;
		float longitudeRange;
		float startLatitude;
		float latitudeRange;
	};

	typedef std::vector<GLushort> IndexArray;

	class QymMesh
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

		QymAABB BuildAABB();
		QymOBB BuildOBB(const mat4 & transform);

	public:
		static std::shared_ptr<QymMesh> BuildGlobe(const float uScale = 1.0f, const float vScale = 1.0f);
		static std::shared_ptr<QymMesh> BuildSpherePatch(SpherePatchParams params, const float uScale = 1.0f, const float vScale = 1.0f);
		static std::shared_ptr<QymMesh> BuildQuad();
		static std::shared_ptr<QymMesh> BuildQuadPatch(SpherePatchParams params);
		static std::shared_ptr<QymMesh> BuildTesselledQuad(const int horizontal, const int vertical);

	private:
		std::vector<vec3> m_vfPosition;
		std::vector<vec4> m_vfColor;
		std::vector<vec2> m_vfUV0;
		std::vector<vec2> m_vfUV1;

		std::vector<GLushort> m_vsIndices;

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