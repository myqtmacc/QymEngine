#include "QymMath.h"

using namespace QymEngine::Math;

Vector3f QymIntersection::PlaneRayIntersection(const QymPlane & plane, const QymRay & ray) {
	Vector3f origin(0.0f, 0.0f, 0.0f);
	float d = -glm::dot(plane.m_vNormal, (plane.m_vPoint - origin));
	float rl = -glm::dot(plane.m_vNormal, ray.m_vDirection);

	float t = d / rl;

	Vector3f r = ray.GetPoint(t);
	return r;
}


int QymIntersection::PlaneSphereIntersection(const QymPlane & plane, const QymSphere & sphere) {
	float ds = glm::dot(plane.m_vNormal, sphere.m_vCenter);
	float dp = glm::dot(plane.m_vNormal, plane.m_vPoint);
	if ((ds - dp) >= sphere.m_fRadius) {
		return 1;
	}
	else if ((ds - dp) <= -sphere.m_fRadius) {
		return -1;
	}
	return 0;
}


int QymIntersection::PlaneAABBIntersection(const QymPlane & plane, const QymAABB & aabb) {
	float minD, maxD;
	const Vector3f & n = plane.m_vNormal;
	const Vector3f & min = aabb.m_vMin;
	const Vector3f & max = aabb.m_vMax;
	if (n[0] > 0.0f) {
		minD = n[0] * min[0];
		maxD = n[0] * max[0];
	}
	else {
		minD = n[0] * max[0];
		maxD = n[0] * min[0];
	}

	if (n[1] > 0.0f) {
		minD += n[1] * min[1];
		maxD += n[1] * max[1];
	}
	else {
		minD = n[1] * max[1];
		maxD = n[1] * min[1];
	}

	if (n[2] > 0.0f) {
		minD = n[2] * min[2];
		maxD = n[2] * max[2];
	}
	else {
		minD = n[2] * max[2];
		maxD = n[2] * min[2];
	}

	float d = glm::dot(plane.m_vNormal, plane.m_vPoint);
	if (minD >= d) {
		return 1;
	}
	if (maxD <= d) {
		return -1;
	}
	return 0;
}

template<typename BB>
bool QymIntersection::FrustrumIntersection(const Matrix4x4f & vpm, const BB &bb)
{
	bool valid = true;
	Vector4f pts[8];
	bb.GetPts(pts);

	Vector4f
	v1 = vpm * pts[0], v2 = vpm * pts[1],
	v3 = vpm * pts[2], v4 = vpm * pts[3],
	v5 = vpm * pts[4], v6 = vpm * pts[5],
	v7 = vpm * pts[6], v8 = vpm * pts[7];

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
template bool QymIntersection::FrustrumIntersection<QymAABB>(const Matrix4x4f & vpm, const QymAABB &bb);
template bool QymIntersection::FrustrumIntersection<QymOBB>(const Matrix4x4f & vpm, const QymOBB &bb);