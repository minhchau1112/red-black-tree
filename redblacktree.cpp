#include<iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    bool color; // 1-Red,0-Black
};

struct RedBlackTree {
    Node* root;
};

void Init(RedBlackTree& t);
void RB_Left_Rotate(RedBlackTree& T, Node* x);
void RB_Right_Rotate(RedBlackTree& T, Node* y);
void RB_Insert_FixUp(RedBlackTree& T, Node* z);
void RB_Insert_Node(RedBlackTree& T, Node* z);
void swapColors(Node* x1, Node* x2);
void swapValues(Node* u, Node* v);
Node* Uncle(Node* cur);
void fixRedRed(Node* x, RedBlackTree& t);
Node* findMaxValueNode_Left(Node* cur);
Node* BSTreplace(Node* x);
Node* Sibling(Node* cur);
void deleteNode(Node* v, RedBlackTree& t);
bool hasRedChild(Node* cur);
void fixDoubleBlack(Node* x, RedBlackTree& t);
void deleteByVal(RedBlackTree& t, int n);

void Init(RedBlackTree& t) {
    t.root = NULL;
}

Node* findNode(Node* root, int data) {
    if (root == NULL)return NULL;
    if (data < root->data)return findNode(root->left, data);
    else if (data > root->data) return findNode(root->right, data);
    else return root;
    return NULL;
}

void RB_Left_Rotate(RedBlackTree &T, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL) T.root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RB_Right_Rotate(RedBlackTree &T, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != NULL) x->right->parent = x;
    x->parent = y->parent;
    if (y->parent == NULL) T.root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}

void RB_Insert_FixUp(RedBlackTree &T,Node* z) {
    while (z->parent != NULL && z->parent->color == 1)
    {
        // Case [1], [2], [3]
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y == NULL) {
                y = new Node;
                y->color = 0;
            }
            if (y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    RB_Left_Rotate(T, z);
                    z->parent->color = 0;
                    z->parent->parent->color = 1;
                    RB_Right_Rotate(T, z->parent->parent);
                }
                else {
                    z->parent->color = 0;
                    z->parent->parent->color = 1;
                    RB_Right_Rotate(T, z->parent->parent);
                }
            }
        }
        else {
            Node* y2 = z->parent->parent->left;
            if (y2 == NULL) {
                y2 = new Node;
                y2->color = 0;
            }
            if (y2->color == 1) {
                z->parent->color = 0;
                y2->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RB_Right_Rotate(T, z);
                    z->parent->color = 0;
                    z->parent->parent->color = 1;
                    RB_Left_Rotate(T, z->parent->parent);
                }
                else {
                    z->parent->color = 0;
                    z->parent->parent->color = 1;
                    RB_Left_Rotate(T, z->parent->parent);
                }
            }
        }
    }
    T.root->color = 0;
}

void RB_Insert_Node(RedBlackTree &T,Node* z) { // T: tree; z: new node
    Node* y = NULL; 
    Node* x = T.root; // Travese tree to
    while (x != NULL){ // go to leaf node
    y = x; // y: is parent node of x 
    if (z->data < x->data) x = x->left;
    else x = x->right;
    }
    z->parent = y; // assign parent of z
    if (y == NULL) T.root = z; // if parent is null -> z: root
    else if (z->data < y->data) y->left = z; //choose branch for z
    else y->right = z;
    z->left = NULL;
    z->right = NULL;
    z->color = 1; // new node (z) is Red
    RB_Insert_FixUp(T, z); // check and balance tree
}

void swapColors(Node* x1, Node* x2) {
    int temp;
    temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
}

void swapValues(Node* u, Node* v) {
    int temp;
    temp = u->data;
    u->data = v->data;
    v->data = temp;
}

Node* Uncle(Node* cur) {
    // Nếu không có node cha hoặc node ông, thì không có node chú
    if (cur->parent == NULL or cur->parent->parent == NULL)
        return NULL;

    return (cur->parent == cur->parent->parent->left) ? cur->parent->parent->right : cur->parent->parent->left;
}

// sửa màu đỏ đỏ tại nút nhất định
void fixRedRed(Node* x,RedBlackTree& t) {
    // nếu x là màu gốc, nó là màu đen và trả về 
    if (x == t.root) {
        x->color = 0;
        return;
    }

    // khởi tạo cha mẹ, ông bà, chú
    Node* parent = x->parent, * grandparent = parent->parent, * uncle = Uncle(x);

    if (parent->color != 0) {
        if (uncle != NULL && uncle->color == 1) {
            // chú màu đỏ, thực hiện tô màu và đệ quy
            parent->color = 0;
            uncle->color = 0;
            grandparent->color = 1;
            fixRedRed(grandparent,t);
        }
        else {
            // Các hoạt động khác LR, LL, RL, RR
            if (parent== parent->parent->left) {
                if (x==x->parent->left) {
                    // cho left right 
                    swapColors(parent, grandparent);
                }
                else {
                    RB_Left_Rotate(t, parent);
                    swapColors(x, grandparent);
                }
                // cho left left và left right 
                RB_Right_Rotate(t, grandparent);
            }
            else {
                if (x == x->parent->left) {
                    // cho right left 
                    RB_Right_Rotate(t, parent);
                    swapColors(x, grandparent);
                }
                else {
                    swapColors(parent, grandparent);
                }

                // cho right right và right left 
                RB_Left_Rotate(t, grandparent);
            }
        }
    }
}

Node* findMaxValueNode_Left(Node* cur) {
    Node* temp = cur;
    while (temp->right != NULL)
        temp = temp->right;
    return temp;
}

