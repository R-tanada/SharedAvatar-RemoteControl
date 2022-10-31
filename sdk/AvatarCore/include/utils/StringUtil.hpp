#include <string>

#include "OSType.h"
#include "StringUtil.h"

namespace utils {

#ifdef __WINDOWS__
#define PATH_SPLIT_CHAR '\\'
#else
#define PATH_SPLIT_CHAR '/'

#endif

#define _lowerChar(C) (((C) >= 'A' && (C) <= 'Z') ? ((C) - 'A' + 'a') : (C))
template <typename T>
void StringUtilT<T>::SplitPathFileName(const std::basic_string<T> &str,
                                       std::basic_string<T> &path,
                                       std::basic_string<T> &file) {
  std::basic_string<T> fullPath(str);
  NormalizePathSlashes(&fullPath[0]);
  size_t i = fullPath.find_last_of(PATH_SPLIT_CHAR);
  if (i != -1) {
    path = fullPath.substr(0, i + 1);
    file = fullPath.substr(i + 1, fullPath.length() - i - 1);
  } else {
    path = fullPath;
    file = "";
  }
}
template <typename T>
void StringUtilT<T>::SplitPathExt(const std::basic_string<T> &str,
                                  std::basic_string<T> &path,
                                  std::basic_string<T> &ext) {
  std::basic_string<T> fullPath(str);
  NormalizePathSlashes(&fullPath[0]);
  int i = fullPath.find_last_of('.');
  if (i != -1) {
    path = fullPath.substr(0, i);
    ext = fullPath.substr(i + 1, fullPath.length() - i - 1);
  } else {
    path = fullPath;
    ext = "";
  }
}

template <typename T> void StringUtilT<T>::NormalizePathSlashes(T *str) {
#ifdef __WINDOWS__
  for (T *p = str; *p; p++) {
    if (*p == '/')
      *p = '\\';
  }
#else
  for (T *p = str; *p; p++) {
    if (*p == '\\')
      *p = '/';
  }
#endif
}

template <typename T>
std::basic_string<T>
StringUtilT<T>::FindAndReplace(const std::basic_string<T> &a,
                               const std::basic_string<T> &findStr,
                               const std::basic_string<T> &replaceStr) {
  std::basic_string<T> ret = a;
  size_t pos = 0;
  while ((pos = ret.find(findStr, pos)) != std::basic_string<T>::npos) {
    ret.replace(pos, findStr.length(), replaceStr);
    pos += replaceStr.length();
  }
  return ret;
}

template <typename T>
bool StringUtilT<T>::BeginsWith(const std::basic_string<T> &a,
                                const std::basic_string<T> &b,
                                bool caseSensitive) {
  const T *aptr = a.c_str();
  const T *bptr = b.c_str();
  while (*bptr != 0) {
    if (caseSensitive) {
      if (*aptr != *bptr)
        break;
    } else if (_lowerChar(*aptr) != _lowerChar(*bptr))
      break;
    ++aptr;
    ++bptr;
  }
  return *bptr == 0;
}

template <typename T>
std::basic_string<T> StringUtilT<T>::ToLower(const std::basic_string<T> &o) {
  static const T diff = 'a' - 'A';
  std::basic_string<T> ret;
  if (o.length() == 0)
    return ret;
  const T *ptr = o.c_str();
  ret.resize(o.length()); //+1);
  for (size_t i = 0; i < o.length(); ++i) {
    if (ptr[i] >= 'A' && ptr[i] <= 'Z')
      ret[i] = ptr[i] + diff;
    else
      ret[i] = ptr[i];
  }
  //	ret[o.length()]=0;
  return ret;
}

//! Makes the tstring upper case.
template <typename T>
std::basic_string<T> StringUtilT<T>::ToUpper(const std::basic_string<T> &o) {
  static const T diff = 'A' - 'a';
  std::basic_string<T> ret;
  if (o.length() == 0)
    return ret;
  const T *ptr = o.c_str();
  ret.resize(o.length()); //+1);
  for (size_t i = 0; i < o.length(); ++i) {
    if (ptr[i] >= 'a' && ptr[i] <= 'z')
      ret[i] = ptr[i] + diff;
    else
      ret[i] = ptr[i];
  }
  //	ret[o.length()]=0;
  return ret;
}
// make "path" relative to "root"
// example:	root=c:\game\bin
//			path=c:\game\data
// result:	..\data
template <typename T>
std::basic_string<T>
StringUtilT<T>::MakePathRelative(const std::basic_string<T> &path,
                                 const std::basic_string<T> &root) {
  int matched = 0;
  //	path.make_lower();
  //	app.make_lower();
  std::basic_string<T> cpy(path);
  std::basic_string<T> cpyRoot(root);
  NormalizePathSlashes(&cpy[0]);
  NormalizePathSlashes(&cpyRoot[0]);

  for (size_t i = 0; i < cpy.length() && i < cpyRoot.length(); ++i) {
    if (tolower(cpy[i]) != tolower(cpyRoot[i]))
      break;
    else if (cpy[i] == PATH_SPLIT_CHAR)
      matched = i;
  }
  std::basic_string<T> minPath;
  int appFolders = 1;
  for (size_t i = matched + 1; i < cpyRoot.length(); ++i) {
    if (cpyRoot[i] == PATH_SPLIT_CHAR) {
      minPath += aT("..");
      minPath += PATH_SPLIT_CHAR;
      ++appFolders;
    }
  }
  minPath += path.substr(matched + 1, path.length() - matched);

  return minPath;
}

template <typename T>
std::basic_string<T> StringUtilT<T>::Trim(const std::basic_string<T> &str,
                                          const std::basic_string<T> &chars) {
  std::basic_string<T> ret;
  bool found = false;
  for (size_t i = 0; i < str.length(); ++i) {
    if (chars.find(str[i]) == -1 || found) {
      found = true;
      ret.append(1, str[i]);
    }
  }
  for (size_t i = ret.length() - 1; i >= 0; --i) {
    if (chars.find(ret[i]) == -1) {
      ret = ret.substr(0, i + 1);
      break;
    }
  }
  return ret.c_str();
}
template <typename T>
std::vector<std::basic_string<T>>
StringUtilT<T>::Split(const std::basic_string<T> &str,
                      const std::basic_string<T> &delimStr, uint maxSplits) {
  std::vector<std::basic_string<T>> ret;
  ret.reserve(maxSplits ? maxSplits : 10);

  std::basic_string<T> temp;
  const T *p = str.c_str();
  uint cnt = 0;
  for (; *p != 0; p++) {
    if (delimStr.find(*p) != -1) {
      if (temp != aT("\0")) {
        ret.push_back(temp);
        temp = aT("\0");
        cnt++;
        if (maxSplits != 0 && cnt >= maxSplits)
          break;
      }
    } else
      temp += *p;
  }
  if (maxSplits == 0 || cnt < maxSplits) {
    if (temp != aT("\0")) {
      ret.push_back(temp);
    }
  }
  return ret;
}

template <typename T>
bool StringUtilT<T>::EqualsIgnoreCase(const std::basic_string<T> &a,
                                      const std::basic_string<T> &b) {
  const T *mptr = a.c_str();
  const T *optr = b.c_str();
  while (*mptr && *optr) {
    if (tolower(*mptr) != tolower(*optr))
      return false;
    ++mptr;
    ++optr;
  }

  return *mptr == *optr;
}

template <typename T>
bool StringUtilT<T>::EqualsN(const std::basic_string<T> &a, const T *str,
                             int n) {
  if (!str)
    return false;
  int i;
  const T *ptr = a.c_str();
  for (i = 0; *ptr && *str && i < n; ++i)
    if (*ptr != *str)
      return false;

  // if one (or both) of the tstrings was smaller then they
  // are only equal if they have the same length
  return (i == n); // || (!*ptr && !*str);
}

} // namespace utils