#ifndef CCYSTL_UNINITIALIZED_H_
#define CCYSTL_UNINITIALIZED_H_

/**
 * @file uninitialized.h
 * @brief 该头文件用于对未初始化的空间进行元素构造操作。
 *
 * 提供了多种模板函数用于在未初始化的内存区域内构造对象，包括拷贝构造、填充构造和移动构造等。
 */

#include "ccystl/algorithm/algobase.h"
#include "ccystl/iterator/iterator.h"
#include "ccystl/utils/utils.h"

namespace ccystl {
/**
 * @brief 在未初始化的内存区域内拷贝对象。
 *
 * 将 `[first, last)` 范围内的对象拷贝到以 `result` 为起始的未初始化空间内。
 *
 * @tparam InputIter 输入迭代器类型。
 * @tparam ForwardIter 前向迭代器类型。
 * @param first 输入范围的起始迭代器。
 * @param last 输入范围的结束迭代器。
 * @param result 目标未初始化空间的起始迭代器。
 * @return ForwardIter 拷贝结束的位置。
 */
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                  ForwardIter result, std::true_type) {
  return ccystl::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                  ForwardIter result, std::false_type) {
  auto cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      ccystl::construct(&*cur, *first);
    }
  } catch (...) {
    for (; result != cur; --cur) ccystl::destroy(&*cur);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last,
                               ForwardIter result) {
  return ccystl::unchecked_uninit_copy(
      first, last, result,
      std::is_trivially_copy_assignable<
          typename iterator_traits<ForwardIter>::value_type>{});
}

/**
 * @brief 在未初始化的内存区域内拷贝指定数量的对象。
 *
 * 将 `[first, first + n)` 范围内的对象拷贝到以 `result`
 * 为起始的未初始化空间内。
 *
 * @tparam InputIter 输入迭代器类型。
 * @tparam Size 拷贝的对象数量类型。
 * @tparam ForwardIter 前向迭代器类型。
 * @param first 输入范围的起始迭代器。
 * @param n 拷贝的对象数量。
 * @param result 目标未初始化空间的起始迭代器。
 * @return ForwardIter 拷贝结束的位置。
 */
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result,
                                    std::true_type) {
  return ccystl::copy_n(first, n, result).second;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result,
                                    std::false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++cur, ++first) {
      ccystl::construct(&*cur, *first);
    }
  } catch (...) {
    for (; result != cur; --cur) ccystl::destroy(&*cur);
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
  return ccystl::unchecked_uninit_copy_n(
      first, n, result,
      std::is_trivially_copy_assignable<
          typename iterator_traits<InputIter>::value_type>{});
}

/**
 * @brief 在未初始化的内存区域内填充对象。
 *
 * 在 `[first, last)` 范围内填充指定的值。
 *
 * @tparam ForwardIter 前向迭代器类型。
 * @tparam T 填充的对象类型。
 * @param first 目标范围的起始迭代器。
 * @param last 目标范围的结束迭代器。
 * @param value 用于填充的值。
 */
template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value,
                           std::true_type) {
  ccystl::fill(first, last, value);
}

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value,
                           std::false_type) {
  auto cur = first;
  try {
    for (; cur != last; ++cur) {
      ccystl::construct(&*cur, value);
    }
  } catch (...) {
    for (; first != cur; ++first) ccystl::destroy(&*first);
  }
}

template <class ForwardIter, class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
  ccystl::unchecked_uninit_fill(
      first, last, value,
      std::is_trivially_copy_assignable<
          typename iterator_traits<ForwardIter>::value_type>{});
}

/**
 * @brief 在未初始化的内存区域内填充指定数量的对象。
 *
 * 从 `first` 位置开始，填充 `n` 个指定的值。
 *
 * @tparam ForwardIter 前向迭代器类型。
 * @tparam Size 填充的对象数量类型。
 * @tparam T 填充的对象类型。
 * @param first 目标范围的起始迭代器。
 * @param n 填充的对象数量。
 * @param value 用于填充的值。
 * @return ForwardIter 填充结束的位置。
 */
template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value,
                                    std::true_type) {
  return ccystl::fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value,
                                    std::false_type) {
  auto cur = first;
  try {
    for (; n > 0; --n, ++cur) {
      ccystl::construct(&*cur, value);
    }
  } catch (...) {
    for (; first != cur; ++first) ccystl::destroy(&*first);
  }
  return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
  return ccystl::unchecked_uninit_fill_n(
      first, n, value,
      std::is_trivially_copy_assignable<
          typename iterator_traits<ForwardIter>::value_type>{});
}

/**
 * @brief 在未初始化的内存区域内移动对象。
 *
 * 将 `[first, last)` 范围内的对象移动到以 `result` 为起始的未初始化空间内。
 *
 * @tparam InputIter 输入迭代器类型。
 * @tparam ForwardIter 前向迭代器类型。
 * @param first 输入范围的起始迭代器。
 * @param last 输入范围的结束迭代器。
 * @param result 目标未初始化空间的起始迭代器。
 * @return ForwardIter 移动结束的位置。
 */
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first, InputIter last,
                                  ForwardIter result, std::true_type) {
  return ccystl::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first, InputIter last,
                                  ForwardIter result, std::false_type) {
  ForwardIter cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      ccystl::construct(&*cur, ccystl::move(*first));
    }
  } catch (...) {
    ccystl::destroy(result, cur);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last,
                               ForwardIter result) {
  return ccystl::unchecked_uninit_move(
      first, last, result,
      std::is_trivially_move_assignable<
          typename iterator_traits<InputIter>::value_type>{});
}

/**
 * @brief 在未初始化的内存区域内移动指定数量的对象。
 *
 * 将 `[first, first + n)` 范围内的对象移动到以 `result`
 * 为起始的未初始化空间内。
 *
 * @tparam InputIter 输入迭代器类型。
 * @tparam Size 移动的对象数量类型。
 * @tparam ForwardIter 前向迭代器类型。
 * @param first 输入范围的起始迭代器。
 * @param n 移动的对象数量。
 * @param result 目标未初始化空间的起始迭代器。
 * @return ForwardIter 移动结束的位置。
 */
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result,
                                    std::true_type) {
  return ccystl::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result,
                                    std::false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++first, ++cur) {
      ccystl::construct(&*cur, ccystl::move(*first));
    }
  } catch (...) {
    for (; result != cur; ++result) ccystl::destroy(&*result);
    throw;
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
  return ccystl::unchecked_uninit_move_n(
      first, n, result,
      std::is_trivially_move_assignable<
          typename iterator_traits<InputIter>::value_type>{});
}
}  // namespace ccystl

#endif  // CCYSTL_UNINITIALIZED_H_
