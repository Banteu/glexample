#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <cmath>

#define LOG_ERROR(msg) printf("ERROR: " msg "\n");

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

class Material;

class Geometry
{
    public:
        virtual void draw() = 0;
        virtual ~Geometry();
    protected:
        void initBuffers();
        std::vector<Vec3> mPositions{};
        std::vector<Vec3> mNormals{};
        std::vector<uint32_t> mIndices{};
        uint32_t mVao{};
        uint32_t mBuffers[3]; 
};

class Renderer
{
    public:
        ~Renderer();
        bool init();
        void resize(uint32_t width, uint32_t height);
        void draw();
    private:
        std::unique_ptr<Material> mMat{}; 
        std::vector<std::unique_ptr<Geometry> > mObjects{};
};


#endif