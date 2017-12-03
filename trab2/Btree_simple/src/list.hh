#ifndef LIST_HH
#define LIST_HH

#include <iostream>

template <class T>
class List {
private:
    struct node_t {
        T elem;
        node_t *next;

        node_t(T _elem) : elem(_elem) {}
    };

    node_t *_firstNode;
    node_t *_lastNode;
    int _size;
public:
    List();

    // removes all elements from the list
    void clean();

    // call delete for all elements and remove then
    void deleteAll();

    // return if the list is empty
    bool isEmpty();

    // return the size of the list;
    int size();

    // add elem into the front of the list
    int push_front(T elem);
    // add elem into the back of the list
    int push_back(T elem);

    // remove the element with index idx
    int remove(int idx);

    // remove the FIST element of the list
    int pop_front();
    // remove the LAST element of the list
    int pop_back();

    // return the FIRST element
    // this function cannot be call when the list is empty
    const T &first();
    // return the LAST element
    // this function cannot be call when the list is empty
    const T &last();

    // return the LAST elemet and remove it
    // this function cannot be call when the list is empty
    const T takeFirst();
    // return the FIST elemet and remove it
    // this function cannot be call when the list is empty
    const T takeLast();
};

// we have to implement the methods here
// because the template complains if we do not

template <class T>
List<T>::List() {
    _firstNode = NULL;
    _lastNode = NULL;
    _size = 0;
}

template <class T>
void List<T>::clean() {
    node_t *curr = _firstNode;
    while(curr != NULL) {
        node_t *next = curr->next;
        delete curr;
        curr = next;
    }

    _firstNode = NULL;
    _lastNode = NULL;

    _size = 0;
}

template <class T>
void List<T>::deleteAll() {
    node_t *curr = _firstNode;
    while(curr != NULL) {
        delete curr->elem;

        node_t *next = curr->next;
        delete curr;
        curr = next;
    }

    _firstNode = NULL;
    _lastNode = NULL;

    _size = 0;
}

template <class T>
bool List<T>::isEmpty() {
    return (_size == 0);
}

template <class T>
int List<T>::size() {
    return _size;
}

template <class T>
int List<T>::push_front(T elem) {
    node_t *node = new node_t(elem);
    if(node == NULL) {
        return -1;
    }

    node->next = _firstNode;
    _firstNode = node;

    if(isEmpty()) {
        _lastNode = node;
    }

    _size++;
    return 0;
}

template <class T>
int List<T>::push_back(T elem) {
    node_t *node = new node_t(elem);
    if(node == NULL) {
        return -1;
    }

    node->next = NULL;

    if(isEmpty()) {
        _firstNode = node;
    } else {
        _lastNode->next = node;
    }
    _lastNode = node;

    _size++;
    return 0;
}

template <class T>
int List<T>::remove(int idx) {
    if(idx < 0 || idx >= size()) {
        return -1;
    }

    node_t *node = _firstNode;
    int i = 0;
    while(i < idx) {
        node = node->next;
    }

    delete node;

    return 0;
}

template <class T>
int List<T>::pop_front() {
    if(isEmpty()) {
        std::cout << "[Error] List::pop_front() : Empty list.\n";
        return -2;
    }

    node_t *rmNode = _firstNode;
    _firstNode = _firstNode->next;

    delete rmNode;

    if(size() == 1) {
        _lastNode = NULL;
    }

    _size--;
    return 0;
}

template <class T>
int List<T>::pop_back() {
    if(isEmpty()) {
        std::cout << "[Error] List::pop_back() : Empty list.\n";
        return -2;
    }

    node_t *rmNode = _lastNode;

    if(size() == 1) {
        _firstNode = NULL;
        _lastNode = NULL;
        _size--;

        delete rmNode;
        return 0;
    }

    node_t *lastNode = NULL;
    node_t *currNode = _firstNode;

    while(currNode != last()) {
        lastNode = currNode;
        currNode = currNode->next;
    }

    lastNode->next = NULL;
    _size--;

    delete rmNode;
    return 0;
}

template <class T>
const T &List<T>::first() {
    if(isEmpty()) {
        std::cout << "[Error] List::fist() : Empty list.\n";
        abort();
    }

    return _firstNode->elem;
}

template <class T>
const T &List<T>::last() {
    if(isEmpty()) {
        std::cout << "[Error] List::fist() : Empty list.\n";
        abort();
    }

    return _lastNode->_elem;
}

template <class T>
const T List<T>::takeFirst() {
    const T elem = first();

    pop_front();

    return elem;
}

template <class T>
const T List<T>::takeLast() {
    T elem = last();

    pop_back();

    return elem;
}

#endif // LIST_HH
