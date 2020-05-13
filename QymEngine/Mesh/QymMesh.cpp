#include "Mesh/QymMesh.h"
#include <algorithm>
#include "Render/QymRenderCommon.h"
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace QymEngine;
using namespace QymEngine::Math;

template< typename _attrib_type_ >
void PackVertexAttribute(std::vector<GLubyte> & packed, const std::vector<_attrib_type_> & attrib,
	const int glLocation, const int glType, const int glComponents)
{
	if (attrib.size() > 0)
	{
		const size_t offset = packed.size();
		const size_t size = attrib.size() * sizeof(attrib[0]);

		packed.resize(offset + size);
		memcpy(&packed[offset], attrib.data(), size);

		glEnableVertexAttribArray(glLocation);
		glVertexAttribPointer(glLocation, glComponents, glType, false, sizeof(attrib[0]), (void *)(offset));
	}
	else
	{
		glDisableVertexAttribArray(glLocation);
	}
};

QymMesh::QymMesh() :
m_iVBO(0),
m_iEBO(0),
m_iVAO(0),
m_bGLInited(false),
m_iVertexCount(0),
m_iIndexCount(0),
m_bValidMesh(false)
{
}

QymMesh::QymMesh(const QymMesh & _mesh) :
m_vfPosition(_mesh.m_vfPosition),
m_vfColor(_mesh.m_vfColor),
m_vfUV0(_mesh.m_vfUV0),
m_vfUV1(_mesh.m_vfUV1),
m_vsIndices(_mesh.m_vsIndices),
m_iVBO(0),
m_iEBO(0),
m_iVAO(0),
m_bGLInited(false),
m_iVertexCount(0),
m_iIndexCount(0),
m_bValidMesh(false)
{
	if (!this->_validVertexData()) {
		this->ClearMesh();
	}
	else {
		this->m_iVertexCount = this->m_vfPosition.size();
		this->m_iIndexCount = this->m_vsIndices.size();
		this->m_bValidMesh = true;
	}

	this->_initGLObjects();
	this->_buildVAOFromMesh();
}

QymMesh::QymMesh(const VertexAttribs & vectices, const IndexArray & indices) :
m_vfPosition(vectices.position),
m_vfColor(vectices.color),
m_vfUV0(vectices.uv0),
m_vfUV1(vectices.uv1),
m_vsIndices(indices),
m_iVBO(0),
m_iEBO(0),
m_iVAO(0),
m_bGLInited(false),
m_iVertexCount(0),
m_iIndexCount(0),
m_bValidMesh(false)
{
	if (!this->_validVertexData()) {
		this->ClearMesh();
	}
	else {
		this->m_iVertexCount = this->m_vfPosition.size();
		this->m_iIndexCount = this->m_vsIndices.size();
		this->m_bValidMesh = true;
	}

	this->_initGLObjects();
	this->_buildVAOFromMesh();
}

QymMesh::QymMesh(VertexAttribs && vectices, IndexArray && indices) :
m_iVBO(0),
m_iEBO(0),
m_iVAO(0),
m_bGLInited(false),
m_iVertexCount(0),
m_iIndexCount(0),
m_bValidMesh(false)
{
	this->m_vfPosition.swap(vectices.position);
	this->m_vfColor.swap(vectices.color);
	this->m_vfUV0.swap(vectices.uv0);
	this->m_vfUV1.swap(vectices.uv1);

	this->m_vsIndices.swap(indices);

	if (!this->_validVertexData()) {
		this->ClearMesh();
	}
	else {
		this->m_iVertexCount = this->m_vfPosition.size();
		this->m_iIndexCount = this->m_vsIndices.size();
		this->m_bValidMesh = true;
	}

	this->_initGLObjects();
	this->_buildVAOFromMesh();
}

QymMesh::~QymMesh() {
	this->_clearGLObjects();
	this->ClearMesh();
}

QymMesh & QymMesh::operator = (const QymMesh & _mesh) {
	this->m_vfPosition = _mesh.m_vfPosition;
	this->m_vfColor = _mesh.m_vfColor;
	this->m_vfUV0 = _mesh.m_vfUV0;
	this->m_vfUV1 = _mesh.m_vfUV1;
	this->m_vsIndices = _mesh.m_vsIndices;

	if (!this->_validVertexData()) {
		this->ClearMesh();
	}
	else {
		this->m_iVertexCount = this->m_vfPosition.size();
		this->m_iIndexCount = this->m_vsIndices.size();
		this->m_bValidMesh = true;
	}

	this->_buildVAOFromMesh();

	return *this;
}

