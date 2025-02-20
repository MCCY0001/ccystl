#ifndef CCYSTL_HEAP_ALGO_H_
#define CCYSTL_HEAP_ALGO_H_

/**
 * @file heap_algo.h
 * @brief 该头文件包含了堆的四个算法：push_heap, pop_heap, sort_heap,
 * make_heap。
 *
 * 这些算法用于管理和操作堆数据结构，支持最大堆或自定义比较的堆操作。
 */

#include "ccystl/iterator/iterator.h"

namespace ccystl {
/**
 * @brief 插入新元素到堆中，并保持堆的性质。
 *
 * 该函数接受两个迭代器，表示一个堆的首尾，并假设新元素已经插入到底层容器的最尾端。
 * 通过上溯调整，使得容器重新符合堆的性质（默认最大堆）。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Distance 用于计算距离的类型。
 * @tparam T 元素类型。
 * @param first 容器的起始迭代器。
 * @param holeIndex 待调整元素的索引。
 * @param topIndex 堆顶元素的索引。
 * @param value 待插入的元素值。
 */
template <class RandomIter, class Distance, class T>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex,
                   T value) {
  auto parent = (holeIndex - 1) / 2;  // 计算父节点索引
  while (holeIndex > topIndex && *(first + parent) < value) {
    // 上溯调整
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;  // 插入新值
}

/**
 * @brief 对堆进行 push 操作的辅助函数，适用于默认比较规则。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Distance 用于计算距离的类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param 迭代器距离类型指针。
 */
template <class RandomIter, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*) {
  ccystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                        *(last - 1));
}

/**
 * @brief 将新元素插入到堆中，并保持堆的性质（默认最大堆）。
 *
 * 假设新元素已经置于底层容器的末尾，通过上溯操作保持堆的性质。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 */
template <class RandomIter>
void push_heap(RandomIter first, RandomIter last) {
  ccystl::push_heap_d(first, last, distance_type(first));
}

/**
 * @brief 插入新元素到堆中，并使用自定义比较函数保持堆的性质。
 *
 * 通过使用自定义比较函数 `comp`，该函数将新元素插入到堆中，并调整堆的结构。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Distance 用于计算距离的类型。
 * @tparam T 元素类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param holeIndex 待调整元素的索引。
 * @param topIndex 堆顶元素的索引。
 * @param value 待插入的元素值。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Distance, class T, class Compared>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex,
                   T value, Compared comp) {
  auto parent = (holeIndex - 1) / 2;  // 计算父节点索引
  while (holeIndex > topIndex && comp(*(first + parent), value)) {
    // 使用自定义比较函数调整
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;  // 插入新值
}

/**
 * @brief 对堆进行 push 操作的辅助函数，适用于自定义比较规则。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Distance 用于计算距离的类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param 迭代器距离类型指针。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Compared, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp) {
  ccystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                        *(last - 1), comp);
}

/**
 * @brief 将新元素插入到堆中，并使用自定义比较函数保持堆的性质。
 *
 * 假设新元素已经置于底层容器的末尾，通过上溯操作和自定义比较函数保持堆的性质。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Compared>
void push_heap(RandomIter first, RandomIter last, Compared comp) {
  ccystl::push_heap_d(first, last, distance_type(first), comp);
}

/**
 * @brief 从堆中弹出根元素，并保持堆的性质。
 *
 * 将堆的根节点与最后一个元素交换位置后调整堆，使堆再次满足堆的性质。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam T 元素类型。
 * @tparam Distance 用于计算距离的类型。
 * @param first 容器的起始迭代器。
 * @param holeIndex 待调整元素的索引。
 * @param len 堆的长度。
 * @param value 待插入的元素值。
 */
template <class RandomIter, class T, class Distance>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value) {
  auto topIndex = holeIndex;
  auto rchild = 2 * holeIndex + 2;  // 计算右子节点索引
  while (rchild < len) {
    // 下溯调整
    if (*(first + rchild) < *(first + rchild - 1)) --rchild;
    *(first + holeIndex) = *(first + rchild);
    holeIndex = rchild;
    rchild = 2 * (rchild + 1);
  }
  if (rchild == len) {
    // 如果没有右子节点
    *(first + holeIndex) = *(first + (rchild - 1));
    holeIndex = rchild - 1;
  }
  ccystl::push_heap_aux(first, holeIndex, topIndex, value);  // 上溯调整
}

/**
 * @brief 对堆进行 pop 操作的辅助函数，适用于默认比较规则。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam T 元素类型。
 * @tparam Distance 用于计算距离的类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param result 结果存放位置的迭代器。
 * @param value 待插入的元素值。
 * @param 迭代器距离类型指针。
 */
template <class RandomIter, class T, class Distance>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                  Distance*) {
  *result = *first;  // 先将首值调至尾节点
  ccystl::adjust_heap(first, static_cast<Distance>(0), last - first,
                      value);  // 调整堆
}

