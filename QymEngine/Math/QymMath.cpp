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