void QymMesh::_initGLObjects() {
	this->_clearGLObjects();

	glGenBuffers(1, &this->m_iVBO);
	glGenBuffers(1, &this->m_iEBO);
	glGenVertexArrays(1, &this->m_iVAO);

	this->m_bGLInited = true;
}

void QymMesh::_clearGLObjects() {
	if (glIsVertexArray(this->m_iVAO)) {
		glDeleteVertexArrays(1, &this->m_iVAO);
		this->m_iVAO = 0;
	}

	if (glIsBuffer(this->m_iVBO)) {
		glDeleteBuffers(1, &this->m_iVBO);
		this->m_iVBO = 0;
	}

	if (glIsBuffer(this->m_iEBO)) {
		glDeleteBuffers(1, &this->m_iEBO);
		this->m_iEBO = 0;
	}
}

void QymMesh::_buildVAOFromMesh() {
	if (!this->m_bValidMesh || !this->m_bGLInited) {
		return;
	}

	glBindVertexArray(this->m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_iVBO);

	std::vector<GLubyte> packed;
	PackVertexAttribute(packed, this->m_vfPosition, VERTEX_ATTRIBUTE_LOCATION_POSITION, GL_FLOAT, 3);
	PackVertexAttribute(packed, this->m_vfColor, VERTEX_ATTRIBUTE_LOCATION_COLOR, GL_FLOAT, 4);
	PackVertexAttribute(packed, this->m_vfUV0, VERTEX_ATTRIBUTE_LOCATION_UV0, GL_FLOAT, 2);
	PackVertexAttribute(packed, this->m_vfUV1, VERTEX_ATTRIBUTE_LOCATION_UV1, GL_FLOAT, 2);

	glBufferData(GL_ARRAY_BUFFER, packed.size() * sizeof(packed[0]), packed.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_iEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_iIndexCount * sizeof(this->m_vsIndices[0]), this->m_vsIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_POSITION);
	glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_COLOR);
	glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_UV0);
	glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_UV1);
}

void QymMesh::ClearMesh() {
	this->m_vfPosition.clear();
	this->m_vfColor.clear();
	this->m_vfUV0.clear();
	this->m_vfUV1.clear();
	this->m_vsIndices.clear();

	this->m_iVertexCount = 0;
	this->m_iIndexCount = 0;

	this->m_bValidMesh = false;
}

bool QymMesh::_validVertexData() {

	std::vector<size_t> sizeList;
	if (!this->m_vfPosition.empty()) {
		sizeList.push_back(this->m_vfPosition.size());
	}
	if (!this->m_vfColor.empty()) {
		sizeList.push_back(this->m_vfColor.size());
	}
	if (!this->m_vfUV0.empty()) {
		sizeList.push_back(this->m_vfUV0.size());
	}
	if (!this->m_vfUV1.empty()) {
		sizeList.push_back(this->m_vfUV1.size());
	}

	if (sizeList.empty()) {
		return false;
	}

	bool r = true;
	size_t first = sizeList[0];
	for (size_t i = 0; i < sizeList.size(); i++) {
		if (sizeList[i] != first) {
			r = false;
			break;
		}
	}

	return r;
}

QymAABB QymMesh::BuildAABB() {
	if (this->m_vfPosition.empty()) {
		return QymAABB();
	}

	Vector3f minV = this->m_vfPosition[0], maxV = this->m_vfPosition[0];
	for (size_t i = 0; i < this->m_vfPosition.size(); i++) {
		minV[0] = std::min(minV[0], this->m_vfPosition[i][0]);
		minV[1] = std::min(minV[1], this->m_vfPosition[i][1]);
		minV[2] = std::min(minV[2], this->m_vfPosition[i][2]);
		maxV[0] = std::max(maxV[0], this->m_vfPosition[i][0]);
		maxV[1] = std::max(maxV[1], this->m_vfPosition[i][1]);
		maxV[2] = std::max(maxV[2], this->m_vfPosition[i][2]);
	}

	return QymAABB(minV, maxV);
}

