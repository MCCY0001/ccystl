#ifndef CCYSTL_UTILS_H_
#define CCYSTL_UTILS_H_

/**
 * @file utils.h
 * @brief 该头文件定义了多个实用的模板函数和类，包括 move、forward、swap 以及
 * pair 类等。
 *
 * 这些工具函数和类实现了 C++ 中的移动语义、完美转发、交换操作和配对结构，
 * 是 ccystl 库中的基础工具组件。
 */

#include <cstddef>
#include <type_traits>

#include "ccystl/internal/type_traits.h"

namespace ccystl {
/**
 * @brief 将传入参数转换为右值引用，以便实现资源的转移。
 *
 * 该函数实现了 C++ 的 move 语义，用于将传入的左值或右值参数转换为右值引用。
 * 通过使用 `std::remove_reference`
 * 移除传入参数类型的引用，然后将其转换为右值引用，
 * 实现对象的资源转移，从而避免不必要的拷贝。
 *
 * @tparam T 传入参数的类型。可以是左值或右值引用类型。
 * @param arg 需要转换为右值引用的参数。
 * @return typename std::remove_reference<T>::type&&
 * 返回传入参数的右值引用，用于资源转移。
 *
 * @note 该函数不会实际移动对象，而是通过类型转换提供了资源转移的可能性。
 * 需要注意传递的参数类型，因为错误使用可能会导致未定义行为。
 *
 * @example
 * @code
 * int a = 10;
 * auto b = ccystl::move(a); // 将 a 转换为右值引用，允许资源转移。
 * @endcode
 */
template <class T>
std::remove_reference_t<T>&& move(T&& arg) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(arg);
}

/**
 * @brief 实现完美转发的函数，将传入的参数按原类型转发。
 *
 * 该函数利用了转发引用的性质，通过引用折叠实现完美转发，
 * 保持传入参数的左值或右值特性，从而避免不必要的拷贝或移动操作。
 *
 * @tparam T 传入参数的类型，可以是左值或右值引用类型。
 * @param arg 需要转发的参数，传入的应是左值引用。
 * @return T&&
 * 返回传入参数的原始引用类型（左值引用或右值引用），用于保持其转发特性。
 *
 * @note
 * 使用此函数时，务必确保传入的参数类型符合预期，否则可能会导致未定义行为。
 *
 * @example
 * @code
 * int a = 10;
 * ccystl::forward<int&>(a);  // 完美转发左值 a
 * ccystl::forward<int>(10);  // 完美转发右值 10
 * @endcode
 */
template <class T>
T&& forward(std::remove_reference_t<T>& arg) noexcept {
  return static_cast<T&&>(arg);
}

/**
 * @brief 实现完美转发的函数，用于将右值参数按原类型转发。
 *
 * 该函数通过引用折叠实现完美转发，并确保传入的参数为右值引用。
 * 通过 `static_assert` 检查模板参数 T
 * 是否是左值引用，如果是，则会触发编译错误，
 * 旨在确保该函数只用于右值的完美转发。
 *
 * @tparam T 传入参数的类型，必须是非左值引用类型。
 * @param arg 需要转发的右值参数，传入的应是右值引用。
 * @return T&& 返回传入参数的原始右值引用类型，用于保持其转发特性。
 *
 * @note 使用此函数时，务必确保传入的是右值参数，否则将触发编译错误。
 *
 * @example
 * @code
 * ccystl::forward<int>(std::move(a));  // 完美转发右值 a
 * @endcode
 */
template <class T>
T&& forward(std::remove_reference_t<T>&& arg) noexcept {
  static_assert(!std::is_lvalue_reference_v<T>, "bad forward");
  return static_cast<T&&>(arg);
}

/**
 * @brief 交换两个对象的值。
 *
 * 使用移动语义实现交换操作，避免不必要的拷贝操作。
 *
 * @tparam Tp 交换的对象类型。
 * @param lhs 左侧对象的引用。
 * @param rhs 右侧对象的引用。
 */
