

/********************************************************************
        created:	2011/02/23
        created:	23:2:2011   15:17
        filename:
i:\Programing\GameEngine\mrayEngine\mrayEngine\include\StringUtil.h file path:
i:\Programing\GameEngine\mrayEngine\mrayEngine\include file base:
StringUtil file ext:	h author:		Mohamad Yamen Saraiji

        purpose:
*********************************************************************/

#ifndef StringUtil_h__
#define StringUtil_h__

#include <cstring>
#include <stdlib.h>

#include "atypes.h"

namespace utils {

template <typename T> class StringUtilT {
private:
protected:
public:
  static void SplitPathFileName(const std::basic_string<T> &str,
                                std::basic_string<T> &path,
                                std::basic_string<T> &file);
  static void SplitPathExt(const std::basic_string<T> &str,
                           std::basic_string<T> &path,
                           std::basic_string<T> &ext);

  //! Makes the basic_string lower case.
  static std::basic_string<T> ToLower(const std::basic_string<T> &o);

  //! Makes the basic_string upper case.
  static std::basic_string<T> ToUpper(const std::basic_string<T> &o);

  static bool BeginsWith(const std::basic_string<T> &a,
                         const std::basic_string<T> &b,
                         bool caseSensitive = false);

  static std::basic_string<T>
  FindAndReplace(const std::basic_string<T> &a,
                 const std::basic_string<T> &findStr,
                 const std::basic_string<T> &replaceStr);

  // make "path" relative to "root"
  // example:	root=c:\game\bin
  //			path=c:\game\data
  // result:	..\data
  static std::basic_string<T>
  MakePathRelative(const std::basic_string<T> &path,
                   const std::basic_string<T> &root);

  // convert '/' chars into '\\'
  static void NormalizePathSlashes(T *str);

  static std::basic_string<T>
  Trim(const std::basic_string<T> &str,
       const std::basic_string<T> &chars = aT(" \t"));

  static std::vector<std::basic_string<T>>
  Split(const std::basic_string<T> &str, const std::basic_string<T> &delimStr,
        uint maxSplits = 0);

  static void Utf8ToUtf16(const std::string &utf8, std::wstring &utf16);
  static void Utf16ToUtf8(const std::wstring &utf16, std::string &utf8);

  static bool EqualsIgnoreCase(const std::basic_string<T> &a,
                               const std::basic_string<T> &b);

  //! compares the first n characters of the tstrings
  static bool EqualsN(const std::basic_string<T> &a, const T *str, int n);
};

typedef StringUtilT<char> StringUtilA;
typedef StringUtilT<wchar_t> StringUtilW;
typedef StringUtilT<tchar> StringUtil;

#define strToInt atoi
#define strToFloat atof
#define strToUL strtoul
#define strToL strtol
#define strLength strlen
#define compareStr strcmp
#define strCopy strcpy
#define strSearch strstr
#define strPrint sprintf

} // namespace utils

#include "StringUtil.hpp"

#endif // StringUtil_h__tolower