// tìm nút thay thế nút đã xóa trong BST 
Node* BSTreplace(Node* x) {
    // khi nút có 2 con
    if (x->left != NULL && x->right != NULL) {
        return findMaxValueNode_Left(x->left);
    }
    else {
        return (x->left) ? x->left : x->right;
    }
}

// trả về con trỏ cho node anh chị em
Node* Sibling(Node* cur) {
    // Node anh rỗng nếu không tồn tại node cha 
    if (cur->parent == NULL)
        return NULL;

    if (cur == cur->parent->left)
        return cur->parent->right;

    return cur->parent->left;
}

// xóa nút đã cho
void deleteNode(Node* v, RedBlackTree& t) {
    Node* u = BSTreplace(v);

    // Đúng khi u và v đều đen 
    bool uvBlack = ((u == NULL || u->color == 0) && (v->color == 0));
    Node* parent = v->parent;

    if (u == NULL) {
        // u là NULL do đó v là lá 
        if (v == t.root) {
            // v là root, làm cho root là null 
            t.root = NULL;
        }
        else {
            if (uvBlack) {
                // u và v đều đen
                // v là lá, sửa màu đen kép tại v 
                fixDoubleBlack(v, t);
            }
            else {
                // u hoặc v là đỏ
                Node* sibling = Sibling(v);
                if (sibling != NULL)
                    // node anh chị em không rỗng, làm cho nó màu đỏ" 
                    sibling->color = 1;
            }

            // xóa v khỏi cây 
            if (v == v->parent->left) {
                parent->left = NULL;
            }
            else {
                parent->right = NULL;
            }
        }
        delete v;
        return;
    }

    if (v->left == NULL|| v->right == NULL) {
        // v có 1 node con
        if (v == t.root) {
            // v là gốc, gán giá trị của u cho v và xóa u 
            v->data = u->data;
            v->left = v->right = NULL;
            delete u;
        }
        else {
            // Tách v khỏi cây và di chuyển u lên
            if (v == v->parent->left) {
                parent->left = u;
            }
            else {
                parent->right = u;
            }
            delete v;
            u->parent = parent;
            if (uvBlack) {
                // u và v đều đen, sửa hai màu đen ở u 
                fixDoubleBlack(u,t);
            }
            else {
                // u hoặc v đỏ, màu u đen 
                u->color = 0;
            }
        }
        return;
    }

    // v có 2 con, hoán đổi giá trị với kế nhiệm và đệ quy 
    swapValues(u, v);
    deleteNode(u,t);
}

bool hasRedChild(Node* cur) {
    if (cur->left != NULL) {
        if (cur->left->color == 1)return true;
    }
    else if (cur->right != NULL) {
        if (cur->right->color == 1)return true;
    }
    return false;
}

void fixDoubleBlack(Node* x,RedBlackTree& t) {
    if (x == t.root)
        // x là node gốc thì return 
        return;

    Node* sibling = Sibling(x), * parent = x->parent;
    if (sibling == NULL) {
        // Không có sibiling, màu đen kép được đẩy lên 
        fixDoubleBlack(parent,t);
    }
    else {
        if (sibling->color == 1) {
            // Anh chị em màu đỏ 
            parent->color = 1;
            sibling->color = 0;
            if (sibling== sibling->parent->left) {
                // trường hợp left 
                RB_Right_Rotate(t,parent);
            }
            else {
                // trường hợp right 
                RB_Left_Rotate(t,parent);
            }
            fixDoubleBlack(x,t);
        }
        else {
            // Anh chị em đen 
            if (hasRedChild(sibling)) {
                // ít nhất 1 trẻ em màu đỏ 
                if (sibling->left != NULL and sibling->left->color == 1) {
                    if (sibling== sibling->parent->left) {
                        // left left 
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        RB_Right_Rotate(t, parent);
                    }
                    else {
                        // right left 
                        sibling->left->color = parent->color;
                        RB_Right_Rotate(t, sibling);
                        RB_Left_Rotate(t, parent);
                    }
                }
                else {
                    if (sibling == sibling->parent->left) {
                        // left right 
                        sibling->right->color = parent->color;
                        RB_Left_Rotate(t, sibling);
                        RB_Right_Rotate(t, parent);
                    }
                    else {
                        // right right 
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        RB_Left_Rotate(t, parent);
                    }
                }
                parent->color = 0;
            }
            else {
                // hai con đen 
                sibling->color = 1;
                if (parent->color == 0)
                    fixDoubleBlack(parent,t);
                else
                    parent->color = 0;
            }
        }
    }
}
void deleteByVal(RedBlackTree& t,int n) {
    if (t.root == NULL)
        // Tree is empty 
        return;

    Node* v = findNode(t.root,n);

    if (v==NULL) {
        cout << "Doesn't exist " << n << endl;
        return;
    }

    deleteNode(v,t);
}

int main() {
    RedBlackTree t;
    Init(t);
    Node* z = new Node;
    z->data = 10;
    RB_Insert_Node(t, z);
    Node* z2 = new Node;
    z2->data = 20;
    RB_Insert_Node(t, z2);
    Node* z3 = new Node;
    z3->data = 30;
    RB_Insert_Node(t, z3);
    Node* z4 = new Node;
    z4->data = 40;
    RB_Insert_Node(t, z4);
    Node* z5 = new Node;
    z5->data = 50;
    RB_Insert_Node(t, z5);
    Node* z6 = new Node;
    z6->data = 45;
    RB_Insert_Node(t, z6);
    Node* z7 = new Node;
    z7->data = 42;
    RB_Insert_Node(t, z7);
    deleteByVal(t, 20);
    return 0;
}