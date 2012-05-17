#ifndef __XLBE_2DVECTOR_H__
#define __XLBE_2DVECTOR_H__

#include <cmath>
#include <algorithm>

//////////////////////////////////////////////////////////////
//namespace xlbe{

//////////////////////////////////////////////////////////////
struct Vector2D
{
    Vector2D()  {x_=0.0f; y_=0.0f; }
    Vector2D(float x, float y): x_(x), y_(y) {}
    Vector2D(const Vector2D &a): x_(a.x_), y_(a.y_) {}

    const Vector2D &operator = (const Vector2D &a)   { x_=a.x_; y_=a.y_; return *this; }
    bool operator == (const Vector2D &a)      { return x_==a.x_&&y_==a.y_; }
    bool operator != (const Vector2D &a)      { return x_!=a.x_|| y_!=a.y_; }
    
    Vector2D operator -() const               { return Vector2D(-x_, -y_);  }
    Vector2D operator + (const Vector2D& a)   { return Vector2D(x_+a.x_, y_+a.y_); }	
    Vector2D operator - (const Vector2D& a)   { return Vector2D(x_-a.x_, y_-a.y_); }
    Vector2D operator * (float a)	            { return Vector2D(x_*a, y_*a); }
    Vector2D operator / (float a)	            { return Vector2D(-x_/a, -y_/a); }
    Vector2D& operator += (const Vector2D& a) { x_ += a.x_; y_ += a.y_; return *this; }	
    Vector2D& operator -= (const Vector2D& a) { x_ -= a.x_; y_ -= a.y_; return *this; }
    Vector2D& operator *= (float a)	          { x_ *= a; y_ *= a; return *this; }
    Vector2D& operator /= (float a)	          { x_ /= a; y_ /= a; return *this; }

    void zero()                               { x_ = 0.0f; y_ = 0.0f; }
    void set(float x, float y)                { x_ = x; y_ = y;  }
    float length()                            { return std::sqrtf(x_*x_ + y_*y_);}

    Vector2D normalize()                      { if (length() == 0) return Vector2D(0,0);
                                                return Vector2D(x_/length(), y_/length());}

    int fast_length()                         { int x = std::abs(x_);
                                                int y = std::abs(y_);
                                                int min = std::min(x, y);
                                                return (x+y-(min>>1)-(min>>2)+(min>>4)); }

    

    float x_, y_;
};

//////////////////////////////////////////////////////////////
inline Vector2D operator + (const Vector2D &v1, const Vector2D &v2)
{
    return Vector2D(v1.x_+v2.x_, v1.y_+v2.y_);
}

//////////////////////////////////////////////////////////////
inline Vector2D operator - (const Vector2D &v1, const Vector2D &v2)
{
    return Vector2D(v1.x_-v2.x_, v1.y_-v2.y_);
}

//////////////////////////////////////////////////////////////
inline Vector2D operator * (float a, const Vector2D &v)
{
    return Vector2D(a*v.x_, a*v.y_);
}

//////////////////////////////////////////////////////////////
//}
#endif 