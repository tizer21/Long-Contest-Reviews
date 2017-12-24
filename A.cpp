#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* next;

    Node(int _data = 0) {
        data = _data;
        next = NULL;
    }
};

class List {
private:
    Node* head;

    void erase() {
        if (head == NULL) {
            return;
        }

        Node* cur = head->next;
        delete head;
        head = cur;
    }

public:
    List(Node* _head = NULL) {
        head = _head;
    }

    void add(int _data = 0) {
        Node* newNode = new Node(_data);
        if (head == NULL) {
            head = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }

    void clear() {
        while (head != NULL) {
            this->erase();
        }
    }

    void print() {
        Node* cur = head;
        while (cur != NULL) {
            cout << cur->data << " ";
            cur = cur->next;
        }
    }

    void sort() {
        if (head == NULL) {
            return;
        }

        int cnt = 0;

        Node* cur = head;
        while (cur != NULL) {
            ++cnt;
            cur = cur->next;
        }

        if (cnt == 1) {
            return;
        }

        cur = head;
        for (int i = 0; i < (cnt - 1) / 2; ++i) {
            cur = cur->next;
        }

        List l1 = List(head);
        List l2 = List(cur->next);
        cur->next = NULL;

        l1.sort();
        l2.sort();

        head = NULL;
        cur = NULL;
        while (l1.head != NULL && l2.head != NULL) {
            if ((l1.head->data) < (l2.head->data)) {
                if (cur == NULL) {
                    cur = l1.head;
                } else {
                    cur->next = l1.head;
                    cur = cur->next;
                }
                l1.head = l1.head->next;
            } else {
                if (cur == NULL) {
                    cur = l2.head;
                } else {
                    cur->next = l2.head;
                    cur = cur->next;
                }
                l2.head = l2.head->next;
            }
            if (head == NULL) {
                head = cur;
            }
        }
        while (l1.head != NULL) {
            cur->next = l1.head;
            cur = cur->next;
            l1.head = l1.head->next;
        }
        while (l2.head != NULL) {
            cur->next = l2.head;
            cur = cur->next;
            l2.head = l2.head->next;
        }
    }

    ~List() {
        this->clear();
    }
};

int main() {
    List l = List();

    int n;
    cin >> n;

    int el;
    for (int i = 0; i != n; ++i) {
        cin >> el;

        l.add(el);
    }

    l.sort();
    l.print();

    return 0;
}
