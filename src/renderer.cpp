#include <renderer.h>
#include <string>
#include <GL/glew.h>

std::string readAllFile(const std::string& path)
{
	FILE* in = fopen(path.c_str(), "r");
    if (in == nullptr)
    {
        return "";
    }
	fseek(in, 0, SEEK_END);
	uint32_t size = ftell(in);
	fseek(in, 0, SEEK_SET);
	std::string ret_val;
	ret_val.resize(size);

	fread(&ret_val[0], sizeof(unsigned char), size, in);
	fclose(in);
	return ret_val;
}


class Material
{
    public:
        Material(): mVertexShaderHandle(0), mFrShaderHandle(0), mProgramHandle(0)
        {
        }

        ~Material()
        {
            if (mVertexShaderHandle)
                glDeleteShader(mVertexShaderHandle);
            if (mFrShaderHandle)
                glDeleteShader(mFrShaderHandle);
            if (mProgramHandle)
                glDeleteProgram(mProgramHandle);
        }

        bool init()
        {
            const char* vertexShaderPath = "src/shader/SolidShader.vs";
            const char* frShaderPath = "src/shader/SolidShader.fs";
            const std::string vsSrc = readAllFile(vertexShaderPath);
            const std::string fsSrc = readAllFile(frShaderPath);
            if (vsSrc.empty() || fsSrc.empty())
            {
                LOG_ERROR("Failed to load shaders.");
                return false;
            }
            mVertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
            mFrShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
            const char* shaderSrcPtr[] = {vsSrc.c_str()};
            int32_t shaderSrcLength[] = {static_cast<int32_t>(vsSrc.length())};
            glShaderSource(mVertexShaderHandle, 1, shaderSrcPtr, shaderSrcLength);
            glCompileShader(mVertexShaderHandle);

            shaderSrcPtr[0] = fsSrc.c_str();
            shaderSrcLength[0] = static_cast<int32_t>(fsSrc.length());
            glShaderSource(mFrShaderHandle, 1, shaderSrcPtr, shaderSrcLength);
            glCompileShader(mFrShaderHandle);
            mProgramHandle = glCreateProgram();
            glAttachShader(mProgramHandle, mVertexShaderHandle);
            glAttachShader(mProgramHandle, mFrShaderHandle);
            glLinkProgram(mProgramHandle);
            return true;
        }

        void bind()
        {
            glUseProgram(mProgramHandle);
        }

    private:
        uint64_t mVertexShaderHandle;
        uint64_t mFrShaderHandle;
        uint64_t mProgramHandle;
};

Geometry::~Geometry()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(3, mBuffers);
}

void Geometry::initBuffers()
{
    glCreateBuffers(3, mBuffers);
    glCreateVertexArrays(1, &mVao);
    glNamedBufferData(mBuffers[0], mPositions.size() * sizeof(Vec3), &mPositions[0], GL_DYNAMIC_DRAW);
    glNamedBufferData(mBuffers[1], mNormals.size() * sizeof(Vec3), &mNormals[0], GL_DYNAMIC_DRAW);
    glNamedBufferData(mBuffers[2], mIndices.size() * sizeof(uint32_t), &mIndices[0], GL_DYNAMIC_DRAW);
    for (uint32_t loc = 0; loc < 2; ++loc)
    {
        glVertexArrayAttribFormat(mVao, loc, 3, GL_FLOAT, 0, 0);
        glVertexArrayAttribBinding(mVao, loc, loc);
        glEnableVertexArrayAttrib(mVao, loc);
        glVertexArrayVertexBuffer(mVao, loc, mBuffers[loc], 0, 12);
    }
    glVertexArrayElementBuffer(mVao, mBuffers[2]);
}


class Triangle : public Geometry
{
    public:
    Triangle()
    {

    }

    void draw() override
    {

    }
};

class SineWave : public Geometry
{
    public:
    SineWave(uint64_t resolution) : mResolution(resolution)
    {
        // Make grid
        mPositions.reserve(resolution * resolution);
        mNormals.reserve(resolution * resolution);
        const float scale = 1.0f / resolution;

        for (uint64_t i = 0; i < resolution; ++i)
            for (uint64_t j = 0; j < resolution; ++j)
            {
                mPositions.push_back(Vec3{i * scale, j * scale, 0.0f});
                mNormals.push_back(Vec3{0, 0, 0});
            }
        
        for (uint64_t i = 0; i + 1 < resolution; ++i)
            for (uint64_t j = 0; j + 1 < resolution; ++j)
            {
                // Triangle 1
                mIndices.push_back(i);
                mIndices.push_back(i + 1);
                mIndices.push_back(i + 1 + resolution);

                // Triangle 2
                mIndices.push_back(i);
                mIndices.push_back(i + 1 + resolution);
                mIndices.push_back(i + resolution);
            }
    }
    
    Vec3 getTriangleNormal(const Vec3& a, const Vec3& b, const Vec3& c)
    {
        return (b - a).cross(c - a);
    }

    void update()
    {
        const float scale = 1.0f / mResolution;
        mState += 0.01f;
        for (auto& p : mPositions)
            p.z = sin(mState + p.y * scale);
        
    
        // Recompute normals
        mNormals.assign(mNormals.size(), Vec3{0, 0, 0});
        for (uint64_t triangle = 0; triangle < mIndices.size() / 3; ++triangle)
        {
            uint32_t i1 = mIndices[triangle];
            uint32_t i2 = mIndices[triangle + 1];
            uint32_t i3 = mIndices[triangle + 2];

            const Vec3 normal = getTriangleNormal(mPositions[i1], mPositions[i2], mPositions[i3]);
            mNormals[i1] += normal;
            mNormals[i2] += normal;
            mNormals[i3] += normal;
        }

        for (auto& n : mNormals)
            n.normalize();
    }

    void draw() override
    {
        // Do something each frame
        update();
        glBindVertexArray(mVao);
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    }

    private:
        uint64_t mResolution{};
        float mState{0};   
};

Renderer::~Renderer()
{}


bool Renderer::init()
{
    glClearColor(0, 0, 0, 0);
    mMat = std::make_unique<Material>();
    if (!mMat->init())
    {
        LOG_ERROR("Failed create material");
        return false;
    }
    mObjects.push_back(std::make_unique<SineWave>(32));
    mObjects.push_back(std::make_unique<Triangle>());
    return true;
}

void Renderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (const auto& o : mObjects)
    {
        o->draw();
    }
}

void Renderer::resize(uint32_t width, uint32_t height)
{
    glViewport(0, 0, width, height);
}