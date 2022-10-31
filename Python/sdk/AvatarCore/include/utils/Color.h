

#ifndef ___RenderMaterial__
#define ___RenderMaterial__

#include "atypes.h"
#include "avatarutils_config.h"

namespace utils {

class AvaUtils_API Color {
   public:
    typedef uint RGBA;
    typedef uint ARGB;
    typedef uint BGRA;
    typedef uint ABGR;

   public:
    float R;
    float G;
    float B;
    float A;

    Color() : R(0), G(0), B(0), A(0) {}
    Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
    Color(float c) : R(c), G(c), B(c), A(c) {}

    inline bool operator==(const Color &c) const {
        return (R == c.R && G == c.G && B == c.B && A == c.A);
    }
    inline bool operator!=(const Color &c) const {
        return (R != c.R || G != c.G || B != c.B || A != c.A);
    }

    inline Color operator=(float c) {
        R = G = B = A = c;
        return *this;
    }

    inline Color operator+(float c) const {
        return Color((R + c), (G + c), (B + c), (A + c));
    }
    inline Color operator-(float c) const {
        return Color((R - c), (G - c), (B - c), (A - c));
    }

    inline Color operator*(float c) const {
        return Color((R * c), (G * c), (B * c), (A * c));
    }
    inline Color operator/(float c) const {
        return Color((R / c), (G / c), (B / c), (A / c));
    }

    inline Color operator*(const Color &c) const {
        return Color(R * c.R, G * c.G, B * c.B, A * c.A);
    }
    inline Color operator+(const Color &c) const {
        return Color(R + c.R, G + c.G, B + c.B, A + c.A);
    }
    inline Color operator-(const Color &c) const {
        return Color(R - c.R, G - c.G, B - c.B, A - c.A);
    }
    inline Color &operator+=(float c) {
        R += c;
        B += c;
        G += c;
        A += c;
        return *this;
    }
    inline Color &operator-=(float c) {
        R -= c;
        B -= c;
        G -= c;
        A -= c;
        return *this;
    }

    inline Color &operator*=(float c) {
        R *= c;
        B *= c;
        G *= c;
        A *= c;
        return *this;
    }
    inline Color &operator/=(float c) {
        R /= c;
        B /= c;
        G /= c;
        A /= c;
        return *this;
    }

    inline Color &operator+=(const Color &c) {
        R += c.R;
        B += c.B;
        G += c.G;
        A += c.A;
        return *this;
    }
    inline Color &operator-=(const Color &c) {
        R -= c.R;
        B -= c.B;
        G -= c.G;
        A -= c.A;
        return *this;
    }

    inline float *ptr() { return &R; }
    inline const float *ptr() const { return &R; }

    Color &setColorRGBA(RGBA c);
    Color &setColorARGB(ARGB c);
    Color &setColorBGRA(BGRA c);
    Color &setColorABGR(ABGR c);

    RGBA getColorRGBA() const;
    ARGB getColorARGB() const;
    BGRA getColorBGRA() const;
    ABGR getColorABGR() const;

    inline float getLuminance() const {
        return (float)(0.3f * R + 0.59f * B + 0.11 * G);
    }

    inline void ToArray(float *arr) const {
        arr[0] = R;
        arr[1] = G;
        arr[2] = B;
        arr[3] = A;
    }
    inline void ToByteArrayRGBA(byte *arr) const {
        arr[0] = (byte)(R * 255);
        arr[1] = (byte)(G * 255);
        arr[2] = (byte)(B * 255);
        arr[3] = (byte)(A * 255);
    }

    Color Interpolated(const Color &c, float i) const;

    inline void Set(float r, float g, float b, float a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    void setHSL(float Hue, float Saturation, float Lightness);

    void saturate() {
        if (R < 0)
            R = 0;
        else if (R > 1)
            R = 1;

        if (G < 0)
            G = 0;
        else if (G > 1)
            G = 1;

        if (B < 0)
            B = 0;
        else if (B > 1)
            B = 1;

        if (A < 0)
            A = 0;
        else if (A > 1)
            A = 1;
    }
};

}  // namespace utils

#endif