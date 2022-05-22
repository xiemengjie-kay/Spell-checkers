// AVLSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"
#include <queue>
#include <algorithm>
#include <cmath>


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;

private:
    struct Node
    {
        ElementType value;
        int height;
        Node* left;
        Node* right;
    };

private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    unsigned int sz;
    bool shouldBalance;
    Node* root;

private:
    // insert() follows nptr to recursively find an appropriate place to 
    // add the element, and returns nptr
    Node* insert(Node* nptr, const ElementType& element);
    
    // maxHeight() calculates and returns the max height of the AVLTree
    // that nptr points to
    int maxHeight(Node* nptr) const noexcept;

    // preorderT() recursively traverses the elements in preorder
    // in the AVL Tree and calls the visit function on each element
    void preorderT(Node* nptr, VisitFunction visit) const;
    
    // inorderT() recursively traverses the elements in inorder
    // in the AVL Tree and calls the visit function on each element
    void inorderT(Node* nptr, VisitFunction visit) const;

    // postorderT() recursively traverses the elements in postorder
    // in the AVL Tree and calls the visit function on each element
    void postorderT(Node* nptr, VisitFunction visit) const;
    
    // copyNodes() copies the nodes in the source to the target
    void copyNodes(Node*& target, const Node* source);
    
    // deallocateTree() deallocates the AVLTree that root points to
    void deallocateTree() noexcept;

    // difference() calculate the difference between the height of
    // the current tree's left sub-tree and the height of its right sub-tree
    int difference(Node* n) const noexcept;

    // LL() implements LL rotation on the current Node
    Node* LL(Node* n);
    
    // RR() implements RR rotation on the current Node
    Node* RR(Node* n);
    
    // LR() implements LR rotation on the current Node
    Node* LR(Node* n);
    
    // RL() implements RL rotation on the current Node
    Node* RL(Node* n);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    :sz{0}, shouldBalance{shouldBalance}, root{nullptr}
{
}


