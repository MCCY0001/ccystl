#ifndef CCYSTL_ASTRING_H_
#define CCYSTL_ASTRING_H_

// 定义了 string, wstring, u16string, u32string 类型

#include "ccystl/container/sequence_container/basic_string.h"

namespace ccystl {
using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;
}  // namespace ccystl
#endif  // !CCYSTL_ASTRING_H_
