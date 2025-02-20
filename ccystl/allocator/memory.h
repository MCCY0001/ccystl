#ifndef CCYSTL_MEMORY_H_
#define CCYSTL_MEMORY_H_

/**
 * @file memory.h
 * @brief
 * 该头文件负责高级动态内存管理，包含基本函数、空间配置器、未初始化的储存空间管理，以及一个模板类
 * `auto_ptr`。
 */

#include <climits>
#include <cstddef>
#include <cstdlib>

#include "ccystl/allocator/construct.h"
#include "ccystl/allocator/uninitialized.h"

namespace ccystl {
/**
 * @brief 获取对象的地址。
 *
 * @tparam Tp 对象的类型。
 * @param value 要获取地址的对象。
 * @return constexpr Tp* 对象的指针。
 */
template <class Tp>
constexpr Tp* address_of(Tp& value) noexcept {
  return &value;
}

/**
 * @brief 获取临时缓冲区的辅助函数。
 *
 * 尝试分配指定大小的缓冲区，如果分配失败则逐步减小大小重试。
 *
 * @tparam T 缓冲区中元素的类型。
 * @param len 请求的缓冲区大小。
 * @param unused 用于类型推断的指针（不使用）。
 * @return pair<T*, ptrdiff_t> 返回一个指向分配内存的指针及其大小的对。
 */
template <class T>
pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T* unused) {
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
    len = INT_MAX / sizeof(T);
  while (len > 0) {
    if (T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T))))
      return pair<T*, ptrdiff_t>(tmp, len);
    len /= 2;  // 申请失败时减少 len 的大小
  }
  return pair<T*, ptrdiff_t>(nullptr, 0);
}

/**
 * @brief 获取临时缓冲区。
 *
 * 尝试分配指定大小的缓冲区，如果分配失败则逐步减小大小重试。
 *
 * @tparam T 缓冲区中元素的类型。
 * @param len 请求的缓冲区大小。
 * @return pair<T*, ptrdiff_t> 返回一个指向分配内存的指针及其大小的对。
 */
template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
  return get_buffer_helper(len, static_cast<T*>(nullptr));
}

/**
 * @brief 获取临时缓冲区。
 *
 * 尝试分配指定大小的缓冲区，如果分配失败则逐步减小大小重试。
 *
 * @tparam T 缓冲区中元素的类型。
 * @param len 请求的缓冲区大小。
 * @param unused 用于类型推断的指针（不使用）。
 * @return pair<T*, ptrdiff_t> 返回一个指向分配内存的指针及其大小的对。
 */
template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T* unused) {
  return get_buffer_helper(len, static_cast<T*>(nullptr));
}

/**
 * @brief 释放临时缓冲区。
 *
 * 释放由 `get_temporary_buffer` 分配的缓冲区。
 *
 * @tparam T 缓冲区中元素的类型。
 * @param ptr 指向要释放的缓冲区的指针。
 */
template <class T>
void release_temporary_buffer(T* ptr) {
  free(ptr);
}

/**
 * @brief 类模板 `temporary_buffer` 进行临时缓冲区的申请与释放。
 *
 * 该类用于在需要时动态申请和释放一段临时的内存缓冲区。
 *
 * @tparam ForwardIterator 前向迭代器类型。
 * @tparam T 缓冲区中元素的类型。
 */
template <class ForwardIterator, class T>
class temporary_buffer {
 private:
  ptrdiff_t original_len{};  ///< 缓冲区申请的大小
  ptrdiff_t len;             ///< 缓冲区实际的大小
  T* buffer;                 ///< 指向缓冲区的指针

 public:
  /**
   * @brief 构造函数，初始化缓冲区。
   *
   * @param first 缓冲区的起始迭代器。
   * @param last 缓冲区的结束迭代器。
   */
  temporary_buffer(ForwardIterator first, ForwardIterator last);

  // 禁止拷贝构造和赋值
  temporary_buffer(const temporary_buffer&) = delete;

  /**
   * @brief 析构函数，释放缓冲区。
   */
  ~temporary_buffer() {
    ccystl::destroy(buffer, buffer + len);
    free(buffer);
  }

  void operator=(const temporary_buffer&) = delete;

  /**
   * @brief 返回缓冲区的大小。
   *
   * @return ptrdiff_t 缓冲区的实际大小。
   */
  [[nodiscard]] ptrdiff_t size() const noexcept { return len; }

  /**
   * @brief 返回请求的缓冲区大小。
   *
   * @return ptrdiff_t 缓冲区的请求大小。
   */
  [[nodiscard]] ptrdiff_t requested_size() const noexcept {
    return original_len;
  }

  /**
   * @brief 返回缓冲区的起始指针。
   *
   * @return T* 指向缓冲区起始位置的指针。
   */
  T* begin() noexcept { return buffer; }

