#include <cstddef>
#include <forward_list>
#include <iostream>
#include <vector>

namespace pr {
template <typename K, typename V>
class HashTable {
 public:
  class Entry {
   public:
    const K key;
    V value;

    Entry(K key, V value) : key(key), value(value) {}
    friend std::ostream& operator<<(std::ostream& os, const Entry& e) {
      (void)e;
      os << "[key: " << e.key << ", value: " << e.value << "]";
      return os;
    }

    bool operator==(const Entry& e) { return value == e.value; }
  };
  typedef std::vector<std::forward_list<Entry>> bucket_t;
  bucket_t buckets;
  size_t sz;

 public:
  class Iterator {
    bucket_t* ptr;

   public:
    typename bucket_t::iterator vit;
    typename std::forward_list<Entry>::iterator lit;

    Iterator(bucket_t& buckets) : ptr(&buckets) {
      for (size_t i = 0; i < buckets.size(); ++i) {
        if (!buckets[i].empty()) {
          vit = buckets.begin() + i;
          lit = buckets[i].begin();
          return;
        }
      }
    }

    Iterator& operator++() {
      ++lit;
      if (lit == vit->end()) {
        ++vit;
        for (auto& it = vit; vit != ptr->end(); ++it) {
          if (!it->empty()) {
            vit = it;
            lit = it->begin();
            return *this;
          }
        }
      }
      return *this;
    }

    bool operator!=(const Iterator& o) { return vit != o.vit || lit != o.lit; }

    Entry& operator*() { return *lit; }
  };

  Iterator begin() { return Iterator(buckets); }

  Iterator end() {
    Iterator it(buckets);

    it.vit = buckets.end();
    it.lit = buckets[buckets.size() - 1].end();
    return it;
  }

  static size_t count(Iterator begin, Iterator end) {
    size_t i;
    for (i = 0; begin != end; ++begin) ++i;
    return i;
  }

  template <typename T>
  static size_t count_if_equal(Iterator begin, Iterator end, const T& val) {
    size_t i;
    for (i = 0; begin != end; ++begin) {
      if (*begin == val) ++i;
    }
    return i;
  }

  HashTable(size_t init = 100) {
    sz = 0;
    buckets.reserve(init);
    for (size_t i = 0; i < init; ++i)
      buckets.push_back(std::forward_list<Entry>());
  }

  V* get(const K& key) {
    size_t h = std::hash<K>()(key);
    std::forward_list<Entry>& l = buckets[h % buckets.size()];

    for (Entry& x : l)
      if (x.key == key) return &x.value;
    return nullptr;
  }

  bool put(const K& key, const V& value) {
    size_t size = sz;
    if ((((size == 0) ? 1 : size) * 100 / buckets.size()) >= 80) this->grow();

    size_t h = std::hash<K>()(key);
    std::forward_list<Entry>& l = buckets[h % buckets.size()];

    sz++;
    for (Entry& x : l) {
      if (x.key == key) {
        x.value = value;
        return true;
      }
    }
    l.push_front(Entry(key, value));
    return false;
  }

  size_t size() { return sz; }

  void grow() {
    size_t len = buckets.size();
    std::vector<Entry> vec;
    vec.reserve(len);
    sz = 0;

    for (std::forward_list<Entry>& l : buckets) {
      for (Entry& e : l) vec.push_back(e);
    }

    len *= 2;
    buckets.clear();
    buckets.reserve(len);
    for (size_t i = 0; i < len; ++i)
      buckets.push_back(std::forward_list<Entry>());

    for (Entry& e : vec) this->put(e.key, e.value);
  }
};

}  // namespace pr
