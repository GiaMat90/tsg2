#pragma once

/* tsg includes */
#include <tsg/math.h>
#include <tsg/logger.h>

/* std includes */
#include <initializer_list>
#include <cassert>
#include <cmath>
#include <limits>
#include <concepts>	// c++20
#include <array>	// c++11
#include <expected>	// c++23
#include <numbers>  // pi_v

namespace geometry {
	template <std::size_t Dim>
	concept Dimension2D = Dim == 2;

	template <std::size_t Dim>
	concept Dimension3D = Dim == 3;

	template <std::size_t Dim>
	concept GeometricDimension = Dimension2D<Dim> || Dimension3D<Dim>;

	enum axes : std::size_t {
		x = 0u,
		y = 1u,
		z = 2u
	};

	enum measure {
		width = 0,
		height = 1,
		depth = 2
	};

	enum class shape_type {
		none,
		rectangle,
		circle,
		triangle,
		sphere,
		pyramid
	};

#ifdef DOUBLE_PRECISION
	using scalar = double;
	using degree_t = tsg::degree<scalar>;
	scalar inline abs(const scalar v) {
		return std::abs(v);
	}
	scalar inline sqrt(const scalar v) {
		return std::sqrt(v);
	}
	scalar inline arccos(const scalar v) {
		return std::acos(v);
	}
	scalar inline cos(const scalar v) {
		return std::cos(v);
	}
	scalar inline sin(const scalar v) {
		return std::sin(v);
	}
	constexpr scalar epsilon = std::numeric_limits<double>::epsilon();
	constexpr scalar scalar_zero{ 1e-6 };
	constexpr scalar pi{ std::numbers::pi_v<double> };
	constexpr scalar nan{ std::numeric_limits<double>::quiet_NaN() };
#else
#define SINGLE_PRECISION
	using scalar = float;
	using degree_t = tsg::degree<scalar>;
	scalar inline abs(const scalar v) {
		return std::fabsf(v);
	}
	scalar inline sqrt(const scalar v) {
		return std::sqrtf(v);
	}
	scalar inline arccos(const scalar v) {
		return std::acosf(v);
	}
	scalar inline arcsin(const scalar v) {
		return std::asinf(v);
	}
	scalar inline cos(const scalar v) {
		return std::cosf(v);
	}
	scalar inline sin(const scalar v) {
		return std::sinf(v);
	}
	constexpr scalar epsilon = std::numeric_limits<float>::epsilon();
	constexpr scalar scalar_zero{ 1e-6f };
	constexpr scalar pi{ std::numbers::pi_v<float> };
	constexpr scalar nan{ std::numeric_limits<float>::quiet_NaN() };
#endif
	using point2D = tsg::vector<scalar, 2>;
	using point3D = tsg::vector<scalar, 3>;
	using vector2D = tsg::vector<scalar, 2>;
	using vector3D = tsg::vector<scalar, 3>;
	using matrix3D = tsg::matrix<scalar, 3, 3>;
	/*
	* Quaternion class for orientation rappresentation
	*/
	class quaternion {
	public:
		quaternion(const scalar w = scalar(1), const scalar i = scalar(0), const scalar j = scalar(0), const scalar k = scalar(0));
		quaternion(const tsg::vector<scalar, 3>& vec);
		void normalize();
		quaternion& operator *=(const quaternion& other);
		inline friend quaternion& operator*(quaternion& lhs, const quaternion& rhs) {
			return lhs *= rhs;
		};
		quaternion& operator+=(const quaternion& other);
		inline friend quaternion& operator+(quaternion& lhs, const quaternion& rhs) {
			return lhs += rhs;
		};
		quaternion& operator+=(const tsg::vector<scalar, 3>& vector);
		quaternion& rotate(const tsg::vector<scalar, 3>& vector);
	private:
		scalar m_w{};
		scalar m_i{};
		scalar m_j{};
		scalar m_k{};
	};
	
