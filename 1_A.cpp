#include <iostream>

template <typename T>
struct Node {
    T data;
    Node<T>* next;

    Node(T _data) {
        data = _data;
        next = nullptr;
    }
};

template <typename T>
class List {
private:
    Node<T>* head;
    Node<T>* tail;

    List(Node<T>* _head, Node<T>* _tail) {
        head = _head;
        tail = _tail;
    }

    void free() {
        head = nullptr;
        tail = nullptr;
    }

    void clear() {
        while (head != nullptr) {
            this->pop_front();
        }
    }

public:
    List() {
        head = nullptr;
        tail = nullptr;
    }

    void push_back(T _data) {
        Node<T>* new_node = new Node<T>(_data);

        if (head == nullptr) {
            head = new_node;
        } else {
            tail->next = new_node;
        }

        tail = new_node;
    }

    void pop_front() {
        if (head == tail) {
            tail = nullptr;
        }

        Node<T>* tmp = head->next;
        delete head;
        head = tmp;
    }

    T front() {
        return head->data;
    }

    bool empty() {
        return head == nullptr;
    }

    List<T> split() {
        Node<T>* mid = head;
        Node<T>* tmp = head;

        while (tmp != tail) {
            tmp = tmp->next;

            if (tmp == tail) {
                break;
            }

            tmp = tmp->next;
            mid = mid->next;
        }

        Node<T>* other_tail = tail;
        Node<T>* other_head = mid->next;

        mid->next = nullptr;
        tail = mid;

        return List(other_head, other_tail);
    }

    void merge(List& other) {
        List res;

        while (!this->empty() && !other.empty()) {
            if (this->front() < other.front()) {
                res.push_back(this->front());
                this->pop_front();
            } else {
                res.push_back(other.front());
                other.pop_front();
            }
        }
        while (!this->empty()) {
            res.push_back(this->front());
            this->pop_front();
        }
        while (!other.empty()) {
            res.push_back(other.front());
            other.pop_front();
        }

        head = res.head;
        tail = res.tail;

        res.free();
    }

    void sort() {
        if (head == tail) {
            return;
        }

        List other = this->split();

        this->sort();
        other.sort();
        
        this->merge(other);
    }

    void print() {
        Node<T>* cur = head;
        while (cur != nullptr) {
            std::cout << cur->data << " ";
            cur = cur->next;
        }
        std::cout << std::endl;
    }

    ~List() {
        this->clear();
    }
};

int main() {
    std::ios::sync_with_stdio(false);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    List<int> my_list;

    size_t n;
    std::cin >> n;

    int val;
    for (size_t i = 0; i != n; ++i) {
        std::cin >> val;
        my_list.push_back(val);
    }

    my_list.sort();
    my_list.print();

    return 0;
}
