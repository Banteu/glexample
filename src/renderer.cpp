#include <renderer.h>
#include <string>
#include <GL/glew.h>

const char* vertexShaderSrc = "#version 450 core\n \
layout(location = 0) in vec3 position; \
layout(location = 1) in vec3 normalIn; \
layout(location = 12) uniform mat4 projectionMatrix; \
layout(location = 16) uniform mat4 modelMatrix; \
out vec3 normal; \
out vec3 varPosition; \
                        \
void main()             \
{                       \
	normal = (modelMatrix * vec4(normalIn, 0)).xyz; \
	varPosition = position; \
	gl_Position =  projectionMatrix * modelMatrix * vec4(position.x, position.y, position.z, 1.0); \
}";

const char* fragmentShaderSrc = "#version 450 core\n \
const vec3 sunPosition = vec3(100, 100, 100);   \
const float ambientLight = 0.2;                 \
out vec4 diffuseColor;                          \
layout(location = 25) uniform vec3 viewerPosition;  \
in vec3 varPosition;                                \
in vec3 normal;                                     \
\
void main() \
{            \
	vec3 viewerDir = viewerPosition - varPosition;  \
	vec3 dirToLight = normalize(sunPosition - varPosition); \
	diffuseColor = vec4(0.8, 0.8, 0.8, 1) * (max(0.0f, dot(normal, dirToLight) * dot(normal, normalize(viewerDir))) + ambientLight); \
	diffuseColor.w = 1; \
}";

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
            const std::string vsSrc = vertexShaderSrc;
            const std::string fsSrc = fragmentShaderSrc;
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
        uint32_t mVertexShaderHandle;
        uint32_t mFrShaderHandle;
        uint32_t mProgramHandle;
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
    printf("Vao %d", mVao);
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

Vec3 getTriangleNormal(const Vec3 &a, const Vec3 &b, const Vec3 &c)
{
    return (b - a).cross(c - a);
}

class Triangle : public Geometry
{
    public:
    Triangle()
    {
        mTransform = Mt44::identity();
        mTransform.setTranslation(Vec3(-0.5f, 0.f, 0.f));
        mPositions = {Vec3(0.f, 0.f, 0.f), Vec3(0.2f, 0.6f, 0.f), Vec3(0.0f, 0.3f, 1.f)};
        mIndices = {0, 1, 2};
        const auto normal = getTriangleNormal(mPositions[0], mPositions[1], mPositions[2]);
        mNormals.assign(3, normal);
        initBuffers();
    }

    void draw() override
    {
        glBindVertexArray(mVao);
        glUniformMatrix4fv(16, 1, GL_TRUE, &mTransform.v[0]);
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(mIndices.size()), GL_UNSIGNED_INT, 0);
    }
};

class SineWave : public Geometry
{
    public:
    SineWave(uint32_t resolution) : mResolution(resolution)
    {
        mTransform = Mt44::identity();
        // Make grid
        mPositions.reserve(resolution * resolution);
        mNormals.reserve(resolution * resolution);
        const float scale = 1.0f / resolution;

        for (uint32_t i = 0; i < resolution; ++i)
            for (uint32_t j = 0; j < resolution; ++j)
            {
                mPositions.push_back(Vec3{i * scale, j * scale, 0.0f});
                mNormals.push_back(Vec3{0, 0, 0});
            }
        
        for (uint32_t i = 0; i + 1 < resolution; ++i)
            for (uint32_t j = 0; j + 1 < resolution; ++j)
            {
                uint32_t offset = i * resolution + j;
                // Triangle 1
                mIndices.push_back(offset);
                mIndices.push_back(offset + 1);
                mIndices.push_back(offset + 1 + resolution);

                // Triangle 2
                mIndices.push_back(offset);
                mIndices.push_back(offset + 1 + resolution);
                mIndices.push_back(offset + resolution);
            }
            initBuffers();
    }
    


    void update()
    {
        const float scale = 128.0f / mResolution;
        mState += 0.01f;
        for (auto& p : mPositions)
            p.z = sin(mState + p.y * scale) * 0.5f;
        
    
        // Recompute normals
        mNormals.assign(mNormals.size(), Vec3{0, 0, 0});
        for (uint64_t triangle = 0; triangle < mIndices.size(); triangle += 3)
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

        glNamedBufferData(mBuffers[0], mPositions.size() * sizeof(Vec3), &mPositions[0], GL_DYNAMIC_DRAW);
        glNamedBufferData(mBuffers[1], mNormals.size() * sizeof(Vec3), &mNormals[0], GL_DYNAMIC_DRAW);
    }

    void draw() override
    {
        // Do something each frame
        update();
        glBindVertexArray(mVao);
        glUniformMatrix4fv(16, 1, GL_TRUE, &mTransform.v[0]);
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(mIndices.size()), GL_UNSIGNED_INT, 0);
    }

    private:
        uint32_t mResolution{};
        float mState{0};
};

Renderer::Renderer() {}

Renderer::~Renderer()
{}


bool Renderer::init()
{
    if (glewInit() != GLEW_OK)
    {
        LOG_ERROR("GLEW init failed...");
        return false;
    }
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    mMat = std::make_unique<Material>();
    if (!mMat->init())
    {
        LOG_ERROR("Failed create material");
        return false;
    }
    mObjects.push_back(std::make_unique<SineWave>(32));
    mObjects.push_back(std::make_unique<Triangle>());
    mMat->bind();
    
    Vec3 viewerCenter(50, 50, 50);
    Vec3 aim(0, 0, 0);

    Mt44 viewMatrix = getViewMatrix(viewerCenter, aim);
    glUniformMatrix4fv(12, 1, GL_TRUE, &viewMatrix.v[0]);
    glUniform3f(25, viewerCenter.x, viewerCenter.y, viewerCenter.z);

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