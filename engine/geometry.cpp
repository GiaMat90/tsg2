#include "geometry.h"

namespace geometry {
	/*
	* Quaternion class implementation
	*/
	quaternion::quaternion(const scalar w, const scalar i, const scalar j, const scalar k) :
		m_w(w), m_i(i), m_j(j), m_k(k) {
	};
	quaternion::quaternion(const tsg::vector<scalar, 3>& vec) :
		m_w(scalar(0)), m_i(vec[axes::x]), m_j(vec[axes::y]), m_k(vec[axes::z]) {
	};
	void quaternion::normalize() {
		scalar d = m_w * m_w + m_i * m_i + m_j * m_j + m_k * m_k;

		// Check for zero length quaternion, and use the no-rotation
		// quaternion in that case.
		if (d < epsilon) {
			m_w = 1;
		}
		else {
			d = scalar(1) / sqrt(d);  //((real)1.0) / real_sqrt(d);
			m_w *= d;
			m_i *= d;
			m_j *= d;
			m_k *= d;
		}
	}
	quaternion& quaternion::operator*=(const quaternion& other)
	{
		m_w = m_w * other.m_w - m_i * other.m_i - m_j * other.m_j - m_k * other.m_k;
		m_i = m_w * other.m_i + m_i * other.m_w + m_j * other.m_k - m_k * other.m_j;
		m_j = m_w * other.m_j + m_j * other.m_w + m_k * other.m_i - m_i * other.m_k;
		m_k = m_w * other.m_k + m_k * other.m_w + m_i * other.m_j - m_j * other.m_i;
		return *this;
	}
	quaternion& quaternion::operator+=(const quaternion& other)
	{
		quaternion retVal;
		/*
		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		// Scalar component is:
		// ps * qs - pv . qv
		retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

		return retVal;
		*/

		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		vector3D pv({ m_i, m_j, m_k });
		vector3D qv({ other.m_i, other.m_j, other.m_k });
		vector3D newVec = m_w * qv + other.m_w * pv + vector3D::cross(pv, qv);
		this->m_i = newVec[axes::x];
		this->m_j = newVec[axes::y];
		this->m_k = newVec[axes::z];

		// Scalar component is:
		// ps * qs - pv . qv
		this->m_w = m_w * other.m_w - vector3D::dot(pv, qv);

		return *this;
	}

	quaternion& quaternion::operator+=(const tsg::vector<scalar, 3>& vector)
	{
		quaternion q(vector);
		q *= *this;
		m_w += q.m_w * scalar(0.5);
		m_i += q.m_i * scalar(0.5);
		m_j += q.m_j * scalar(0.5);
		m_k += q.m_k * scalar(0.5);
		return *this;
	}
	quaternion& quaternion::rotate(const tsg::vector<scalar, 3>& vector) {
		(*this) *= quaternion(vector);
		return *this;
	}

	/*
	* Bounding volume class implementation
	*/
	bounding_volume::bounding_volume(const type t, const std::size_t d) : m_type(t), m_dimension(d) {};
	bounding_volume::bounding_volume(const bounding_volume& other) { assert(false); /* must be implemented in derived classes */ };
	void bounding_volume::translate(const tsg::vector<scalar, 2>& pos) {
		/* can't be pure virtual because of it depends on the dimension... */
		assert(0); // not implemented
	};
	void bounding_volume::translate(const tsg::vector<scalar, 3>& pos) {
		/* can't be pure virtual because of it depends on the dimension... */
		assert(0); // not implemented
	};
	bool bounding_volume::contains(const tsg::vector<scalar, 2>& pos) const {
		/* can't be pure virtual because of it depends on the dimension... */
		assert(0); // not implemented
		return false;
	};
	bool bounding_volume::contains(const tsg::vector<scalar, 3>& pos) const {
		/* can't be pure virtual because of it depends on the dimension... */
		assert(0); // not implemented
		return false;
	};

	/*
	* Intertia factory impl
	*/
	matrix3D inertia_tensor_factory::get_inverse(const TYPE t) {
		if (TYPE::CUBE == t) {
			return matrix3D(matrix3D::TYPE::DIAGONAL, scalar(1) / scalar(12));
		}
		else {
			tsg::logger::get_instance().write("Wrong inertia tensor type inserted");
			assert(false);
		}
	}
}
