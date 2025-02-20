#ifndef CCYSTL_UNORDERED_MULTIMAP_H_
#define CCYSTL_UNORDERED_MULTIMAP_H_

// 这个头文件包含模板类 unordered_multimap
// 功能与用法与 map 和 multimap 类似，不同的是使用 hashtable
// 作为底层实现机制，容器内的元素不会自动排序

// notes:
//
// 异常保证：
// ccystl::unordered_map<Key, T> / ccystl::unordered_multimap<Key, T>
// 满足基本异常保证，对以下等函数做强异常安全保证：
//   * emplace
//   * emplace_hint
//   * insert

#include "ccystl/internal/hash_table.h"

namespace ccystl {

// 模板类 unordered_multimap，键值允许重复
// 参数一代表键值类型，参数二代表实值类型，参数三代表哈希函数，缺省使用
// ccystl::hash 参数四代表键值比较方式，缺省使用 ccystl::equal_to
template <class Key, class T, class Hash = ccystl::hash<Key>,
          class KeyEqual = ccystl::equal_to<Key>>
class unordered_multimap {
 private:
  // 使用 hashtable 作为底层机制
  typedef hashtable<pair<const Key, T>, Hash, KeyEqual> base_type;
  base_type ht_;

 public:
  // 使用 hashtable 的型别
  typedef typename base_type::allocator_type allocator_type;
  typedef typename base_type::key_type key_type;
  typedef typename base_type::mapped_type mapped_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::hasher hasher;
  typedef typename base_type::key_equal key_equal;

  typedef typename base_type::size_type size_type;
  typedef typename base_type::difference_type difference_type;
  typedef typename base_type::pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::reference reference;
  typedef typename base_type::const_reference const_reference;

  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::local_iterator local_iterator;
  typedef typename base_type::const_local_iterator const_local_iterator;

  allocator_type get_allocator() const { return ht_.get_allocator(); }

 public:
  // 构造、复制、移动函数

  unordered_multimap() : ht_(100, Hash(), KeyEqual()) {}

  explicit unordered_multimap(size_type bucket_count, const Hash& hash = Hash(),
                              const KeyEqual& equal = KeyEqual())
      : ht_(bucket_count, hash, equal) {}

  template <class InputIterator>
  unordered_multimap(InputIterator first, InputIterator last,
                     const size_type bucket_count = 100,
                     const Hash& hash = Hash(),
                     const KeyEqual& equal = KeyEqual())
      : ht_(ccystl::max(bucket_count,
                        static_cast<size_type>(ccystl::distance(first, last))),
            hash, equal) {
    for (; first != last; ++first) ht_.insert_multi_noresize(*first);
  }

  unordered_multimap(std::initializer_list<value_type> ilist,
                     const size_type bucket_count = 100,
                     const Hash& hash = Hash(),
                     const KeyEqual& equal = KeyEqual())
      : ht_(ccystl::max(bucket_count, static_cast<size_type>(ilist.size())),
            hash, equal) {
    for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
      ht_.insert_multi_noresize(*first);
  }

  unordered_multimap(const unordered_multimap& rhs) : ht_(rhs.ht_) {}
  unordered_multimap(unordered_multimap&& rhs) noexcept
      : ht_(ccystl::move(rhs.ht_)) {}

  unordered_multimap& operator=(const unordered_multimap& rhs) {
    ht_ = rhs.ht_;
    return *this;
  }
  unordered_multimap& operator=(unordered_multimap&& rhs) {
    ht_ = ccystl::move(rhs.ht_);
    return *this;
  }

  unordered_multimap& operator=(std::initializer_list<value_type> ilist) {
    ht_.clear();
    ht_.reserve(ilist.size());
    for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
      ht_.insert_multi_noresize(*first);
    return *this;
  }

  ~unordered_multimap() = default;

  // 迭代器相关

  iterator begin() noexcept { return ht_.begin(); }
  const_iterator begin() const noexcept { return ht_.begin(); }
  iterator end() noexcept { return ht_.end(); }
  const_iterator end() const noexcept { return ht_.end(); }

  const_iterator cbegin() const noexcept { return ht_.cbegin(); }
  const_iterator cend() const noexcept { return ht_.cend(); }

  // 容量相关

  bool empty() const noexcept { return ht_.empty(); }
  size_type size() const noexcept { return ht_.size(); }
  size_type max_size() const noexcept { return ht_.max_size(); }

