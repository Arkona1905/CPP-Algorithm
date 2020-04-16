#include<bits/stdc++.h>

using namespace std;

long long BIG_NUMBER = 1000000000000;

class Node {
public:
    long long key;
    Node *left;
    Node *right;
    long long height;
};

long long height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

Node* newNode(long long key) {
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

Node *rightRotate(Node *node) {
    Node *son = node->left;
    Node *grandson = son->right;
    son->right = node;
    node->left = grandson;
    node->height = max(height(node->left), height(node->right)) + 1;
    son->height = max(height(son->left), height(son->right)) + 1;
    return son;
}

Node *leftRotate(Node *node) {
    Node *son = node->right;
    Node *grandson = son->left;
    son->left = node;
    node->right = grandson;
    node->height = max(height(node->left), height(node->right)) + 1;
    son->height = max(height(son->left), height(son->right)) + 1;
    return son;
}

long long getBalance(Node *node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

Node * maxValueNode(Node* node) {
    Node* current = node;
    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

Node * minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node *find(Node *node, long long key) {
    if (node == NULL) {
        return newNode(key + 1);
    }
    if(node->right && key > node->key) {
        return find(node->right, key);
    }
    if (node->left && key < node->key) {
        return find(node->left, key);
    }
    return node;
}

bool exists(Node* node, long long key) {
    return find(node, key)->key == key;
}

Node* insert(Node* node, long long key) {
    if (node == NULL) {
        return (newNode(key));
    }
    if (key < node->key) {
        node->left = insert(node->left, key);
    }
    if (key > node->key) {
        node->right = insert(node->right, key);
    }
    if (key == node->key) {
        return node;
    }
    node->height = 1 + max(height(node->left), height(node->right));
    long long balance = getBalance(node);
    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }
    if (balance < -1 && key > node->right->key) {
        return leftRotate(node);
    }
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node* deleteNode(Node* node, long long key) {

    if (node == NULL) {
        return node;
    }
    if (key < node->key) {
        node->left = deleteNode(node->left, key);
    }
    if(key > node->key) {
        node->right = deleteNode(node->right, key);
    }
    if(key == node->key) {
        if( (node->left == NULL) || (node->right == NULL) ) {
            Node *son = node->left ? node->left : node->right;
            if (son == NULL) {
                son = node;
                node = NULL;
            } else {
                *node = *son;
            }
            free(son);
        } else {
            Node* minRightNode = minValueNode(node->right);
            node->key = minRightNode->key;
            node->right = deleteNode(node->right, minRightNode->key);
        }
    }
    if (node == NULL) {
        return node;
    }
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rightRotate(node);
    }
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) {
        return leftRotate(node);
    }
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void preOrder(Node *node) {
    if(node != NULL) {
        cout << node->key << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
}


Node* prev(Node *node, long long key, long long ans) {
    if (node == NULL) {
        return newNode(-BIG_NUMBER);
    }
    if (node->left && key < node->key) {
        return prev(node->left, key, ans);
    }
    if (node->right && key > node->key) {
        ans = max(ans, node->key);
        return prev(node->right, key, ans);
    }
    if (node->key == key) {
        if (node->left) {
            ans = max(ans, maxValueNode(node->left)->key);
        }
    }
    if (node->key < key) {
        ans = max(ans, node->key);
    }
    return newNode(ans);
}

Node* next(Node *node, long long key, long long ans) {
    if (node == NULL) {
        return newNode(BIG_NUMBER);
    }
    if (node->right && key > node->key) {
        return next(node->right, key, ans);
    }
    if (node->left && key < node->key) {
        ans = min(ans, node->key);
        return next(node->left, key, ans);
    }
    if (node->key == key) {
        if (node->right) {
            ans = min(ans, minValueNode(node->right)->key);
        }
    }
    if (node->key > key) {
        ans = min(ans, node->key);
    }
    return newNode(ans);
}

int main() {
    Node *root = NULL;
    string str;
    long long value;
    while(cin >> str) {
        if (str == "insert") {
            cin >> value;
            root = insert(root, value);
        } else if (str == "delete") {
            cin >> value;
            root = deleteNode(root, value);
        } else if (str == "exists") {
            cin >> value;
            cout << (exists(root, value) ? "true" : "false") << endl;
        } else if (str == "prev") {
            cin >> value;
            long long prev_value = prev(root, value, -BIG_NUMBER)->key;
            if (prev_value != -BIG_NUMBER) {
                cout << prev_value << endl;
            } else {
                cout << "none" << endl;
            }
        } else if (str == "next") {
            cin >> value;
            long long next_value = next(root, value, BIG_NUMBER)->key;
            if (next_value != BIG_NUMBER) {
                cout << next_value << endl;
            } else {
                cout << "none" << endl;
            }
        }
    }
    return 0;
} 