/**
 * @brief 将根元素弹出，并保持堆的性质（默认最大堆）。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 */
template <class RandomIter>
void pop_heap(RandomIter first, RandomIter last) {
  ccystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                       distance_type(first));
}

/**
 * @brief 从堆中弹出根元素，并使用自定义比较函数保持堆的性质。
 *
 * 通过使用自定义比较函数 `comp`，该函数将根元素弹出，并调整堆的结构。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam T 元素类型。
 * @tparam Distance 用于计算距离的类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param holeIndex 待调整元素的索引。
 * @param len 堆的长度。
 * @param value 待插入的元素值。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class T, class Distance, class Compared>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                 Compared comp) {
  auto topIndex = holeIndex;
  auto rchild = 2 * holeIndex + 2;
  while (rchild < len) {
    // 下溯调整
    if (comp(*(first + rchild), *(first + rchild - 1))) --rchild;
    *(first + holeIndex) = *(first + rchild);
    holeIndex = rchild;
    rchild = 2 * (rchild + 1);
  }
  if (rchild == len) {
    *(first + holeIndex) = *(first + (rchild - 1));
    holeIndex = rchild - 1;
  }
  ccystl::push_heap_aux(first, holeIndex, topIndex, value, comp);  // 上溯调整
}

/**
 * @brief 对堆进行 pop 操作的辅助函数，适用于自定义比较规则。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam T 元素类型。
 * @tparam Distance 用于计算距离的类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param result 结果存放位置的迭代器。
 * @param value 待插入的元素值。
 * @param 迭代器距离类型指针。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class T, class Distance, class Compared>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                  Distance*, Compared comp) {
  *result = *first;  // 先将首值调至尾节点
  ccystl::adjust_heap(first, static_cast<Distance>(0), last - first, value,
                      comp);  // 调整堆
}

/**
 * @brief 将根元素弹出，并使用自定义比较函数保持堆的性质。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Compared>
void pop_heap(RandomIter first, RandomIter last, Compared comp) {
  ccystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                       distance_type(first), comp);
}

/**
 * @brief 对堆进行排序。
 *
 * 不断执行 pop_heap
 * 操作，将堆的最大元素移动到末尾，直到堆只剩下一个元素，完成排序。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 */
template <class RandomIter>
void sort_heap(RandomIter first, RandomIter last) {
  while (last - first > 1) {
    ccystl::pop_heap(first, last--);  // 每次 pop_heap 将最大元素移动到末尾
  }
}

/**
 * @brief 对堆进行排序，使用自定义比较函数。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Compared>
void sort_heap(RandomIter first, RandomIter last, Compared comp) {
  while (last - first > 1) {
    ccystl::pop_heap(first, last--, comp);  // 使用自定义比较函数进行排序
  }
}

/**
 * @brief 构造一个堆。
 *
 * 将一个无序容器通过一系列调整操作构造成一个堆。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Distance 用于计算距离的类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param 迭代器距离类型指针。
 */
template <class RandomIter, class Distance>
void make_heap_aux(RandomIter first, RandomIter last, Distance*) {
  if (last - first < 2) return;
  auto len = last - first;
  auto holeIndex = (len - 2) / 2;
  while (true) {
    ccystl::adjust_heap(first, holeIndex, len, *(first + holeIndex));  // 调整堆
    if (holeIndex == 0) return;
    --holeIndex;
  }
}

/**
 * @brief 构造一个堆（默认最大堆）。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 */
template <class RandomIter>
void make_heap(RandomIter first, RandomIter last) {
  ccystl::make_heap_aux(first, last, distance_type(first));
}

/**
 * @brief 构造一个堆，使用自定义比较函数。
 *
 * 使用自定义比较函数 `comp`，将一个无序容器通过一系列调整操作构造成一个堆。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Distance 用于计算距离的类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param 迭代器距离类型指针。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Distance, class Compared>
void make_heap_aux(RandomIter first, RandomIter last, Distance*,
                   Compared comp) {
  if (last - first < 2) return;
  auto len = last - first;
  auto holeIndex = (len - 2) / 2;
  while (true) {
    ccystl::adjust_heap(first, holeIndex, len, *(first + holeIndex),
                        comp);  // 调整堆
    if (holeIndex == 0) return;
    --holeIndex;
  }
}

/**
 * @brief 构造一个堆，使用自定义比较函数。
 *
 * @tparam RandomIter 随机访问迭代器类型。
 * @tparam Compared 自定义比较函数类型。
 * @param first 容器的起始迭代器。
 * @param last 容器的结束迭代器。
 * @param comp 自定义比较函数对象。
 */
template <class RandomIter, class Compared>
void make_heap(RandomIter first, RandomIter last, Compared comp) {
  ccystl::make_heap_aux(first, last, distance_type(first), comp);
}
}  // namespace ccystl

#endif  // CCYSTL_HEAP_ALGO_H_
