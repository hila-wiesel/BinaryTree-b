#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <stack>


namespace ariel{
template<typename T> class BinaryTree{
        private:   
            struct Node{
                T data;
                Node *parent;
                Node *left;
                Node *right;

                Node(const T &value): data(value),left(nullptr), right(nullptr), parent(nullptr) {}            
            };

            Node *root ;

            // helper methods:

            void destroy(Node* node){
                if(node != nullptr){
                    destroy(node->right);
                    destroy(node->left);
                    delete node;
                }
            }

            Node *is_in_tree(Node *node ,const T& data) {
                if (node != nullptr) {
                    if (node->data == data) {
                        return node;
                    }
                    Node *ans = is_in_tree(node->left,data);
                    if (ans == nullptr) {
                        ans = is_in_tree(node->right,data);
                    }

                    return ans;

                }
                return nullptr;
            }

            void insert_nodes(Node *thisTree, Node *otherTree) {
                if (otherTree->left != nullptr) {
                    add_left(thisTree->data, otherTree->left->data);
                    insert_nodes(thisTree->left, otherTree->left);
                }
                if (otherTree->right != nullptr) {
                    add_right(thisTree->data, otherTree->right->data);
                    insert_nodes(thisTree->right, otherTree->right);
                }
            }


        public:

            class iteratorInOrder{
                private:
                    Node* curr_node;

                public:
                    iteratorInOrder(Node* ptr = nullptr) :curr_node(ptr){}
                   
                    T* operator->() const { return &(curr_node->data);}
                    T& operator*() const{ return curr_node->data;}
                    bool operator==(const iteratorInOrder& other) const { return curr_node == other.curr_node; }
                    bool operator!=(const iteratorInOrder& other) const { return curr_node != other.curr_node; }


                    iteratorInOrder &operator++() {
                        Node *p = nullptr;
                        if (curr_node == nullptr) {
                            throw std::invalid_argument("Cant increment end of iter");
                        }
                        if (curr_node->right != nullptr) {
                            curr_node = curr_node->right;
                            while (curr_node->left != nullptr) {
                                curr_node = curr_node->left;
                            }
                        } else {
                            p = curr_node->parent;
                            while (p != nullptr && curr_node == p->right) {
                                curr_node = p;
                                p = p->parent;
                            }
                            curr_node = p;
                        }
                        return *this;
                    }


                    iteratorInOrder operator++(int) {
                iteratorInOrder temp = *this;
                Node *p = nullptr;
                if (curr_node == nullptr) {
                    return temp;
                }
                if (curr_node->right != nullptr) {
                    curr_node = curr_node->right;
                    while (curr_node->left != nullptr) {
                        curr_node = curr_node->left;
                    }
                } else {
                    p = curr_node->parent;
                    while (p != nullptr && curr_node == p->right) {
                        curr_node = p;
                        p = p->parent;
                    }
                    curr_node = p;
                }
                return temp;
            }


            };   //end class iteratorInOrder
            
            class iteratorPreOrder{
                private:
                   Node* curr_node;

                public:
                    iteratorPreOrder(Node* ptr_node = nullptr): curr_node(ptr_node){}
                    
                    T* operator->() const{return &(curr_node->data);}
                    T& operator*() const{return curr_node->data;}
                    bool operator==(const iteratorPreOrder& other) const {return curr_node == other.curr_node; }
                    bool operator!=(const iteratorPreOrder& other) const { return curr_node != other.curr_node; }

                    iteratorPreOrder &operator++() {
                        if (curr_node == nullptr) { 
                            throw std::invalid_argument("iter is nullptr, so cant increment end of iter");
                        }
                        if (curr_node->left != nullptr) {
                            curr_node = curr_node->left;
                            return *this;
                        }
                        if (curr_node->right != nullptr) {
                            curr_node = curr_node->right;
                            return *this;
                        }
                        if (curr_node->parent != nullptr && curr_node->parent->right != nullptr &&
                            curr_node->parent->left ==
                            curr_node) { 
                            curr_node = curr_node->parent->right;
                            return *this;
                        }
                        if (curr_node->parent != nullptr) { 
                            while (curr_node == curr_node->parent->right && curr_node ==curr_node->parent->right) {
                                curr_node = curr_node->parent;
                                if (curr_node->parent == nullptr) {
                                    curr_node = nullptr;
                                    return *this;
                                }
                            }
                            if (curr_node->parent->right != nullptr) { 
                                curr_node = curr_node->parent->right;
                                return *this;
                            }
                        }
                        if (curr_node->parent != nullptr && curr_node->parent->left == curr_node) {
                            while (curr_node->parent->right == nullptr || curr_node->parent->right == curr_node) { 
                                curr_node = curr_node->parent;
                                if (curr_node->parent == nullptr) {
                                    curr_node = nullptr;
                                    return *this;
                                }
                            }
                            curr_node = curr_node->parent->right;
                            return *this;
                        }
                        curr_node = nullptr;
                        return *this;
                    }

                    iteratorPreOrder operator++(int) {
                        iteratorPreOrder tmp = *this;
                        if (curr_node == nullptr) { 
                            return tmp;
                        }
                        if (curr_node->left != nullptr) {
                            curr_node = curr_node->left;
                            return tmp;
                        }
                        if (curr_node->right != nullptr) {
                            curr_node = curr_node->right;
                            return tmp;
                        }
                        if (curr_node->parent != nullptr && curr_node->parent->right != nullptr &&
                            curr_node->parent->left ==
                            curr_node) { 
                            curr_node = curr_node->parent->right;
                            return tmp;
                        }
                        if (curr_node->parent != nullptr && curr_node ==curr_node->parent->right) { 
                            while (curr_node == curr_node->parent->right) {
                                curr_node = curr_node->parent;
                                if (curr_node->parent == nullptr) {
                                    curr_node = nullptr;
                                    return tmp;
                                }
                            }
                            if (curr_node->parent->right != nullptr) { 
                                curr_node = curr_node->parent->right;
                                return tmp;
                            }
                        }
                        if (curr_node->parent != nullptr) {
                            while (curr_node->parent->right == nullptr) { 
                                curr_node = curr_node->parent;
                                if (curr_node->parent == nullptr) {
                                    curr_node = nullptr;
                                    return tmp;
                                }
                            }
                            curr_node = curr_node->parent->right;
                            return tmp;
                        }
                        curr_node = nullptr;
                        return tmp;
                    }


            };   //end class iteratorPreOrder

            class iteratorPostOrder{
                private:
                   Node* curr_node;

                public:
                    iteratorPostOrder(Node* ptr = nullptr): curr_node(ptr){} 

                    T* operator->() const{ return &(curr_node->data); }
                    T& operator*() const { return curr_node->data;}
                    bool operator==(const iteratorPostOrder& other) const { return curr_node == other.curr_node;}
                    bool operator!=(const iteratorPostOrder& other) const { return curr_node != other.curr_node;}

                    iteratorPostOrder &operator++() {
                        if (curr_node == nullptr) {
                            throw std::invalid_argument("Cant increment end of iter");
                        }
                        if (curr_node->parent == nullptr) {
                            curr_node = nullptr;
                            return *this;
                        }
                        if (curr_node->parent->right == curr_node) { // right child
                            curr_node = curr_node->parent;
                            return *this;
                        }   // left child
                        if (curr_node->parent->right != nullptr) {
                            curr_node = curr_node->parent->right;
                            while (curr_node->left != nullptr || curr_node->right != nullptr) {
                                if (curr_node->left != nullptr) {
                                    curr_node = curr_node->left;
                                } else if (curr_node->right != nullptr) {
                                    curr_node = curr_node->right;
                                }
                            }
                            return *this;
                        }
                        curr_node = curr_node->parent;
                        return *this;
                    }


                    iteratorPostOrder operator++(int) {
                iteratorPostOrder temp = *this;
                if (curr_node == nullptr) {
                    return temp;
                }
                if (curr_node->parent == nullptr) {
                    curr_node = nullptr;
                    return temp;
                }
                if (curr_node->parent->right == curr_node) { // right child
                    curr_node = curr_node->parent;
                    return temp;
                }   // left child
                if (curr_node->parent->right != nullptr) {
                    curr_node = curr_node->parent->right;
                    while (curr_node->left != nullptr || curr_node->right != nullptr) {
                        if (curr_node->left != nullptr) {
                            curr_node = curr_node->left;
                        } else if (curr_node->right != nullptr) {
                            curr_node = curr_node->right;
                        }
                    }
                    return temp;
                }
                curr_node = curr_node->parent;
                return temp;
            }

                    
            };  //end class iteratorPostOrder

            BinaryTree<T>(): root(nullptr){}
            ~BinaryTree(){ destroy(this->root);}


            BinaryTree(BinaryTree const &tree) : root(nullptr){
                add_root(tree.root->data);
                insert_nodes(this->root, tree.root);
            }
            
            BinaryTree(BinaryTree &&other) noexcept {
                this->root = other.root;
                other.root = nullptr;
            }


            BinaryTree<T> &operator=(const BinaryTree<T> &other_tree) {
                if (this != &other_tree) {
                    add_root(other_tree.root->data);
                    insert_nodes(this->root, other_tree.root);
                }
                return *this;
            }
            
            BinaryTree &operator=(BinaryTree &&other_tree) noexcept {
                this->root = other_tree.root;
                other_tree.root = nullptr;
                return *this;
            }
            
            BinaryTree& add_root(const T &root_val){
                if (this->root == nullptr){
                    Node *newNode = new Node(root_val);
                    root = newNode;
                }
                root->data = root_val;
                return *this;
            }

            BinaryTree& add_left(const T& parent_val,const T& left_val){
                Node *parent = is_in_tree(this->root,parent_val);
                if(parent == nullptr){
                    throw std::invalid_argument("This node is not exist");
                }
                if(parent->left != nullptr){  
                    parent->left->data = left_val;
                }
                else{
                    Node *new_left = new Node(left_val);
                    new_left->parent = parent;
                    parent->left = new_left;
                }
                return *this;    
            }

            BinaryTree& add_right(const T& parent_val,const T& right_val){
                Node *parent = is_in_tree(root,parent_val);
                if(parent == nullptr){
                    throw std::invalid_argument{"This node is not exist"};
                }
                if(parent->right != nullptr){  
                    parent->right->data = right_val;
                }
                else{
                    Node *new_right = new Node(right_val);
                    new_right->parent = parent;
                    parent->right = new_right;
                }
                return *this;  
            }
            

            // begin & end iterators:

            iteratorInOrder begin_inorder(){
                if (root == nullptr){
                    return iteratorInOrder(nullptr);
                }

                Node * node = root;
                while (node->left != nullptr){
                    node = node->left;
                }
                return iteratorInOrder(node);
            }

            iteratorPreOrder begin_preorder(){return iteratorPreOrder(this->root);}

            iteratorPostOrder begin_postorder(){
                if (root == nullptr){
                    return iteratorPostOrder(nullptr);
                }
                Node * node = root;
                while (node->left != nullptr || node->right != nullptr){
                    if (node->left != nullptr) {
                        node = node->left;
                    } 
                    else if (node->right != nullptr ){
                        node = node->right;
                    }                        
                }
                return iteratorPostOrder(node);
            }

            iteratorInOrder end_inorder(){return iteratorInOrder(nullptr);}
            iteratorPreOrder end_preorder(){ return iteratorPreOrder(nullptr);}
            iteratorPostOrder end_postorder(){return iteratorPostOrder(nullptr);}


            iteratorInOrder begin() {return begin_inorder();}
            iteratorInOrder end() {return iteratorInOrder(nullptr);}

            friend std::ostream& operator<<(std::ostream& out,const BinaryTree<T> &tree){return out;}

    };
}





