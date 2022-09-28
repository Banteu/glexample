#pragma once
#include <cmath>
#include <cstdint>


struct Vec3
{
    float x, y, z;
    Vec3(){}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3 &in) const
    {
        return Vec3(x + in.x, y + in.y, z + in.z);
    }
    Vec3 operator+(const float in) const
    {
        return Vec3(x + in, y + in, z + in);
    }
    Vec3 operator-(const float in) const
    {
        return Vec3(x - in, y - in, z - in);
    }
    Vec3 operator-(const Vec3 &in) const
    {
        return Vec3(x - in.x, y - in.y, z - in.z);
    }
    Vec3 cross(const Vec3 &in) const
    {
        return Vec3(y * in.z - in.y * z, z * in.x - x * in.z, x * in.y - in.x * y);
    }
    Vec3 operator*(const float in) const
    {
        return Vec3(x * in, y * in, z * in);
    }
    Vec3 &operator+=(const Vec3 &in)
    {
        x += in.x;
        y += in.y;
        z += in.z;
        return *this;
    }
    float norm() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    void normalize()
    {
        float mlt = 1.0f / norm();
        x *= mlt;
        y *= mlt;
        z *= mlt;
    }
};

struct Vec4
{
    float x, y, z, w;
    Vec4() = default;
    explicit Vec4(float v) : x(v), y(v), z(v), w(v) {}
    Vec4(float xi, float yi, float zi, float wi) : x(xi), y(yi), z(zi), w(wi) {}
    Vec4(const Vec3& in, float wi = 0): x(in.x), y(in.y), z(in.z), w(wi){}
};


struct Mt44
{
    float v[16];
    Mt44() {}
    static Mt44 zero();
    static Mt44 identity();

    void setCol(uint32_t id, float x, float y, float z, float w)
    {
        v[id] = x;
        v[id + 4] = y;
        v[id + 8] = z;
        v[id + 12] = w;
    }

    Vec4 getCol(uint32_t idx)
    {
        return {v[idx], v[idx + 4], v[idx + 8], v[idx + 12]};
    }

    void setCol(uint32_t id, const Vec4 &in)
    {
        v[id] = in.x;
        v[id + 4] = in.y;
        v[id + 8] = in.z;
        v[id + 12] = in.w;
    }

    void setRow(uint32_t id, const Vec4 &in)
    {
        v[id * 4] = in.x;
        v[id * 4 + 1] = in.y;
        v[id * 4 + 2] = in.z;
        v[id * 4 + 3] = in.w;
    }

    float &operator[](uint32_t i)
    {
        return v[i];
    }

    const float &operator[](uint32_t i) const
    {
        return v[i];
    }

    Vec3 getTranslation() const
    {
        return Vec3(v[3], v[7], v[11]);
    }

    void setTranslation(const Vec3 &translation)
    {
        v[3] = translation.x;
        v[7] = translation.y;
        v[11] = translation.z;
    }

    Mt44 operator*(const Mt44 &in) const;
    Mt44 &operator*=(const float in);

    Mt44 getInversed() const;
};

Mt44 getOrhographicProjection(float left, float right, float bottom, float top, float near, float far);

Mt44 getViewMatrix(const Vec3& viewerCenter, const Vec3& aim);