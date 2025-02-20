#ifndef _CCYSTL_CONSTRUCT_H_
#define _CCYSTL_CONSTRUCT_H_

/**
 * @file construct.h
 * @brief 该头文件定义了 ccystl
 * 中的对象构造和析构函数，包括构造对象和销毁对象的操作。
 *
 * 提供了使用 placement new 进行对象构造的模板函数，以及用于安全析构对象的函数。
 */

#include "ccystl/iterator/iterator.h"
#include "ccystl/utils/utils.h"

namespace ccystl {
/**
 * @brief 使用 placement new 在指定内存位置构造对象的默认构造函数。
 *
 * @tparam Ty 要构造的对象类型。
 * @param ptr 指向要构造对象的内存的指针。
 *
 * @note 该函数在 ptr 指向的内存位置使用默认构造函数构造对象。
 */
template <class Ty>
void construct(Ty* ptr) {
  ::new (static_cast<void*>(ptr)) Ty();
}

/**
 * @brief 使用 placement new 在指定内存位置构造对象，并用指定值进行初始化。
 *
 * @tparam Ty1 要构造的对象类型。
 * @tparam Ty2 用于初始化对象的值的类型。
 * @param ptr 指向要构造对象的内存的指针。
 * @param value 用于初始化对象的值。
 *
 * @note 该函数在 ptr 指向的内存位置使用 value 进行对象的拷贝构造。
 */
template <class Ty1, class Ty2>
void construct(Ty1* ptr, const Ty2& value) {
  ::new (static_cast<void*>(ptr)) Ty1(value);
}

/**
 * @brief 使用 placement new 在指定内存位置构造对象，并使用可变参数进行初始化。
 *
 * @tparam Ty 要构造的对象类型。
 * @tparam Args 用于构造对象的参数类型。
 * @param ptr 指向要构造对象的内存的指针。
 * @param args 用于初始化对象的参数。
 *
 * @note 该函数使用传入的参数在 ptr 指向的内存位置构造对象。
 */
template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args) {
  ::new (static_cast<void*>(ptr)) Ty(ccystl::forward<Args>(args)...);
}

/**
 * @brief 通过类型属性判断是否需要调用析构函数。
 *
 * 如果类型是 trivially destructible，则不做操作，否则调用析构函数。
 *
 * @tparam Ty 要销毁的对象类型。
 * @param ptr 指向要销毁的对象的指针。
 */
template <class Ty>
void destroy_one(Ty* ptr, std::true_type) {}

/**
 * @brief 销毁对象，调用其析构函数。
 *
 * 如果指针非空且类型不是 trivially destructible，则调用对象的析构函数。
 *
 * @tparam Ty 要销毁的对象类型。
 * @param ptr 指向要销毁的对象的指针。
 */
template <class Ty>
void destroy_one(Ty* ptr, std::false_type) {
  if (ptr != nullptr) {
    ptr->~Ty();
  }
}

/**
 * @brief 根据类型属性决定是否需要销毁范围内的对象。
 *
 * 如果对象类型是 trivially
 * destructible，则不做操作，否则对范围内的每个对象调用析构函数。
 *
 * @tparam ForwardIter 迭代器类型。
 * @param first 指向要销毁的第一个对象的迭代器。
 * @param last 指向要销毁的最后一个对象之后的迭代器。
 */
template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::true_type) {}

/**
 * @brief 对范围内的对象调用析构函数。
 *
 * @tparam ForwardIter 迭代器类型。
 * @param first 指向要销毁的第一个对象的迭代器。
 * @param last 指向要销毁的最后一个对象之后的迭代器。
 */
template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
  for (; first != last; ++first) {
    destroy(&*first);
  }
}

/**
 * @brief 销毁单个对象，调用其析构函数。
 *
 * 根据类型的 trivially destructible 属性决定是否调用析构函数。
 *
 * @tparam Ty 要销毁的对象类型。
 * @param ptr 指向要销毁的对象的指针。
 */
template <class Ty>
void destroy(Ty* ptr) {
  destroy_one(ptr, std::is_trivially_destructible<Ty>{});
}

/**
 * @brief 销毁范围内的对象，调用它们的析构函数。
 *
 * 根据对象类型的 trivially destructible 属性决定是否调用析构函数。
 *
 * @tparam ForwardIter 迭代器类型。
 * @param first 指向要销毁的第一个对象的迭代器。
 * @param last 指向要销毁的最后一个对象之后的迭代器。
 */
template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last) {
  destroy_cat(first, last,
              std::is_trivially_destructible<
                  typename iterator_traits<ForwardIter>::value_type>{});
}
}  // namespace ccystl

#endif  // _CCYSTL_CONSTRUCT_H_
