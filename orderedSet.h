#ifndef INC_7_LAB_ORDEREDSET_H
#define INC_7_LAB_ORDEREDSET_H
#include <iostream>


class orderedSet {
    struct node;
    // список ведущих
    struct Node {
        int key;        // ключ
        int* count;     // количество предшественников
        // (ссылка на int для изменения и доступа и из ведущих, и из ведомых)

        Node* nexT{};   // следующий типа Node
        node* trail{};  // список узлов, для которых данный является ведомым

        explicit Node(int key) {
            this->count = new int;
            this->key = key;
            this->nexT = nullptr;
        }

        // все зануляем для переноса в новый список
        void transfer () {
            this->count = nullptr;
            this->nexT = nullptr;

        }

        // конструктор для наследника
        explicit Node(Node* id) {
            this->key = id->key;
            this->count = id->count;
        }
    };

    // список ведомых
    struct node : public Node {
    // нет доступа к данным полям
    private:
        using Node::nexT;
        using Node::trail;

    public:
        node* next;

        explicit node(Node* id) : Node(id) {
            this->next = nullptr;
        }
    };

private:
    Node* head;

private:
    // ищем в списке key, если не нашли – возвращаем nullptr
    Node* findKey(int key);

    // получает key.next без предшественников, удаляется из списка ведущих и как предшественник у других узлов
    // если надо удалить голову – получает nullptr на вход
    void deletE(Node* key);

    // возвращает PREV у первого встретившегося узла Node, не имеющего предшественников\
    // голову проверяем отдельно ДО этого метода
    // возвращает nullptr – когда больше нет элементов с count=0 (зацикливание или кончились элементы в списке)
    Node* noPrev();

    static void destructor(Node* heaD);

    // удаляет trail у узла, начиная с head trail
    static void deleteTrail(node* headTrail);

public:
    orderedSet();

    ~orderedSet();

    // передаем сразу пару
    void insert(int slaveKey, int leadingKey);

    bool makeOrderly();

    void print();
};


#endif //INC_7_LAB_ORDEREDSET_H
