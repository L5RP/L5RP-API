#pragma once

namespace L5RP
{

    class Vector3
        : public rage::vector3
    {

    public:

        Vector3();
        Vector3(float x, float y, float z);
        explicit Vector3(rage::vector3 const& v);

        void setX(float x);
        void setY(float y);
        void setZ(float z);

        float getX() const;
        float getY() const;
        float getZ() const;

        virtual void toJSON(nlohmann::json& data) const;

        float calcDistance(Vector3 const& other) const;
        float calcXYDistance(Vector3 const& other) const; //<--- Calculates distance only in XY angles.

        bool operator==(Vector3 const& other) const;
        bool operator!=(Vector3 const& other) const;
    };

    class Transform
        : public Vector3
    {
        float h;

    public:

        Transform();
        Transform(float x, float y, float z, float h);
        explicit Transform(rage::vector3 const& v, float h);
        explicit Transform(Vector3 v, float h);

        void setH(float h);

        float getH() const;

        void toJSON(nlohmann::json& data) const override;
    };

    class Vector4
        : public Vector3
    {
        float w;

    public:

        Vector4();
        Vector4(float x, float y, float z, float w);
        explicit Vector4(rage::vector3 const& v, float h);
        explicit Vector4(Vector3 v, float h);

        void setW(float w);
        float getW() const;

        void toJSON(nlohmann::json& data) const override;
    };
    typedef Vector4 Quaternion;

    class Color
    {

        uint8_t r;
        uint8_t g;
        uint8_t b;

    public:

        Color();
        Color(uint8_t r, uint8_t g, uint8_t b);
        explicit Color(rage::rgb_t const& c);

        void setR(uint8_t r);
        void setG(uint8_t g);
        void setB(uint8_t b);

        uint8_t getR() const;
        uint8_t getG() const;
        uint8_t getB() const;

        virtual void toJSON(nlohmann::json& data) const;

    };

    class ColorAlpha
        : public Color
    {

        uint8_t a;

    public:

        ColorAlpha();
        ColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        explicit ColorAlpha(rage::rgb_t const& c, uint8_t a);
        explicit ColorAlpha(rage::rgba_t const& c);
        explicit ColorAlpha(Color const& c, uint8_t a);

        void setA(uint8_t a);

        uint8_t getA() const;

        void toJSON(nlohmann::json& data) const override;
    };

}