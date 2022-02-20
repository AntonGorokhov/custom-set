#include <iostream>
#include <cstddef>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <ctime>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <cassert>
#include <list>
#include <bitset>
#include <deque>
#include <utility>

using namespace std;

inline void fastio() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

template<class ValueType>
class Set {
private:
    class Node {
    public:
        Node *l = nullptr, *r = nullptr;
        ValueType key;
        int height = 1;
        size_t size = 1;
        Node *prev = nullptr, *next = nullptr;

        Node() {}

        Node(ValueType k) : key(k) {}
    };

    Node *local_end = new Node();

    Node *root = nullptr;

    int GetHeight(Node *a) const {
        return (a ? a->height : 0);
    }

    size_t GetSize(Node *a) const {
        return (a ? a->size : 0);
    }

    void Update(Node *a) {
        if (!a) {
            return;
        }
        a->height = max(GetHeight(a->l), GetHeight(a->r)) + 1;
        a->size = GetSize(a->l) + 1 + GetSize(a->r);
    }

    Node *RightRotate(Node *a) {
        Node *b = a->l;
        Node *c = a->r;
        Node *d = b->l;
        Node *e = b->r;
        b->l = d;
        b->r = a;
        a->l = e;
        a->r = c;
        Update(a);
        Update(b);
        return b;
    }

    Node *LeftRotate(Node *a) {
        Node *b = a->l;
        Node *c = a->r;
        Node *d = c->l;
        Node *e = c->r;
        c->l = a;
        c->r = e;
        a->l = b;
        a->r = d;
        Update(a);
        Update(c);
        return c;
    }

    int Bfactor(Node *a) const {
        return GetHeight(a->r) - GetHeight(a->l);
    }

    Node *Balance(Node *a) {
        Update(a);
        if (Bfactor(a) == 2) {
            if (Bfactor(a->r) < 0) {
                a->r = RightRotate(a->r);
            }
            return LeftRotate(a);
        }
        if (Bfactor(a) == -2) {
            if (Bfactor(a->l) > 0) {
                a->l = LeftRotate(a->l);
            }
            return RightRotate(a);
        }
        return a;
    }

    void Fill(Node *prev, Node *a, Node *next) {
        a->prev = prev;
        a->next = next;
        if (prev) {
            prev->next = a;
        }
        if (next) {
            next->prev = a;
        }
    }

    Node *Insert(Node *a, ValueType k, Node *prev, Node *next) {
        if (!a) {
            auto nn = new Node(k);
            Fill(prev, nn, next);
            return nn;
        }
        if (k < a->key) {
            a->l = Insert(a->l, k, prev, next);
        } else {
            a->r = Insert(a->r, k, prev, next);
        }
        return Balance(a);
    }

    Node *RemoveMin(Node *a) {
        if (!a->l) {
            return a->r;
        }
        a->l = RemoveMin(a->l);
        return Balance(a);
    }

    Node *Remove(Node *a, ValueType k) {
        if (!a) {
            return local_end;
        }
        if (k < a->key) {
            a->l = Remove(a->l, k);
        } else if (a->key < k) {
            a->r = Remove(a->r, k);
        } else {
            Node *left = a->l, *right = a->r;
            delete a;
            if (!right || right == local_end) {
                return left;
            }
            Node *mn = FindMin(right);
            mn->r = RemoveMin(right);
            mn->l = left;
            return Balance(mn);
        }
        return Balance(a);
    }

    Node *FindMin(Node *a) const {
        if (!a) {
            return nullptr;
        }
        return a->l ? FindMin(a->l) : a;
    }

    Node *FindMax(Node *a) const {
        if (!a) {
            return nullptr;
        }
        return a->r ? FindMax(a->r) : a;
    }

    Node *Find(Node *a, ValueType k) const {
        if (!a) {
            return nullptr;
        }
        if (a->key < k) {
            return Find(a->r, k);
        } else if (k < a->key) {
            return Find(a->l, k);
        } else {
            return a;
        }
    }

    Node *LowerBound(Node *a, ValueType k) const {
        if (!a) {
            return nullptr;
        }
        if (k < a->key) {
            auto left = LowerBound(a->l, k);
            return left ? left : a;
        } else if (a->key < k) {
            return LowerBound(a->r, k);
        } else {
            return a;
        }
    }

    Node *Lower(Node *a, ValueType k) const {
        if (!a) {
            return nullptr;
        }
        if (a->key < k) {
            auto right = Lower(a->r, k);
            return right ? right : a;
        } else if (k < a->key) {
            return Lower(a->l, k);
        } else {
            return a;
        }
    }

