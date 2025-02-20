#ifndef CCYSTL_STACK_H_
#define CCYSTL_STACK_H_

/**
 * @file stack.h
 * @brief 该头文件包含了模板类 stack 的定义，基于 deque。
 *
 * stack 是一个栈容器，支持基本的栈操作，如入栈、出栈、访问栈顶元素等。
 */

#include "ccystl/container/sequence_container/deque.h"

namespace ccystl {
/**
 * @brief 模板类 stack，表示一个栈容器。
 *
 * 栈是一种后进先出（LIFO）的数据结构。此类提供基本的栈操作，如入栈、出栈、访问栈顶元素等。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型，默认为 `deque<T>`。
 */
template <class T, class Container = deque<T>>
class stack {
 public:
  using container_type = Container;                   ///< 底层容器的类型。
  using value_type = typename Container::value_type;  ///< 栈中元素的类型。
  using size_type = typename Container::size_type;    ///< 用于表示大小的类型。
  using reference = typename Container::reference;    ///< 栈元素的引用类型。
  using const_reference =
      typename Container::const_reference;  ///< 栈元素的常量引用类型。

  static_assert(std::is_same_v<T, value_type>,
                "底层容器的 value_type 应该与 T 类型相同");

 private:
  container_type c_;  ///< 底层容器，用于存储栈中的元素。

 public:
  /**
   * @brief 默认构造函数。
   *
   * 构造一个空的栈。
   */
  stack() = default;

  /**
   * @brief 构造一个包含 `n` 个默认构造元素的栈。
   *
   * @param n 要创建的元素数量。
   */
  explicit stack(size_type n) : c_(n) {}

  /**
   * @brief 构造一个包含 `n` 个指定值的栈。
   *
   * @param n 要创建的元素数量。
   * @param value 用于初始化每个元素的值。
   */
  stack(size_type n, const value_type& value) : c_(n, value) {}

  /**
   * @brief 从指定范围的元素构造栈。
   *
   * @tparam IIter 输入迭代器类型。
   * @param first 范围的起始迭代器。
   * @param last 范围的结束迭代器。
   */
  template <class IIter>
  stack(IIter first, IIter last) : c_(first, last) {}

  /**
   * @brief 从初始化列表构造栈。
   *
   * @param initializer_list 用于构造的初始化列表。
   */
  stack(std::initializer_list<T> initializer_list)
      : c_(initializer_list.begin(), initializer_list.end()) {}

  /**
   * @brief 从现有容器构造栈。
   *
   * @param c 用于初始化栈的容器。
   */
  explicit stack(const Container& c) : c_(c) {}

  /**
   * @brief 从现有容器移动构造栈。
   *
   * @param c 要移动到栈的容器。
   */
  explicit stack(Container&& c) noexcept(
      std::is_nothrow_move_constructible_v<Container>)
      : c_(ccystl::move(c)) {}

  /**
   * @brief 拷贝构造函数。
   *
   * @param rhs 要拷贝的栈。
   */
  stack(const stack& rhs) : c_(rhs.c_) {}

  /**
   * @brief 移动构造函数。
   *
   * @param rhs 要移动的栈。
   */
  stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible_v<Container>)
      : c_(ccystl::move(rhs.c_)) {}

  /**
   * @brief 拷贝赋值操作符。
   *
   * @param rhs 要拷贝的栈。
   * @return 引用指向赋值后的对象。
   */
  stack& operator=(const stack& rhs) {
    if (this != &rhs) {
      c_ = rhs.c_;
    }
    return *this;
  }

  /**
   * @brief 移动赋值操作符。
   *
   * @param rhs 要移动的栈。
   * @return 引用指向赋值后的对象。
   */
  stack& operator=(stack&& rhs) noexcept(
      std::is_nothrow_move_assignable_v<Container>) {
    if (this != &rhs) {
      c_ = ccystl::move(rhs.c_);
    }
    return *this;
  }

  /**
   * @brief 初始化列表赋值操作符。
   *
   * @param initializer_list 用于赋值的初始化列表。
   * @return 引用指向赋值后的对象。
   */
  stack& operator=(std::initializer_list<T> initializer_list) {
    c_ = initializer_list;
    return *this;
  }

