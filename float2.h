#ifndef __FLOAT2_H_INCLUDED__  
#define __FLOAT2_H_INCLUDED__ 

#define _USE_MATH_DEFINES

class float2 {
public:
    //members
    float x;
    float y;
    
    //constructors
    float2():x(0.0f),y(0.0f){}
    float2(float x, float y):x(x),y(y){}

    // return random points
    static float2 random() {
        return float2( (float) rand()/ RAND_MAX * 2 - 1,
                      (float) rand() / RAND_MAX * 2 - 1);
    }
    
    // operators
    float2 operator-() const {
        return float2(-x,-y);
    }

    float2 operator+(float2 b) const {
        return float2(x + b.x, y + b.y);
    }
    
    float2 operator-(float2 b) const {
        return float2(x - b.x, y - b.x);
    }
    
    float operator*(float2 b) const {
        return (x * b.x) - (y * b.y);
    }
    
    float2 operator*(int b) const {
        return float2((x * b),(y * b));
    }

    float2 operator*(double b) const {
        return float2((x * b),(y * b));
    }

    float2 operator*(float b) const {
        return float2((x * b),(y * b));
    }
    
    float2 operator/(float c) const {
        return float2(x/c, y/c);
    }
    
    void operator+=(float2 b) {
        x += b.x;
        y += b.y;
    }
    
    void operator-=(float2 b) {
        x -= b.x;
        y -= b.y;
    }
    
    void operator /=(float c) {
        x /= c;
        y /= c;
    }
    
    //methods

    //determines the length squared
    float lengthSquared() const {
        return x*x + y*y;
    }
    //returns the length
    float length () const {
        return sqrtf(lengthSquared());
    }

    // to determine the unit vector
    float2 normalize () {
        *this /= length();
        return *this;
    }
    
    // print information about the point
    void print() {
        std::cout << "X: " << x << "\n" << "Y: " << y << "\n";
    }
};

#endif