QymOBB QymMesh::BuildOBB(const Matrix4x4f& transform)
{
	if (this->m_vfPosition.empty()) {
		return QymOBB();
	}

	Matrix4x4f inverse = Transpose(transform);

	Vector3f minV = inverse * this->m_vfPosition[0], maxV = inverse * this->m_vfPosition[0];
	for (size_t i = 0; i < this->m_vfPosition.size(); i++) {
		Vector3f pos = inverse * this->m_vfPosition[i];
		minV[0] = std::min(minV[0], pos[0]);
		minV[1] = std::min(minV[1], pos[1]);
		minV[2] = std::min(minV[2], pos[2]);
		maxV[0] = std::max(maxV[0], pos[0]);
		maxV[1] = std::max(maxV[1], pos[1]);
		maxV[2] = std::max(maxV[2], pos[2]);
	}

	Vector3f pts[8] = {
		transform * Vector3f(minV[0], minV[1], minV[2]),
		transform * Vector3f(minV[0], minV[1], maxV[2]),
		transform * Vector3f(minV[0], maxV[1], minV[2]),
		transform * Vector3f(minV[0], maxV[1], maxV[2]),
		transform * Vector3f(maxV[0], minV[1], minV[2]),
		transform * Vector3f(maxV[0], minV[1], maxV[2]),
		transform * Vector3f(maxV[0], maxV[1], minV[2]),
		transform * Vector3f(maxV[0], maxV[1], maxV[2]),
	};
	return QymOBB(pts);
}

std::shared_ptr<QymMesh> QymMesh::BuildQuad() {

	VertexAttribs attribs;
	attribs.position = {
		Vector3f(-1.0f, -1.0f, 0.0f),
		Vector3f(1.0f, -1.0f, 0.0f),
		Vector3f(1.0f, 1.0f, 0.0f),
		Vector3f(-1.0f, 1.0f, 0.0f)
	};

	attribs.uv0 = {
		Vector2f(0.0f, 0.0f),
		Vector2f(1.0f, 0.0f),
		Vector2f(1.0f, 1.0f),
		Vector2f(0.0f, 1.0f)
	};

	IndexArray indices = { 
		0, 1, 2,
		0, 2, 3 
	};

	return std::make_shared<QymMesh>(attribs, indices);
}

std::shared_ptr<QymMesh> QymMesh::BuildQuadPatch(SpherePatchParams params) {
	float startX = params.startLongitude / 360.0f;
	float endX = (params.startLongitude + params.longitudeRange) / 360.0f;
	float startY = params.startLatitude / 180.0f;
	float endY = (params.startLatitude + params.latitudeRange) / 180.0f;

	float ratioX = 4.0f;
	float ratioY = 2.0f;

	startX = (startX * 2.0f - 1.0f) * ratioX;
	endX = (endX * 2.0f - 1.0f) * ratioX;
	startY = (startY * 2.0f - 1.0f) * ratioY;
	endY = (endY * 2.0f - 1.0f) * ratioY;

	VertexAttribs attribs;
	attribs.position = {
		Vector3f(startX, startY, -1.0f),
		Vector3f(endX, startY, -1.0f),
		Vector3f(endX, endY, -1.0f),
		Vector3f(startX, endY, -1.0f)
	};

	float paddingX = 1.0f / 480.0f;

	attribs.uv0 = {
		Vector2f(0.0f, 0.0f),
		Vector2f(1.0f, 0.0f),
		Vector2f(1.0f, 1.0f),
		Vector2f(0.0f, 1.0f)
	};

	IndexArray indices = {
		0, 1, 2,
		0, 2, 3
	};

	return std::make_shared<QymMesh>(attribs, indices);
}

std::shared_ptr<QymMesh> QymMesh::BuildTesselledQuad(const int horizontal, const int vertical) {

	const int vertexCount = (horizontal + 1) * (vertical + 1);

	VertexAttribs attribs;
	attribs.position.resize(vertexCount);
	attribs.uv0.resize(vertexCount);

	for (int y = 0; y <= vertical; y++)
	{
		const float yf = (float)y / (float)vertical;
		for (int x = 0; x <= horizontal; x++)
		{
			const float xf = (float)x / (float)horizontal;
			const int index = y * (horizontal + 1) + x;
			attribs.position[index][0] = -1 + xf * 2;
			attribs.position[index][1] = -1 + yf * 2;
			attribs.position[index][2] = 0;
			attribs.uv0[index][0] = xf;
			attribs.uv0[index][1] =  yf;
		}
	}

	IndexArray indices;
	indices.resize(horizontal * vertical * 6);

	// If this is to be used to draw a linear format texture, like
	// a surface texture, it is better for cache performance that
	// the triangles be drawn to follow the side to side linear order.
	int index = 0;
	for (GLushort y = 0; y < vertical; y++)
	{
		for (GLushort x = 0; x < horizontal; x++)
		{
			indices[index + 0] = y * (horizontal + 1) + x;
			indices[index + 1] = y * (horizontal + 1) + x + 1;
			indices[index + 2] = (y + 1) * (horizontal + 1) + x;
			indices[index + 3] = (y + 1) * (horizontal + 1) + x;
			indices[index + 4] = y * (horizontal + 1) + x + 1;
			indices[index + 5] = (y + 1) * (horizontal + 1) + x + 1;
			index += 6;
		}
	}
	return std::make_shared<QymMesh>(attribs, indices);
}

