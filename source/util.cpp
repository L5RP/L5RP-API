#include "api.h"

using namespace L5RP;

Vector3::Vector3()
    : Vector3::Vector3(0, 0, 0) {}

Vector3::Vector3(rage::vector3 const& v)
    : Vector3::Vector3(v.x, v.y, v.z) {}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3::setX(float x)
{
    this->x = x;
}

void Vector3::setY(float y)
{
    this->y = y;
}

void Vector3::setZ(float z)
{
    this->z = z;
}

float Vector3::getX() const
{
    return this->x;
}

float Vector3::getY() const
{
    return this->y;
}

float Vector3::getZ() const
{
    return this->z;
}

void Vector3::toJSON(nlohmann::json& data) const {
    data["x"] = x;
    data["y"] = y;
    data["z"] = z;
}

float Vector3::calcDistance(Vector3 const& other) const
{
    float x = this->x - other.getX();
    float y = this->y - other.getY();
    float z = this->z - other.getZ();

    return sqrtf(
        x * x + y * y + z * z
    );
}

float Vector3::calcXYDistance(Vector3 const& other) const {
    float x = this->x - other.getX();
    float y = this->y - other.getY();

    return sqrtf(
        x * x + y * y
    );
}

bool Vector3::operator==(Vector3 const& other) const {
    return this->x == other.getX() && this->y == other.getY() && this->z == other.getZ();
}

bool Vector3::operator!=(Vector3 const& other) const {
    return !this->operator==(other);
}

Transform::Transform()
    : Vector3()
{

}

Transform::Transform(float x, float y, float z, float h)
    : Vector3(x, y, z)
{
    this->h = h;
}

Transform::Transform(rage::vector3 const& v, float h)
    : Vector3(v)
{
    this->h = h;
}

Transform::Transform(Vector3 v, float h)
    : Vector3(v)
{
    this->h = h;
}

void Transform::setH(float h)
{
    this->h = h;
}

float Transform::getH() const
{
    return this->h;
}

void Transform::toJSON(nlohmann::json& data) const {
    Vector3::toJSON(data);

    data["h"] = h;
}

Vector4::Vector4()
    : Vector3()
{

}

Vector4::Vector4(float x, float y, float z, float h)
    : Vector3(x, y, z)
{
    this->w = h;
}

Vector4::Vector4(rage::vector3 const& v, float h)
    : Vector3(v)
{
    this->w = h;
}

Vector4::Vector4(Vector3 v, float h)
    : Vector3(v)
{
    this->w = h;
}

void Vector4::setW(float h)
{
    this->w = h;
}

float Vector4::getW() const
{
    return this->w;
}

void Vector4::toJSON(nlohmann::json& data) const {
    Vector3::toJSON(data);

    data["w"] = this->w;
}

Color::Color()
    : Color(0, 0, 0)
{

}

Color::Color(rage::rgb_t const& c)
    : Color(c.rgba[0], c.rgba[1], c.rgba[2]) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void Color::setR(uint8_t r)
{
    this->r = r;
}

void Color::setG(uint8_t g)
{
    this->g = g;
}

void Color::setB(uint8_t b)
{
    this->b = b;
}

uint8_t Color::getR() const
{
    return this->r;
}

uint8_t Color::getG() const
{
    return this->g;
}

uint8_t Color::getB() const
{
    return this->b;
}

void Color::toJSON(nlohmann::json& data) const {
    data["r"] = this->r;
    data["g"] = this->g;
    data["b"] = this->b;
}

ColorAlpha::ColorAlpha()
    : Color()
{
    this->a = 0;
}

ColorAlpha::ColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : Color(r, g, b)
{
    this->a = a;
}

ColorAlpha::ColorAlpha(rage::rgb_t const& c, uint8_t a)
    : ColorAlpha(c.rgba[0], c.rgba[1], c.rgba[2], a)
{}

ColorAlpha::ColorAlpha(rage::rgba_t const& c)
    : ColorAlpha(c.rgba[0], c.rgba[1], c.rgba[2], c.rgba[3])
{}

ColorAlpha::ColorAlpha(Color const& c, uint8_t a)
    : ColorAlpha(c.getR(), c.getG(), c.getG(), a)
{}

void ColorAlpha::setA(uint8_t a)
{
    this->a = a;
}

uint8_t ColorAlpha::getA() const
{
    return this->a;
}

void ColorAlpha::toJSON(nlohmann::json& data) const {
    Color::toJSON(data);

    data["a"] = this->a;
}