template <class Tp>
void swap(Tp& lhs, Tp& rhs) noexcept {
  auto tmp = ccystl::move(lhs);
  lhs = ccystl::move(rhs);
  rhs = ccystl::move(tmp);
}

/**
 * @brief 交换两个范围内的元素。
 *
 * 通过迭代器对两个范围内的元素进行逐个交换，范围由起始迭代器和结束迭代器确定。
 *
 * @tparam ForwardIter1 第一个范围前向迭代器类型。
 * @tparam ForwardIter2 第二个范围前向迭代器类型。
 * @param first1 第一个范围的起始迭代器。
 * @param last1 第一个范围的结束迭代器。
 * @param first2 第二个范围的起始迭代器。
 * @return ForwardIter2 返回第二个范围的结束迭代器。
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2) {
  for (; first1 != last1; ++first1, (void)++first2) {
    ccystl::swap(*first1, *first2);
  }
  return first2;
}

/**
 * @brief 交换两个数组。
 *
 * 使用 `swap_range` 函数对两个数组的所有元素进行交换。
 *
 * @tparam Tp 数组中元素的类型。
 * @tparam N 数组的大小。
 * @param a 第一个数组的引用。
 * @param b 第二个数组的引用。
 */
template <class Tp, size_t N>
void swap(Tp (&a)[N], Tp (&b)[N]) noexcept {
  ccystl::swap_range(a, a + N, b);
}

/**
 * @brief 模板类 pair，表示一个键值对。
 *
 * pair 类用于存储一对相关的值，通常用于函数返回多个值或构建关联容器。
 * 支持多种构造方式，包括默认构造、拷贝构造、移动构造等。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 */
template <class Ty1, class Ty2>
struct pair {
  using first_type = Ty1;   ///< 第一个元素的类型
  using second_type = Ty2;  ///< 第二个元素的类型

  first_type first;    ///< 第一个元素
  second_type second;  ///< 第二个元素

  /**
   * @brief 默认构造一个空的 pair 对象。
   *
   * 需要 Ty1 和 Ty2 支持默认构造。
   */
  template <class Other1 = Ty1, class Other2 = Ty2,
            std::enable_if_t<std::is_default_constructible_v<Other1> &&
                                 std::is_default_constructible_v<Other2>,
                             void>>
  constexpr pair() : first(), second() {}

  /**
   * @brief 从两个给定的值构造一个 pair 对象。
   *
   * 需要 Ty1 和 Ty2 支持拷贝构造，且传入的类型必须可转换为对应的类型。
   *
   * @param a 第一个值。
   * @param b 第二个值。
   */
  template <class U1 = Ty1, class U2 = Ty2,
            std::enable_if_t<std::is_copy_constructible_v<U1> &&
                                 std::is_copy_constructible_v<U2> &&
                                 std::is_convertible_v<const U1&, Ty1> &&
                                 std::is_convertible_v<const U2&, Ty2>,
                             int> = 0>
  constexpr pair(const Ty1& a, const Ty2& b) : first(a), second(b) {}

  // 显式构造函数
  /**
   * @brief 显式从两个给定的值构造一个 pair 对象。
   *
   * 需要 Ty1 和 Ty2 支持拷贝构造，但禁止隐式类型转换。
   *
   * @param a 第一个值。
   * @param b 第二个值。
   */
  template <class U1 = Ty1, class U2 = Ty2,
            std::enable_if_t<std::is_copy_constructible_v<U1> &&
                                 std::is_copy_constructible_v<U2> &&
                                 (!std::is_convertible_v<const U1&, Ty1> ||
                                  !std::is_convertible_v<const U2&, Ty2>),
                             int> = 0>
  explicit constexpr pair(const Ty1& a, const Ty2& b) : first(a), second(b) {}

  pair(const pair& rhs) = default;  ///< 默认拷贝构造
  pair(pair&& rhs) = default;       ///< 默认移动构造

