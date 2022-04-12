#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>

namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    float newR = min(this->r + c.r, (float)1);
    float newG = min(this->g + c.g, (float)1);
    float newB = min(this->b + c.b, (float)1);

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    float newR = max(this->r - c.r, (float)0);
    float newG = max(this->g - c.g, (float)0);
    float newB = max(this->b - c.b, (float)0);

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    float newR = this->r * c.r;
    float newG = this->g * c.g;
    float newB = this->b * c.b;

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

bool RGBColor::operator == (const RGBColor& c) const {
    return (abs(this->r - c.r) <= std::numeric_limits<float>::epsilon()
        && abs(this->g - c.g) <= std::numeric_limits<float>::epsilon()
        && abs(this->b - c.b) <= std::numeric_limits<float>::epsilon());
}

bool RGBColor::operator != (const RGBColor& b) const {
    return !(abs(this->r - b.r) <= std::numeric_limits<float>::epsilon()
        && abs(this->g - b.g) <= std::numeric_limits<float>::epsilon()
        && abs(this->b - b.b) <= std::numeric_limits<float>::epsilon());
}

RGBColor RGBColor::clamp() const {
    float newR = this->r / (float)255;
    float newG = this->g / (float)255;
    float newB = this->b / (float)255;

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

RGBColor RGBColor::gamma(float gam) const {
    float newR;
    float newG;
    float newB;

    newR = pow((this->r / (float)255) ,(1 / gam)) * (float)255;
    newG = pow((this->g / (float)255), (1 / gam)) * (float)255;
    newB = pow((this->b / (float)255), (1 / gam)) * (float)255;

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;

}

float RGBColor::luminance() const {
    return (0.2126* this->r + 0.7152* this->g + 0.0722* this->b);
}

RGBColor operator * (float scalar, const RGBColor& c) {
    float tempR = scalar * c.r;
    float newR;
    newR = tempR <= 1 ? tempR : 1;
    newR = tempR >= 0 ? tempR : 0;

    float tempG = scalar * c.g;
    float newG;
    newG = tempG <= 1 ? tempG : 1;
    newG = tempG >= 0 ? tempG : 0;

    float tempB = scalar * c.b;
    float newB;
    newB = tempB <= 1 ? tempB : 1;
    newB = tempB >= 0 ? tempB : 0;

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

RGBColor operator * (const RGBColor& c, float scalar) {
    float tempR = scalar * c.r;
    float newR;
    newR = tempR <= 1 ? tempR : 1;
    newR = tempR >= 0 ? tempR : 0;

    float tempG = scalar * c.g;
    float newG;
    newG = tempG <= 1 ? tempG : 1;
    newG = tempG >= 0 ? tempG : 0;

    float tempB = scalar * c.b;
    float newB;
    newB = tempB <= 1 ? tempB : 1;
    newB = tempB >= 0 ? tempB : 0;

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

RGBColor operator / (const RGBColor& c, float scalar) {
    float tempR = c.r / scalar;
    float newR;
    newR = tempR <= 1 ? tempR : 1;
    newR = tempR >= 0 ? tempR : 0;

    float tempG = c.g / scalar;
    float newG;
    newG = tempG <= 1 ? tempG : 1;
    newG = tempG >= 0 ? tempG : 0;

    float tempB = c.b / scalar;
    float newB;
    newB = tempB <= 1 ? tempB : 1;
    newB = tempB >= 0 ? tempB : 0;

    RGBColor rgbVec = RGBColor(newR, newG, newB);
    return rgbVec;
}

}
