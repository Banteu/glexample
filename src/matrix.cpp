#include <matrix.h>
#include <cstring>

Mt44 Mt44::zero()
{
    Mt44 mtr;
    memset(mtr.v, 0, sizeof(float) * 16);
    return mtr;
}
Mt44 Mt44::identity()
{
    Mt44 mtr;
    memset(mtr.v, 0, sizeof(float) * 16);
    mtr[0] = mtr[5] = mtr[10] = mtr[15] = 1;
    return mtr;
}

Mt44 Mt44::operator*(const Mt44 &in) const
{
    Mt44 n;
    for (uint32_t i = 0; i < 4; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            n[i * 4 + j] = 0;
            for (uint32_t k = 0; k < 4; ++k)
            {
                n[i * 4 + j] += v[i * 4 + k] * in.v[k * 4 + j];
            }
        }
    }
    return n;
}
Mt44 &Mt44::operator*=(const float in)
{
    for (uint32_t i = 0; i < 16; ++i)
    {
        v[i] *= in;
    }
    return *this;
}

float Mt44::getDet()
{
    return 0.0f;
}

Mt44 Mt44::getInversed() const
{
    Mt44 ret_val;

    ret_val[0] = (+v[7] * v[9] * v[14] + v[6] * v[11] * v[13] + v[5] * v[10] * v[15] - v[7] * v[10] * v[13] - v[6] * v[9] * v[15] - v[5] * v[11] * v[14]);

    ret_val[4] = -(+v[7] * v[8] * v[14] + v[6] * v[11] * v[12] + v[4] * v[10] * v[15] - v[7] * v[10] * v[12] - v[6] * v[8] * v[15] - v[4] * v[11] * v[14]);

    ret_val[8] = (+v[7] * v[8] * v[13] + v[5] * v[11] * v[12] + v[4] * v[9] * v[15] - v[7] * v[9] * v[12] - v[5] * v[8] * v[15] - v[4] * v[11] * v[13]);

    ret_val[12] = -(+v[6] * v[8] * v[13] + v[5] * v[10] * v[12] + v[4] * v[9] * v[14] - v[6] * v[9] * v[12] - v[5] * v[8] * v[14] - v[4] * v[10] * v[13]);

    ret_val[1] = -(+v[3] * v[9] * v[14] + v[2] * v[11] * v[13] + v[1] * v[10] * v[15] - v[3] * v[10] * v[13] - v[2] * v[9] * v[15] - v[1] * v[11] * v[14]);

    ret_val[5] = (+v[3] * v[8] * v[14] + v[2] * v[11] * v[12] + v[0] * v[10] * v[15] - v[3] * v[10] * v[12] - v[2] * v[8] * v[15] - v[0] * v[11] * v[14]);

    ret_val[9] = -(+v[3] * v[8] * v[13] + v[1] * v[11] * v[12] + v[0] * v[9] * v[15] - v[3] * v[9] * v[12] - v[1] * v[8] * v[15] - v[0] * v[11] * v[13]);

    ret_val[13] = (+v[2] * v[8] * v[13] + v[1] * v[10] * v[12] + v[0] * v[9] * v[14] - v[2] * v[9] * v[12] - v[1] * v[8] * v[14] - v[0] * v[10] * v[13]);

    ret_val[2] = (+v[3] * v[5] * v[14] + v[2] * v[7] * v[13] + v[1] * v[6] * v[15] - v[3] * v[6] * v[13] - v[2] * v[5] * v[15] - v[1] * v[7] * v[14]);

    ret_val[6] = -(+v[3] * v[4] * v[14] + v[2] * v[7] * v[12] + v[0] * v[6] * v[15] - v[3] * v[6] * v[12] - v[2] * v[4] * v[15] - v[0] * v[7] * v[14]);

    ret_val[10] = (+v[3] * v[4] * v[13] + v[1] * v[7] * v[12] + v[0] * v[5] * v[15] - v[3] * v[5] * v[12] - v[1] * v[4] * v[15] - v[0] * v[7] * v[13]);

    ret_val[14] = -(+v[2] * v[4] * v[13] + v[1] * v[6] * v[12] + v[0] * v[5] * v[14] - v[2] * v[5] * v[12] - v[1] * v[4] * v[14] - v[0] * v[6] * v[13]);

    ret_val[3] = -(+v[3] * v[5] * v[10] + v[2] * v[7] * v[9] + v[1] * v[6] * v[11] - v[3] * v[6] * v[9] - v[2] * v[5] * v[11] - v[1] * v[7] * v[10]);

    ret_val[7] = (+v[3] * v[4] * v[10] + v[2] * v[7] * v[8] + v[0] * v[6] * v[11] - v[3] * v[6] * v[8] - v[2] * v[4] * v[11] - v[0] * v[7] * v[10]);

    ret_val[11] = -(+v[3] * v[4] * v[9] + v[1] * v[7] * v[8] + v[0] * v[5] * v[11] - v[3] * v[5] * v[8] - v[1] * v[4] * v[11] - v[0] * v[7] * v[9]);

    ret_val[15] = (+v[2] * v[4] * v[9] + v[1] * v[6] * v[8] + v[0] * v[5] * v[10] - v[2] * v[5] * v[8] - v[1] * v[4] * v[10] - v[0] * v[6] * v[9]);

    float det = ret_val[0] * v[0] + ret_val[4] * v[1] + ret_val[8] * v[2] + ret_val[12] * v[3];

    ret_val *= (1.0f / det);

    return ret_val;
}

Mt44 Mt44::getTransposed() const
{
    Mt44 nm;
    for (uint32_t i = 0; i < 4; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            nm.v[i * 4 + j] = v[j * 4 + i];
        }
    }
    return nm;
}

Mt44 getOrhographicProjection(float left, float right, float bottom, float top, float near, float far)
{
    Mt44 mtr = Mt44::zero();
    mtr[0] = 2.0f / (right - left);
    mtr[5] = 2.0f / (top - bottom);
    mtr[10] = -2.0f / (far - near);
    mtr[15] = 1;

    mtr[3] = -(right + left) / (right - left);
    mtr[7] = -(top + bottom) / (top - bottom);
    mtr[11] = -(far + near) / (far - near);

    return mtr;
}

Mt44 getViewMatrix(const Vec3 &viewerCenter, const Vec3 &aim)
{
    auto front = (aim - viewerCenter);
    front.normalize();
    auto right = front.cross(Vec3(0, 0, 1));
    right.normalize();
    auto up = right.cross(front);

    Mt44 camTransform{};
    camTransform.setCol(0, right);
    camTransform.setCol(1, up);
    camTransform.setCol(2, front * (-1.0f));
    camTransform.setCol(3, Vec4(viewerCenter, 1));

    return getOrhographicProjection(-1, 1, -1, 1, 0.5, 100) * camTransform.getInversed();
}