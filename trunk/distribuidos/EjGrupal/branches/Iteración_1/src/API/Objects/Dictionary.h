#ifndef DICTIONARY_H
#define	DICTIONARY_H

#include <map>
#include <iostream>

template <class T>
class Dictionary {
public:
    Dictionary();
    T* get(long key);
    bool insert(long key, T* odc);
    bool erase(long key);
    virtual ~Dictionary();
private:
    std::map<long, T*> dictionaryODC_;
    typename std::map<long, T*>::iterator it_;
};

template <class T>
Dictionary<T>::Dictionary() {
}

template <class T>
T* Dictionary<T>::get(long key) {
    it_ = dictionaryODC_.find(key);
    if (it_ == dictionaryODC_.end()) {
        return NULL;
    }
    else {
        return it_->second;
    }
}

template <class T>
bool Dictionary<T>::insert(long key, T* odc) {
    it_ = dictionaryODC_.find(key);
    if (it_ != dictionaryODC_.end()) {
        return false;
    }
    else {
        dictionaryODC_.insert( std::pair<long, T*>(key, odc));
        return true;
    }
}

template <class T>
bool Dictionary<T>::erase(long key) {
    it_ = dictionaryODC_.find(key);
    if (it_ != dictionaryODC_.end()) {
        T* odc = it_->second;
        dictionaryODC_.erase(it_);
        delete odc;
        return true;
    }
    return false;
}

template <class T>
Dictionary<T>::~Dictionary() {
    for (it_ = dictionaryODC_.begin(); it_ != dictionaryODC_.end(); ++it_) {
        T* odc = it_->second;
        dictionaryODC_.erase(it_);
        delete odc;
    }
}

#endif	/* DICTIONARY_H */

