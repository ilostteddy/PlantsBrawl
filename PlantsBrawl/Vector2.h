#pragma once

#include <cmath>

class Vector2
{
public:
    // 向量的x、y分量，默认初始化为0
    float x = 0.0f;
    float y = 0.0f;

    // 默认构造函数：创建一个(0, 0)的向量
    Vector2() = default;

    // 析构函数
    ~Vector2() = default;

    // 带参数的构造函数：用给定的x和y创建向量
    Vector2(float x_, float y_) : x(x_), y(y_) {}

    // --------------------------
    // 复合赋值运算符（向量-向量）
    // --------------------------

    // 向量加法赋值：当前向量 += 另一个向量
    Vector2& operator+=(const Vector2& other)
    {
        x = x + other.x;
        y = y + other.y;
        return *this; // 返回自身，支持链式调用
    }

    // 向量减法赋值：当前向量 -= 另一个向量
    Vector2& operator-=(const Vector2& other)
    {
        x = x - other.x;
        y = y - other.y;
        return *this;
    }

    // 向量逐元素乘法赋值：当前向量的每个分量分别乘以另一个向量的对应分量
    Vector2& operator*=(const Vector2& other)
    {
        x = x * other.x;
        y = y * other.y;
        return *this;
    }

    // 向量逐元素除法赋值：当前向量的每个分量分别除以另一个向量的对应分量
    Vector2& operator/=(const Vector2& other)
    {
        x = x / other.x;
        y = y / other.y;
        return *this;
    }

    // --------------------------
    // 复合赋值运算符（向量-标量）
    // --------------------------

    // 向量标量乘法赋值：当前向量整体乘以一个数（缩放）
    Vector2& operator*=(float scalar)
    {
        x = x * scalar;
        y = y * scalar;
        return *this;
    }

    // 向量标量除法赋值：当前向量整体除以一个数
    Vector2& operator/=(float scalar)
    {
        x = x / scalar;
        y = y / scalar;
        return *this;
    }

    // --------------------------
    // 一元运算符
    // --------------------------

    // 负向量：返回一个分量都取反的新向量（不修改原向量）
    Vector2 operator-() const
    {
        float new_x = -x;
        float new_y = -y;
        return Vector2(new_x, new_y);
    }

    // --------------------------
    // 比较运算符
    // --------------------------

    // 判断两个向量是否相等：分量都相等才返回true
    bool operator==(const Vector2& other) const
    {
        bool x_equal = (x == other.x);
        bool y_equal = (y == other.y);
        return x_equal && y_equal;
    }

    // 判断两个向量是否不等：对==取反
    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }

    // --------------------------
    // 常用向量运算
    // --------------------------

    // 点积（点乘）：返回两个向量的点积值
    float dot(const Vector2& other) const
    {
        float x_product = x * other.x;
        float y_product = y * other.y;
        return x_product + y_product;
    }

    // 向量长度的平方：避免开根号，常用于比较长度（性能更高）
    float length_sq() const
    {
        float x_squared = x * x;
        float y_squared = y * y;
        return x_squared + y_squared;
    }

    // 向量的长度（模）：根据勾股定理计算
    float length() const
    {
        float squared_length = length_sq();
        return std::sqrt(squared_length);
    }

    // 归一化（单位化）：返回一个长度为1的新向量（不修改原向量）
    Vector2 normalized() const
    {
        // 先获取当前向量的长度
        float len = length();

        // 特殊情况：如果向量长度为0，直接返回零向量，避免除以0
        if (len == 0.0f)
        {
            return Vector2(0.0f, 0.0f);
        }

        // 用每个分量除以长度，得到单位向量
        float normalized_x = x / len;
        float normalized_y = y / len;
        return Vector2(normalized_x, normalized_y);
    }

    // --------------------------
    // 二元运算符（友元函数）
    // --------------------------

    // 向量加法：返回两个向量相加的结果（不修改原向量）
    friend Vector2 operator+(Vector2 a, const Vector2& b)
    {
        a += b; // 复用已实现的+=运算符
        return a;
    }

    // 向量减法：返回两个向量相减的结果（不修改原向量）
    friend Vector2 operator-(Vector2 a, const Vector2& b)
    {
        a -= b; // 复用已实现的-=运算符
        return a;
    }

    // 向量逐元素乘法：返回两个向量逐元素相乘的结果（不修改原向量）
    friend Vector2 operator*(Vector2 a, const Vector2& b)
    {
        a *= b; // 复用已实现的*=运算符
        return a;
    }

    // 向量逐元素除法：返回两个向量逐元素相除的结果（不修改原向量）
    friend Vector2 operator/(Vector2 a, const Vector2& b)
    {
        a /= b; // 复用已实现的/=运算符
        return a;
    }

    // 向量×标量：返回向量整体缩放后的结果（不修改原向量）
    friend Vector2 operator*(Vector2 v, float scalar)
    {
        v *= scalar; // 复用已实现的*=运算符
        return v;
    }

    // 标量×向量：支持交换律，效果与向量×标量相同（不修改原向量）
    friend Vector2 operator*(float scalar, Vector2 v)
    {
        v *= scalar; // 复用已实现的*=运算符
        return v;
    }

    // 向量÷标量：返回向量整体缩放后的结果（不修改原向量）
    friend Vector2 operator/(Vector2 v, float scalar)
    {
        v /= scalar; // 复用已实现的/=运算符
        return v;
    }

private:
    // 没有私有成员变量
};