#ifndef CCYSTL_FUNCTIONAL_H_
#define CCYSTL_FUNCTIONAL_H_

/**
 * @file functional.h
 * @brief 该头文件包含了 ccystl 的函数对象与哈希函数的实现。
 *
 * 提供了常用的算术、比较、逻辑函数对象以及哈希函数对象的实现。
 */

#include <cstddef>

namespace ccystl {
/**
 * @brief 定义一元函数的参数类型和返回值类型的基类。
 *
 * @tparam Arg 函数的参数类型。
 * @tparam Result 函数的返回值类型。
 */
template <typename Arg, typename Result>
struct unarg_function {
  typedef Arg argument_type;   ///< 参数类型
  typedef Result result_type;  ///< 返回值类型
};

/**
 * @brief 定义二元函数的参数类型和返回值类型的基类。
 *
 * @tparam Arg1 第一个参数类型。
 * @tparam Arg2 第二个参数类型。
 * @tparam Result 函数的返回值类型。
 */
template <typename Arg1, typename Arg2, typename Result>
struct binary_function {
  typedef Arg1 first_argument_type;   ///< 第一个参数类型
  typedef Arg2 second_argument_type;  ///< 第二个参数类型
  typedef Result result_type;         ///< 返回值类型
};

/**
 * @brief 加法函数对象。
 *
 * 实现加法操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct plus : binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x + y; }
};

/**
 * @brief 减法函数对象。
 *
 * 实现减法操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct minus : binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x - y; }
};

/**
 * @brief 乘法函数对象。
 *
 * 实现乘法操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct multiplies : binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x * y; }
};

/**
 * @brief 除法函数对象。
 *
 * 实现除法操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct divides : binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x / y; }
};

/**
 * @brief 取模函数对象。
 *
 * 实现取模操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct modulus : binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x % y; }
};

/**
 * @brief 否定函数对象。
 *
 * 实现否定操作的函数对象，继承自 `unarg_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct negate : unarg_function<T, T> {
  T operator()(const T& x) const { return -x; }
};

/**
 * @brief 等于比较函数对象。
 *
 * 实现等于比较的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct equal_to : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x == y; }
};

/**
 * @brief 不等于比较函数对象。
 *
 * 实现不等于比较的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct not_equal_to : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x != y; }
};

/**
 * @brief 大于比较函数对象。
 *
 * 实现大于比较的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct greater : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x > y; }
};

/**
 * @brief 小于比较函数对象。
 *
 * 实现小于比较的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct less : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x < y; }
};

/**
 * @brief 大于等于比较函数对象。
 *
 * 实现大于等于比较的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct greater_equal : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x >= y; }
};

/**
 * @brief 小于等于比较函数对象。
 *
 * 实现小于等于比较的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct less_equal : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x <= y; }
};

/**
 * @brief 逻辑与函数对象。
 *
 * 实现逻辑与操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct logical_and : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x && y; }
};

/**
 * @brief 逻辑或函数对象。
 *
 * 实现逻辑或操作的函数对象，继承自 `binary_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct logical_or : binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x || y; }
};

/**
 * @brief 逻辑非函数对象。
 *
 * 实现逻辑非操作的函数对象，继承自 `unarg_function`。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct logical_not : unarg_function<T, bool> {
  bool operator()(const T& x) const { return !x; }
};

/**
 * @brief 返回加法的单位元。
 *
 * @tparam T 操作数的类型。
 * @param plus 仿函数对象。
 * @return T 加法的单位元（0）。
 */
template <typename T>
T identity_element(plus<T>) {
  return T(0);
}

/**
 * @brief 返回乘法的单位元。
 *
 * @tparam T 操作数的类型。
 * @param multiplies 仿函数对象。
 * @return T 乘法的单位元（1）。
 */
template <typename T>
T identity_element(multiplies<T>) {
  return T(1);
}

/**
 * @brief 恒等函数对象。
 *
 * 返回传入参数的本身，不改变值。
 *
 * @tparam T 操作数的类型。
 */
template <typename T>
struct identity : unarg_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};

/**
 * @brief 选择函数：返回 pair 的第一个元素。
 *
 * @tparam Pair 包含两个元素的 pair 类型。
 */
template <typename Pair>
struct selectfirst : public unarg_function<Pair, typename Pair::first_type> {
  const typename Pair::first_type& operator()(const Pair& x) const {
    return x.first;
  }
};

/**
 * @brief 选择函数：返回 pair 的第二个元素。
 *
 * @tparam Pair 包含两个元素的 pair 类型。
 */
template <typename Pair>
struct selectsecond : unarg_function<Pair, typename Pair::second_type> {
  const typename Pair::second_type& operator()(const Pair& x) const {
    return x.second;
  }
};

