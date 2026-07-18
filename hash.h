/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   
namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
          typename Hash = std::hash<T>,
          typename EqPred = std::equal_to<T>,
          typename A = std::allocator<T> >
class unordered_set
{
public:
   //
   // Construct
   //
   unordered_set() : maxLoadFactor(1.0), numElements(0), buckets(8)
   {
   }
    
   unordered_set(size_t numBuckets) : maxLoadFactor(1.0), numElements(0), buckets(numBuckets)
   {
   }
   unordered_set(const unordered_set&  rhs) 
   {
       *this = rhs;
   }
   unordered_set(unordered_set&& rhs) 
   {
       *this = std::move(rhs);
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last)
   {
       reserve(last - first);
       for (iterator it = first; first != last; it++)
           insert(it);
   }

   //
   // Assign
   //
   unordered_set& operator=(const unordered_set& rhs)
   {
       numElements = rhs.numElements;
       maxLoadFactor = rhs.maxLoadFactor;
       buckets = rhs.buckets;
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
       numElements = rhs.numElements;
       maxLoadFactor = rhs.maxLoadFactor;
       buckets = std::move(rhs.buckets);
       
       // set rhs to default
       rhs.numElements = 0;
       rhs.maxLoadFactor = 1.0;
       rhs.buckets.resize(8);
      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
       clear();
       reserve(il.size());
       for (const auto &item : il)
           insert(item);
      return *this;
   }
   void swap(unordered_set& rhs)
   {
       std::swap(numElements, rhs.numElements);
       std::swap(maxLoadFactor, rhs.maxLoadFactor);
       std::swap(buckets, rhs.buckets);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      if (empty() || buckets.empty())
         return end();
      
      for (auto it = buckets.begin(); it != buckets.end(); it++)
      {
         if (!(*it).empty())
            return iterator(buckets.end(), it, (*it).begin());
      }
      return end();
   }
   
   iterator end()
   {
      typename custom::list<T, A>::iterator endList;
      if (!buckets.empty())
         endList = buckets[0].end();
      
      return iterator(buckets.end(), buckets.end(), endList);
   }
   
   local_iterator begin(size_t iBucket)
   {
      if (iBucket < buckets.size())
         return local_iterator(buckets[iBucket].begin());
      return local_iterator();
   }
   local_iterator end(size_t iBucket)
   {
      if (iBucket < buckets.size())
         return local_iterator(buckets[iBucket].end());
      return local_iterator();
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      Hash hasher;
      return hasher(t) % buckets.size();
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);
   void rehash(size_t numBuckets);
   void reserve(size_t num)
   {
   }

   // 
   // Remove
   //
   void clear() noexcept
   {
      for (size_t i = 0; i < buckets.size(); ++i)
      {
         buckets[i].clear();
      }
      numElements = 0;
   }
   
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const 
   { 
      return numElements;
   }
   bool empty() const 
   { 
      return numElements == 0;
   }
   size_t bucket_count() const 
   { 
      return buckets.size();
   }
   size_t bucket_size(size_t i) const
   {

      return buckets[i].size();
   }
   float load_factor() const noexcept 
   { 
      if (buckets.empty())
         return 0.0;
      return numElements / buckets.size();
   }
   float max_load_factor() const noexcept
   { 
      return maxLoadFactor;
   }
   void  max_load_factor(float m)
   {
      maxLoadFactor = m;
   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif

   size_t min_buckets_required(size_t num) const
   {
      return std::ceil(num / maxLoadFactor);
   }

   custom::vector<custom::list<T,A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
public:
   // 
   // Construct
   iterator() : itVectorEnd(), itList(), itVector()
   {
      this->itVectorEnd.p = nullptr;
      this->itVector.p    = nullptr;
      this->itList.p      = nullptr;
   }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator &itList)
   {
      this->itVectorEnd = itVectorEnd;
      this->itVector = itVector;
      this->itList = itList;
   }
   iterator(const iterator& rhs) 
   { 
      *this = rhs;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      this->itVectorEnd = rhs.itVectorEnd;
      this->itVector    = rhs.itVector;
      this->itList      = rhs.itList;
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   {
      return !(*this == rhs);
   }
   
   bool operator == (const iterator& rhs) const
   {
      if (this->itVector == this->itVectorEnd && rhs.itVector == rhs.itVectorEnd)
         return true;
      
      if ((this->itVector == this->itVectorEnd) != (rhs.itVector == rhs.itVectorEnd))
         return false;
      
      return (this->itVector == rhs.itVector && this->itList == rhs.itList);
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      iterator old = *this;
      ++(*this);
      return old;
   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
public:
   // 
   // Construct
   //
   local_iterator() : itList()
   {
      this->itList.p = nullptr;
   }
   local_iterator(const typename custom::list<T>::iterator& itList) 
   {
      this->itList = itList;
   }
   local_iterator(const local_iterator& rhs) 
   {
      *this = rhs;
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      this->itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return this->itList != rhs.itList;
   }
   bool operator == (const local_iterator& rhs) const
   {
      return this->itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      local_iterator old = *this;
      ++(*this);
      return old;
   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T,Hash,E,A>::erase(const T& t)
{
   return iterator();
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
    // check if it's empty and rehash if necessary
    if (buckets.empty())
        rehash(8);
    
    // get the index
    size_t index = bucket(t);
    
    // check for the element in the buckets
    for (auto it = buckets[index].begin(); it != buckets[index].end(); it++)
    {
        // if it is there, return
        if (*it == t)
        {
            typename custom::vector<custom::list<T, A>>::iterator itVec(index, buckets);
            iterator itHash(buckets.end(), itVec, it);
            
            return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, false);
        }
    }
    
    // check if we need more space
    if (min_buckets_required(numElements + 1) > bucket_count())
    {
        // reserve more space
        reserve(numElements * 2);
        index = bucket(t);
    }
    
    // add the element
    buckets[index].push_back(t);
    numElements++;
    
    // get iterator to new element
    typename custom::list<T, A>::iterator itAdded = buckets[index].find(t);
    // get vector iterator
    typename custom::vector<custom::list<T, A>>::iterator itVec(index, buckets);
    // create hash iterator
    iterator itHash(buckets.end(), itVec, itAdded);
    // return results
    return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, true);
}
template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T> & il)
{
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
}


/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
    // is it empty already?
    if (buckets.empty())
        return end();
    
    // get the index of t
    size_t index = bucket(t);
    
    // find t in the list and put it in an iterator
    typename custom::list<T, A>::iterator itList = buckets[index].find(t);
    
    if (itList != buckets[index].end())
    {
        typename custom::vector<custom::list<T, A>>::iterator itVector(index, buckets);
        return iterator(buckets.end(), itVector, itList);
    }
    else
        return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator & unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   if (itVector == itVectorEnd)
      return *this;
   ++itList;
   while (itVector != itVectorEnd && itList == (*itVector).end())
   {
      ++itVector;
      if (itVector != itVectorEnd)
      {
         itList = (*itVector).begin();
      }
   }
   if (itVector == itVectorEnd)
   {
      itList = typename custom::list<T>::iterator();
   }
   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
    lhs.swap(rhs);
}

}