  // 修改容器相关

  // emplace / emplace_hint

  template <class... Args>
  iterator emplace(Args&&... args) {
    return ht_.emplace_multi(ccystl::forward<Args>(args)...);
  }

  template <class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args) {
    return ht_.emplace_multi_use_hint(hint, ccystl::forward<Args>(args)...);
  }

  // insert

  iterator insert(const value_type& value) { return ht_.insert_multi(value); }
  iterator insert(value_type&& value) {
    return ht_.emplace_multi(ccystl::move(value));
  }

  iterator insert(const_iterator hint, const value_type& value) {
    return ht_.insert_multi_use_hint(hint, value);
  }
  iterator insert(const_iterator hint, value_type&& value) {
    return ht_.emplace_multi_use_hint(hint, ccystl::move(value));
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    ht_.insert_multi(first, last);
  }

  // erase / clear

  void erase(iterator it) { ht_.erase(it); }
  void erase(iterator first, iterator last) { ht_.erase(first, last); }

  size_type erase(const key_type& key) { return ht_.erase_multi(key); }

  void clear() { ht_.clear(); }

  void swap(unordered_multimap& other) noexcept { ht_.swap(other.ht_); }

  // 查找相关

  size_type count(const key_type& key) const { return ht_.count(key); }

  iterator find(const key_type& key) { return ht_.find(key); }
  const_iterator find(const key_type& key) const { return ht_.find(key); }

  pair<iterator, iterator> equal_range(const key_type& key) {
    return ht_.equal_range_multi(key);
  }
  pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
    return ht_.equal_range_multi(key);
  }

  // bucket interface

  local_iterator begin(size_type n) noexcept { return ht_.begin(n); }
  const_local_iterator begin(size_type n) const noexcept {
    return ht_.begin(n);
  }
  const_local_iterator cbegin(size_type n) const noexcept {
    return ht_.cbegin(n);
  }

  local_iterator end(size_type n) noexcept { return ht_.end(n); }
  const_local_iterator end(size_type n) const noexcept { return ht_.end(n); }
  const_local_iterator cend(size_type n) const noexcept { return ht_.cend(n); }

  size_type bucket_count() const noexcept { return ht_.bucket_count(); }
  size_type max_bucket_count() const noexcept { return ht_.max_bucket_count(); }

  size_type bucket_size(size_type n) const noexcept {
    return ht_.bucket_size(n);
  }
  size_type bucket(const key_type& key) const { return ht_.bucket(key); }

  // hash policy

  float load_factor() const noexcept { return ht_.load_factor(); }

  float max_load_factor() const noexcept { return ht_.max_load_factor(); }
  void max_load_factor(float ml) { ht_.max_load_factor(ml); }

  void rehash(size_type count) { ht_.rehash(count); }
  void reserve(size_type count) { ht_.reserve(count); }

  hasher hash_fcn() const { return ht_.hash_fcn(); }
  key_equal key_eq() const { return ht_.key_eq(); }

 public:
  friend bool operator==(const unordered_multimap& lhs,
                         const unordered_multimap& rhs) {
    return lhs.ht_.equal_range_multi(rhs.ht_);
  }
  friend bool operator!=(const unordered_multimap& lhs,
                         const unordered_multimap& rhs) {
    return !lhs.ht_.equal_range_multi(rhs.ht_);
  }
};

// 重载比较操作符
template <class Key, class T, class Hash, class KeyEqual>
bool operator==(const unordered_multimap<Key, T, Hash, KeyEqual>& lhs,
                const unordered_multimap<Key, T, Hash, KeyEqual>& rhs) {
  return lhs == rhs;
}

template <class Key, class T, class Hash, class KeyEqual>
bool operator!=(const unordered_multimap<Key, T, Hash, KeyEqual>& lhs,
                const unordered_multimap<Key, T, Hash, KeyEqual>& rhs) {
  return lhs != rhs;
}

// 重载 ccystl 的 swap
template <class Key, class T, class Hash, class KeyEqual>
void swap(unordered_multimap<Key, T, Hash, KeyEqual>& lhs,
          unordered_multimap<Key, T, Hash, KeyEqual>& rhs) {
  lhs.swap(rhs);
}

}  // namespace ccystl
#endif  // !CCYSTL_UNORDERED_MULTIMAP_H_