	/*
	* class to compute a finite line in 3D space
	*/
	// impl
	template <std::size_t Dim> requires GeometricDimension<Dim>
	class segment {
		using point = tsg::vector<scalar, Dim>;
		using vector = tsg::vector<scalar, Dim>;
	public:
		segment() = default;
		virtual ~segment() = default;
		segment(const point& start, const point& end) noexcept :
			m_start(start), m_end(end), m_vector((end - start).get_normalized()), m_lenght((end - start).get_norm()) {
		};
		segment(const segment& other) noexcept :
			m_start(other.m_start), m_end(other.m_end), m_lenght(other.m_lenght), m_vector(other.m_vector) {
		};
	public:
		vector get_direction() const { return m_vector; };
		scalar get_lenght() const { return m_lenght; };
		point get_start() const { return m_start; };
		point get_end() const { return m_end; };
	public:
		/* point in the segment closest to p*/
		point closest_to(const point& p) {
			if constexpr (Dim == 2) {
				/* TODO  */
				assert(0);
				// 2D case
				vector2D segment_vector = (m_vector * m_lenght);
				scalar t = vector2D::dot(segment_vector, (p - m_start));
				t = std::max(scalar(0), std::min(scalar(1), t));
				return m_start + (segment_vector * t);
			}
			else if constexpr (Dim == 3) {
				// 3D case
				vector3D segment_vector = (m_vector * m_lenght);
				scalar t = vector3D::dot(segment_vector, (p - m_start));
				t = std::max(scalar(0), std::min(scalar(1), t));
				return m_start + (segment_vector * t);
			}
		};
		/* point in the segment closer to segment other */
		std::pair<point, point> closest_to(const segment& other) {
			if constexpr (Dim == 2) {
				/* TODO */
				assert(0);
				return closest_to(other.m_start, other.m_end);
			}
			else if constexpr (Dim == 3) {
				/* Explaination:
				* segment this:		p(s) = m_center + s * da		s \in [0,1]
				* segment other:	q(t) = other.m_center + t * db	t \in [0,1]
				* The result is given to the minimum of this function:
				* d^2(s,t) = |p(s) - q(t)|^2 =
				*		= |m_center + s * da - other.m_center - t * db|^2
				*		= |r + s * da - t * db |^2
				* with r = m_center - other.m_center
				* => d^2(s,t) = dot( (r + s * da - t * db), (r + s * da - t * db) ) =
				*		= dot(r,r) + 2s*dot(da,r) - 2t*dot(db,r) + s^2*dot(da,da) - 2st*dot(da,db) + t^2*dot(db,db)
				*		= s^2 * da2 - 2st * dab + t^2 * db2 + 2s * dot(da,r) - 2t * dot(db,r) + r2
				* The minimus is given by the solution of the sistem of partial derivation:
				*	df / ds = 0		=>		2s * da2 - 2t * dab 2 * dot(da,r)
				*	df / dt = 0		=>		-2s * dab + 2t * db2 -2 * dot(db,r)
				*	=>	s*da2 - t*dab = -dot(da,r)
				*	=>	-s*dab + tdb2 = dot(db,r)
				* In matrix form:
				*	|  da2	-dab | | s |   | -dot(da,r) |
				*	|			 | |   | = |			|
				*	| -dab	 db2 | | t |   |  dot(db,r) |
				*/
				vector3D da = m_end - m_start;				// direction of segment a
				vector3D db = other.m_end - other.m_start;	// direction of segment b
				vector3D r = m_start - other.m_start;		// 

				scalar da2 = vector3D::dot(da, da);	// |da|^2
				scalar db2 = vector3D::dot(db, db); // |db|^2
				scalar dab = vector3D::dot(da, db); // parallelism
				scalar da_over_r = vector3D::dot(da, r);	// projection r over va
				scalar db_over_r = vector3D::dot(db, r);	// projection r over vb

				/* scalar parameters of the segment parametric equation  */
				scalar s{};
				scalar t{};
				/* checking for almost parallel edges */
				scalar denom = da2 * db2 - dab * dab;	// determinant of the cramer system
				if (denom < scalar_zero) {
					t = dab > da2 ? da_over_r / dab : db_over_r / db2;
				}
				else {
					s = (dab * db_over_r - db2 * da_over_r) / denom;
					t = (da2 * db_over_r - dab * da_over_r) / denom;
				}

				return {
					m_start + da * std::max(scalar(0), std::min(scalar(1), s)),
					other.m_start + db * std::max(scalar(0), std::min(scalar(1), t))
				};
			}
			else {
				assert(0); // unsupported dimension
				return { point(), point() };
			}
		}
	public:
		segment& operator=(const segment& other) {
			this->m_end = other.m_end;
			this->m_start = other.m_start;
			this->m_lenght = other.m_lenght;
			this->m_vector = other.m_vector;
			return *this;
		};
	private:
		point m_start{};
		point m_end{};
		vector m_vector{};
		scalar m_lenght{};
	};
	/*
	* class to compute a finite/limited plane, like a face or a wall, in 3D space
	*/
	template<std::size_t Dim> requires GeometricDimension<Dim>
	class finite_plane {
		using point = tsg::vector<scalar, Dim>;
		using vector = tsg::vector<scalar, Dim>;
	public:
		finite_plane() = default;
		finite_plane(const point& c) noexcept : m_center(c) {}
		virtual ~finite_plane() = default;
	public:
		inline vector get_normal() const { return m_normal; };
		inline point get_center() const { return m_center; };
	public:
		inline vector3D project(const point& p) {
			return p - m_normal * vector::dot(m_normal, p - m_center);
		}
		inline bool contain(const point3D& p) const {
			vector3D projection{ p - m_center };
			scalar u{ vector::dot(projection, m_base[1])};
			scalar v{ vector::dot(projection, m_base[2])};
			return geometry::abs(u) <= m_half_sizes[0] && geometry::abs(v) <= m_half_sizes[1];
		}
	protected:
		point m_center{};
		vector m_normal{};
		vector m_base[2];
		vector2D m_half_sizes{};
	};

