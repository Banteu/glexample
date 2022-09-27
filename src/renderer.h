#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#define LOG_ERROR(msg) printf("ERROR: " msg "\n");

struct Vec3
{
    float x,y,z;
}

class Material;

class Geometry
{
    public:
        virtual void draw() = 0;
        virtual ~Geometry();
    private:
        void initBuffers();
        std::vector<Vec3> mPos{};
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