  // 隐式移动构造函数
  /**
   * @brief 从两个给定的右值构造一个 pair 对象。
   *
   * 需要 Ty1 和 Ty2 支持移动构造，且传入的类型必须可转换为对应的类型。
   *
   * @param a 第一个右值。
   * @param b 第二个右值。
   */
  template <class Other1, class Other2,
            std::enable_if_t<std::is_constructible_v<Ty1, Other1> &&
                                 std::is_constructible_v<Ty2, Other2> &&
                                 std::is_convertible_v<Other1&&, Ty1> &&
                                 std::is_convertible_v<Other2&&, Ty2>,
                             int> = 0>
  constexpr pair(Other1&& a, Other2&& b)
      : first(ccystl::forward<Other1>(a)), second(ccystl::forward<Other2>(b)) {}

  // 显式移动构造函数
  /**
   * @brief 显式从两个给定的右值构造一个 pair 对象。
   *
   * 需要 Ty1 和 Ty2 支持移动构造，但禁止隐式类型转换。
   *
   * @param a 第一个右值。
   * @param b 第二个右值。
   */
  template <class Other1, class Other2,
            std::enable_if_t<std::is_constructible_v<Ty1, Other1> &&
                                 std::is_constructible_v<Ty2, Other2> &&
                                 (!std::is_convertible_v<Other1, Ty1> ||
                                  !std::is_convertible_v<Other2, Ty2>),
                             int> = 0>
  explicit constexpr pair(Other1&& a, Other2&& b)
      : first(ccystl::forward<Other1>(a)), second(ccystl::forward<Other2>(b)) {}

  // 隐式拷贝构造函数
  /**
   * @brief 从另一个 pair 对象拷贝构造。
   *
   * 需要 Ty1 和 Ty2
   * 支持对应类型的拷贝构造，且传入的类型必须可转换为对应的类型。
   *
   * @param other 另一个 pair 对象。
   */
  template <class Other1, class Other2,
            std::enable_if_t<std::is_constructible_v<Ty1, const Other1&> &&
                                 std::is_constructible_v<Ty2, const Other2&> &&
                                 std::is_convertible_v<const Other1&, Ty1> &&
                                 std::is_convertible_v<const Other2&, Ty2>,
                             int> = 0>
  constexpr explicit pair(const pair<Other1, Other2>& other)
      : first(other.first), second(other.second) {}

  // 显式拷贝构造函数
  /**
   * @brief 显式从另一个 pair 对象拷贝构造。
   *
   * 需要 Ty1 和 Ty2 支持对应类型的拷贝构造，但禁止隐式类型转换。
   *
   * @param other 另一个 pair 对象。
   */
  template <class Other1, class Other2,
            std::enable_if_t<std::is_constructible_v<Ty1, const Other1&> &&
                                 std::is_constructible_v<Ty2, const Other2&> &&
                                 (!std::is_convertible_v<const Other1&, Ty1> ||
                                  !std::is_convertible_v<const Other2&, Ty2>),
                             int> = 0>
  explicit constexpr pair(const pair<Other1, Other2>& other)
      : first(other.first), second(other.second) {}

  // 隐式移动构造函数
  /**
   * @brief 从另一个右值 pair 对象移动构造。
   *
   * 需要 Ty1 和 Ty2
   * 支持对应类型的移动构造，且传入的类型必须可转换为对应的类型。
   *
   * @param other 另一个右值 pair 对象。
   */
  template <class Other1, class Other2,
            std::enable_if_t<std::is_constructible_v<Ty1, Other1> &&
                                 std::is_constructible_v<Ty2, Other2> &&
                                 std::is_convertible_v<Other1, Ty1> &&
                                 std::is_convertible_v<Other2, Ty2>,
                             int> = 0>
  constexpr explicit pair(pair<Other1, Other2>&& other)
      : first(ccystl::forward<Other1>(other.first)),
        second(ccystl::forward<Other2>(other.second)) {}