template <typename ElementType>
void AVLSet<ElementType>::deallocateTree() noexcept
{
    if (root != nullptr)
    {
        std::queue<Node*> Q;
        Q.push(root);
        while (!Q.empty())
        {
            Node* temp = Q.front();
            Q.pop();
            if (temp->left != nullptr) Q.push(temp->left);
            if (temp->right != nullptr) Q.push(temp->right);
            delete temp;
        }
    }
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{   
    deallocateTree();
}


template <typename ElementType>
void AVLSet<ElementType>::copyNodes(Node*& target, const Node* source)
{
    if (source != nullptr)
    {
        target = new Node{source->value, source->height, nullptr, nullptr};
        copyNodes(target->left, source->left);
        copyNodes(target->right, source->right);
    }
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
    : sz{0}, shouldBalance{true}, root{nullptr}
{
    Node* tempTree = nullptr;
    copyNodes(tempTree, s.root);
    sz = s.sz;
    shouldBalance = s.shouldBalance;
    root = tempTree;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
    : sz{0}, shouldBalance{true}, root{nullptr}
{
    std::swap(sz, s.sz);
    std::swap(shouldBalance, s.shouldBalance);
    std::swap(root, s.root);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if (this != &s)
    {
        Node* tempTree = nullptr;
        copyNodes(tempTree, s.root);
        sz = s.sz;
        shouldBalance = s.shouldBalance;
        deallocateTree();  // deallocate the old AVLTree
        root = tempTree;
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    std::swap(sz, s.sz);
    std::swap(shouldBalance, s.shouldBalance);
    std::swap(root, s.root);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
int AVLSet<ElementType>::difference(Node* n) const noexcept
{
    return std::abs(((n->left != nullptr) ? n->left->height : -1) - 
        ((n->right != nullptr) ? n->right->height : -1));
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::LL(Node* n)
{
    Node* B = n;
    Node* T2 = n->left->right;
    n = n->left;
    n->right = B;
    B->left = T2;
    // adjust new height
    B->height = std::max(((B->left != nullptr) ? B->left->height : -1), 
        ((B->right != nullptr) ? B->right->height : -1)) + 1;
    n->height = std::max(((n->left != nullptr) ? n->left->height : -1), 
        ((n->right != nullptr) ? n->right->height : -1)) + 1;
    return n;
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::RR(Node* n)
{
    Node* A = n;
    Node* T2 = n->right->left;
    n = n->right;
    n->left = A;
    A->right = T2;
    // adjust new height
    A->height = std::max(((A->left != nullptr) ? A->left->height :-1),
        ((A->right != nullptr) ? A->right->height : -1)) + 1;
    n->height = std::max(((n->left != nullptr) ? n->left->height :-1),
        ((n->right != nullptr) ? n->right->height : -1)) + 1;
    return n;
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::LR(Node* n)
{
    Node* C = n;
    Node* A = n->left;
    Node* B = n->left->right;
    Node* T2 = B->left;
    Node* T3 = B->right;
    n = B;
    B->left = A;
    B->right = C;
    A->right = T2;
    C->left = T3;
    A->height = std::max(((A->left != nullptr) ? A->left->height :-1),
        ((A->right != nullptr) ? A->right->height : -1)) + 1;
    C->height = std::max(((C->left != nullptr) ? C->left->height :-1),
        ((C->right != nullptr) ? C->right->height : -1)) + 1;
    n->height = std::max(((n->left != nullptr) ? n->left->height :-1),
        ((n->right != nullptr) ? n->right->height : -1)) + 1;
   return n;
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::RL(Node* n)
{
    Node* A = n;
    Node* C = A->right;
    Node* B = C->left;
    Node* T2 = B->left;
    Node* T3 = B->right;
    n = B;
    B->left = A;
    B->right = C;
    A->right = T2;
    C->left = T3;
    A->height = std::max(((A->left != nullptr) ? A->left->height :-1),
        ((A->right != nullptr) ? A->right->height : -1)) + 1;
    C->height = std::max(((C->left != nullptr) ? C->left->height :-1),
        ((C->right != nullptr) ? C->right->height : -1)) + 1;
    n->height = std::max(((n->left != nullptr) ? n->left->height :-1),
        ((n->right != nullptr) ? n->right->height : -1)) + 1;
   return n;
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::insert(Node* n, const ElementType& element)
{
    if (n == nullptr)
    {
        n = new Node{element, 0, nullptr, nullptr};
        sz++;
        return n;
    }
    else
    {
        if (element < n->value)
        {
            n->left = insert(n->left, element);
        }
        else if (element > n->value)
        {
            n->right = insert(n->right, element);
        }
        n->height++;
        if (shouldBalance && difference(n) > 1)
        {
            if (element < n->value && element < n->left->value) n = LL(n);
            else if (element > n->value &&element >n->right->value) n = RR(n);
            else if (element < n->value &&element >n->left->value) n = LR(n);
            else if (element > n->value &&element <n->right->value) n = RL(n);
        }
        n->height = std::max(((n->left != nullptr) ? n->left->height : -1), 
            ((n->right != nullptr) ? n->right->height : -1)) + 1;
    }
    return n;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    root = insert(root, element);
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    Node* current = root;
    while(current != nullptr)
    {
        if (current->value == element) return true;
        if (element < current->value)
        {
            current = current->left;
        }
        else if (element > current->value)
        {
            current = current->right;
        }
    }
    return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
int AVLSet<ElementType>::maxHeight(Node* nptr) const noexcept
{
    if (nptr == nullptr) 
    {
        return -1;
    }
    else
    {
        return nptr->height;
    }
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return maxHeight(root);
}


template <typename ElementType>
void AVLSet<ElementType>::preorderT(Node* nptr, VisitFunction visit) const
{
    if (nptr != nullptr)
    {
        visit(nptr->value);
        preorderT(nptr->left, visit);
        preorderT(nptr->right, visit);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderT(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::inorderT(Node* nptr, VisitFunction visit) const
{
    if (nptr != nullptr)
    {
        inorderT(nptr->left, visit);
        visit(nptr->value);
        inorderT(nptr->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderT(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::postorderT(Node* nptr, VisitFunction visit) const
{
    if (nptr != nullptr)
    {
        postorderT(nptr->left, visit);
        postorderT(nptr->right, visit);
        visit(nptr->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderT(root, visit);
}



#endif

