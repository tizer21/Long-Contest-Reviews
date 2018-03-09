#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    static const size_t DEFAULT_SIZE = 1;
    std::vector<std::list<std::pair<const KeyType, ValueType>>> __data;
    size_t __sz;
    Hash __hash;

    void restruct(size_t new_size) {
        if (new_size < DEFAULT_SIZE) {
            return;
        }

        std::vector<std::list<std::pair<const KeyType, ValueType>>> new_data(new_size);
        for (auto & l : __data) {
            for (auto & v : l) {
                size_t pos = __hash(v.first) % new_size;
                new_data[pos].push_back(v);
            }
        }

        std::swap(__data, new_data);
    }

    void augmentation() {
        if (__data.size() != __sz) {
            return;
        }

        this->restruct(__data.size() * 2);
    }

    void diminution() {
        if (__data.size() < __sz * 4) {
            return;
        }

        this->restruct(__data.size() / 4);
    }

    template <bool isConst>
    class Iterator {
    private:
        using iterator_category = std::forward_iterator_tag;

        using value_type = std::pair<const KeyType, ValueType>;

        using reference = typename std::conditional<isConst,
                const value_type&,
                value_type&>::type;

        using pointer = typename std::conditional<isConst,
                const value_type*,
                value_type*>::type;

        using differenceType = std::ptrdiff_t;

        using hashMapType = typename std::conditional<isConst,
                const HashMap<KeyType, ValueType, Hash>*,
                HashMap<KeyType, ValueType, Hash>*>::type;

        using listIteratorType = typename std::conditional<isConst,
                typename std::list<value_type>::const_iterator,
                typename std::list<value_type>::iterator>::type;

        hashMapType __hashMap;
        size_t __index;
        listIteratorType __iter;

        void next() {
            ++__index;
            while (__index != __hashMap->__data.size() && __hashMap->__data[__index].empty()) {
                ++__index;
            }

            if (__index == __hashMap->__data.size()) {
                __iter = __hashMap->__data[--__index].end();
            } else {
                __iter = __hashMap->__data[__index].begin();
            }
        }

    public:
        Iterator() {}

        Iterator(hashMapType hashMap, size_t index, listIteratorType iter)
                : __hashMap(hashMap)
                , __index(index)
                , __iter(iter) {}

        Iterator(hashMapType hashMap)
                : __hashMap(hashMap) {
            __index = 0;

            if (__hashMap->__data[__index].empty()) {
                this->next();
            } else {
                __iter = __hashMap->__data[__index].begin();
            }
        }

        Iterator& operator++() {
            ++__iter;

            if (__iter == __hashMap->__data[__index].end()) {
                this->next();
            }

            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return __index == other.__index && __iter == other.__iter;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        reference operator*() const {
            return *__iter;
        }

        pointer operator->() {
            return &(*__iter);
        }
    };

public:
    HashMap(const Hash hash = Hash()) : __hash(hash) {
        __data.resize(DEFAULT_SIZE);
        __sz = 0;
    }

    template <typename Iter>
    HashMap(Iter begin, Iter end, Hash hash = Hash()) : __hash(hash) {
        __data.resize(DEFAULT_SIZE);
        __sz = 0;

        while (begin != end) {
            this->insert(*begin);
            ++begin;
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list, Hash hash = Hash()) : __hash(hash) {
        HashMap(list.being(), list.end(), __hasher);
    }

    size_t size() const {
        return __sz;
    }

    bool empty() const {
        return this->size() == 0;
    }

    Hash hash_function() const {
        return __hash;
    }

    void insert(const std::pair<const KeyType, ValueType>& value) {
        size_t pos = __hash(value.first) % __data.size();

        if (this->find(value.first) == this->end()) {
            ++__sz;
            __data[pos].push_back(value);
            this->augmentation();
        }
    }

    void print() {
        for (auto& l : __data) {
            for (auto& v : l) {
                std::cerr << v.first << " " << v.second << std::endl;
            }
        }
    }

    void erase(const KeyType& key) {
        size_t pos = __hash(key) % __data.size();
        for (auto it = __data[pos].begin(); it != __data[pos].end(); ++it) {
            if (it->first == key) {
                __data[pos].erase(it);
                --__sz;
                break;
            }
        }

        this->diminution();
    }

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    iterator find(const KeyType& key) {
        size_t pos = __hash(key) % __data.size();

        for (auto it = __data[pos].begin(); it != __data[pos].end(); ++it) {
            if (it->first == key) {
                return Iterator<false>(this, pos, it);
            }
        }

        return this->end();
    }

    const_iterator find(const KeyType& key) const {
        size_t pos = __hash(key) % __data.size();

        for (auto it = __data[pos].begin(); it != __data[pos].end(); ++it) {
            if (it->first == key) {
                return Iterator<true>(this, pos, it);
            }
        }

        return this->end();
    }

    ValueType& operator[] (const KeyType& key) {
        iterator it = this->find(key);

        if (it == this->end()) {
            this->insert({key, ValueType()});
            it = this->find(key);
        }

        return it->second;
    }

    const ValueType& at(const KeyType& key) const {
        const_iterator it = this->find(key);

        if (it == this->end()) {
            throw std::out_of_range("Element doesn't exist!");
        }

        return it->second;
    }

    iterator begin() {
        return Iterator<false>(this);
    }

    iterator end() {
        return Iterator<false>(this, __data.size() - 1, __data[__data.size() - 1].end());
    }

    const_iterator begin() const {
        return Iterator<true>(this);
    }

    const_iterator end() const {
        return Iterator<true>(this, __data.size() - 1, __data[__data.size() - 1].end());
    }

    void clear() {
        __sz = 0;
        __data.clear();
        __data.resize(DEFAULT_SIZE);
    }

    HashMap& operator=(const HashMap other) {
        this->clear();

        for (auto& l : other.__data) {
            for (auto& v : l) {
                this->insert(v);
            }
        }

        return *this;
    }
};
