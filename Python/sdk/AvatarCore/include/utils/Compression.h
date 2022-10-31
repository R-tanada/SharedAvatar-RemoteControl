
#include <string>

#include "avatarutils_config.h"

namespace utils {
class AvaUtils_API Compression {
   public:
    static int buffer_compress(const char *data, int length, char *encBuf);
    static int buffer_decompress(const char *data, int length, char *decBuf);

    static std::string string_compress_encode(const std::string &data);
    static std::string string_decompress_decode(const std::string &data);
};

}  // namespace utils