std::shared_ptr<QymMesh> QymMesh::BuildGlobe(const float uScale, const float vScale) {
	// Make four rows at the polar caps in the place of one
	// to diminish the degenerate triangle issue.
	const int poleVertical = 3;
	const int uniformVertical = 64;
	const int horizontal = 128;
	const int vertical = uniformVertical + poleVertical * 2;
	const float radius = 100.0f;

	const int vertexCount = (horizontal + 1) * (vertical + 1);

	VertexAttribs attribs;
	attribs.position.resize(vertexCount);
	attribs.uv0.resize(vertexCount);
	//attribs.color.resize(vertexCount);

	for (int y = 0; y <= vertical; y++)
	{
		float yf;
		if (y <= poleVertical)
		{
			yf = (float)y / (poleVertical + 1) / uniformVertical;
		}
		else if (y >= vertical - poleVertical)
		{
			yf = (float)(uniformVertical - 1 + ((float)(y - (vertical - poleVertical - 1)) / (poleVertical + 1))) / uniformVertical;
		}
		else
		{
			yf = (float)(y - poleVertical) / uniformVertical;
		}
		const float lat = static_cast<float>((yf - 0.5f) * M_PI);
		const float cosLat = cosf(lat);
		for (int x = 0; x <= horizontal; x++)
		{
			const float xf = (float)x / (float)horizontal;
			const float lon = static_cast<float>((0.25f + xf) * M_PI * 2);
			const int index = y * (horizontal + 1) + x;

			if (x == horizontal)
			{
				// Make sure that the wrap seam is EXACTLY the same
				// xyz so there is no chance of pixel cracks.
				attribs.position[index] = attribs.position[y * (horizontal + 1) + 0];
			}
			else
			{
				attribs.position[index][0] = radius * cosf(lon) * cosLat;
				attribs.position[index][1] = radius * sinf(lat);
				attribs.position[index][2] = radius * sinf(lon) * cosLat;
			}

			// With a normal mapping, half the triangles degenerate at the poles,
			// which causes seams between every triangle.  It is better to make them
			// a fan, and only get one seam.
			if (y == 0 || y == vertical)
			{
				attribs.uv0[index][0] = 0.5f;
			}
			else
			{
				attribs.uv0[index][0] = xf * uScale;
			}
			attribs.uv0[index][1] = (yf) * vScale;
			/*for (int i = 0; i < 4; i++)
			{
				attribs.color[index][i] = 1.0f;
			}*/
		}
	}

	IndexArray indices;
	indices.resize(horizontal * vertical * 6);

	int index = 0;
	for (GLushort x = 0; x < horizontal; x++)
	{
		for (GLushort y = 0; y < vertical; y++)
		{
			indices[index + 0] = y * (horizontal + 1) + x;
			indices[index + 1] = y * (horizontal + 1) + x + 1;
			indices[index + 2] = (y + 1) * (horizontal + 1) + x;
			indices[index + 3] = (y + 1) * (horizontal + 1) + x;
			indices[index + 4] = y * (horizontal + 1) + x + 1;
			indices[index + 5] = (y + 1) * (horizontal + 1) + x + 1;
			index += 6;
		}
	}

	return std::make_shared<QymMesh>(attribs, indices);
}

