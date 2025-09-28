#ifndef VEC2_H
#define VEC2_H

#include <iostream>

// Vec2 represents a 2D vector with x and y components.
// It is used for positions, velocities, forces, etc.
struct Vec2 {
    float x, y;

    // Default constructor
    Vec2();
    // Constructor with initial values
    Vec2(float x, float y);

    // Member functions for vector operations
    void Add(const Vec2& v);
    void Sub(const Vec2& v);
    void Scale(float s);
    Vec2 Rotate(float angle) const; // Rotates the vector by an angle in radians

    // Static functions for vector operations that return a new vector
    static Vec2 Add(const Vec2& v1, const Vec2& v2);
    static Vec2 Sub(const Vec2& v1, const Vec2& v2);
    static Vec2 Scale(const Vec2& v, float s);

    // Common vector calculations
    float Magnitude() const;
    float MagnitudeSquared() const;
    Vec2& Normalize();
    Vec2 UnitVector() const;
    float Dot(const Vec2& v) const;
    float Cross(const Vec2& v) const; // 2D cross product returns a scalar (z-component)

    // Operator overloads for convenience
    Vec2& operator = (const Vec2& v);
    bool operator == (const Vec2& v) const;
    bool operator != (const Vec2& v) const;

    Vec2 operator + (const Vec2& v) const;
    Vec2 operator - (const Vec2& v) const;
    Vec2 operator * (float s) const;
    Vec2 operator / (float s) const;
    Vec2 operator - ();

    Vec2& operator += (const Vec2& v);
    Vec2& operator -= (const Vec2& v);
    Vec2& operator *= (float s);
    Vec2& operator /= (float s);

    // Friend function to overload the stream insertion operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};

#endif // VEC2_H
