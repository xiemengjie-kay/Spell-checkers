// HashSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"
#include <algorithm>


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    struct Node
    {
        ElementType value;
        Node* next;
    };

private:
    HashFunction hashFunction;

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    unsigned int sz;
    unsigned int cap;
    Node** hashArray;
private:
    // deallocateHashTable() deallocates the hash table that hashArray
    // points to
    void deallocateHashTable() noexcept;
    
    // copyHashArray() copies all the elements in the source into the target
    void copyHashArray(Node** target, Node** source, unsigned int cap);
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, sz{0}, cap{DEFAULT_CAPACITY},
      hashArray{new Node*[DEFAULT_CAPACITY]}
{
    // make all the cells in the hashTable (Array) point to NULL 
    // when initializing
   for (unsigned int i = 0; i < cap; i++)
   {
        hashArray[i] = nullptr;
   }
}


template <typename ElementType>
void HashSet<ElementType>::deallocateHashTable() noexcept
{
    // deallocate all the Nodes in the hash Array
    for (unsigned int i = 0; i < cap; i++)
    {
        Node* current = hashArray[i];
        while (current != nullptr)
        {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
    }
    delete[] hashArray;    
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    deallocateHashTable();
}


template <typename ElementType>
void HashSet<ElementType>::copyHashArray(Node** target, Node** source, unsigned int cap)
{
    for (unsigned int i = 0; i < cap; i++)
    {
        Node* current = source[i];
        while (current != nullptr)
        {
            Node* after = target[i];
            target[i] = new Node{current->value, after};
            current = current->next;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>},
    sz{0}, cap{DEFAULT_CAPACITY}, hashArray{new Node*[DEFAULT_CAPACITY]}
{
    for (unsigned int i = 0; i < cap; i++)
    {
        hashArray[i] = nullptr;
    }

    unsigned int tempCap = s.cap;
    Node** tempArray = new Node*[tempCap];
    for (unsigned int i = 0; i < tempCap; i++)
    {
        tempArray[i] = nullptr;
    }
    
    copyHashArray(tempArray, s.hashArray, s.cap);

    deallocateHashTable();
    hashArray = tempArray;
    hashFunction = s.hashFunction;
    sz = s.sz;
    cap = s.cap;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>},
    sz{0}, cap{DEFAULT_CAPACITY}, hashArray{new Node*[DEFAULT_CAPACITY]}
{
    for (unsigned int i = 0; i < cap; i++)
    {
        hashArray[i] = nullptr;
    }
    std::swap(sz, s.sz);
    std::swap(cap, s.cap);
    std::swap(hashFunction, s.hashFunction);
    std::swap(hashArray, s.hashArray);
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        unsigned int tempCap = s.cap;
        Node** tempArray = new Node*[tempCap];
        for (unsigned int i = 0; i < tempCap; i++)
        {
            tempArray[i] = nullptr;
        }
        
        copyHashArray(tempArray, s.hashArray, s.cap);

        deallocateHashTable();
        hashArray = tempArray;
        hashFunction = s.hashFunction;
        sz = s.sz;
        cap = s.cap;
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if (this != &s)
    {
        std::swap(sz, s.sz);
        std::swap(cap, s.cap);
        std::swap(hashArray, s.hashArray);
        std::swap(hashFunction, s.hashFunction);
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
   if (!contains(element))
   {
        unsigned int hashValue = hashFunction(element);
        unsigned int index = hashValue % cap;
        Node* after = hashArray[index];
        hashArray[index] = new Node{element, after};
        sz++;
        if (static_cast<float>(sz)/cap > 0.8)
        {
            unsigned int newCap = cap * 2 + 1;
            Node** tempArray = new Node*[newCap];
            for (unsigned int i = 0; i < newCap; i++)
            {
                tempArray[i] = nullptr;
            }
            
            // rehash into a temporary array
            for (unsigned int i = 0; i < cap; i++)
            {
                Node* current = hashArray[i];
                while (current != nullptr)
                {
                    unsigned int newIndex = hashFunction(current->value) 
                        % newCap;
                    Node* after = tempArray[newIndex];
                    tempArray[newIndex] = new Node{current->value, after};
                    current = current->next;
                }
            }

            deallocateHashTable();
            hashArray = tempArray;
            cap = newCap;
        }
   }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int hashValue = hashFunction(element);
    unsigned int index = hashValue % cap;
    Node* searchBucket = hashArray[index];
    while (searchBucket != nullptr)
    {
        if (searchBucket->value == element) return true;
        searchBucket = searchBucket->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if (index >= cap) return 0;
    Node* current = hashArray[index];
    unsigned int count = 0;
    while (current != nullptr)
    {
        count++;
        current = current->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index >= cap) return false;
    Node* current = hashArray[index];
    while (current != nullptr)
    {
        if (current->value == element) return true;
        current = current->next;
    }
    return false;
}



#endif

