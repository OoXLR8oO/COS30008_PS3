
// COS30008, Problem Set 3, 2023

#pragma once

#include "DoublyLinkedList.h"
#include "DoublyLinkedListIterator.h"

#include <algorithm>
#include <cassert>

template<typename T>
class List
{
private:
    using Node = typename DoublyLinkedList<T>::Node;
    
    Node fHead;		// first element
    Node fTail;		// last element
    size_t fSize;	// number of elements
    
public:

    using Iterator = DoublyLinkedListIterator<T>;
    
    List() noexcept :
        fHead(nullptr),
        fTail(nullptr),
        fSize(0)
    {}

	// copy semantics
    List(const List& aOther) : 
        fHead(nullptr), 
        fTail(nullptr), 
        fSize(0) 
    {
        Node lCurrentNode = aOther.fHead;
        while (lCurrentNode) 
        {
            push_back(lCurrentNode->fData);
            lCurrentNode = lCurrentNode->fNext;
        }
    }

    List& operator=(const List& aOther)
    {
        if (this != &aOther)
        {
            this->~List();  // Free this List
                new (this) List(aOther);  // In-place new copy List initialization
        }
        return *this;
    }

	// move semantics
    List(List&& aOther) noexcept : fHead(nullptr), fTail(nullptr), fSize(0)
    {
        swap(aOther);
    }

    List& operator=(List&& aOther) noexcept
    {
        if (this != &aOther)
        {
            swap(aOther);
        }
        return *this;
    }

    void swap(List& aOther) noexcept
    {
        std::swap(fHead, aOther.fHead);
        std::swap(fTail, aOther.fTail);
        std::swap(fSize, aOther.fSize);
    }

	// basic operations
    size_t size() const noexcept
    {
        return fSize;
    }

    template<typename U>
    void push_front(U&& aData)
    {
        Node lNewNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
        if (!fHead)
        {
            fHead = fTail = lNewNode;
        }
        else
        {
            lNewNode->fNext = fHead;
            fHead->fPrevious = lNewNode;
            fHead = lNewNode;
        }
        ++fSize;
    }

    template<typename U>
    void push_back(U&& aData)
    {
        Node lNewNode = DoublyLinkedList<T>::makeNode(std::forward<U>(aData));
        if (!fHead)
        {
            fHead = fTail = lNewNode;
        }
        else
        {
            lNewNode->fPrevious = fTail;
            fTail->fNext = lNewNode;
            fTail = lNewNode;
        }
        ++fSize;
    }

    void remove(const T& aElement) noexcept 
    {
        Node lCurrentNode = fHead;
        while (lCurrentNode) 
        {
            if (lCurrentNode->fData == aElement) 
            {
                if (lCurrentNode == fHead) 
                {
                    fHead = lCurrentNode->fNext;
                }
                if (lCurrentNode == fTail) 
                {
                    fTail = lCurrentNode->fPrevious.lock();
                }
                lCurrentNode->isolate();
                --fSize;
                return;
            }
            lCurrentNode = lCurrentNode->fNext;
        }
    }


    const T& operator[](size_t aIndex) const
    {
        assert(aIndex < fSize);
        Node lCurrentNode = fHead; 
        for (size_t i = 0; i < aIndex; ++i) 
        {
            lCurrentNode = lCurrentNode->fNext;
        }
        return lCurrentNode->fData;
    }

	// iterator interface
    Iterator begin() const noexcept {
        return Iterator(fHead, fTail).begin();
    }

    Iterator end() const noexcept {
        return Iterator(fHead, fTail).end();
    }

    Iterator rbegin() const noexcept {
        return Iterator(fHead, fTail).rbegin();
    }

    Iterator rend() const noexcept {
        return Iterator(fHead, fTail).rend();
    }
};