  /**
   * @brief 析构函数。
   */
  ~stack() = default;

  // 访问元素相关操作

  /**
   * @brief 返回栈顶元素的引用。
   *
   * @return 引用指向栈顶元素。
   */
  reference top() { return c_.back(); }

  /**
   * @brief 返回栈顶元素的常量引用。
   *
   * @return 常量引用指向栈顶元素。
   */
  const_reference top() const { return c_.back(); }

  // 容量相关操作

  /**
   * @brief 检查栈是否为空。
   *
   * @return 如果栈为空，返回 `true`，否则返回 `false`。
   */
  [[nodiscard]] bool empty() const noexcept { return c_.empty(); }

  /**
   * @brief 返回栈中元素的数量。
   *
   * @return 栈中的元素数量。
   */
  size_type size() const noexcept { return c_.size(); }

  /**
   * @brief 在栈中插入一个新元素。
   *
   * 元素使用传入的参数构造。
   *
   * @tparam Args 转发给构造函数的参数类型。
   * @param args 转发给构造函数的参数。
   */
  template <class... Args>
  void emplace(Args&&... args) {
    c_.emplace_back(ccystl::forward<Args>(args)...);
  }

  /**
   * @brief 在栈中插入一个新元素。
   *
   * 元素会被复制到栈中。
   *
   * @param value 要插入的元素值。
   */
  void push(const value_type& value) { c_.push_back(value); }

  /**
   * @brief 在栈中插入一个新元素。
   *
   * 元素会被移动到栈中。
   *
   * @param value 要插入的元素值。
   */
  void push(value_type&& value) { c_.push_back(ccystl::move(value)); }

  /**
   * @brief 移除栈顶元素。
   */
  void pop() { c_.pop_back(); }

  /**
   * @brief 移除栈中的所有元素。
   */
  void clear() {
    while (!empty()) pop();
  }

  /**
   * @brief 交换两个栈的内容。
   *
   * @param rhs 要交换的另一个栈。
   */
  void swap(stack& rhs) noexcept(noexcept(ccystl::swap(c_, rhs.c_))) {
    ccystl::swap(c_, rhs.c_);
  }

  // 比较操作符

  /**
   * @brief 检查两个栈是否相等。
   *
   * @param lhs 左侧栈。
   * @param rhs 右侧栈。
   * @return 如果两个栈相等，返回 `true`，否则返回 `false`。
   */
  friend bool operator==(const stack& lhs, const stack& rhs) {
    return lhs.c_ == rhs.c_;
  }

  /**
   * @brief 检查左侧栈是否小于右侧栈。
   *
   * @param lhs 左侧栈。
   * @param rhs 右侧栈。
   * @return 如果左侧栈小于右侧栈，返回 `true`，否则返回 `false`。
   */
  friend bool operator<(const stack& lhs, const stack& rhs) {
    return lhs.c_ < rhs.c_;
  }
};

// 重载比较操作符

/**
 * @brief 检查两个栈是否相等。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 * @return 如果两个栈相等，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return lhs == rhs;
}

/**
 * @brief 检查左侧栈是否小于右侧栈。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 * @return 如果左侧栈小于右侧栈，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return lhs < rhs;
}

/**
 * @brief 检查两个栈是否不相等。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 * @return 如果两个栈不相等，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs == rhs);
}

/**
 * @brief 检查左侧栈是否大于右侧栈。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 * @return 如果左侧栈大于右侧栈，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
  return rhs < lhs;
}

/**
 * @brief 检查左侧栈是否小于或等于右侧栈。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 * @return 如果左侧栈小于或等于右侧栈，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(rhs < lhs);
}

/**
 * @brief 检查左侧栈是否大于或等于右侧栈。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 * @return 如果左侧栈大于或等于右侧栈，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
  return !(lhs < rhs);
}

// 重载 ccystl 的 swap

/**
 * @brief 交换两个栈的内容。
 *
 * @tparam T 栈中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧栈。
 * @param rhs 右侧栈。
 */
template <class T, class Container>
void swap(stack<T, Container>& lhs,
          stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}
}  // namespace ccystl
#endif  // !CCYSTL_STACK_H_