	class bounding_volume {
	public:
		enum class type {
			aabb,	// axes aligned bounding box
			obb,	// oriented bounding box
			sphere,
			polygon,
			unknown
		};
	public:
		bounding_volume(const type t = type::unknown, const std::size_t d = 0u);
		bounding_volume(const bounding_volume& other);
		virtual ~bounding_volume() = default;
		virtual void clone(bounding_volume&) = 0;
		inline const type get_type() const { return m_type; };
		inline const std::size_t get_dimension() const { return m_dimension; };
	public:
		virtual void translate(const tsg::vector<scalar, 2>& pos);
		virtual void translate(const tsg::vector<scalar, 3>& pos);
		virtual bool contains(const tsg::vector<scalar, 2>& pos) const;
		virtual bool contains(const tsg::vector<scalar, 3>& pos) const;
	public:
		/* pure virtual */
		virtual void rotate(const scalar angle) = 0;
		inline virtual scalar get_min(const std::size_t axes) const = 0;
		inline virtual scalar get_max(const std::size_t axes) const = 0;
	public:
		/* factory method */
		template<std::size_t Dim>
		static bounding_volume* create(const type t);
	protected:
		type m_type{ type::unknown };
		std::size_t m_dimension{};
	};

	/*
	* The box has validity only for 2 or 3 dimension (as per now). 
	* This giustify the use of the concept GeometricDimension.
	*/
	template <std::size_t Dim> requires GeometricDimension<Dim>
	class box : public bounding_volume {
	public:
		using vector = tsg::vector<scalar, Dim>;
		using point = tsg::vector<scalar, Dim>;
		using vertex = tsg::vector<scalar, Dim>;
		using edge = segment<Dim>;
		using face = finite_plane<Dim>;
		using vertexes = std::array<vertex, Dim == 3 ? 8 : 4>;
		using edges = std::array<edge, Dim == 3 ? 12 : 4>;
		using faces = std::array<face, Dim == 3 ? 6 : 1>;
	public:
		box(const type t = type::aabb) : bounding_volume(t, Dim), m_center(), m_half_sizes() {};
		box(const tsg::vector<scalar, Dim>& center, const tsg::vector<scalar, Dim>& half_sizes, const type t = type::aabb) :
			bounding_volume(t, Dim), m_center(center), m_half_sizes(half_sizes)
		{
			compute();
		};
		box(const box<Dim>& other) : 
			bounding_volume(other.m_type),
			m_center(other.m_center),
			m_half_sizes(other.m_half_sizes),
			m_direction(other.m_direction),
			m_base(other.m_base)
		{
			for (std::size_t i = 0u; i < m_vertexes.size(); ++i) {
				m_vertexes[i] = other.m_vertexes[i];
			}
			for (std::size_t i = 0u; i < m_edges.size(); ++i) {
				m_edges[i] = other.m_edges[i];
			}
			for (std::size_t i = 0u; i < m_faces.size(); ++i) {
				m_faces[i] = other.m_faces[i];
			}
		};
		virtual ~box() {};
		box<Dim>& operator=(const box<Dim>& other) {
			other.clone(this);
			return *this;
		};
		void clone(bounding_volume& other) override {
			if(auto b = dynamic_cast<box<Dim>*>(&other)) {
				this->clone(*b);
			}
			else {
				assert(0); // invalid type for cloning
			}
		}
		void clone(box<Dim>& other) {
			other.m_center = this->m_center;
			other.m_half_sizes = this->m_half_sizes;
			other.m_direction = this->m_direction;
			other.m_base = this->m_base;
			for (std::size_t i = 0u; i < m_vertexes.size(); ++i) {
				other.m_vertexes[i] = this->m_vertexes[i];
			}
			for (std::size_t i = 0u; i < m_edges.size(); ++i) {
				other.m_edges[i] = this->m_edges[i];
			}
			for (std::size_t i = 0u; i < m_faces.size(); ++i) {
				other.m_faces[i] = this->m_faces[i];
			}
			other.m_type = this->m_type;
			other.m_dimension = this->m_dimension;
		}
	public:
		// setter
		inline void set_center(const tsg::vector<scalar, Dim>& c) { m_center = c; }
		inline void set_half_sizes(const tsg::vector<scalar, Dim>& hs) { m_half_sizes = hs; }
		// getters
		inline tsg::vector<scalar, Dim> get_center() const { return m_center; };
		template <std::size_t Ax>
		inline tsg::vector<scalar, Dim> get_max_point() const {
			static_assert(Ax < Dim);
			return m_center[Ax] + m_half_sizes[Ax];
		}
		template <std::size_t Ax>
		inline scalar get_size() const {
			static_assert(Ax < Dim);
			return scalar(2) * m_half_sizes[Ax];
		}
		template <std::size_t Ax>
		inline scalar get_half_size() const {
			static_assert(Ax < Dim);
			return m_half_sizes[Ax];
		}
		inline tsg::vector<scalar, Dim> get_sizes() const {
			return scalar(2) * m_half_sizes;
		}
		inline tsg::vector<scalar, Dim> get_half_sizes() const {
			return m_half_sizes;
		}
		inline scalar get_max(const std::size_t axes) const override {
			assert(axes < Dim);
			return m_center[axes] + m_half_sizes[axes];
		};
		inline scalar get_min(const std::size_t axes) const override {
			assert(axes < Dim);
			return m_center[axes] - m_half_sizes[axes];
		};
		inline const tsg::matrix<scalar, Dim, Dim>& get_axes() const { return m_base; };
		inline vertexes get_vertexes() const { return m_vertexes; }
		inline edges get_edges() const { return m_edges; };
		inline faces get_faces() const { return m_faces; };
	public:
		bool operator==(const box<Dim>& other) {
			return (m_center == other.m_center) && (m_half_sizes == other.m_half_sizes);
		}
	public:
		/* tronsform methods */
		void translate(const tsg::vector<scalar, Dim>& pos) override {
			m_center += pos;
			compute();
		}
		void rotate(const scalar angle) override {
			/* TODO */
			assert(false);
		}
	public:
		bool contains(const point& p) const override {
			vector v = p - m_center;
			for (size_t i = 0u; i < Dim; ++i) {
				scalar projection = vector::dot(v, m_base.get_row(i));
				if (geometry::abs(projection) > m_half_sizes.get<axes::x>() + scalar_zero) {
					 return false;
				}
			}
			return true;
		};
	private:
		/*
		* Private methods to compute the box vertexes, edges and faces
		*/
		void compute() {
			if constexpr (Dim == 2) {
				/*
				* Compute vertexes: center ± sizes computed with bit operations, very efficent way
				*   i | Binario | Bit1 | Bit0 | Y    | X
				*	--|---------|------|------|------|------
				*	0 |    00   |  0   |  0   | -ext | -ext
				*	1 |    01   |  0   |  1   | -ext | +ext
				*	2 |    10   |  1   |  0   | +ext | -ext
				*	3 |    11   |  1   |  1   | +ext | +ext
				*  The order is this:
				*
				*   2----------3
				*   |          |
				*   |          |
				*   |          |
				*   |          |
				*   0----------1
				*/
				// TODO: verify rows or columns.
				point vertex = m_center;
				for (std::size_t i = 0; i < m_vertexes.size(); ++i) {
					m_vertexes[i] = m_center +
						m_base.get_row<0>() * ((i & 1) ? m_half_sizes[0] : -m_half_sizes[0]) +
						m_base.get_row<1>() * ((i & 2) ? m_half_sizes[1] : -m_half_sizes[1]);
				}
				/*
				* Compute edges:
				* Given the vertexe's order (see above), the indexes that determine every edges are the following 12:
				*/
				std::size_t vertexes_index[4][2] = {
					{0,1}, {1, 3}, {3, 2}, {2, 0}
				};
				/* With these vertex index we can compute every edges as edge(end - start). */
				for (std::size_t i = 0u; i < m_edges.size(); ++i) {
					m_edges[i] = edge(m_vertexes[vertexes_index[i][0]], m_vertexes[vertexes_index[i][1]]);
				}
				/*
				* Compute faces
				*/
				for (std::size_t i = 0u; i < m_faces.size(); ++i) {
					m_faces[i] = face(m_center);
				}
			}
			else if constexpr (Dim == 3) {
				/*
				* Compute vertexes: center ± sizes computed with bit operations, very efficent way
				*   i | Binario | Bit2 | Bit1 | Bit0 | Z    | Y    | X
				*	--|---------|------|------|------|------|------|------
				*	0 |   000   |  0   |  0   |  0   | -ext | -ext | -ext
				*	1 |   001   |  0   |  0   |  1   | -ext | -ext | +ext
				*	2 |   010   |  0   |  1   |  0   | -ext | +ext | -ext
				*	3 |   011   |  0   |  1   |  1   | -ext | +ext | +ext
				*	4 |   100   |  1   |  0   |  0   | +ext | -ext | -ext
				*	5 |   101   |  1   |  0   |  1   | +ext | -ext | +ext
				*	6 |   110   |  1   |  1   |  0   | +ext | +ext | -ext
				*	7 |   111   |  1   |  1   |  1   | +ext | +ext | +ext
				*  The order is this:
				*      6----------7
				*     /|         /|
				*    / |        / |
				*   2----------3  |
				*   |  |       |  |
				*   |  4-------|--5
				*   | /        | /
				*   |/         |/
				*   0----------1
				*/
				// TODO: verify rows or columns.
				point vertex = m_center;
				for (std::size_t i = 0; i < m_vertexes.size(); ++i) {
					m_vertexes[i] = m_center +
						m_base.get_row<0>() * ((i & 1) ? m_half_sizes[0] : -m_half_sizes[0]) +
						m_base.get_row<1>() * ((i & 2) ? m_half_sizes[1] : -m_half_sizes[1]) +
						m_base.get_row<2>() * ((i & 4) ? m_half_sizes[2] : -m_half_sizes[2]);
				}
				/*
				* Compute edges:
				* Given the vertexe's order (see above), the indexes that determine every edges are the following 12:
				*/
				std::size_t vertexes_index[12][2] = {
					{0,1}, {1, 3}, {3, 2}, {2, 0},	// front face's ages
					{4,5}, {5,7}, {7,6}, {6,4},		// behind face's edges
					{1,5}, {0,4}, {3,7}, {2,6}		// horizontal edges
				};
				/* With these vertex index we can compute every edges as edge(end - start). */
				for (std::size_t i = 0u; i < m_edges.size(); ++i) {
					m_edges[i] = edge(m_vertexes[vertexes_index[i][0]], m_vertexes[vertexes_index[i][1]]);
				}
				/*
				* Compute faces
				*/
				for (std::size_t i = 0u; i < m_faces.size(); ++i) {
					m_faces[i] = face(m_center);
				}
			}
			else {
				assert(0); // unsupported dimension
			}
		};
	private:
		/*
		* Quantities necessaries for creation
		*/
		point m_center{};
		vector m_half_sizes{};
		/*
		* Derived quantities
		*/
		vertexes m_vertexes;
		faces m_faces;
		edges m_edges;
		/* TODO: Leave only one rappresetation:
		* quaternions less space
		* matrix less computations -> more efficiency at real time.
		* Orientation quantities
		*/
		quaternion m_direction{};
		tsg::matrix<scalar, Dim, Dim> m_base{ tsg::matrix<scalar, Dim, Dim>::TYPE::IDENTITY };
	};

