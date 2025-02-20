#ifndef CCYSTL_PRIORITY_QUEUE_H_
#define CCYSTL_PRIORITY_QUEUE_H_

/**
 * @file priority_queue.h
 * @brief 该头文件包含了模板类 priority_queue 的定义。
 *
 * priority_queue 是一个优先队列容器，支持基本的优先队列操作。
 */

#include "ccystl/algorithm/heap_algo.h"
#include "ccystl/container/sequence_container/deque.h"
#include "ccystl/container/sequence_container/vector.h"
#include "ccystl/functor/functional.h"

namespace ccystl {
/**
 * @brief 一个基于堆实现的优先队列类。
 *
 * 这是一个优先队列的模板类。该队列使用堆结构维护元素，
 * 顶部元素为具有最高优先级的元素，优先级根据提供的比较函数确定。
 *
 * @tparam T 队列中存储的元素类型。
 * @tparam Container 底层容器类型，用于存储元素，默认为 `vector<T>`。
 * @tparam Compare 用于比较元素优先级的函数对象类型，默认为
 * `less<typename Container::value_type>`。
 */
template <class T, class Container = vector<T>,
          class Compare = less<typename Container::value_type>>
class priority_queue {
 public:
  using container_type = Container;                   ///< 底层容器的类型。
  using value_compare = Compare;                      ///< 比较函数对象的类型。
  using value_type = typename Container::value_type;  ///< 队列中元素的类型。
  using size_type = typename Container::size_type;    ///< 用于表示大小的类型。
  using reference = typename Container::reference;    ///< 队列元素的引用类型。
  using const_reference =
      typename Container::const_reference;  ///< 队列元素的常量引用类型。

  static_assert(std::is_same_v<T, value_type>,
                "底层容器的 value_type 应该与 T 类型相同");

 private:
  container_type c_;    ///< 用于存储元素的底层容器。
  value_compare comp_;  ///< 用于维护堆顺序的比较对象。

 public:
  // 构造、复制和移动函数

  /**
   * @brief 默认构造函数。
   *
   * 构造一个空优先队列。
   */
  priority_queue() = default;

  /**
   * @brief 使用指定的比较函数构造一个空优先队列。
   *
   * @param c 用于排序元素的比较函数。
   */
  explicit priority_queue(const Compare& c) : c_(), comp_(c) {}

