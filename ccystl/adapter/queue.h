#ifndef CCYSTL_QUEUE_H_
#define CCYSTL_QUEUE_H_

/**
 * @file queue.h
 * @brief 该头文件包含了模板类 queue 的定义，基于 deque。
 *
 * queue 是一个队列容器，支持基本的队列操作，如入队、出队等。
 */

#include "ccystl/container/sequence_container/deque.h"
#include "ccystl/container/sequence_container/vector.h"

namespace ccystl {
/**
 * @brief 模板类 queue，表示一个队列容器。
 *
 * 队列是一种先进先出（FIFO）的数据结构。此类提供基本的队列操作，如入队、出队、查看队首和队尾元素等。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型，默认为 `ccystl::deque<T>`。
 */
template <class T, class Container = deque<T>>
class queue {
 public:
  using container_type = Container;                   ///< 底层容器的类型。
  using value_type = typename Container::value_type;  ///< 队列中元素的类型。
  using size_type = typename Container::size_type;    ///< 用于表示大小的类型。
  using reference = typename Container::reference;    ///< 队列元素的引用类型。
  using const_reference =
      typename Container::const_reference;  ///< 队列元素的常量引用类型。

  static_assert(std::is_same_v<T, value_type>,
                "底层容器的 value_type 应该与 T 类型相同");

 private:
  container_type c_;  ///< 底层容器，用于存储队列中的元素。

 public:
  // 构造、复制、移动函数

  /**
   * @brief 默认构造函数。
   *
   * 构造一个空的队列。
   */
  queue() = default;

  /**
   * @brief 构造一个包含 `n` 个默认构造元素的队列。
   *
   * @param n 要创建的元素数量。
   */
  explicit queue(size_type n) : c_(n) {}

  /**
   * @brief 构造一个包含 `n` 个指定值的队列。
   *
   * @param n 要创建的元素数量。
   * @param value 用于初始化每个元素的值。
   */
  queue(size_type n, const value_type& value) : c_(n, value) {}

  /**
   * @brief 从指定范围的元素构造队列。
   *
   * @tparam IIter 输入迭代器类型。
   * @param first 范围的起始迭代器。
   * @param last 范围的结束迭代器。
   */
  template <class IIter>
  queue(IIter first, IIter last) : c_(first, last) {}

  /**
   * @brief 从初始化列表构造队列。
   *
   * @param initializer_list 用于构造的初始化列表。
   */
  queue(std::initializer_list<T> initializer_list)
      : c_(initializer_list.begin(), initializer_list.end()) {}

  /**
   * @brief 从现有容器构造队列。
   *
   * @param c 用于初始化队列的容器。
   */
  explicit queue(const Container& c) : c_(c) {}

  /**
   * @brief 从现有容器移动构造队列。
   *
   * @param c 要移动到队列的容器。
   */
  explicit queue(Container&& c) noexcept(
      std::is_nothrow_move_constructible_v<Container>)
      : c_(ccystl::move(c)) {}

  /**
   * @brief 拷贝构造函数。
   *
   * @param rhs 要拷贝的队列。
   */
  queue(const queue& rhs) : c_(rhs.c_) {}

  /**
   * @brief 移动构造函数。
   *
   * @param rhs 要移动的队列。
   */
  queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible_v<Container>)
      : c_(ccystl::move(rhs.c_)) {}

  /**
   * @brief 拷贝赋值操作符。
   *
   * @param rhs 要拷贝的队列。
   * @return 引用指向赋值后的对象。
   */
  queue& operator=(const queue& rhs) = default;

  /**
   * @brief 移动赋值操作符。
   *
   * @param rhs 要移动的队列。
   * @return 引用指向赋值后的对象。
   */
  queue& operator=(queue&& rhs) noexcept(
      std::is_nothrow_move_assignable_v<Container>) {
    c_ = ccystl::move(rhs.c_);
    return *this;
  }

  /**
   * @brief 初始化列表赋值操作符。
   *
   * @param initializer_list 用于赋值的初始化列表。
   * @return 引用指向赋值后的对象。
   */
  queue& operator=(std::initializer_list<T> initializer_list) {
    c_ = initializer_list;
    return *this;
  }

  /**
   * @brief 析构函数。
   */
  ~queue() = default;

  // 访问元素相关操作