  /**
   * @brief 返回缓冲区的结束指针。
   *
   * @return T* 指向缓冲区结束位置的指针。
   */
  T* end() noexcept { return buffer + len; }

 private:
  /**
   * @brief 分配缓冲区内存。
   */
  void allocate_buffer();

  /**
   * @brief 初始化缓冲区，根据类型是否为平凡构造进行处理。
   *
   * @param value 用于初始化缓冲区的值。
   * @param is_trivial 表示类型是否为平凡构造的类型。
   */
  static void initialize_buffer(const T& value, std::true_type is_trivial) {}

  void initialize_buffer(const T& value, std::false_type) {
    ccystl::uninitialized_fill_n(buffer, len, value);
  }
};

// temporary_buffer 构造函数实现
template <class ForwardIterator, class T>
temporary_buffer<ForwardIterator, T>::temporary_buffer(ForwardIterator first,
                                                       ForwardIterator last) {
  try {
    len = ccystl::distance(first, last);
    allocate_buffer();
    if (len > 0) {
      initialize_buffer(*first, std::is_trivially_default_constructible<T>());
    }
  } catch (...) {
    free(buffer);
    buffer = nullptr;
    len = 0;
  }
}

// allocate_buffer 函数实现
template <class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::allocate_buffer() {
  original_len = len;
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
    len = INT_MAX / sizeof(T);
  while (len > 0) {
    buffer = static_cast<T*>(malloc(len * sizeof(T)));
    if (buffer) break;
    len /= 2;  // 申请失败时减少申请空间大小
  }
}

/**
 * @brief 模板类 `auto_ptr`，实现严格对象所有权的小型智能指针。
 *
 * `auto_ptr` 用于管理动态分配的对象，并负责对象的生命周期管理。
 *
 * @tparam T 指针所管理的对象类型。
 */
template <class T>
class auto_ptr {
 public:
  using elem_type = T;  ///< 指针所管理的对象类型

 private:
  T* m_ptr;  ///< 实际指针

 public:
  /**
   * @brief 构造函数，初始化智能指针。
   *
   * @param p 初始化指针，默认为 nullptr。
   */
  explicit auto_ptr(T* p = nullptr) : m_ptr(p) {}

  /**
   * @brief 拷贝构造函数，转移对象所有权。
   *
   * @param rhs 被拷贝的 `auto_ptr`。
   */
  auto_ptr(auto_ptr& rhs) : m_ptr(rhs.release()) {}

  /**
   * @brief 模板拷贝构造函数，转移对象所有权。
   *
   * @tparam U 被拷贝的 `auto_ptr` 管理的对象类型。
   * @param rhs 被拷贝的 `auto_ptr`。
   */
  template <class U>
  explicit auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {}

  /**
   * @brief 赋值操作符，转移对象所有权。
   *
   * @param rhs 被赋值的 `auto_ptr`。
   * @return auto_ptr& 当前 `auto_ptr`。
   */
  auto_ptr& operator=(const auto_ptr& rhs) {
    if (this != &rhs) {
      delete m_ptr;
      m_ptr = rhs.release();
    }
    return *this;
  }

  /**
   * @brief 模板赋值操作符，转移对象所有权。
   *
   * @tparam U 被赋值的 `auto_ptr` 管理的对象类型。
   * @param rhs 被赋值的 `auto_ptr`。
   * @return auto_ptr& 当前 `auto_ptr`。
   */
  template <class U>
  auto_ptr& operator=(auto_ptr<U>& rhs) {
    if (this->get() != rhs.get()) {
      delete m_ptr;
      m_ptr = rhs.release();
    }
    return *this;
  }

  /**
   * @brief 析构函数，释放所管理的对象。
   */
  ~auto_ptr() { delete m_ptr; }

 public:
  /**
   * @brief 重载 `operator*`，访问所管理的对象。
   *
   * @return T& 返回所管理的对象的引用。
   */
  T& operator*() const { return *m_ptr; }

  /**
   * @brief 重载 `operator->`，访问所管理的对象的成员。
   *
   * @return T* 返回所管理的对象的指针。
   */
  T* operator->() const { return m_ptr; }

  /**
   * @brief 获取所管理的对象的指针。
   *
   * @return T* 返回所管理的对象的指针。
   */
  T* get() const { return m_ptr; }

  /**
   * @brief 释放所管理的对象的所有权。
   *
   * @return T* 返回所管理的对象的指针，并将内部指针置为空。
   */
  T* release() {
    T* tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  /**
   * @brief 重置所管理的对象。
   *
   * @param p 新的指针，默认为 nullptr。
   */
  void reset(T* p = nullptr) {
    if (m_ptr != p) {
      delete m_ptr;
      m_ptr = p;
    }
  }
};
}  // namespace ccystl

#endif  // CCYSTL_MEMORY_H_