  /**
   * @brief 构造一个包含 `n` 个默认构造元素的优先队列。
   *
   * @param n 要创建的元素数量。
   */
  explicit priority_queue(size_type n) : c_(n) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 构造一个包含 `n` 个指定值的优先队列。
   *
   * @param n 要创建的元素数量。
   * @param value 用于初始化每个元素的值。
   */
  priority_queue(size_type n, const value_type& value) : c_(n, value) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 从指定范围的元素构造优先队列。
   *
   * @tparam IIter 输入迭代器类型。
   * @param first 范围的起始迭代器。
   * @param last 范围的结束迭代器。
   */
  template <class IIter>
  priority_queue(IIter first, IIter last) : c_(first, last) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 从初始化列表构造优先队列。
   *
   * @param initializer_list 用于构造的初始化列表。
   */
  priority_queue(std::initializer_list<T> initializer_list)
      : c_(initializer_list) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 从现有容器构造优先队列。
   *
   * @param s 用于初始化优先队列的容器。
   */
  explicit priority_queue(const Container& s) : c_(s) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 从现有容器移动构造优先队列。
   *
   * @param s 要移动到优先队列的容器。
   */
  explicit priority_queue(Container&& s) : c_(ccystl::move(s)) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 拷贝构造函数。
   *
   * @param rhs 要拷贝的优先队列。
   */
  priority_queue(const priority_queue& rhs) : c_(rhs.c_), comp_(rhs.comp_) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 移动构造函数。
   *
   * @param rhs 要移动的优先队列。
   */
  priority_queue(priority_queue&& rhs) noexcept
      : c_(ccystl::move(rhs.c_)), comp_(rhs.comp_) {
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 拷贝赋值操作符。
   *
   * @param rhs 要拷贝的优先队列。
   * @return 引用指向赋值后的对象。
   */
  priority_queue& operator=(const priority_queue& rhs) {
    c_ = rhs.c_;
    comp_ = rhs.comp_;
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
    return *this;
  }

  /**
   * @brief 移动赋值操作符。
   *
   * @param rhs 要移动的优先队列。
   * @return 引用指向赋值后的对象。
   */
  priority_queue& operator=(priority_queue&& rhs) noexcept {
    c_ = ccystl::move(rhs.c_);
    comp_ = rhs.comp_;
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
    return *this;
  }

  /**
   * @brief 初始化列表赋值操作符。
   *
   * @param initializer_list 用于赋值的初始化列表。
   * @return 引用指向赋值后的对象。
   */
  priority_queue& operator=(std::initializer_list<T> initializer_list) {
    c_ = initializer_list;
    comp_ = value_compare();
    ccystl::make_heap(c_.begin(), c_.end(), comp_);
    return *this;
  }

  /**
   * @brief 析构函数。
   */
  ~priority_queue() = default;

  // 访问元素相关操作

  /**
   * @brief 返回优先队列中的最高优先级元素的引用。
   *
   * @return 常量引用指向队列顶部元素。
   */
  const_reference top() const { return c_.front(); }

  // 容量相关操作

  /**
   * @brief 检查优先队列是否为空。
   *
   * @return 如果队列为空，返回 `true`，否则返回 `false`。
   */
  [[nodiscard]] bool empty() const noexcept { return c_.empty(); }

  /**
   * @brief 返回优先队列中元素的数量。
   *
   * @return 队列中的元素数量。
   */
  size_type size() const noexcept { return c_.size(); }

  // 修改容器相关操作

  /**
   * @brief 在优先队列中插入一个新元素。
   *
   * 元素使用传入的参数构造。
   *
   * @tparam Args 转发给构造函数的参数类型。
   * @param args 转发给构造函数的参数。
   */
  template <class... Args>
  void emplace(Args&&... args) {
    c_.emplace_back(ccystl::forward<Args>(args)...);
    ccystl::push_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 在优先队列中插入一个新元素。
   *
   * 元素会被复制到队列中。
   *
   * @param value 要插入的元素值。
   */
  void push(const value_type& value) {
    c_.push_back(value);
    ccystl::push_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 在优先队列中插入一个新元素。
   *
   * 元素会被移动到队列中。
   *
   * @param value 要插入的元素值。
   */
  void push(value_type&& value) {
    c_.push_back(ccystl::move(value));
    ccystl::push_heap(c_.begin(), c_.end(), comp_);
  }

  /**
   * @brief 移除优先队列中的最高优先级元素。
   */
  void pop() {
    ccystl::pop_heap(c_.begin(), c_.end(), comp_);
    c_.pop_back();
  }

  /**
   * @brief 移除优先队列中的所有元素。
   */
  void clear() {
    while (!empty()) pop();
  }

  /**
   * @brief 交换两个优先队列的内容。
   *
   * @param rhs 要交换的另一个优先队列。
   */
  void swap(priority_queue& rhs) noexcept(noexcept(ccystl::swap(c_, rhs.c_)) &&
                                          noexcept(ccystl::swap(comp_,
                                                                rhs.comp_))) {
    ccystl::swap(c_, rhs.c_);
    ccystl::swap(comp_, rhs.comp_);
  }

  // 比较操作符

  /**
   * @brief 检查两个优先队列是否相等。
   *
   * @param lhs 左侧优先队列。
   * @param rhs 右侧优先队列。
   * @return 如果两个队列相等，返回 `true`，否则返回 `false`。
   */
  friend bool operator==(const priority_queue& lhs, const priority_queue& rhs) {
    return lhs.c_ == rhs.c_;
  }

  /**
   * @brief 检查两个优先队列是否不相等。
   *
   * @param lhs 左侧优先队列。
   * @param rhs 右侧优先队列。
   * @return 如果两个队列不相等，返回 `true`，否则返回 `false`。
   */
  friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs) {
    return lhs.c_ != rhs.c_;
  }
};

// 重载比较操作符

/**
 * @brief 检查两个优先队列是否相等。
 *
 * @tparam T 队列中存储的元素类型。
 * @tparam Container 底层容器类型，用于存储元素。
 * @tparam Compare 用于比较元素优先级的函数对象类型。
 * @param lhs 左侧优先队列。
 * @param rhs 右侧优先队列。
 * @return 如果两个队列相等，返回 `true`，否则返回 `false`。
 */
template <class T, class Container, class Compare>
bool operator==(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) {
  return lhs == rhs;
}

/**
 * @brief 检查两个优先队列是否不相等。
 *
 * @tparam T 队列中存储的元素类型。
 * @tparam Container 底层容器类型，用于存储元素。
 * @tparam Compare 用于比较元素优先级的函数对象类型。
 * @param lhs 左侧优先队列。
 * @param rhs 右侧优先队列。
 * @return 如果两个队列不相等，返回 `true`，否则返回 `false`。
 */
template <class T, class Container, class Compare>
bool operator!=(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) {
  return lhs != rhs;
}

// 重载 ccystl 的 swap

/**
 * @brief 交换两个优先队列的内容。
 *
 * @tparam T 队列中存储的元素类型。
 * @tparam Container 底层容器类型，用于存储元素。
 * @tparam Compare 用于比较元素优先级的函数对象类型。
 * @param lhs 左侧优先队列。
 * @param rhs 右侧优先队列。
 */
template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
          priority_queue<T, Container, Compare>&
              rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}
}  // namespace ccystl

#endif  // !CCYSTL_PRIORITY_QUEUE_H_