	using box2D = box<2>;
	using box3D = box<3>;

	template<std::size_t Dim> requires GeometricDimension<Dim>
	class sphere : public bounding_volume {
		using point = tsg::vector<scalar, Dim>;
		using vector = tsg::vector<scalar, Dim>;
	public:
		sphere(const point c = point(), const scalar r = 0.0f) :
			bounding_volume(type::sphere), m_center(c), m_radius(r) {}
	public:
		inline point get_center() const { return m_center; }
		inline scalar get_radius() const { return m_radius; }
	public:
		void translate(const vector& t) override  { 
			m_center.translate(t); 
		};
		void rotate(const scalar angle) override { 
			/* nothing to do */ 
			return;
		};
		inline scalar get_min(const std::size_t axes) const override {
			assert(axes < Dim);
			return m_center[axes] - m_radius;
		};
		inline scalar get_max(const std::size_t axes) const override {
			assert(axes < Dim);
			return m_center[axes] + m_radius;
		};
		inline bool contains(const point& p) const override {
			return (p - m_center).get_norm() <= m_radius + scalar_zero;
		};
	private:
		point m_center;
		scalar m_radius;
	};
	/* Polygon class still TODO */
	template<std::size_t Dim> requires GeometricDimension<Dim>
	class polygon : public bounding_volume {
		using point = tsg::vector<scalar, Dim>;
		using vector = tsg::vector<scalar, Dim>;
	public:
		polygon() : bounding_volume(type::polygon) { static_assert(0); }
		~polygon() = default;
	};

