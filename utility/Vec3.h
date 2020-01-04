#ifndef RAYTRACING_VEC3_H
#define RAYTRACING_VEC3_H
#include <cmath>

// The type used for the 3-dimensional vectors.
// In most cases, this will be either double or float.
using value_type = double;

// Represents a Euclidean vector in 3-dimensional space.
// If no arguments are provided, defaults to (0.0, 0.0, 0.0).
// Assumes vectors take the form of:
//      [x]
//      [y]
//      [z]
struct Vec3 {
public:
    Vec3() : x_{0.0}, y_{0.0}, z_{0.0} {}
    constexpr Vec3(const value_type x, const value_type y, const value_type z)
            : x_{x}, y_{y}, z_{z} {}

    inline constexpr value_type x() const { return this->x_; }
    inline constexpr value_type y() const { return this->y_; }
    inline constexpr value_type z() const { return this->z_; }

    inline constexpr value_type& x() { return this->x_; }
    inline constexpr value_type& y() { return this->y_; }
    inline constexpr value_type& z() { return this->z_; }

    inline constexpr value_type operator[](int i) const {
        switch (i) {
            case 0: return x_;
            case 1: return y_;
            case 2: return z_;
            default: throw std::invalid_argument("Vec3 out of bounds access. For Vec3[i], 0 <= i <= 2");
        }
    }
    inline constexpr value_type& operator[](int i) {
        switch (i) {
            case 0: return x_;
            case 1: return y_;
            case 2: return z_;
            default: throw std::invalid_argument("Vec3 out of bounds access. For Vec3[i], 0 <= i <= 2");
        }
    }

    inline value_type length() const {
        return std::hypot(this->x(), this->y(), this->z());
    }

    inline value_type squared_length() const {
        return x() * x() + y() * y() + z() * z();
    }

private:
    // Represents the x-dimension value of the vector.
    value_type x_;
    // Represents the y-dimension value of the vector.
    value_type y_;
    // Represents the z-dimension value of the vector.
    value_type z_;
};

// A 3-dimensional free vector, which has no initial point. It has two main criteria:
// (1) direction, and (2) magnitude.
// If no arguments are provided, defaults to (0.0, 0.0, 0.0).
struct FreeVec3 : Vec3 {
    using Vec3::Vec3;

    constexpr explicit FreeVec3(const Vec3& vec3) : Vec3::Vec3{vec3} {}

    inline constexpr value_type dot(const Vec3& other) const {
        return this->x() * other.x() + this->y() * other.y() + this->z() * other.z();
    }

    inline constexpr FreeVec3 cross(const Vec3& other) const {
        return FreeVec3{this->y() * other.z() - this->z() * other.y(),
                        this->z() * other.x() - this->x() * other.z(),
                        this->x() * other.y() - this->y() * other.x()};
    }

    inline constexpr FreeVec3& operator+=(const FreeVec3& other) {
        this->x() += other.x();
        this->y() += other.y();
        this->z() += other.z();
        return *this;
    }

    inline constexpr FreeVec3& operator-=(const FreeVec3& other) {
        this->x() -= other.x();
        this->y() -= other.y();
        this->z() -= other.z();
        return *this;
    }

    inline constexpr FreeVec3& operator*=(const value_type scalar) {
        this->x() *= scalar;
        this->y() *= scalar;
        this->z() *= scalar;
        return *this;
    }

    inline constexpr FreeVec3& operator/=(const value_type scalar) {
        this->x() /= scalar;
        this->y() /= scalar;
        this->z() /= scalar;
        return *this;
    }
};

inline constexpr FreeVec3 operator+(const FreeVec3& v) { return v; }

inline constexpr FreeVec3 operator-(const FreeVec3& v) {
    return FreeVec3{-v.x(), -v.y(), -v.z()};
}

inline constexpr FreeVec3 operator+(FreeVec3 v1, const FreeVec3& v2) {
    return v1 += v2;
}

inline constexpr FreeVec3 operator-(FreeVec3 v1, const FreeVec3& v2) {
    return v1 -= v2;
}

inline constexpr FreeVec3 operator*(FreeVec3 v, const value_type scalar) {
    return v *= scalar;
}

inline constexpr FreeVec3 operator/(FreeVec3 v, const value_type scalar) {
    return v /= scalar;
}

// A 3-dimensional bounded vector has a fixed start and end point. It represents a fixed point
// in space, relative to some frame of reference.
// If no arguments are provided, defaults to (0.0, 0.0, 0.0).
struct BoundVec3 : Vec3 {
    using Vec3::Vec3;

    constexpr explicit BoundVec3(const Vec3& vec3) : Vec3::Vec3{vec3} {}

    inline constexpr value_type dot(const Vec3& other) const {
        return this->x() * other.x() + this->y() * other.y() + this->z() * other.z();
    }