  // 显式移动构造函数
  /**
   * @brief 显式从另一个右值 pair 对象移动构造。
   *
   * 需要 Ty1 和 Ty2 支持对应类型的移动构造，但禁止隐式类型转换。
   *
   * @param other 另一个右值 pair 对象。
   */
  template <class Other1, class Other2,
            std::enable_if_t<std::is_constructible_v<Ty1, Other1> &&
                                 std::is_constructible_v<Ty2, Other2> &&
                                 (!std::is_convertible_v<Other1, Ty1> ||
                                  !std::is_convertible_v<Other2, Ty2>),
                             int> = 0>
  explicit constexpr pair(pair<Other1, Other2>&& other)
      : first(ccystl::forward<Other1>(other.first)),
        second(ccystl::forward<Other2>(other.second)) {}

  /**
   * @brief 拷贝赋值操作符。
   *
   * @param rhs 另一个 pair 对象。
   * @return pair& 返回赋值后的对象。
   */
  pair& operator=(const pair& rhs) {
    if (this != &rhs) {
      first = rhs.first;
      second = rhs.second;
    }
    return *this;
  }

  /**
   * @brief 移动赋值操作符。
   *
   * @param rhs 另一个右值 pair 对象。
   * @return pair& 返回赋值后的对象。
   */
  pair& operator=(pair&& rhs) noexcept {
    if (this != &rhs) {
      first = ccystl::move(rhs.first);
      second = ccystl::move(rhs.second);
    }
    return *this;
  }

  ~pair() = default;  ///< 默认析构函数

  /**
   * @brief 交换两个 pair 对象的内容。
   *
   * @param other 另一个 pair 对象。
   */
  void swap(pair& other) noexcept {
    if (this != other) {
      ccystl::swap(first, other.first);
      ccystl::swap(second, other.second);
    }
  }
};

/**
 * @brief 比较两个 pair 对象是否相等。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 * @return bool 如果两个 pair 对象的元素都相等，返回 true，否则返回 false。
 */
template <class Ty1, class Ty2>
bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

/**
 * @brief 比较两个 pair 对象的大小。
 *
 * 先比较第一个元素，如果相等则比较第二个元素。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 * @return bool 如果左侧 pair 对象小于右侧，返回 true，否则返回 false。
 */
template <class Ty1, class Ty2>
bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return lhs.first < rhs.first ||
         (lhs.first == rhs.first && lhs.second < rhs.second);
}

/**
 * @brief 比较两个 pair 对象是否不相等。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 * @return bool 如果两个 pair 对象的元素不完全相等，返回 true，否则返回 false。
 */
template <class Ty1, class Ty2>
bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return !(lhs == rhs);
}

/**
 * @brief 比较两个 pair 对象的大小。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 * @return bool 如果左侧 pair 对象大于右侧，返回 true，否则返回 false。
 */
template <class Ty1, class Ty2>
bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return rhs < lhs;
}

/**
 * @brief 比较两个 pair 对象的大小。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 * @return bool 如果左侧 pair 对象小于或等于右侧，返回 true，否则返回 false。
 */
template <class Ty1, class Ty2>
bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return !(rhs < lhs);
}

/**
 * @brief 比较两个 pair 对象的大小。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 * @return bool 如果左侧 pair 对象大于或等于右侧，返回 true，否则返回 false。
 */
template <class Ty1, class Ty2>
bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return !(lhs < rhs);
}

/**
 * @brief 交换两个 pair 对象的内容。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param lhs 左侧的 pair 对象。
 * @param rhs 右侧的 pair 对象。
 */
template <class Ty1, class Ty2>
void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs) noexcept {
  lhs.swap(rhs);
}

/**
 * @brief 创建一个 pair 对象。
 *
 * @tparam Ty1 第一个元素的类型。
 * @tparam Ty2 第二个元素的类型。
 * @param first 第一个元素。
 * @param second 第二个元素。
 * @return pair<Ty1, Ty2> 返回由给定元素构造的 pair 对象。
 */
template <class Ty1, class Ty2>
pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second) {
  return pair<Ty1, Ty2>(ccystl::forward<Ty1>(first),
                        ccystl::forward<Ty2>(second));
}
}  // namespace ccystl

#endif  // !CCYSTL_UTILS_H_
