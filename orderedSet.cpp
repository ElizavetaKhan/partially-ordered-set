#include "orderedSet.h"

orderedSet::orderedSet()
{
    head = nullptr;
}

orderedSet::~orderedSet()
{
    if (!head)
        return;

    Node* current = head;
    while (current) {
        // подчищаем trails у текущего узла, если есть (если вдруг зациклились и надо почистить)
        deleteTrail(current->trail);

        // удаляем сам узел из списка ведущих
        Node* nexT = current->nexT;
        delete current;
        current = nexT;
    }

    this->head = nullptr;
}

void orderedSet::insert(int slaveKey, int leadingKey)
{
    if (slaveKey == leadingKey)
        return;

    Node* slave;
    // если первый элемент
    if (!head) {
        head = new Node(slaveKey);
        slave = head;

    } else {
        slave = findKey(slaveKey);
        // если такого узла еще нет в списке – создаем его
        if (!slave) {
            Node* neW = new Node(slaveKey);
            neW->nexT = head;
            head = neW;
            slave = head;
        }
    }

    // проделываем то же самое с ведущим узлом из пары
    Node* leading = findKey(leadingKey);
    if (!leading){
        Node* neW = new Node(leadingKey);
        neW->nexT = head;
        head = neW;
        leading = head;
    }

    // увеличиваем число предшественников у ведущего узла
    *leading->count += 1;

    // создаем узел, чтобы положить его в trail ведомого
    node* temp = new node(leading);
    // если там пока ничего нет – просто закидываем новый trail
    if (!slave->trail) {
        slave->trail = temp;
    } else {
        node* prev = slave->trail;
        // идем по списку ведомых до конца ИЛИ пока не встретим такой же узел
        for (node* i = slave->trail; i != nullptr; i = i->next) {
            // если встретили его в этом списке
            if (i->key == leading->key)
                return;
            prev = i;
        }
        // если не нашли – добавляем в хвост
        prev->next = temp;
    }
}

orderedSet::Node* orderedSet::findKey(int key)
{
    for (Node* i = head; i != nullptr; i = i->nexT)
        // если такой уже есть – возвращаем его
        if (key == i->key)
            return i;

    return nullptr;
}

orderedSet::Node *orderedSet::noPrev()
{
    Node* prev = head;
    for (Node* i = head->nexT; i != nullptr; i = i->nexT) {
        if (*i->count == 0)
            return prev;
        prev = i;
    }
    return nullptr;
}

bool orderedSet::makeOrderly()
{
    Node *newHead;
    Node* curInNewList;
    Node* curInOldList = head;

    // с головой нового списка работаем отдельно
    // если у СТАРОЙ головы уже count = 0
    if (*head->count == 0)
    {
        // добавляем в новый список
        newHead = head;
        curInNewList = newHead;
        // удаляем из старого, сдвигаем head
        deletE(nullptr);

        // зануляем все у перенесенного элемента
        curInNewList->transfer();

    } else {
        // i.next – первый встретившийся элемент без предшественников
        curInOldList = noPrev();

        // удаляем i.next как предшественника и "вырезаем" из текущего списка
        deletE(curInOldList);

        newHead = curInOldList->nexT;
        curInNewList = newHead;

        // зануляем все у перенесенного элемента
        curInNewList->transfer();
    }

    // пока не зациклились или не кончились элементы в текущем списке
    while (curInOldList) {
        // проверяем всегда: не удаляем ли мы голову

        if (*head->count == 0) {
            // запоминаем в новый список этот элемент, который собираемся удалять
            curInNewList->nexT = curInOldList;
            // удаляем i как предшественника и "вырезаем" из текущего списка
            deletE(nullptr);

        } else {
            // находим элемент с count = 0
            curInOldList = noPrev();

            // запоминаем в новый список этот элемент
            curInNewList->nexT = curInOldList->nexT;
            // удаляем i.next как предшественника и "вырезаем" из текущего списка
            deletE(curInOldList);
        }

        curInNewList = curInNewList->nexT;
        // зануляем все у перенесенного элемента
        curInNewList->transfer();

        // переходим дальше
        // если все занулили уже
        if (!head)
            break;

        // иначе если у головы нет предшественников
        else if (*head->count == 0) {
            curInOldList = head;
            continue;
        }

        // иначе просто ищем следующий без предшественников
        curInOldList = noPrev();
    }

    // если зациклились и голова осталась в текущем списке – подчищаем память в новом списке
    if (head) {
        destructor(newHead);
        return false;
    }

    // если всё супер – присваиваем новую голову
    head = newHead;
    return true;
}

void orderedSet::deletE(orderedSet::Node *key)
{
    Node* cur;
    // если нужно удалить голову
    if (!key) {
        cur = head;
        if (head->nexT)
            head = head->nexT;
        else
            head = nullptr;
    } else {
        cur = key->nexT;
        // если не последний элемент
        if (key->nexT)
            // вырезаем элемент
            key->nexT = cur->nexT;
        else
            key->nexT = nullptr;
    }

    // убираем его как предшественника из других узлов, если есть вообще такие
    if (cur->trail)
        deleteTrail(cur->trail);
}

void orderedSet::deleteTrail(node* headTrail)
{
    node* i = headTrail;

    // вычитаем у каждого элемента списка count, начиная с headTrail
    // а также удаляем память за этим элемента
    while (i) {
        // держим следующий "маленький" узел
        node *nexT;
        i->next ? nexT = i->next : nexT = nullptr;

        *i->count -= 1;
        // чистим память узла
        delete i;
        // переходим к следующему
        i = nexT;
    }
}

void orderedSet::print()
{
    if (!head)
        return;

    std::cout << std::endl;
    for (Node* i = head; i != nullptr; i = i->nexT) {
        if (!i->nexT)
            std::cout << i->key;
        else
            std::cout << i->key << ", ";
    }
    std::cout << std::endl;
}

void orderedSet::destructor(orderedSet::Node *heaD)
{
    if (!heaD)
        return;

    Node* current = heaD;
    while (current) {
        Node* nexT = current->nexT;
        delete current;
        current = nexT;
    }
}
