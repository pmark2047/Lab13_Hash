/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
public:
   
   //
   // Construct
   //
   
   list(const A& a = A())
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }
   
   list(list <T, A> & rhs, const A& a = A())
   {
      numElements = 0;
      pHead = pTail = nullptr;
      
      Node* pCurrent = rhs.pHead;
      
      while (pCurrent != nullptr)
      {
         push_back(pCurrent->data);
         pCurrent = pCurrent->pNext;
      }
   }
   
   list(list <T, A>&& rhs, const A& a = A());
   list(size_t num, const T & t, const A& a = A());
   list(size_t num, const A& a = A());
   list(const std::initializer_list<T>& il, const A& a = A())
   {
      numElements = 0;
      pHead = pTail = nullptr;
      
      for (const auto& item : il)
         push_back(item);
   }
   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A())
   {
      numElements = 0;
      pHead = pTail = nullptr;
      
      while (first != last)
      {
         push_back(*first);
         first++;
      }
   }
   ~list()
   {
       clear();
   }
   
   //
   // Assign
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);
   list <T, A> & operator = (list <T, A> && rhs);
   list <T, A> & operator = (const std::initializer_list<T>& il);

   //
   // Iterator
   //
   
   class iterator;
   iterator begin()  { return iterator (pHead);   }
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   
   //
   // Access
   //
    iterator find(const T & element)
    {
        for (auto it = begin(); it != end(); it++)
        {
            if (*it == element)
                return it;
        }
        return end();
    }
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return numElements == 0; }
   size_t size() const { return numElements; }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list <T, A> :: Node
{
public:
   //
   // Construct
   //
   Node() : data()
   {
      pNext = pPrev = nullptr;
   }
   Node(const T& data) : data(data)
   {
      pNext = pPrev = nullptr;
   }
   Node(T&& data) : data(std::move(data))
   {
      pNext = pPrev = nullptr;
   }

   //
   // Member Variables
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list <T, A> :: iterator
{
public:
   // constructors, destructors, and assignment operator
    iterator() : p(nullptr) {}
   
    iterator(Node * pRHS) : p(pRHS) {}
        
    iterator(const iterator  & rhs) : p(rhs.p) {}
   
    iterator & operator = (const iterator & rhs)
    {
        p = rhs.p;
        return *this;
    }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return p == rhs.p; }
   bool operator != (const iterator & rhs) const { return p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return p->data;                // return what is inside the node
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
       iterator pReturn = *this;
       p = p->pNext;                 // increment to next node
       return pReturn;               // return unincremented iterator
   }

   // prefix increment
   iterator & operator ++ ()
   {
       p = p->pNext;                 // increment to next node
       return *this;                 // return incremented iterator
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
       iterator pReturn = *this;
       p = p->pPrev;                  // increment to previous node
       return pReturn;                // return unchanged iterator
   }

   // prefix decrement
   iterator & operator -- ()
   {
       p = p->pPrev;                  // increment to previous node
       return *this;                  // return updated iterator
   }

   // two friends who need to access p directly
   friend iterator list <T, A> :: insert(iterator it, const T &  data);
   friend iterator list <T, A> :: insert(iterator it,       T && data);
   friend iterator list <T, A> :: erase(const iterator & it);

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif

   typename list <T, A> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const T & t, const A& a)
{
   numElements = 0;
   pHead = pTail = nullptr;
   
   for (size_t i = 0; i < num; ++i)
   {
      push_back(t);
   }
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const A& a)
{
   numElements = 0;
   pHead = pTail = nullptr;
   
   for (size_t i = 0; i < num; ++i)
   {
      Node* pNew = new Node();
      
      if (pHead == nullptr)
      {
         pHead = pTail = pNew;
      }
      else
      {
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;
      }
      numElements++;
   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (list <T, A> && rhs)
{
   if(this == &rhs)
      return *this;
   
   clear();
   
   this->pHead = rhs.pHead;
   this->pTail = rhs.pTail;
   this->numElements = rhs.numElements;
   
   rhs.pHead = nullptr;
   rhs.pTail = nullptr;
   rhs.numElements = 0;
   
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A> & list <T, A> :: operator = (list <T, A> & rhs)
{
   if(this == &rhs)
      return *this;
   
   clear();
   for(Node* pCurrent = rhs.pHead; pCurrent != nullptr; pCurrent = pCurrent->pNext)
   {
      push_back(pCurrent->data);
   }
   
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
{
   clear();
   
   for(const auto & item : rhs)
      push_back(item);
      
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
void list <T, A> :: clear()
{
   Node* pCurrent = pHead;
   
   while(pCurrent != nullptr)
   {
      Node* pNextNode = pCurrent->pNext;
      delete pCurrent;
      pCurrent = pNextNode;
   }
   
   pHead = nullptr;
   pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_back(const T & data)
{
   Node* pNew = new Node(data);
   
   if (pHead == nullptr)
   {
      pHead = pTail = pNew;
   }
   else
   {
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
   }
   
   numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_back(T && data)
{
   Node* pNew = new Node(std::move(data));
   
   if (pHead == nullptr)
   {
      pHead = pTail = pNew;
   }
   else
   {
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
   }
   
   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_front(const T & data)
{
    Node* pNew = new Node(data);                // new node from given data
    
    if (pHead == nullptr)                       // if it's empty
    {
       pHead = pTail = pNew;
    }
    else                                        // if it's not...
    {
       pHead->pPrev = pNew;                     // add new node before current head
       pNew->pNext = pHead;                     // adjust other nodes accordingly
       pHead = pNew;
    }
    
    numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_front(T && data)
{
    Node* pNew = new Node(std::move(data));     // same as previous except using move
    
    if (pHead == nullptr)
    {
       pHead = pTail = pNew;
    }
    else
    {
       pHead->pPrev = pNew;
       pNew->pNext = pHead;
       pHead = pNew;
    }
    
    numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{

}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{

}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: front()
{
    if (empty())                    // throw an error if empty
        throw "ERROR: unable to access data from an empty list";
   return pHead->data;              // otherwise, return the data from front node
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: back()
{
    if (empty())                    // throw error if empty
        throw "ERROR: unable to access data from an empty list";
   return pTail->data;              // return data from back node
}


/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
    iterator itNext = end();
   
    if (!it.p)
        return end();
    if (it.p->pNext)
    {
        it.p->pNext->pPrev = it.p->pPrev;
        itNext = it.p->pNext;
    }
    else
    {
        pTail = pTail->pPrev;
    }
    if (it.p->pPrev)
    {
        it.p->pPrev->pNext = it.p->pNext;
    }
    else
    {
        pHead = pHead->pNext;
    }
    
    delete it.p;
    numElements--;
    return itNext;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
                                                 const T & data)
{
   Node* pNew = new Node(data);
   if(pHead == nullptr)
      pHead = pTail = pNew;
   
   else if(it.p == nullptr)
   {
      pNew->pPrev = pTail;
      pTail->pNext = pNew;
      pTail = pNew;
   }
   else if(it.p == pHead)
   {
      pNew->pNext = pHead;
      pHead->pPrev = pNew;
      pHead = pNew;
   }
   else
   {
      pNew->pNext = it.p;
      pNew->pPrev = it.p->pPrev;
      it.p->pPrev->pNext = pNew;
      it.p->pPrev = pNew;
   }
   
   numElements++;
   return iterator(pNew);
}


/******************************************
 * LIST :: INSERT
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
   T && data)
{
   Node* pNew = new Node(std::move(data));
   if(pHead == nullptr)
      pHead = pTail = pNew;
   
   else if(it.p == nullptr)
   {
      pNew->pPrev = pTail;
      pTail->pNext = pNew;
      pTail = pNew;
   }
   else if(it.p == pHead)
   {
      pNew->pNext = pHead;
      pHead->pPrev = pNew;
      pHead = pNew;
   }
   else
   {
      pNew->pNext = it.p;
      pNew->pPrev = it.p->pPrev;
      it.p->pPrev->pNext = pNew;
      it.p->pPrev = pNew;
   }
   
   numElements++;
   return iterator(pNew);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
    lhs.swap(rhs);
}

}; // namespace custom


