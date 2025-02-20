#ifndef CCYSTL_ITERATOR_H_
#define CCYSTL_ITERATOR_H_

// 输入迭代器、输出迭代器、前向迭代器、双向迭代器、随机访问迭代器

#include <cstddef>
#include <type_traits>

#include "ccystl/internal/type_traits.h"

namespace ccystl {
struct input_iterator_tag {};

struct output_iterator_tag {};

struct forward_iterator_tag : input_iterator_tag {};

struct bidirectional_iterator_tag : forward_iterator_tag {};

struct random_access_iterator_tag : bidirectional_iterator_tag {};

// 迭代器模板
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
  typedef Category iterator_category;
  typedef T value_type;
  typedef Pointer pointer;
  typedef long long ptrdiff_t;
  typedef Reference reference;
  typedef Distance difference_type;
};

// 用于检测一个类型 T 是否具有嵌套类型
// iterator_category。这通常用于确定一个类型是否为迭代器类型。 value
// 是一个静态常量布尔值，用来存储测试结果。 它通过比较 sizeof(test<T>(nullptr))
// 的结果来确定。 如果 test<T>(nullptr) 调用的是 static char test 版本（因为 T
// 有 iterator_category 类型），那么 sizeof 的结果将是 sizeof(char)，即 1。
// 如果调用的是 static two test 版本，那么 sizeof 的结果将是 sizeof(two)，通常是
// 2（因为 two 有两个字符成员）。 最终，value 将为 true 如果 sizeof 结果为
// 1，否则为 false。
template <class T>
struct has_iterator_cat {
 private:
  struct two {
    char a;
    char b;
  };

  template <class U>
  static two test(...);
  template <class U>
  static char test(typename U::iterator_category* = nullptr);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(char);
};

template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>
    : iterator_traits_impl<
          Iterator,
          std::is_convertible_v<typename Iterator::iterator_category,
                                input_iterator_tag> ||
              std::is_convertible_v<typename Iterator::iterator_category,
                                    output_iterator_tag>> {};

template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>
    : iterator_traits_impl<
          Iterator,
          std::is_convertible_v<typename Iterator::iterator_category,
                                input_iterator_tag> ||
              std::is_convertible_v<typename Iterator::iterator_category,
                                    output_iterator_tag>> {};

template <class Iterator>
struct iterator_traits
    : iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

template <class T>
struct iterator_traits<T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef ptrdiff_t difference_type;
};

template <class T>
struct iterator_traits<const T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
  typedef ptrdiff_t difference_type;
};

template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
    : y_bool_constant<std::is_convertible_v<
          typename iterator_traits<T>::iterator_category, U>> {};

template <class T, class U>
struct has_iterator_cat_of<T, U, false> : y_false_type {};

template <class Iter>
struct is_input_iterator : has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator
    : has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator
    : has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator : y_bool_constant<is_input_iterator<Iterator>::value ||
                                     is_output_iterator<Iterator>::value> {};

// 类型萃取：仅获取类型，不获取实际值

// 萃取某个迭代器的 category
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
  typedef typename iterator_traits<Iterator>::iterator_category Category;
  return Category();
}

// 萃取某个迭代器的 distance_type
template <class Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type(
    const Iterator&) {
  return nullptr;
}

// 萃取某个迭代器的 value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
  return nullptr;
}

// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance_dispatch(
    InputIterator first, InputIterator last, input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}

// distance 的 random_access_iterator_tag 的版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type distance_dispatch(
    RandomIter first, RandomIter last, random_access_iterator_tag) {
  return last - first;
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance(
    InputIterator first, InputIterator last) {
  return distance_dispatch(first, last, iterator_category(first));
}

// 以下函数用于让迭代器前进 n 个距离

// advance 的 input_iterator_tag 的版本
template <class InputIterator, class Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
  while (n--) ++i;
}

// advance 的 bidirectional_iterator_tag 的版本
template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n,
                      bidirectional_iterator_tag) {
  if (n >= 0)
    while (n--) ++i;
  else
    while (n++) --i;
}

// advance 的 random_access_iterator_tag 的版本
template <class RandomIter, class Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag) {
  i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n) {
  advance_dispatch(i, n, iterator_category(i));
}

// 模板类 : reverse_iterator
// 代表反向迭代器，使前进为后退，后退为前进
template <class Iterator>
class reverse_iterator {
  Iterator current;  // 记录对应的正向迭代器

 public:
  // 反向迭代器的五种相应型别
  typedef
      typename iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename iterator_traits<Iterator>::value_type value_type;
  typedef typename iterator_traits<Iterator>::difference_type difference_type;
  typedef typename iterator_traits<Iterator>::pointer pointer;
  typedef typename iterator_traits<Iterator>::reference reference;

  typedef Iterator iterator_type;
  typedef reverse_iterator self;

  // 构造函数
  reverse_iterator() = default;
  explicit reverse_iterator(iterator_type i) : current(i) {}
  reverse_iterator(const self& rhs) : current(rhs.current) {}

  // 取出对应的正向迭代器
  iterator_type base() const { return current; }

  // 重载操作符
  reference operator*() const {
    // 实际对应正向迭代器的前一个位置
    auto tmp = current;
    return *--tmp;
  }

  pointer operator->() const { return &(operator*()); }

  // 前进(++)变为后退(--)
  self& operator++() {
    --current;
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    --current;
    return tmp;
  }

  // 后退(--)变为前进(++)
  self& operator--() {
    ++current;
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    ++current;
    return tmp;
  }

  self& operator+=(difference_type n) {
    current -= n;
    return *this;
  }

  self operator+(difference_type n) const { return self(current - n); }

  self& operator-=(difference_type n) {
    current += n;
    return *this;
  }

  self operator-(difference_type n) const { return self(current + n); }

  reference operator[](difference_type n) const { return *(*this + n); }
};

// 重载 operator-
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
    const reverse_iterator<Iterator>& lhs,
    const reverse_iterator<Iterator>& rhs) {
  return rhs.base() - lhs.base();
}

// 重载比较操作符
template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
  return rhs.base() < lhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(lhs == rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
  return rhs < lhs;
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(rhs < lhs);
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(lhs < rhs);
}
}  // namespace ccystl

#endif
