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
    struct node *lastNode;

    unsigned int size;
public:
    List();

    int pushFront(T value);
    int pushBack(T value);

    int removeFront();
    int removeBack();

    int insertAt(int idx, T value);
    int removeAt(int idx);
};

#endif // LIST_H
