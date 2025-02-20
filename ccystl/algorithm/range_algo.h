#ifndef CCYSTL_RANGE_ALGO_H_
#define CCYSTL_RANGE_ALGO_H_

#include "ccystl/iterator/iterator.h"

namespace ccystl {

// 基础版本，使用 < 比较
template <class ForwardIter, class T>
ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& value) {
  while (first != last) {
    ForwardIter mid = first;
    ccystl::advance(mid, ccystl::distance(first, last) / 2);
    if (*mid < value) {
      first = ++mid;
    } else {
      last = mid;
    }
  }
  return first;
}

// 重载版本，使用自定义比较函数 comp
template <class ForwardIter, class T, class Compared>
ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& value,
                        Compared comp) {
  while (first != last) {
    ForwardIter mid = first;
    ccystl::advance(mid, ccystl::distance(first, last) / 2);
    if (comp(*mid, value)) {
      first = ++mid;
    } else {
      last = mid;
    }
  }
  return first;
}
}  // namespace ccystl

#endif  // CCYSTL_RANGE_ALGO_H_
