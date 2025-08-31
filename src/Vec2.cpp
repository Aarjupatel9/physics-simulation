#include "Vec2.h"
#include <cmath>

Vec2::Vec2() : x(0.0f), y(0.0f) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

void Vec2::Add(const Vec2& v) {
    x += v.x;
    y += v.y;
}

void Vec2::Sub(const Vec2& v) {
    x -= v.x;
    y -= v.y;
}

void Vec2::Scale(float s) {
    x *= s;
    y *= s;
}

Vec2 Vec2::Rotate(float angle) const {
    float newX = x * cos(angle) - y * sin(angle);
    float newY = x * sin(angle) + y * cos(angle);
    return Vec2(newX, newY);
}

Vec2 Vec2::Add(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.x + v2.x, v1.y + v2.y);
}

Vec2 Vec2::Sub(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 Vec2::Scale(const Vec2& v, float s) {
    return Vec2(v.x * s, v.y * s);
}

float Vec2::Magnitude() const {
    return sqrt(x * x + y * y);
}

float Vec2::MagnitudeSquared() const {
    return x * x + y * y;
}

Vec2& Vec2::Normalize() {
    float mag = Magnitude();
    if (mag != 0) {
        x /= mag;
        y /= mag;
    }
    return *this;
}

Vec2 Vec2::UnitVector() const {
    Vec2 result = *this;
    return result.Normalize();
}

float Vec2::Dot(const Vec2& v) const {
    return x * v.x + y * v.y;
}

float Vec2::Cross(const Vec2& v) const {
    return x * v.y - y * v.x;
}

Vec2& Vec2::operator=(const Vec2& v) {
    x = v.x;
    y = v.y;
    return *this;
}

bool Vec2::operator==(const Vec2& v) const {
    return x == v.x && y == v.y;
}

bool Vec2::operator!=(const Vec2& v) const {
    return !(*this == v);
}

Vec2 Vec2::operator+(const Vec2& v) const {
    return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator-(const Vec2& v) const {
    return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator*(float s) const {
    return Vec2(x * s, y * s);
}

Vec2 Vec2::operator/(float s) const {
    return Vec2(x / s, y / s);
}

Vec2 Vec2::operator-() {
    return Vec2(-x, -y);
}

Vec2& Vec2::operator+=(const Vec2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vec2& Vec2::operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
}

Vec2& Vec2::operator/=(float s) {
    x /= s;
    y /= s;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}
