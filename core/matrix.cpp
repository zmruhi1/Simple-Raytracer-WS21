#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
    Float4 rows[] = { r1, r2, r3, r4 };
    for (int i = 0; i < 4; i++) {
        mtx[i] = rows[i];
    }
}

Float4& Matrix::operator[](int idx) {
    return mtx[idx];
}

Float4 Matrix::operator[](int idx) const {
    return Float4(mtx[idx][0], mtx[idx][1], mtx[idx][2], mtx[idx][3]);
}

Matrix Matrix::operator+(const Matrix& b) const {
    Matrix newMtx;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            newMtx[i][j] = mtx[i][j] + b.mtx[i][j];
        }
    }

    return newMtx;
}

Matrix Matrix::operator-(const Matrix& b) const {
    Matrix newMtx;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            newMtx[i][j] = mtx[i][j] - b.mtx[i][j];
        }
    }

    return newMtx;
}

Matrix Matrix::transpose() const {
    Matrix newMtx;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            newMtx[j][i] = mtx[i][j];
        }
    }
    return newMtx;
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (mtx[i][j] != b.mtx[i][j])
                return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix& b) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (mtx[i][j] != b.mtx[i][j])
                return true;
        }
    }

    return false;
}

Matrix product(const Matrix& a, const Matrix& b) {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += (a.mtx[i][k] * b.mtx[k][j]);
            }
            res.mtx[i][j] = sum;
        }
    }

    return res;
}

Matrix operator*(const Matrix& a, float scalar) {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.mtx[i][j] = scalar * a.mtx[i][j];
        }
    }
    return res;
}

Matrix operator*(float scalar, const Matrix& a) {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.mtx[i][j] = scalar * a.mtx[i][j];
        }
    }
    return res;
}

Float4 Matrix::operator*(const Float4& b) const {
    Float4 res;
    res.x = mtx[0][0] * b.x + mtx[0][1] * b.y + mtx[0][2] * b.z + mtx[0][3] * b.w;
    res.y = mtx[1][0] * b.x + mtx[1][1] * b.y + mtx[1][2] * b.z + mtx[1][3] * b.w;
    res.z = mtx[2][0] * b.x + mtx[2][1] * b.y + mtx[2][2] * b.z + mtx[2][3] * b.w;
    res.w = mtx[3][0] * b.x + mtx[3][1] * b.y + mtx[3][2] * b.z + mtx[3][3] * b.w;

    return res;
}

Vector Matrix::operator*(const Vector& b) const {
    float x = mtx[0][0] * b.x + mtx[0][1] * b.y + mtx[0][2] * b.z;
    float y = mtx[1][0] * b.x + mtx[1][1] * b.y + mtx[1][2] * b.z;
    float z = mtx[2][0] * b.x + mtx[2][1] * b.y + mtx[2][2] * b.z;
    float w = mtx[3][0] * b.x + mtx[3][1] * b.y + mtx[3][2] * b.z;

    Float4 res(x, y, z, w);

    return Vector(res);
}

Point Matrix::operator*(const Point& b) const {
    float x = mtx[0][0] * b.x + mtx[0][1] * b.y + mtx[0][2] * b.z + mtx[0][3];
    float y = mtx[1][0] * b.x + mtx[1][1] * b.y + mtx[1][2] * b.z + mtx[1][3];
    float z = mtx[2][0] * b.x + mtx[2][1] * b.y + mtx[2][2] * b.z + mtx[2][3];
    float w = mtx[3][0] * b.x + mtx[3][1] * b.y + mtx[3][2] * b.z + mtx[3][3];
    
    Float4 res(x, y, z, w);
    
    return Point(res);
}

void findSubMatrix(const Float4 m[], Float4 subMtx[], int rRow, int rCol, int N) {
    int subI = 0, subJ = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != rRow && j != rCol) {
                subMtx[subI][subJ] = m[i][j];
                subJ++;
                if (subJ == N - 1) {
                    subJ = 0;
                    subI++;
                }
            }
        }
    }
}

float findDeterminant(const Float4 m[], const int N) {    
    if (N == 2) {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }

    float determinant = 0.f;
    Float4 subMtx[4];
    int sign = 1;
    for (int i = 0; i < N; i++) {
        findSubMatrix(m, subMtx, 0, i, N);
        determinant += sign * m[0][i] * findDeterminant(subMtx, N - 1);
        sign = -sign;
    }
    return determinant;
}

float Matrix::det() const {
    return findDeterminant(mtx, 4);
}

Matrix Matrix::zero() {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.mtx[i][j] = 0;
        }
    }

    return res;
}

Matrix Matrix::identity() {
    Matrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(i == j) res.mtx[i][j] = 1;
            else res.mtx[i][j] = 0;
        }
    }

    return res;
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    Float4 f1(e1);
    Float4 f2(e2);
    Float4 f3(e3);
    Float4 f4(0, 0, 0, 1);
    return Matrix(f1, f2, f3, f4);
}

}