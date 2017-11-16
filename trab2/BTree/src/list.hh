#ifndef LIST_H
#define LIST_H


template <class T>
class List {
private:
    struct node {
        T value;
        struct node *nextNode;
    };

    T _header;
    struct node *firstNode;

    unsigned int size;
public:
    List();

    int pushFront(T value);
};

#endif // LIST_H
