#ifndef __STRINGCONVERTER__
#define __STRINGCONVERTER__

#include <iostream>
#include <sstream>
#include <string>

#include "Color.h"
#include "Rect.h"
#include "StringUtil.h"
#include "avatarutils_config.h"
#include "line3D.h"
#include "matrix4x4.h"
#include "point2D.h"
#include "point3D.h"
#include "point4D.h"
#include "quaternion.h"

namespace utils {
class AvaUtils_API StringConverter {
#ifdef UNICODE
    typedef std::wostringstream osstream;
#else
    typedef std::ostringstream osstream;
#endif

   public:
    // Convert To String Methods
    static std::string toString(const float &v, int precision = 8);
    static std::string toString(const bool &v, bool trueFalse = 0);
    static std::string toString(const utils::Point4d<int> &v,
                                int precision = 8);
    static std::string toString(const utils::Point4d<float> &v,
                                int precision = 8);
    static std::string toString(const utils::Point3d<float> &v,
                                int precision = 8);
    static std::string toString(const utils::Point2d<float> &v,
                                int precision = 8);
    static std::string toString(const utils::Point2d<int> &v);
    static std::string toString(const utils::line3d &v);
    static std::string toString(const utils::box3d &v);
    static std::string toString(const utils::Plane &v);
    static std::string toString(const utils::Color &v);
    static std::string toString(const utils::rectf &v);
    static std::string toString(const utils::quaternion &v);
    static std::string toString(const utils::matrix4x4 &v);
    static const std::string &toString(int v, const std::string &enumType);

    template <class T>
    static std::string toString(const T &v) {
        osstream stream;
        stream << v;

        return stream.str().c_str();
    }
    template <class T>
    static std::string toString(const std::basic_string<T> &v) {
        osstream stream;
        stream << v.c_str();

        return stream.str().c_str();
    }

    /// Parse String Methods
    static int toInt(const std::string &str);
    static float toFloat(const std::string &str);
    static double toDouble(const std::string &str);
    static uint toUInt(const std::string &str);
    static long toLong(const std::string &str);
    static ulong toULong(const std::string &str);
    static bool toBool(const std::string &str);
    static utils::vector2d toVector2d(const std::string &str);
    static utils::vector3d toVector3d(const std::string &str);
    static utils::vector4d toVector4d(const std::string &str);
    static utils::quaternion toQuaternion(const std::string &str);
    static utils::matrix4x4 toMatrix(const std::string &str);
    static utils::line3d toLine3d(const std::string &str);
    static utils::box3d toBox3d(const std::string &str);
    static utils::Plane toPlane(const std::string &str);
    static utils::Color toColor(const std::string &str);
    static utils::rectf toRectf(const std::string &str);

    static int toEnum(const std::string &enumType, const std::string &enumVal);

    static std::string toString(const std::string &str);
    static std::string toString(const tchar *str);

    static void parse(const std::string &str, char &v);
    static void parse(const std::string &str, wchar_t &v);
    static void parse(const std::string &str, int &v);
    static void parse(const std::string &str, double &v);
    static void parse(const std::string &str, float &v);
    static void parse(const std::string &str, uint &v);
    static void parse(const std::string &str, long &v);
    static void parse(const std::string &str, ulong &v);
    static void parse(const std::string &str, bool &v);
    static void parse(const std::string &str, std::string &v);
    static void parse(const std::string &str, vector2d &v);
    static void parse(const std::string &str, vector2di &v);
    static void parse(const std::string &str, vector3d &v);
    static void parse(const std::string &str, vector3di &v);
    static void parse(const std::string &str, utils::vector4d &v);
    static void parse(const std::string &str, utils::vector4di &v);
    static void parse(const std::string &str, utils::Color &v);
    static void parse(const std::string &str, utils::quaternion &v);
    static void parse(const std::string &str, utils::matrix4x4 &v);
    static void parse(const std::string &str, utils::line3d &v);
    static void parse(const std::string &str, utils::box3d &v);
    static void parse(const std::string &str, utils::Plane &v);

    static void parse(const std::string &str, int &v,
                      const std::string &enumType);
    template <class T>
    static void parse(const std::string &str, const T &v) {}
    //	static void parse(const std::string&str,string&v){v=(str.c_str()); }

    template <class T>
    static std::vector<T> ParseArray(const std::string &arr) {
        std::vector<std::string> splitArr = StringUtil::Split(arr, aT(","));
        std::vector<T> retArr;
        for (size_t i = 0; i < splitArr.size(); ++i) {
            T v;
            parse(splitArr[i], v);
            retArr.push_back(v);
        }
        return retArr;
    }

#if 0

    int wchar_to_char(char *a, const wchar_t *b)
    {
        ////
        return 0;
    }

    int wchar_to_char(const wchar_t *in, std::basic_string<char> &str)
    {

        int len = wcslen(in);
        str.resize(len);
        return wcstombs(&str[0], in, len);
        //return WideCharToMultiByte(CP_ACP, 0,in,-1,&str[0],len*sizeof(wchar_t)+1,0,0);
    }

    int char_to_wchar(wchar_t *a, const char *b)
    {

        int len = strlen(b);
        return mbstowcs(a, b, len);
        //return MultiByteToWideChar(CP_ACP, 0,b,-1,a,len+1);

        //	int len=strLength(b);
        int k = 0;
        int i = 0;
        for (; i < len; ++i)
            a[i] = b[i];
        a[i] = 0;
        return i;
    }

    int char_to_wchar(const char *in, std::wstring &str)
    {

        int len = strlen(in);
        str.resize(len);
        return mbstowcs(&str[0], in, len);
        //return MultiByteToWideChar(CP_ACP, 0,in,-1,&str[0],len+1);
    }

    int char_to_string(const char *in, std::string &str)
    {
#ifdef UNICODE
        return char_to_wchar(in, str);
#else
        str = in;
        return str.length();
#endif
    }
    static std::string char_to_string(const char *in)
    {
        std::string res;
        char_to_string(in, res);
        return res;
    }

    int wchar_to_string(const wchar_t *in, std::string &str)
    {
#ifdef UNICODE
        str = in;
        return str.length();
#else
        return wchar_to_char(in, str);
#endif
    }

    std::string wchar_to_string(const wchar_t *in)
    {
        std::string res;
        wchar_to_string(in, res);
        return res;
    }

    int string_to_char(const std::string &in, std::basic_string<char> &str)
    {
#ifdef UNICODE
        return wchar_to_char(in.c_str(), str);
#else
        str = in;
        return str.length();
#endif
    }
    std::basic_string<char> string_to_char(const std::string &in)
    {
        std::basic_string<char> res;
        string_to_char(in, res);
        return res;
    }

    int string_to_wchar(const std::string &in, std::wstring &str)
    {
#ifdef UNICODE
        str = in;
        return str.length();
#else
        return char_to_wchar(in.c_str(), str);
#endif
    }
    std::wstring string_to_wchar(const std::string &in)
    {
        std::wstring res;
        string_to_wchar(in, res);
        return res;
    }

#endif
};

}  // namespace utils

#endif
