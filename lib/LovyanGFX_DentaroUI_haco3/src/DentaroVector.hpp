#pragma once //インクルードガード

template <typename T>
class VectorBase {
protected:
    T x;
    T y;
    T z;

public:
    VectorBase(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

    void set(T newX, T newY, T newZ) {
        x = newX;
        y = newY;
        z = newZ;
    }

    VectorBase<T> copy() const {
        return VectorBase<T>(x, y, z);
    }

    VectorBase<T> diff(const VectorBase<T> &other) const {
        return VectorBase<T>(x - other.x, y - other.y, z - other.z);
    }

    VectorBase<T> scale(T scaleFactor) const {
        return VectorBase<T>(x * scaleFactor, y * scaleFactor, z * scaleFactor);
    }

    T getX() const {
        return x;
    }

    T getY() const {
        return y;
    }

    T getZ() const {
        return z;
    }
};

// template <typename T>
// class Vector3 : public VectorBase<T> {
// public:
//     using VectorBase<T>::x;
//     using VectorBase<T>::y;
//     using VectorBase<T>::z;

//     Vector3(T x = 0, T y = 0, T z = 0) : VectorBase<T>(x, y, z) {}

//     void add(const Vector3<T> &vec) {
//         this->x += vec.x;
//         this->y += vec.y;
//         this->z += vec.z;
//     }

// };
template <typename T>
class Vector3 : public VectorBase<T> {
public:
    using VectorBase<T>::x;
    using VectorBase<T>::y;
    using VectorBase<T>::z;

    Vector3(T x = 0, T y = 0, T z = 0) : VectorBase<T>(x, y, z) {}

    void add(const Vector3<T> &vec) {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;
    }

    Vector3<T> scale(T scaleFactor) const {
        return Vector3<T>(this->x * scaleFactor, this->y * scaleFactor, this->z * scaleFactor);
    }

    Vector3<T> diff(const Vector3<T> &other) const {
        return Vector3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
    }
};



template <typename T>
class Vector2 : public VectorBase<T> {
public:
    Vector2(T x = 0, T y = 0) : VectorBase<T>(x, y, 0) {}

    void set(T newX, T newY) {
        VectorBase<T>::set(newX, newY, 0);
    }

    Vector2<T> copy() const {
        return Vector2<T>(this->x, this->y);
    }
};