/**
 * @brief 投射函数：返回第一个参数。
 *
 * @tparam Arg1 第一个参数类型。
 * @tparam Arg2 第二个参数类型。
 */
template <typename Arg1, typename Arg2>
struct projectfirst : binary_function<Arg1, Arg2, Arg1> {
  Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

/**
 * @brief 投射函数：返回第二个参数。
 *
 * @tparam Arg1 第一个参数类型。
 * @tparam Arg2 第二个参数类型。
 */
template <typename Arg1, typename Arg2>
struct projectsecond : binary_function<Arg1, Arg2, Arg2> {
  Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};

/**
 * @brief 哈希函数对象模板。
 *
 * 对于大部分类型，哈希函数什么都不做，需为每种类型特化实现。
 *
 * @tparam Key 哈希对象的类型。
 */
template <typename Key>
struct hash {};

/**
 * @brief 针对指针类型的哈希函数特化。
 *
 * @tparam T 指针指向的对象类型。
 */
template <typename T>
struct hash<T*> {
  size_t operator()(T* p) const noexcept { return reinterpret_cast<size_t>(p); }
};

/**
 * @brief 针对整数类型的哈希函数特化。
 *
 * 定义一个宏 CCYSTL_TRIVIAL_HASH_FCN，用于快速定义整数类型的哈希函数。
 */
#define CCYSTL_TRIVIAL_HASH_FCN(Type)            \
  template <>                                    \
  struct hash<Type> {                            \
    size_t operator()(Type val) const noexcept { \
      return static_cast<size_t>(val);           \
    }                                            \
  };

CCYSTL_TRIVIAL_HASH_FCN(bool)

CCYSTL_TRIVIAL_HASH_FCN(char)

CCYSTL_TRIVIAL_HASH_FCN(signed char)

CCYSTL_TRIVIAL_HASH_FCN(unsigned char)

CCYSTL_TRIVIAL_HASH_FCN(wchar_t)

CCYSTL_TRIVIAL_HASH_FCN(char16_t)

CCYSTL_TRIVIAL_HASH_FCN(char32_t)

CCYSTL_TRIVIAL_HASH_FCN(short)

CCYSTL_TRIVIAL_HASH_FCN(unsigned short)

CCYSTL_TRIVIAL_HASH_FCN(int)

CCYSTL_TRIVIAL_HASH_FCN(unsigned int)

CCYSTL_TRIVIAL_HASH_FCN(long)

CCYSTL_TRIVIAL_HASH_FCN(unsigned long)

CCYSTL_TRIVIAL_HASH_FCN(long long)

CCYSTL_TRIVIAL_HASH_FCN(unsigned long long)

#undef CCYSTL_TRIVIAL_HASH_FCN

/**
 * @brief 逐位哈希函数，用于浮点数类型。
 *
 * 使用 FNV-1a 哈希算法，一种常见的非加密哈希算法。
 *
 * @param first 指向数据起始位置的指针。
 * @param count 数据字节数。
 * @return size_t 返回计算得到的哈希值。
 */
inline size_t bitwise_hash(const unsigned char* first, size_t count) {
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) && __SIZEOF_POINTER__ == 8)
  constexpr size_t fnv_offset = 14695981039346656037ull;
  constexpr size_t fnv_prime = 1099511628211ull;
#else
  constexpr size_t fnv_offset = 2166136261u;
  constexpr size_t fnv_prime = 16777619u;
#endif
  size_t result = fnv_offset;
  for (size_t i = 0; i < count; ++i) {
    result ^= static_cast<size_t>(first[i]);
    result *= fnv_prime;
  }
  return result;
}

/**
 * @brief 针对 float 类型的哈希函数特化。
 */
template <>
struct hash<float> {
  size_t operator()(const float& val) const {
    return val == 0.0f
               ? 0
               : bitwise_hash(reinterpret_cast<const unsigned char*>(&val),
                              sizeof(float));
  }
};

/**
 * @brief 针对 double 类型的哈希函数特化。
 */
template <>
struct hash<double> {
  size_t operator()(const double& val) const {
    return val == 0.0
               ? 0
               : bitwise_hash(reinterpret_cast<const unsigned char*>(&val),
                              sizeof(double));
  }
};

/**
 * @brief 针对 long double 类型的哈希函数特化。
 */
template <>
struct hash<long double> {
  size_t operator()(const long double& val) const {
    return val == 0.0L
               ? 0
               : bitwise_hash(reinterpret_cast<const unsigned char*>(&val),
                              sizeof(long double));
  }
};
}  // namespace ccystl

#endif  // !CCYSTL_FUNCTIONAL_H_
