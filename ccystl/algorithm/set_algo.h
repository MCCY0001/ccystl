#ifndef CCYSTL_SET_ALGO_H_
#define CCYSTL_SET_ALGO_H_

/**
 * @file set_algo.h
 * @brief 该头文件包含了四种集合算法：并集、交集、差集和对称差集。
 *
 * 所有函数都要求输入序列是有序的，以保证集合操作的正确性。这些算法广泛用于处理集合数据，
 * 并返回操作结果的迭代器。
 */

#include "ccystl/algorithm/algobase.h"  ///< 包含基本算法实现

namespace ccystl {
/**
 * @brief 计算两个有序序列的并集，将结果保存到输出区间。
 *
 * 并集操作会合并两个序列中的所有元素，重复元素只保留一个。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_union(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                     InputIter2 last2, OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result++ = *first1++;
    } else if (*first2 < *first1) {
      *result++ = *first2++;
    } else {
      *result++ = *first1++;
      ++first2;
    }
  }
  return ccystl::copy(first2, last2, ccystl::copy(first1, last1, result));
}

/**
 * @brief 计算两个有序序列的并集，使用自定义比较函数，将结果保存到输出区间。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @param comp 自定义比较函数对象。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compared>
OutputIter set_union(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                     InputIter2 last2, OutputIter result, Compared comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result++ = *first1++;
    } else if (comp(*first2, *first1)) {
      *result++ = *first2++;
    } else {
      *result++ = *first1++;
      ++first2;
    }
  }
  return ccystl::copy(first2, last2, ccystl::copy(first1, last1, result));
}

/**
 * @brief 计算两个有序序列的交集，将结果保存到输出区间。
 *
 * 交集操作会返回两个序列中都存在的元素。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      ++first1;
    } else if (*first2 < *first1) {
      ++first2;
    } else {
      *result++ = *first1++;
      ++first2;
    }
  }
  return result;
}

/**
 * @brief 计算两个有序序列的交集，使用自定义比较函数，将结果保存到输出区间。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @param comp 自定义比较函数对象。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compared>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result, Compared comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      ++first1;
    } else if (comp(*first2, *first1)) {
      ++first2;
    } else {
      *result++ = *first1++;
      ++first2;
    }
  }
  return result;
}

/**
 * @brief 计算两个有序序列的差集，将结果保存到输出区间。
 *
 * 差集操作会返回在第一个序列中存在但不在第二个序列中的元素。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result++ = *first1++;
    } else if (*first2 < *first1) {
      ++first2;
    } else {
      ++first1;
      ++first2;
    }
  }
  return ccystl::copy(first1, last1, result);
}

/**
 * @brief 计算两个有序序列的差集，使用自定义比较函数，将结果保存到输出区间。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @param comp 自定义比较函数对象。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compared>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          OutputIter result, Compared comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result++ = *first1++;
    } else if (comp(*first2, *first1)) {
      ++first2;
    } else {
      ++first1;
      ++first2;
    }
  }
  return ccystl::copy(first1, last1, result);
}

/**
 * @brief 计算两个有序序列的对称差集，将结果保存到输出区间。
 *
 * 对称差集包括只在一个序列中存在的元素，即 (S1-S2)∪(S2-S1)。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                    InputIter2 first2, InputIter2 last2,
                                    OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result++ = *first1++;
    } else if (*first2 < *first1) {
      *result++ = *first2++;
    } else {
      ++first1;
      ++first2;
    }
  }
  return ccystl::copy(first2, last2, ccystl::copy(first1, last1, result));
}

/**
 * @brief 计算两个有序序列的对称差集，使用自定义比较函数，将结果保存到输出区间。
 *
 * @tparam InputIter1 第一个输入序列的迭代器类型。
 * @tparam InputIter2 第二个输入序列的迭代器类型。
 * @tparam OutputIter 输出序列的迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first1 第一个输入序列的起始迭代器。
 * @param last1 第一个输入序列的结束迭代器。
 * @param first2 第二个输入序列的起始迭代器。
 * @param last2 第二个输入序列的结束迭代器。
 * @param result 输出序列的起始迭代器。
 * @param comp 自定义比较函数对象。
 * @return OutputIter 返回指向输出序列尾部的迭代器。
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compared>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                    InputIter2 first2, InputIter2 last2,
                                    OutputIter result, Compared comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result++ = *first1++;
    } else if (comp(*first2, *first1)) {
      *result++ = *first2++;
    } else {
      ++first1;
      ++first2;
    }
  }
  return ccystl::copy(first2, last2, ccystl::copy(first1, last1, result));
}
}  // namespace ccystl

#endif  // !CCYSTL_SET_ALGO_H_
