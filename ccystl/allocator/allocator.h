#ifndef CCYSTL_ALLOCATOR_H_
#define CCYSTL_ALLOCATOR_H_

/**
 * @file allocator.h
 * @brief 该头文件定义了 ccystl 的内存分配器类
 * `allocator`，用于对象的内存分配、构造和析构。
 *
 * 通过封装标准的内存管理操作，`allocator` 提供了一套用于分配和释放内存的接口。
 */

#include "ccystl/allocator/construct.h"
#include "ccystl/utils/utils.h"

namespace ccystl {
/**
 * @brief 内存分配器类，用于对象的内存分配、构造和析构。
 *
 * 该类封装了内存分配、释放和对象构造、析构的操作，提供了简单的接口来管理对象的生命周期。
 *
 * @tparam T 分配器管理的对象类型。
 */
template <class T>
class allocator {
 public:
  using value_type = T;               ///< 对象类型
  using pointer = T*;                 ///< 指针类型
  using const_pointer = const T*;     ///< 常量指针类型
  using reference = T&;               ///< 引用类型
  using const_reference = const T&;   ///< 常量引用类型
  using size_type = size_t;           ///< 大小类型
  using difference_type = ptrdiff_t;  ///< 指针差值类型

 public:
  /**
   * @brief 分配单个对象的内存。
   *
   * 使用全局 `::operator new` 分配足够大的内存来存储一个类型为 T 的对象。
   *
   * @return T* 指向分配的内存的指针。
   */
  static T* allocate();

  /**
   * @brief 分配多个对象的内存。
   *
   * 使用全局 `::operator new` 分配足够大的内存来存储 n 个类型为 T 的对象。
   *
   * @param n 要分配的对象数量。
   * @return T* 指向分配的内存的指针，如果 n 为 0 则返回 nullptr。
   */
  static T* allocate(size_type n);

  /**
   * @brief 释放单个对象的内存。
   *
   * 使用全局 `::operator delete` 释放由 `allocate` 分配的内存。
   *
   * @param ptr 指向要释放的内存的指针。
   */
  static void deallocate(T* ptr);

  /**
   * @brief 释放多个对象的内存。
   *
   * 使用全局 `::operator delete` 释放由 `allocate` 分配的内存。
   *
   * @param ptr 指向要释放的内存的指针。
   * @param n 要释放的对象数量（未实际使用，仅作兼容）。
   */
  static void deallocate(T* ptr, size_type n);

  /**
   * @brief 在分配的内存上默认构造对象。
   *
   * 使用 `ccystl::construct` 在指向的内存位置上构造一个对象。
   *
   * @param ptr 指向要构造对象的内存的指针。
   */
  static void construct(T* ptr);

  /**
   * @brief 在分配的内存上构造对象，并使用指定的值进行初始化。
   *
   * 使用 `ccystl::construct`
   * 在指向的内存位置上构造一个对象，并用传入的值进行初始化。
   *
   * @param ptr 指向要构造对象的内存的指针。
   * @param value 用于初始化对象的值。
   */
  static void construct(T* ptr, const T& value);

  /**
   * @brief 在分配的内存上构造对象，并使用右值引用的值进行初始化。
   *
   * 使用 `ccystl::construct`
   * 在指向的内存位置上构造一个对象，并用右值引用进行初始化。
   *
   * @param ptr 指向要构造对象的内存的指针。
   * @param value 用于初始化对象的右值引用。
   */
  static void construct(T* ptr, T&& value);

  /**
   * @brief 在分配的内存上构造对象，并使用可变参数进行初始化。
   *
   * 使用 `ccystl::construct`
   * 在指向的内存位置上构造一个对象，并用传入的可变参数进行初始化。
   *
   * @tparam Args 用于构造对象的参数类型。
   * @param ptr 指向要构造对象的内存的指针。
   * @param args 用于初始化对象的参数。
   */
  template <class... Args>
  static void construct(T* ptr, Args&&... args);

  /**
   * @brief 调用单个对象的析构函数。
   *
   * 使用 `ccystl::destroy` 销毁对象，调用其析构函数而不释放内存。
   *
   * @param ptr 指向要销毁的对象的指针。
   */
  static void destroy(T* ptr);

  /**
   * @brief 调用多个对象的析构函数。
   *
   * 使用 `ccystl::destroy` 销毁范围内的对象，调用其析构函数而不释放内存。
   *
   * @param first 指向要销毁的第一个对象的指针。
   * @param last 指向要销毁的最后一个对象之后的指针。
   */
  static void destroy(T* first, T* last);
};

// 方法实现

template <class T>
T* allocator<T>::allocate() {
  return static_cast<T*>(::operator new(sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n) {
  if (n == 0) return nullptr;
  return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <class T>
void allocator<T>::deallocate(T* ptr) {
  if (ptr == nullptr) return;
  ::operator delete(ptr);
}

template <class T>
void allocator<T>::deallocate(T* ptr, size_type /*size*/) {
  if (ptr == nullptr) return;
  ::operator delete(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr) {
  ccystl::construct(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr, const T& value) {
  ccystl::construct(ptr, value);
}

template <class T>
void allocator<T>::construct(T* ptr, T&& value) {
  ccystl::construct(ptr, ccystl::move(value));
}

template <class T>
template <class... Args>
void allocator<T>::construct(T* ptr, Args&&... args) {
  ccystl::construct(ptr, ccystl::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destroy(T* ptr) {
  ccystl::destroy(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last) {
  ccystl::destroy(first, last);
}
}  // namespace ccystl

#endif  // CCYSTL_ALLOCATOR_H_