	/* bounding volume factory method */
	template<std::size_t Dim>
	bounding_volume* bounding_volume::create(const bounding_volume::type t) {
		bounding_volume* ret = nullptr;
		switch (t) {
		case type::aabb:
			ret = new box<Dim>(t);
			break;
		case type::obb:
			/* TODO */
			assert(0);
			break;
		case type::sphere:
			/* TODO */
			assert(0);
			break;
		case type::polygon:
			/* TODO */
			assert(0);
			break;
		default:
			break;
		}
		return ret;
	}
	/* distance functions  point - point */
	template<std::size_t Dim> requires Dimension2D<Dim> || Dimension3D<Dim>
	inline scalar distance(const tsg::vector<scalar, Dim>& p, const tsg::vector<scalar, Dim>& q) {
		return (p - q).get_norm();
	}
	/* distance functions  point - plane */
	template<std::size_t Dim> requires Dimension3D<Dim>
	inline scalar distance(const tsg::vector<scalar, Dim>& p, const finite_plane<Dim>& f) {
		return  vector3D::dot((p - f.get_center()), f.get_normal());
	};
	/* distance functions  point - segment */
	template<std::size_t Dim> requires Dimension2D<Dim> || Dimension3D<Dim>
	inline scalar distance(const tsg::vector<scalar, Dim>& p, segment<Dim>& s) {
		/* TODO */
		assert(0);
		return scalar(0);
	}
	/* compute normal of a 2D segment */
	inline vector2D compute_normal(const segment<2>& s) {
		vector2D v = s.get_direction();
		if (v.get_norm() < scalar_zero) {
			assert(0); // zero length segment
		}
		return vector2D({ -v.get<axes::y>(), v.get<axes::x>() }).get_normalized();
	}

	class inertia_tensor_factory {
	public:
		enum class TYPE {
			CUBE
		};
		static matrix3D get_inverse(const TYPE t);
	};
}