    void DeleteDfs(Node *a) {
        if (!a) return;
        delete a;
        DeleteDfs(a->l);
        DeleteDfs(a->r);
    }

    Node *CopyDfs(Node *a) {
        if (!a) {
            return nullptr;
        }
        Node *b = new Node(*a);
        b->l = CopyDfs(b->l);
        b->r = CopyDfs(b->r);
        return b;
    }

    void PrintDfs(Node *a, vector<Node *> &v) {
        if (!a) {
            return;
        }
        PrintDfs(a->l, v);
        v.push_back(a);
        PrintDfs(a->r, v);
    }

    void clear() {
        vector<Node*> v;
        PrintDfs(root, v);
        for (auto i : v) {
            delete i;
        }
        delete local_end;
    }

public:
    Set() {}

    Set(std::initializer_list<ValueType> l) {
        for (auto el: l) {
            insert(el);
        }
    }

    ~Set() {
        clear();
    }

    class iterator {
    public:
        Node *a;
        iterator() {}

        iterator(Node *a) : a(a) {}

        bool operator==(const iterator &s) const {
            return a == s.a;
        }

        bool operator!=(const iterator &s) const {
            return a != s.a;
        }

        const ValueType &operator*() const {
            return (*a).key;
        }

        const ValueType *operator->() const {
            return &a->key;
        }

        iterator &operator++() {
            a = a->next;
            return *this;
        };

        iterator operator++(int) {
            auto old = *this;
            a = a->next;
            return old;
        }

        iterator &operator--() {
            if (!a) {
                //
            }
            a = a->prev;
            return *this;
        };

        iterator operator--(int) {
            auto old = *this;
            a = a->prev;
            return old;
        }
    };

    iterator find(ValueType k) const {
        auto x = Find(root, k);
        if (!x) {
            return iterator(local_end);
        } else {
            return iterator(x);
        }
    }

    iterator begin() const {
        auto x = FindMin(root);
        if (!x) {
            return iterator(local_end);
        } else {
            return iterator(x);
        }
    }

    iterator end() const {
        return iterator(local_end);
    }

    void insert(ValueType k) {
        if (this->find(k) == this->end()) {
            Node *next = LowerBound(root, k);
            Node *prev = Lower(root, k);
            root = Insert(root, k, prev, next);
        }
        auto x = FindMax(root);
        local_end->prev = x;
        if (x) {
            x->next = local_end;
        }
    }

    size_t size() const {
        return GetSize(root);
    }

    iterator lower_bound(ValueType k) const {
        auto x = LowerBound(root, k);
        if (!x) {
            return iterator(local_end);
        } else {
            return iterator(x);
        }
    }

    bool empty() const {
        return size() == 0;
    }

    void erase(ValueType k) {
        Node *a = Find(root, k);
        if (!a) {
            return;
        }
        Node *next = a->next;
        Node *prev = a->prev;
        if (next) {
            next->prev = prev;
        }
        if (prev) {
            prev->next = next;
        }
        root = Remove(root, k);
        auto x = FindMax(root);
        local_end->prev = x;
        if (x) {
            x->next = local_end;
        }
    }

    Set<ValueType> &operator=(const Set<ValueType> &s) {
        if (this == &s) {
            return *this;
        }
        clear();
        root = CopyDfs(s.root);
        vector<Node *> v;
        PrintDfs(root, v);
        local_end = new Node();
        for (size_t i = 0; i < v.size(); ++i) {
            if (i > 0) {
                v[i]->prev = v[i - 1];
            } else {
                v[i]->prev = nullptr;
            }
            if (i + 1 < v.size()) {
                v[i]->next = v[i + 1];
            } else {
                v[i]->next = local_end;
                local_end->prev = v[i];
            }
        }
        return *this;
    }

    Set(const Set<ValueType> &s) {
        *this = s;
    }

    Set(ValueType *a, ValueType *b) {
        for (auto i = a; i != b; ++i) {
            insert(*i);
        }
        auto x = FindMax(root);
        local_end->prev = x;
        if (x) {
            x->next = local_end;
        }
    }
};


/*void solve() {
    Set<int> s = {1, 2, 3, 4};
    Set<int> v = {1, 2, 3};
    v = s;
    for (auto i : v) {
        cout << i << ' ';
    }
    cout << '\n';
}


signed main() {
#ifdef lolipop
    freopen("input.txt", "r", stdin);
#else
    fastio();
#endif

    int T = 1;
//    cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}*/