    inline constexpr BoundVec3& operator+=(const FreeVec3& other) {
        this->x() += other.x();
        this->y() += other.y();
        this->z() += other.z();
        return *this;
    }

    inline constexpr BoundVec3& operator-=(const FreeVec3& other) {
        return *this += (-other);
    }
};

inline constexpr FreeVec3 operator-(const BoundVec3& v1, const BoundVec3& v2) {
    return FreeVec3{v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

inline constexpr BoundVec3 operator+(BoundVec3 v1, const FreeVec3& v2) {
    return v1 += v2;
}

inline constexpr BoundVec3 operator-(BoundVec3 v1, const FreeVec3& v2) {
    return v1 -= v2;
}

// Represents a 3-dimensional unit vector, an abstraction over free vectors that guarantees
// a length of 1. To prevent its length from changing, UnitVec3 does not allow
// for mutations.
// If no arguments are provided, defaults to (0.0, 0.0, 0.0).
struct UnitVec3 {
    UnitVec3() : inner_{FreeVec3(0.0,0.0,0.0)} {}
    UnitVec3(value_type x, value_type y, value_type z)
            : UnitVec3{FreeVec3{x, y, z}} {}

    explicit UnitVec3(const Vec3& vec3) : UnitVec3{FreeVec3{vec3}} {}

    explicit UnitVec3(const FreeVec3& free_vec3) : inner_{free_vec3 / free_vec3.length()} {}

    inline constexpr value_type x() const { return this->to_free().x(); }
    inline constexpr value_type y() const { return this->to_free().y(); }
    inline constexpr value_type z() const { return this->to_free().z(); }

    inline constexpr const FreeVec3& to_free() const { return inner_; }

private:
    FreeVec3 inner_;
};

inline constexpr FreeVec3 operator*(const UnitVec3& v, const value_type scalar) {
    return v.to_free() * scalar;
}

inline constexpr FreeVec3 operator/(const UnitVec3& v, const value_type scalar) {
    return v.to_free() / scalar;
}

// Represents a 3-dimensional color vector.
// Assumes vectors take the form of:
//      [red]
//      [green]
//      [blue]
// Each color should be within the bounds [0.0, 1.0].
// If no color provided, defaults to (0.0, 0.0, 0.0).
struct Color3  {
public:
    Color3() : r_{0.0}, g_{0.0}, b_{0.0} {}
    constexpr Color3(const value_type r, const value_type g, const value_type b)
            : r_{r}, g_{g}, b_{b} {}

    inline constexpr value_type r() const { return this->r_; }
    inline constexpr value_type g() const { return this->g_; }
    inline constexpr value_type b() const { return this->b_; }

    inline constexpr value_type& r() { return this->r_; }
    inline constexpr value_type& g() { return this->g_; }
    inline constexpr value_type& b() { return this->b_; }

    inline constexpr value_type operator[](int i) const {
        switch (i) {
            case 0: return r_;
            case 1: return g_;
            case 2: return b_;
            default: throw std::invalid_argument("Color3 out of bounds access. For Color3[i], 0 <= i <= 2");
        }
    }
    inline constexpr value_type& operator[](int i) {
        switch (i) {
            case 0: return r_;
            case 1: return g_;
            case 2: return b_;
            default: throw std::invalid_argument("Color3 out of bounds access. For Color3[i], 0 <= i <= 2");
        }
    }

    inline constexpr Color3& operator+=(const Color3& other) {
        this->r() += other.r();
        this->g() += other.g();
        this->b() += other.b();
        return *this;
    }

    inline constexpr Color3& operator-=(const Color3& other) {
        this->r() -= other.r();
        this->g() -= other.g();
        this->b() -= other.b();
        return *this;
    }

    inline constexpr Color3& operator*=(const value_type scalar) {
        this->r() *= scalar;
        this->g() *= scalar;
        this->b() *= scalar;
        return *this;
    }

    inline constexpr Color3& operator/=(const value_type scalar) {
        this->r() /= scalar;
        this->g() /= scalar;
        this->b() /= scalar;
        return *this;
    }

private:
    // Represents the red value.
    value_type r_;
    // Represents the green value.
    value_type g_;
    // Represents the blue value.
    value_type b_;
};

inline constexpr Color3 operator+(Color3 v1, const Color3& v2) {
    return v1 += v2;
}

inline constexpr Color3 operator-(Color3 v1, const Color3& v2) {
    return v1 -= v2;
}

inline constexpr Color3 operator*(Color3 v, const value_type scalar) {
    return v *= scalar;
}

inline Color3 operator*(const Color3& v1, const Color3& v2) {
    return Color3(v1.r() * v2.r(), v1.g() * v2.g(), v1.b() * v2.b());
}

inline constexpr Color3 operator/(Color3 v, const value_type scalar) {
    return v /= scalar;
}

#endif //RAYTRACING_VEC3_H