std::shared_ptr<QymMesh> QymMesh::BuildSpherePatch(SpherePatchParams params, const float uScale, const float vScale) {


	// Make four rows at the polar caps in the place of one
	// to diminish the degenerate triangle issue.
	const int poleVertical = 3;
	const int uniformVertical = 64;
	const int uniformHorizontal = 128;

	const int horizontal = static_cast<const int>(round(uniformHorizontal * (params.longitudeRange / 360.0f)));
	const int patchUniformVertical = static_cast<const int>(round(uniformVertical * (params.latitudeRange / 180.0f)));

	int temp = patchUniformVertical;
	bool wrapSouthPole = false, wrapNorthPole = false;
	if (params.startLatitude <= 0.0f + FLT_EPSILON) {
		temp += poleVertical;
		wrapSouthPole = true;
	}
	if (params.startLatitude + params.latitudeRange >= 180.0f - FLT_EPSILON) {
		temp += poleVertical;
		wrapNorthPole = true;
	}
	const int vertical = temp;
	const float radius = 100.0f;

	const int vertexCount = (horizontal + 1) * (vertical + 1);

	VertexAttribs attribs;
	attribs.position.resize(vertexCount);
	attribs.uv0.resize(vertexCount);

	for (int y = 0; y <= vertical; y++)
	{
		float yf;
		if (wrapSouthPole && y <= poleVertical)
		{
			yf = (float)y / (poleVertical + 1) / patchUniformVertical;
		}
		else if (wrapNorthPole && y >= vertical - poleVertical)
		{
			yf = (float)(patchUniformVertical - 1 + ((float)(y - (vertical - poleVertical - 1)) / (poleVertical + 1))) / patchUniformVertical;
		}
		else
		{
			if (wrapSouthPole) {
				yf = (float)(y - poleVertical) / patchUniformVertical;
			}
			else {
				yf = (float)(y) / patchUniformVertical;
			}
		}
		float nyf = (params.startLatitude + yf * params.latitudeRange) / 180.0f;
		const float lat = static_cast<float>((nyf - 0.5f) * M_PI);
		const float cosLat = cosf(lat);
		for (int x = 0; x <= horizontal; x++)
		{
			const float xf = (float)x / (float)horizontal;
			float nxf = (params.startLongitude + xf * params.longitudeRange) / 360.0f;
			const float lon = static_cast<float>((0.25f + nxf) * M_PI * 2);
			const int index = y * (horizontal + 1) + x;

			//if (x == horizontal)
			//{
			//	// Make sure that the wrap seam is EXACTLY the same
			//	// xyz so there is no chance of pixel cracks.
			//	attribs.position[index] = attribs.position[y * (horizontal + 1) + 0];
			//}
			//else
			float aa = cosf(lon);
			{
				attribs.position[index][0] = radius * cosf(lon) * cosLat;
				attribs.position[index][1] = radius * sinf(lat);
				attribs.position[index][2] = radius * sinf(lon) * cosLat;
			}

			// With a normal mapping, half the triangles degenerate at the poles,
			// which causes seams between every triangle.  It is better to make them
			// a fan, and only get one seam.
			if ((wrapSouthPole && y == 0) || (wrapNorthPole && y == vertical))
			{
				attribs.uv0[index][0] = 0.5f;
			}
			else
			{
				attribs.uv0[index][0] = xf * uScale;
			}
			attribs.uv0[index][1] = (yf)* vScale;
		}
	}

	IndexArray indices;
	indices.resize(horizontal * vertical * 6);

	int index = 0;
	for (GLushort x = 0; x < horizontal; x++)
	{
		for (GLushort y = 0; y < vertical; y++)
		{
			indices[index + 0] = y * (horizontal + 1) + x;
			indices[index + 1] = y * (horizontal + 1) + x + 1;
			indices[index + 2] = (y + 1) * (horizontal + 1) + x;
			indices[index + 3] = (y + 1) * (horizontal + 1) + x;
			indices[index + 4] = y * (horizontal + 1) + x + 1;
			indices[index + 5] = (y + 1) * (horizontal + 1) + x + 1;
			index += 6;
		}
	}

	return std::make_shared<QymMesh>(attribs, indices);
}

std::shared_ptr<QymMesh> QymMesh::LoadModel(const std::string path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
		throw std::runtime_error(warn + err);
	}

	VertexAttribs attribs;
	IndexArray indices;

	std::unordered_map<QymVertex, uint32_t, QymVertexHash> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			QymVertex vertex;

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.uv0 = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(attribs.position.size());
				attribs.position.push_back(vertex.pos);
				attribs.color.push_back(vertex.color);
				attribs.uv0.push_back(vertex.uv0);
			}
			indices.push_back(uniqueVertices[vertex]);

			// attribs.position.push_back(vertex.pos);
			// attribs.color.push_back(vertex.color);
			// attribs.uv0.push_back(vertex.uv0);
			// indices.push_back(indices.size());
		}
	}

	return std::make_shared<QymMesh>(attribs, indices);
}