  /**
   * @brief 返回队列中第一个元素的引用。
   *
   * @return 引用指向队列的第一个元素。
   */
  reference front() { return c_.front(); }

  /**
   * @brief 返回队列中第一个元素的常量引用。
   *
   * @return 常量引用指向队列的第一个元素。
   */
  const_reference front() const { return c_.front(); }

  /**
   * @brief 返回队列中最后一个元素的引用。
   *
   * @return 引用指向队列的最后一个元素。
   */
  reference back() { return c_.back(); }

  /**
   * @brief 返回队列中最后一个元素的常量引用。
   *
   * @return 常量引用指向队列的最后一个元素。
   */
  const_reference back() const { return c_.back(); }

  // 容量相关操作

  /**
   * @brief 检查队列是否为空。
   *
   * @return 如果队列为空，返回 `true`，否则返回 `false`。
   */
  [[nodiscard]] bool empty() const noexcept { return c_.empty(); }

  /**
   * @brief 返回队列中元素的数量。
   *
   * @return 队列中的元素数量。
   */
  size_type size() const noexcept { return c_.size(); }

  // 修改容器相关操作

  /**
   * @brief 在队列中插入一个新元素。
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
   * @brief 在队列中插入一个新元素。
   *
   * 元素会被复制到队列中。
   *
   * @param value 要插入的元素值。
   */
  void push(const value_type& value) { c_.push_back(value); }

  /**
   * @brief 在队列中插入一个新元素。
   *
   * 元素会被移动到队列中。
   *
   * @param value 要插入的元素值。
   */
  void push(value_type&& value) { c_.emplace_back(ccystl::move(value)); }

  /**
   * @brief 移除队列中的第一个元素。
   */
  void pop() { c_.pop_front(); }

  /**
   * @brief 移除队列中的所有元素。
   */
  void clear() {
    while (!empty()) pop();
  }

  /**
   * @brief 交换两个队列的内容。
   *
   * @param rhs 要交换的另一个队列。
   */
  void swap(queue& rhs) noexcept(noexcept(ccystl::swap(c_, rhs.c_))) {
    ccystl::swap(c_, rhs.c_);
  }

  // 比较操作符

  /**
   * @brief 检查两个队列是否相等。
   *
   * @param lhs 左侧队列。
   * @param rhs 右侧队列。
   * @return 如果两个队列相等，返回 `true`，否则返回 `false`。
   */
  friend bool operator==(const queue& lhs, const queue& rhs) {
    return lhs.c_ == rhs.c_;
  }

  /**
   * @brief 检查左侧队列是否小于右侧队列。
   *
   * @param lhs 左侧队列。
   * @param rhs 右侧队列。
   * @return 如果左侧队列小于右侧队列，返回 `true`，否则返回 `false`。
   */
  friend bool operator<(const queue& lhs, const queue& rhs) {
    return lhs.c_ < rhs.c_;
  }
};

// 重载比较操作符

/**
 * @brief 检查两个队列是否相等。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 * @return 如果两个队列相等，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator==(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
  return lhs == rhs;
}

/**
 * @brief 检查两个队列是否不相等。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 * @return 如果两个队列不相等，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
  return !(lhs == rhs);
}

/**
 * @brief 检查左侧队列是否小于右侧队列。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 * @return 如果左侧队列小于右侧队列，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
  return lhs < rhs;
}

/**
 * @brief 检查左侧队列是否大于右侧队列。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 * @return 如果左侧队列大于右侧队列，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
  return rhs < lhs;
}

/**
 * @brief 检查左侧队列是否小于或等于右侧队列。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 * @return 如果左侧队列小于或等于右侧队列，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
  return !(rhs < lhs);
}

/**
 * @brief 检查左侧队列是否大于或等于右侧队列。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 * @return 如果左侧队列大于或等于右侧队列，返回 `true`，否则返回 `false`。
 */
template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
  return !(lhs < rhs);
}

// 重载 ccystl 的 swap

/**
 * @brief 交换两个队列的内容。
 *
 * @tparam T 队列中元素的类型。
 * @tparam Container 用于存储元素的底层容器类型。
 * @param lhs 左侧队列。
 * @param rhs 右侧队列。
 */
template <class T, class Container>
void swap(queue<T, Container>& lhs,
          queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}
}  // namespace ccystl
#endif  // !CCYSTL_QUEUE_H_
