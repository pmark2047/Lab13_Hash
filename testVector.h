/***********************************************************************
 * Header:
 *    TEST VECTOR
 * Summary:
 *    Unit tests for vector
 * Author:
 *    James Helfrich, PhD. (c) 2022 by Kendall Hunt
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include <vector>
#include "vector.h"
#include "unitTest.h"
#include "spy.h"

#include <cassert>
#include <memory>

class TestVector : public UnitTest
{

public:
   void run()
   {
      reset();
      
      // Construct
      test_construct_default();
      test_construct_sizeZero();
      test_construct_sizeFour();
      test_construct_sizeFourFill(); // NYI
      test_constructCopy_empty();
      test_constructCopy_standard();
      test_constructCopy_partiallyFilled(); // NYI
      test_constructMove_empty();
      test_constructMove_standard();
      test_constructMove_partiallyFilled();
      test_constructInit_empty();
      test_constructInit_standard();
      test_destructor_empty();
      test_destructor_standard();
      test_destructor_partiallyFilled();

      // Assign
      test_assign_empty();
      test_assign_sameSize();
      test_assign_rightBigger();
      test_assign_leftBigger(); // NYI
      test_assignMove_empty();
      test_assignMove_sameSize();
      test_assignMove_rightBigger();
      test_assignMove_leftBigger();
      test_assign_fullToFull();
      test_assignMove_fullToFull();
      test_swap_empty();
      test_swap_sameSize();
      test_swap_rightBigger();
      test_swap_leftBigger();

      // Iterator
      test_iterator_beginEmpty();
      test_iterator_beginFull();
      test_iterator_endFull();
      test_iterator_incrementFull(); // NYI
      test_iterator_dereferenceReadFull();
      test_iterator_dereferenceUpdate();
      test_iterator_construct_default();
      test_iterator_construct_pointer();
      test_iterator_construct_index();
      test_iterator_equals_same();
      test_iterator_equals_different();
      test_iterator_notEquals_same();
      test_iterator_notEquals_different();

      // Access
      test_subscript_read();
      test_subscript_write();
      test_front_read();
      test_front_write();
      test_back_read();
      test_back_write();
      test_back_partiallyfilled(); // NYI

      // Insert
      test_pushback_empty();
      test_pushback_excessCapacity();
      test_pushback_requireReallocate(); // NYI
      test_pushback_moveEmpty();
      test_pushback_moveExcessCapacity();
      test_pushback_moveRequireReallocate(); 
      test_resize_emptyZero();
      test_resize_emptyFourDefault();
      test_resize_emptyFourValue();
      test_resize_fourZero();
      test_resize_fourSixDefault(); // NYI
      test_resize_fourSixValue();
      test_reserve_emptyZero();
      test_reserve_emptyTen();
      test_reserve_fourZero();
      test_reserve_fourFour();
      test_reserve_fourTen();
      test_reserve_standardZero();
      test_reserve_standardTen(); // NYI

      // Remove
      test_popback_empty();
      test_popback_full();
      test_popback_partiallyFilled();
      test_clear_empty();
      test_clear_full();
      test_clear_partiallyFilled(); // NYI
      test_shrink_empty();
      test_shrink_toEmpty();
      test_shrink_standard();
      test_shrink_twoExtraSlots();
      
      // Status
      test_size_empty();
      test_size_full();
      test_empty_empty();
      test_empty_emptyWithCapacity();
      test_empty_full();
      test_capacity_empty();
      test_capacity_full();

      report("Vector");
   }
   
   /***************************************
    * CONSTRUCTOR
    ***************************************/
   
   // Vector default constructor, no allocations
   void test_construct_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v;
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown

   // Vector size constructor: allocate space for zero
   void test_construct_sizeZero()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v(0);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown

   // Vector size constructor: allocate space for four
   void test_construct_sizeFour()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v(4);
      // verify
      assertUnit(Spy::numDefault() == 4);   // Spy default-construct [00,00,00,00]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 00 | 00 | 00 | 00 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      { 
         assertUnit(v.data[0] == Spy());
         assertUnit(v.data[1] == Spy());
         assertUnit(v.data[2] == Spy());
         assertUnit(v.data[3] == Spy());
      }
      assertUnit(v.numElements == 4);
      assertUnit(v.numCapacity == 4);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector size fill constructor: allocate space for four and fill with 10
   void test_construct_sizeFourFill()
   {  // setup
        // VALUE: 99
      // exercise
      // verify
        // Spy copy-construct: [99,99,99,99]
        // Spy allocate: [99,99,99,99]
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 99 | 99 | 99 | 99 |
        //    +----+----+----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }


   /***************************************
    * DESTRUCTOR
    ***************************************/

   // Vector destructor: an empty vector
   void test_destructor_empty()
   {  // setup
      {
         // SRC: empty
         custom::vector<Spy> v;
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
   }  // verify

   // Vector destructor: a 4-element collection
   void test_destructor_standard()
   {  // setup
      {
         //      0    1    2    3
         //    +----+----+----+----+
         //    | 26 | 49 | 67 | 89 |
         //    +----+----+----+----+
         custom::vector<Spy> v;
         setupStandardFixture(v);
         Spy::reset();
      } // exercise
      // verify
      assertUnit(Spy::numDestructor() == 4); // Spy destroy: [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);     // Spy delete:  [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
   }

   // Vector destructor: a 2-element, 4-capacity collection
   void test_destructor_partiallyFilled()
   {  // setup
      {
         //      0    1    2    3
         //    +----+----+----+----+
         //    | 26 | 49 |    |    |
         //    +----+----+----+----+
         custom::vector<Spy> v;
         v.data = v.alloc.allocate(4);
         v.alloc.construct(&v.data[0], Spy(99));
         v.alloc.construct(&v.data[1], Spy(99));
         v.numElements = 2;
         v.numCapacity = 4;
         Spy::reset();
      }  // exercise
      assertUnit(Spy::numDestructor() == 2); // Spy destroy: [26,49]
      assertUnit(Spy::numDelete() == 2);     // Spy delete:  [26,49]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
   }
   
   /***************************************
    * COPY CONSTRUCTOR
    ***************************************/
   
   // Vector copy constructor: an empty vector
   void test_constructCopy_empty()
   {  // setup
      // SRC: empty
      custom::vector<Spy> vSrc;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(vSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown
   
   // Vector copy constructor: a 4-element collection
   void test_constructCopy_standard()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(vSrc);
      // verify
      assertUnit(Spy::numCopy() == 4);     // Spy copy constructor: [26,49,67,89]
      assertUnit(Spy::numAlloc() == 4);    // Spy allocate: [26,49,67,89]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(vSrc.data != vDest.data);
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // Vector copy constructor: a 2-element, 4-capacity collection
   void test_constructCopy_partiallyFilled()
   {  // setup
        // SRC:
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 |    |    |
        //    +----+----+----+----+
      // exercise
      // verify
        // Spy copy constructor: [26,49]
        // Spy allocate: [26,49]
        // SRC:
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 |    |    |
        //    +----+----+----+----+
        // DES:
        //      0    1
        //    +----+----+
        //    | 26 | 49 |
        //    +----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }
   
   /***************************************
    * MOVE CONSTRUCTOR
    ***************************************/
   
   // Vector move constructor: an empty vector
   void test_constructMove_empty()
   {  // setup
      // SRC: empty
      custom::vector<Spy> vSrc;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(std::move(vSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES: empty
      assertEmptyFixture(vDest);
   }  // teardown
   
   // Vector move constructor: a 4-element collection
   void test_constructMove_standard()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      Spy * p = vSrc.data;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(std::move(vSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      assertUnit(p == vDest.data);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // Vector move constructor: a 2-element, 4-capacity collection
   void test_constructMove_partiallyFilled()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(4);
      vSrc.alloc.construct(&vSrc.data[0], Spy(26));
      vSrc.alloc.construct(&vSrc.data[1], Spy(49));
      vSrc.numElements = 2;
      vSrc.numCapacity = 4;
      Spy::reset();
      // exercise
      custom::vector<Spy> vDest(std::move(vSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(26));
         assertUnit(vDest.data[1] == Spy(49));
      }
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.numCapacity == 4);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   /***************************************
    * CONSTRUCTOR INITIALIZE LIST
    ***************************************/
   
   // Vector initialization list: empty
   void test_constructInit_empty()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy> v{};
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector initialization list: the standard fixture
   void test_constructInit_standard()
   {  // setup
      std::initializer_list<Spy> l{Spy(26),Spy(49),Spy(67),Spy(89)};
      Spy::reset();
      // exercise
      custom::vector<Spy> v(l); // same as vector<Spy> v{...}
      // verify
      assertUnit(Spy::numCopy() == 4);       // Spy copy-construct: [26,49,67,89]
      assertUnit(Spy::numAlloc() == 4);      // Spy allocate: [26,49,67,89]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   /***************************************
    * RESIZE, RESERVE AND SHRINK-TO-FIT
    ***************************************/
   
   // Vector resize: an empty vector resized with zero elements
   void test_resize_emptyZero()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.resize(0);
      // verify
      assertUnit(v.numCapacity == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector resize: an empty vector resized to four
   void test_resize_emptyFourDefault()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.resize(4);
      // verify
      assertUnit(Spy::numDefault() == 4);    // Spy default [00,00,00,00]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 00 | 00 | 00 | 00 |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 4);
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[0] == Spy());
         assertUnit(v.data[1] == Spy());
         assertUnit(v.data[2] == Spy());
         assertUnit(v.data[3] == Spy());
      }
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector resize: empty vector to four elements with the provided value 99
   void test_resize_emptyFourValue()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy s(99);
      Spy::reset();
      // exercise
      v.resize(4, s);
      // verify
      assertUnit(Spy::numCopy() == 4);     // Spy copy: [99,99,99,99]
      assertUnit(Spy::numAlloc() == 4);    // Spy allocate: [99,99,99,99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 4);
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[0] == Spy(99));
         assertUnit(v.data[1] == Spy(99));
         assertUnit(v.data[2] == Spy(99));
         assertUnit(v.data[3] == Spy(99));
      }
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector resize: from standard fixture to zero elements
   void test_resize_fourZero()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.resize(0);
      // verify
      assertUnit(Spy::numDestructor() == 4);  // Spy destroy: [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);      // Spy delete: [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector resize: grow the standard fixture to 6 elements with default values
   void test_resize_fourSixDefault()
   {  // setup
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 | 67 | 89 |
        //    +----+----+----+----+
      // exercise
      // verify
        // Spy copy-move: [26,49,67,89]
        // Spy destory now-empty: [26,49,67,89]
        // Spy create: [00,00]
        //      0    1    2    3    4    5
        //    +----+----+----+----+----+----+
        //    | 26 | 49 | 67 | 89 | 00 | 00 |
        //    +----+----+----+----+----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }
   
   // Vector resize: standard fixture to 6 elements with provided value 99
   void test_resize_fourSixValue()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      Spy s(99);
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.resize(6, s);
      // verify
      assertUnit(Spy::numCopyMove() == 4);  // Spy copy-move [26,49,67,89]
      assertUnit(Spy::numDestructor() == 4);// Spy destroy old [26,49,67,89]
      assertUnit(Spy::numCopy() == 2);      // Spy copy-create [99,99]
      assertUnit(Spy::numAlloc() == 2);     // Spy allocate [99,99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 | 99 | 99 |
      //    +----+----+----+----+----+----+
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 6);
      if (v.data && v.numElements >= 6)
      {
         assertUnit(v.data[4] == Spy(99));
         assertUnit(v.data[5] == Spy(99));
      }
      v.numCapacity = 4;
      v.numElements = 4;
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector reserve: zero on an empty vector
   void test_reserve_emptyZero()
   {  // setup
      // SRC: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.reserve(0);
      // verify
      assertUnit(v.numCapacity == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector reserve: increase the capacity on an empty vector to ten
   void test_reserve_emptyTen()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.reserve(10);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3    4    5    6    7    8    9
      //    +----+----+----+----+----+----+----+----+----+----+
      //    |    |    |    |    |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+----+----+----+----+
      assertUnit(v.numCapacity == 10);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector reserve: try to decrease the capacity. Nothing changes!
   void test_reserve_fourZero()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.reserve(0);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector reserve: change the capacity to its current size
   void test_reserve_fourFour()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.reserve(4);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector reserve increase the capacity from 4 slots to 10
   void test_reserve_fourTen()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.reserve(10);
      // verify
      assertUnit(Spy::numCopyMove() == 0);   
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3    4    5    6    7    8    9
      //    +----+----+----+----+----+----+----+----+----+----+
      //    |    |    |    |    |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+----+----+----+----+
      assertUnit(v.numCapacity == 10);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector reserve: attempt to reserve 0 when we already have four elements
   void test_reserve_standardZero()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.reserve(0);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector reserve: reserve six extra spots from the standard fixture
   void test_reserve_standardTen()
   {  // setup
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 | 67 | 89 |
        //    +----+----+----+----+
      // exercise
      // verify
        // Spy copy-move [26,49,67,89]
        // Spy destroy the now-empty [26,49,67,89]
        //      0    1    2    3    4    5    6    7    8    9
        //    +----+----+----+----+----+----+----+----+----+----+
        //    | 26 | 49 | 67 | 89 |    |    |    |    |    |    |
        //    +----+----+----+----+----+----+----+----+----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }
   
   // Vector shrink to fit when already empty
   void test_shrink_empty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.shrink_to_fit();
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector shrink to fit: remove extra capacity when there are no elements
   void test_shrink_toEmpty()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.numElements = 0;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.shrink_to_fit();
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector shrink to fit: attempt to shrink from standard fixture
   void test_shrink_standard()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.shrink_to_fit();
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector shrink to fit: shrink a vector with two extra slots
   void test_shrink_twoExtraSlots()
   {  // setup
      //      0    1    2    3    4    6
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 |    |    |
      //    +----+----+----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(6);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.alloc.construct(&v.data[3], Spy(89));
      v.numElements = 4;
      v.numCapacity = 6;
      Spy::reset();
      // exercise
      v.shrink_to_fit();
      // verify
      assertUnit(Spy::numCopyMove() == 4);  // Spy copy move: [26,49,67,89] to new buffer
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);     // Spy allocated: nothing
      assertUnit(Spy::numDestructor() == 0);// Spy destroyed: nothing
      assertUnit(Spy::numDelete() == 0);    
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    6
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 89 |    |    |
      //    +----+----+----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   /***************************************
    * SIZE EMPTY CAPACITY
    ***************************************/

   // Vector size: an empty vector
   void test_size_empty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      // exercise
      size_t size = v.size();
      // verify
      assertUnit(0 == size);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector size: standard fixture
   void test_size_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      size_t size = v.size();
      // verify
      assertUnit(4 == size);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector empty: an empty vector
   void test_empty_empty()
   {  // setup
      // SRC: empty
      custom::vector<Spy> v;
      // exercise
      bool empty = v.empty();
      // verify
      assertUnit(true == empty);
      // SRC: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector empty: a vector with no size but capacity of 6 
   void test_empty_emptyWithCapacity()
   {  // setup
      //      0    1    2    3    4    5
      //    +----+----+----+----+----+----+
      //    |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(6);
      v.numCapacity = 6;
      v.numElements = 0;
      // exercise
      bool empty = v.empty();
      // verify
      assertUnit(empty == true);
      //      0    1    2    3    4    5
      //    +----+----+----+----+----+----+
      //    |    |    |    |    |    |    |
      //    +----+----+----+----+----+----+
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 0);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector empty: a standard fixture
   void test_empty_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      bool empty = v.empty();
      // verify
      assertUnit(false == empty);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector capacity: an empty vector
   void test_capacity_empty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      // exercise
      size_t capacity = v.capacity();
      // verify
      assertUnit(0 == capacity);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown
   
   // Vector capacity: a 3-element 4-capacity vector
   void test_capacity_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      v.numElements = 3;
      // exercise
      size_t capacity = v.capacity();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      assertUnit(4 == capacity);
      v.numElements = 4;
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   /***************************************
    * SWAP
    ***************************************/

   // Vector swap: two empty vectors
   void test_swap_empty()
   {  // setup
      // SRC: empty
      custom::vector<Spy> vSrc;
      // DES: empty
      custom::vector<Spy> vDest;
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES: empty
      assertEmptyFixture(vDest);
   }  // teardown

   // Vector swap: source and destination are same size
   void test_swap_sameSize()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      vDest.data[0] = Spy(99);
      vDest.data[1] = Spy(99);
      vDest.data[2] = Spy(99);
      vDest.data[3] = Spy(99);
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 0);   
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      assertUnit(vSrc.numCapacity == 4);
      assertUnit(vSrc.numElements == 4);
      assertUnit(vSrc.data != nullptr);
      if (vSrc.data)
      {
         assertUnit(vSrc.data[0] == Spy(99));
         assertUnit(vSrc.data[1] == Spy(99));
         assertUnit(vSrc.data[2] == Spy(99));
         assertUnit(vSrc.data[3] == Spy(99));
      }
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // Vector swap: when the destination is smaller than the source
   void test_swap_rightBigger()
   {  // setup
      // SRC
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vDest;
      vDest.data = vDest.alloc.allocate(2);
      vDest.alloc.construct(&vDest.data[0], Spy(99));
      vDest.alloc.construct(&vDest.data[1], Spy(99));
      vDest.numElements = 2;
      vDest.numCapacity = 2;
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numCopy() == 0);       
      assertUnit(Spy::numAlloc() == 0);      
      assertUnit(Spy::numDestructor() == 0); 
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vSrc.numCapacity == 2);
      assertUnit(vSrc.numElements == 2);
      assertUnit(vSrc.data != nullptr);
      if (vSrc.data)
      {
         assertUnit(vSrc.data[0] == Spy(99));
         assertUnit(vSrc.data[1] == Spy(99));
      }
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // Vector swap: when the destination is bigger than the source
   void test_swap_leftBigger()
   {  // setup
      // SRC:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(2);
      vSrc.alloc.construct(&vSrc.data[0], Spy(99));
      vSrc.alloc.construct(&vSrc.data[1], Spy(99));
      vSrc.numElements = 2;
      vSrc.numCapacity = 2;
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      Spy::reset();
      // exercise
      vDest.swap(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 0); 
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);    
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      // DES:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vDest.numCapacity == 2);
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(99));
         assertUnit(vDest.data[1] == Spy(99));
      }
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }


   /***************************************
    * ASSIGN 
    ***************************************/
   
   // Vector assignment: when both are empty
   void test_assign_empty()
   {  // setup
      // SRC: empty
      custom::vector<Spy> vSrc;
      // DES: empty
      custom::vector<Spy> vDest;
      Spy::reset();
      // exercise
      vDest = vSrc;
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES: empty
      assertEmptyFixture(vDest);
   }  // teardown
   
   // Vector assignment: when source and destination are same size
   void test_assign_sameSize()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      vDest.data[0] = Spy(99);
      vDest.data[1] = Spy(99);
      vDest.data[2] = Spy(99);
      vDest.data[3] = Spy(99);
      Spy::reset();
      // exercise
      vDest = vSrc;
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numAssign() == 4);  // Spy assign: 99<--26,99<--49,99<--67,99<--89
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // Vector assignment: when the destination is smaller than the source
   void test_assign_rightBigger()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vDest;
      vDest.data = vDest.alloc.allocate(2);
      vDest.alloc.construct(&vDest.data[0], Spy(99));
      vDest.alloc.construct(&vDest.data[1], Spy(99));
      vDest.numElements = 2;
      vDest.numCapacity = 2;
      Spy::reset();
      // exercise
      vDest = vSrc;
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numCopy() == 4);       // Spy copy: 99<--26,99<--49,?<--67,?<--89
      assertUnit(Spy::numAlloc() == 4);      // the copy involves a new buffer for [26,49,67,89]
      assertUnit(Spy::numDestructor() == 2); // Spy destroy [99,99]
      assertUnit(Spy::numDelete() == 2);     // Spy delete [99,99]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }
   
   // Vector assignment: when the destination is bigger than the source
   void test_assign_leftBigger()
   {  // setup
        // SRC:
        //      0    1
        //    +----+----+
        //    | 99 | 99 |
        //    +----+----+
        // DES:
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 | 67 | 89 |
        //    +----+----+----+----+
      // exercise
      // verify
        // Spy assign 26<--99,49<--99
        // Spy destroy [67,89]
        // Spy delete [67,89]
        // SRC:
        //      0    1
        //    +----+----+
        //    | 99 | 99 |
        //    +----+----+
        // DES:
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 99 | 99 |    |    |
        //    +----+----+----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }


   // Vector assignment: when destination has same capacity
   void test_assign_fullToFull()
   {  // Setup
      // SRC:
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //    +----+----+----+----+
      //    | 11 | 99 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vDes;
      try
      {
         vDes.data = vDes.alloc.allocate(4);
         vDes.alloc.construct(&vDes.data[0], Spy(11));
         vDes.alloc.construct(&vDes.data[1], Spy(99));
         vDes.numElements = 2;
         vDes.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
      Spy::reset();
      // exercise
      vDes = vSrc;
      // verify
      assertUnit(Spy::numAssign() == 2); // Spy assign: 11<--26, 99<--49
      assertUnit(Spy::numCopy() == 2);   // Spy copy: [67,89]
      assertUnit(Spy::numAlloc() == 2);  // Spy allocate: [67,89]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC:
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vSrc);
      // DES:
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDes);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDes);
   }   

   // Vector move assignment: when there is nothing to copy
   void test_assignMove_empty()
   {  // setup
      custom::vector<Spy> vSrc;
      custom::vector<Spy> vDest;
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertEmptyFixture(vSrc);
      assertEmptyFixture(vDest);
   }  // teardown

   // Vector move assignment: when source and destination are same size
   void test_assignMove_sameSize()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 99 | 99 | 99 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      vDest.data[0] = Spy(99);
      vDest.data[1] = Spy(99);
      vDest.data[2] = Spy(99);
      vDest.data[3] = Spy(99);
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numDestructor() == 4);  // Spy destroy: [99,99,99,99]
      assertUnit(Spy::numDelete() == 4);      // Spy delete:  [99,99,99,99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // Vector move assignment: when the destination is smaller than the source
   void test_assignMove_rightBigger()
   {  // setup
      // SRC:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vDest;
      vDest.data = vDest.alloc.allocate(2);
      vDest.alloc.construct(&vDest.data[0], Spy(99));
      vDest.alloc.construct(&vDest.data[1], Spy(99));
      vDest.numElements = 2;
      vDest.numCapacity = 2;
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numDestructor() == 2); // Spy destroy: [99,99]
      assertUnit(Spy::numDelete() == 2);     // Spy delete:  [99,99]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDest);
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // Vector move assignment: when the destination is bigger than the source
   void test_assignMove_leftBigger()
   {  // setup
      // SRC:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      custom::vector<Spy> vSrc;
      vSrc.data = vSrc.alloc.allocate(2);
      vSrc.alloc.construct(&vSrc.data[0], Spy(99));
      vSrc.alloc.construct(&vSrc.data[1], Spy(99));
      vSrc.numElements = 2;
      vSrc.numCapacity = 2;
      // DES:
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vDest;
      setupStandardFixture(vDest);
      Spy::reset();
      // exercise
      vDest = std::move(vSrc);
      // verify
      assert(vDest.data != vSrc.data);
      assertUnit(Spy::numDestructor() == 4); // Spy destroy: [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);     // Spy delete:  [26,49,67,89]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC:
      //    empty
      assertEmptyFixture(vSrc);
      // DES:
      //      0    1
      //    +----+----+
      //    | 99 | 99 |
      //    +----+----+
      assertUnit(vDest.numCapacity == 2);
      assertUnit(vDest.numElements == 2);
      assertUnit(vDest.data != nullptr);
      if (vDest.data)
      {
         assertUnit(vDest.data[0] == Spy(99));
         assertUnit(vDest.data[1] == Spy(99));
      }
      // teardown
      teardownStandardFixture(vSrc);
      teardownStandardFixture(vDest);
   }

   // Vector assignment move: when destination has same capacity
   void test_assignMove_fullToFull()
   {  // setup
      // SRC:
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      // DES:
      //    +----+----+----+----+
      //    | 11 | 99 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> vDes;
      try
      {
         vDes.data = vDes.alloc.allocate(4);
         vDes.alloc.construct(&vDes.data[0], Spy(11));
         vDes.alloc.construct(&vDes.data[1], Spy(99));
         vDes.numElements = 2;
         vDes.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
      Spy::reset();
      // exercise
      vDes = std::move(vSrc);
      // verify
      assertUnit(Spy::numDestructor() == 2); // Spy destroy: [11,99]
      assertUnit(Spy::numDelete() == 2);     // Spy delete:  [11,99]
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      // SRC: empty
      assertEmptyFixture(vSrc);
      // DES:
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(vDes);
      // teardown
      teardownStandardFixture(vDes);
   }

   /***************************************
    * SUBSCRIPT
    ***************************************/

   // Vector subscript: read slot 1 into the spy 99
   void test_subscript_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      const custom::vector<Spy> v(vSrc); // sorry, I need a copy constructor
      teardownStandardFixture(vSrc);
      // VALUE: 99
      Spy value(99);
      Spy::reset();
      // exercise
      value = v[1];
      // verify
      assertUnit(Spy::numAssign() == 1);    // Spy assign: 99 <-- 49
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // VALUE: 49
      assertUnit(value == Spy(49));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
   }  // teardown

   // Vector write one element using square brackets
   void test_subscript_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // VALUE: 99
      Spy value(99);
      Spy::reset();
      // exercise
      v[1] = value;
      // verify
      assertUnit(Spy::numAssign() == 1);  // Spy assign: 49 <-- 99
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 99 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(v.data[1] == Spy(99));
      v.data[1] = Spy(49);
      assertStandardFixture(v);
      // VALUE: 99
      assertUnit(value == Spy(99));
      // teardown
      teardownStandardFixture(v);
   }

   // Vector front: read the first value from standard fixture
   void test_front_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      const custom::vector<Spy> v(vSrc); // sorry, need the copy constructor
      teardownStandardFixture(vSrc);
      // VALUE: 99
      Spy value(99);
      Spy::reset();
      // exercise
      value = v.front();
      // verify
      assertUnit(Spy::numAssign() == 1);    // Spy assign: 99 <-- 26
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // VALUE: 26
      assertUnit(value == Spy(26));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
   }

   // Vector front: write the value 99 to the front of the standard fixture
   void test_front_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // VALUE: 99
      Spy value(99);
      Spy::reset();
      // exercise
      v.front() = value;
      // verify
      assertUnit(Spy::numAssign() == 1);  // SPY assign 26 <-- 99
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 99 | 49 | 67 | 89 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      { 
         assertUnit(v.data[0] == Spy(99));
         v.data[0] = Spy(26);
      }
      assertStandardFixture(v);
      // VALUE: 99
      assertUnit(Spy(99) == value);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector back: read the back element of the standard fixture into the value 99
   void test_back_read()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> vSrc;
      setupStandardFixture(vSrc);
      const custom::vector<Spy> v(vSrc);
      teardownStandardFixture(vSrc);  // sorry, need the copy constructor
      // VALUE: 99
      Spy value(99);
      Spy::reset();
      // exercise
      value = v.back();
      // verify
      assertUnit(Spy::numAssign() == 1);    // Spy assign: 99 <-- 89
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // VALUE: 89
      assertUnit(value == Spy(89));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+      
      assertStandardFixture(v);
   }  // teardown

   // Vector back: set the value 99 to the back of the standard fixture
   void test_back_write()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // VALUE: 99
      Spy value(99);
      Spy::reset();
      // exercise
      v.back() = value;
      // verify
      assertUnit(Spy::numAssign() == 1);  // Spy assign: 89 <-- 99
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[3] == Spy(99));
         v.data[3] = Spy(89);
      }
      assertStandardFixture(v);
      // VALUE: 99
      assertUnit(value == Spy(99));
      // teardown
      teardownStandardFixture(v);
   }

   // Vector back: read an element from the back of a partially filled vector
   void test_back_partiallyfilled()
   {  // setup
        //      0    1
        //    +----+----+----+----+
        //    | 11 | 22 |    |    |
        //    +----+----+----+----+
        // VALUE: 99
      // exercise
      // verify
        // Spy assign: 99 <-- 22
        // VALUE: 22
        //       0    2
        //    +----+----+----+----+
        //    | 11 | 22 |    |    |
        //    +----+----+----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }


   /***************************************
    * POP BACK
    ***************************************/

   // Vector pop back: remove an element from an empty vector
   void test_popback_empty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.pop_back();
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown

   // Vector pop back: from standard fixture
   void test_popback_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.pop_back();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // Spy destroy: [89]
      assertUnit(Spy::numDelete() == 1);       // Spy delete:  [89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 3);
      assertUnit(v.data != nullptr);
      if (v.data != nullptr)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(49));
         assertUnit(v.data[2] == Spy(67));
      }
      // teardown
      teardownStandardFixture(v);
   }

   // Vector pop back: from a partially filled vector
   void test_popback_partiallyFilled()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 |    |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.numElements = 2;
      v.numCapacity = 4;
      Spy::reset();
      // exercise
      v.pop_back();
      // verify
      assertUnit(Spy::numDestructor() == 1);   // Spy destroy: [49]
      assertUnit(Spy::numDelete() == 1);       // Spy delete:  [49]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 1);
      assertUnit(v.data != nullptr);
      if (v.data != nullptr)
      {
         assertUnit(v.data[0] == Spy(26));
      }      // teardown
      teardownStandardFixture(v);
   }

   /***************************************
    * CLEAR
    ***************************************/
   
   // Vector clear: an empty collection
   void test_clear_empty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy::reset();
      // exercise
      v.clear();
      // verify
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      // V: empty
      assertEmptyFixture(v);
   }  // teardown

   // Vector clear: from standard fixture
   void test_clear_full()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      v.clear();
      // verify
      assertUnit(Spy::numDestructor() == 4);   // Spy destroy: [26,49,67,89]
      assertUnit(Spy::numDelete() == 4);       // Spy delete: [26,49,67,89]
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    |    |    |    |    |
      //    +----+----+----+----+
      assertUnit(v.numCapacity == 4);
      assertUnit(v.numElements == 0);
      assertUnit(v.data != nullptr);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vector clear: from a partially filled vector
   void test_clear_partiallyFilled()
   {  // setup
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 |    |    |
        //    +----+----+----+----+
      // exercise
      // verify
        // Spy destroy: [26,49]
        // Spy delete: [26,49]
        //      0    1    2    3
        //    +----+----+----+----+
        //    |    |    |    |    |
        //    +----+----+----+----+
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }

   
   /***************************************
    * PUSH BACK
    ***************************************/
   
   // Vector push back: to an empty vector
   void test_pushback_empty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);           // copy: [99]
      assertUnit(Spy::numAlloc() == 1);          // allocate: [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(v.data != nullptr);
      if (v.data)
         assertUnit(v.data[0] == Spy(99));
      assertUnit(v.numCapacity == 1);
      assertUnit(v.numElements == 1);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector push back: add an element to the back when there is room. No reallocation
   void test_pushback_excessCapacity()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 |    |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 4;
      Spy s(89);
      Spy::reset();
      // exercise
      v.push_back(s);
      // verify
      assertUnit(Spy::numCopy() == 1);           // Spy copy: [99]
      assertUnit(Spy::numAlloc() == 1);          // Spy allocate: [99]
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector push back: add an element to the back when there is not room. Capacity should double
   void test_pushback_requireReallocate()
   {  // setup
        //      0    1    2  
        //    +----+----+----+
        //    | 26 | 49 | 67 |
        //    +----+----+----+
        // VALUE: 99
      // exercise
      // verify
        // Spy move: [26,49,67] 
        // Spy destroy: [26,49,67]
        // Spy copy: [99]
        // Spy allocate: [99]
        //      0    1    2    3    4    5   
        //    +----+----+----+----+----+----+
        //    | 26 | 49 | 67 | 99 |    |    |
        //    +----+----+----+----+----+----+
        // VALUE: 99
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }

   // Vector push back move: add an element to the back when empty
   void test_pushback_moveEmpty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      // Value: 99
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);       // Spy copy move: [99]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);       
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(v.data != nullptr);
      if (v.data)
         assertUnit(v.data[0] == Spy(99));
      assertUnit(v.numCapacity == 1);
      assertUnit(v.numElements == 1);
      // VALUE: 0
      assertUnit(s == Spy());
      // teardown
      teardownStandardFixture(v);
   }

   // Vector push back move: add an element to the back when there is room. No reallocation
   void test_pushback_moveExcessCapacity()
   {  // setup
     //      0    1    2    3
     //    +----+----+----+----+
     //    | 26 | 49 | 67 |    |
     //    +----+----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(4);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 4;
      // VALUE: 89
      Spy s(89);
      Spy::reset();
      // exercise
      v.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 1);   // Spy copy move: [89] 
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);         
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 99 |
      //    +----+----+----+----+
      assertStandardFixture(v);
      // VALUE: 0
      assertUnit(s == Spy());
      // teardown
      teardownStandardFixture(v);
   }

   // Vector push back move: add an element to the back when there is not room. Capacity should double
   void test_pushback_moveRequireReallocate()
   {  // setup
      //      0    1    2  
      //    +----+----+----+
      //    | 26 | 49 | 67 |
      //    +----+----+----+
      custom::vector<Spy> v;
      v.data = v.alloc.allocate(3);
      v.alloc.construct(&v.data[0], Spy(26));
      v.alloc.construct(&v.data[1], Spy(49));
      v.alloc.construct(&v.data[2], Spy(67));
      v.numElements = 3;
      v.numCapacity = 3;
      // VALUE: 99
      Spy s(99);
      Spy::reset();
      // exercise
      v.push_back(std::move(s));
      // verify
      assertUnit(Spy::numCopyMove() == 4);       // move [26,49,67] and [99]
      assertUnit(Spy::numDestructor() == 3);     // destroy empty [26,49,67]
      assertUnit(Spy::numCopy() == 0);           
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3    4    5   
      //    +----+----+----+----+----+----+
      //    | 26 | 49 | 67 | 99 |    |    |
      //    +----+----+----+----+----+----+
      assertUnit(v.data != nullptr);
      if (v.data && v.numElements >= 4)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(49));
         assertUnit(v.data[2] == Spy(67));
         assertUnit(v.data[3] == Spy(99));
      }
      assertUnit(v.numCapacity == 6);
      assertUnit(v.numElements == 4);
      // VALUE: 0
      assertUnit(s == Spy());      
      // teardown
      teardownStandardFixture(v);
   }


   /***************************************
    * ITERATOR
    ***************************************/
   
   // Vector begin interator: empty collection
   void test_iterator_beginEmpty()
   {  // setup
      // V: empty
      custom::vector<Spy> v;
      // exercise
      custom::vector<Spy>::iterator it = v.begin();
      // verify
      // it: NULL
      assertUnit(it.p == nullptr);
      // V:empty
      assertEmptyFixture(v);
   }  // teardown
  
   // Vector begin iterator: the first element of the standard fixture
   void test_iterator_beginFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      custom::vector<Spy>::iterator it = v.begin();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //      it
      assertUnit(v.data != nullptr);
      assertUnit(it.p != nullptr);
      if (v.data && it.p)
      {
         assertUnit(it.p == &(v.data[0]));
         assertUnit(*(it.p) == 26);
      }
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector end: the end iterator for the standard fixture
   void test_iterator_endFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      // exercise
      custom::vector<Spy>::iterator it = v.end();
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                           it
      assertUnit(it.p == &(v.data[4]));
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector prefix increment: from middle of standard fixture
   void test_iterator_incrementFull()
   {  // setup
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 | 67 | 89 |
        //    +----+----+----+----+
        //           it
      // exercise
      // verify
        //      0    1    2    3
        //    +----+----+----+----+
        //    | 26 | 49 | 67 | 89 |
        //    +----+----+----+----+
        //                it
      assertUnit(NOT_YET_IMPLEMENTED);
      // teardown
   }

   // Vector iterator dereference: from the middle of the standard fixture
   void test_iterator_dereferenceReadFull()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator it;
      it.p = &(v.data[1]);
      // exercise
      Spy value = *it;
      // verify
      // IT = 49
      assertUnit(value == Spy(49));
      assertUnit(it.p == &(v.data[1]));
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector iterator dereference: change a value
   void test_iterator_dereferenceUpdate()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator it;
      it.p = &(v.data[1]);
      // exercise
      *it = Spy(99);
      // verify
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 99 | 67 | 89 |
      //    +----+----+----+----+
      //           it
      assertUnit(v.data != nullptr);
      if (v.data)
      {
         assertUnit(v.data[0] == Spy(26));
         assertUnit(v.data[1] == Spy(99));
         assertUnit(v.data[2] == Spy(67));
         assertUnit(v.data[3] == Spy(89));
         assertUnit(it.p == &(v.data[1]));
      }
      assertUnit(v.numElements == 4);
      assertUnit(v.numCapacity == 4);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector interator default constructor
   void test_iterator_construct_default()
   {  // setup
      Spy::reset();
      // exercise
      custom::vector<Spy>::iterator it;
      // verify
      assertUnit(Spy::numCopyMove() == 0); 
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.p == nullptr);
   }  // teardown

   // Vector iterator non-default constructor: create an iterator at a specific location in the vector
   void test_iterator_construct_pointer()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      custom::vector<Spy>::iterator it(v.data + 2);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      assertUnit(it.p == v.data + 2);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }
   
   // Vecotr iterator non-default constructor: create an iterator based on a vector and an index
   void test_iterator_construct_index()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      custom::vector<Spy> v;
      setupStandardFixture(v);
      Spy::reset();
      // exercise
      custom::vector<Spy>::iterator it(2, v);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                it
      assertUnit(it.p == v.data + 2);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector iterator equals: same
   void test_iterator_equals_same()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                itLHS
      //                itRHS
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator itLHS;
      custom::vector<Spy>::iterator itRHS;
      itLHS.p = itRHS.p = v.data + 2;
      bool b = false;
      Spy::reset();
      // exercise
      b = (itLHS == itRHS);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(b == true);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                itLHS
      //                itRHS
      assertStandardFixture(v);
      assertUnit(itLHS.p == v.data + 2);
      assertUnit(itRHS.p == v.data + 2);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector iterator equals : different
   void test_iterator_equals_different()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //         itLHS       itRHS
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator itLHS;
      custom::vector<Spy>::iterator itRHS;
      itLHS.p = v.data + 1;
      itRHS.p = v.data + 3;
      bool b = true;
      Spy::reset();
      // exercise
      b = (itLHS == itRHS);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(b == false);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //         itLHS       itRHS
      assertUnit(itLHS.p == v.data + 1);
      assertUnit(itRHS.p == v.data + 3);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector iterator not equals: the same
   void test_iterator_notEquals_same()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                itLHS
      //                itRHS
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator itLHS;
      custom::vector<Spy>::iterator itRHS;
      itLHS.p = itRHS.p = v.data + 2;
      bool b = true;
      Spy::reset();
      // exercise
      b = (itLHS != itRHS);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(b == false);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //                itLHS
      //                itRHS
      assertUnit(itLHS.p == v.data + 2);
      assertUnit(itRHS.p == v.data + 2);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   // Vector iterator not queals :  differnet
   void test_iterator_notEquals_different()
   {  // setup
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //         itLHS       itRHS
      custom::vector<Spy> v;
      setupStandardFixture(v);
      custom::vector<Spy>::iterator itLHS;
      custom::vector<Spy>::iterator itRHS;
      itLHS.p = v.data + 1;
      itRHS.p = v.data + 3;
      bool b = false;
      Spy::reset();
      // exercise
      b = (itLHS != itRHS);
      // verify
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(b == true);
      //      0    1    2    3
      //    +----+----+----+----+
      //    | 26 | 49 | 67 | 89 |
      //    +----+----+----+----+
      //         itLHS       itRHS
      assertUnit(itLHS.p == v.data + 1);
      assertUnit(itRHS.p == v.data + 3);
      assertStandardFixture(v);
      // teardown
      teardownStandardFixture(v);
   }

   /*************************************************************
    * SETUP STANDARD FIXTURE
    *      0    1    2    3
    *    +----+----+----+----+
    *    | 26 | 49 | 67 | 89 |
    *    +----+----+----+----+
    *************************************************************/
   void setupStandardFixture(custom::vector<Spy>& v)
   {
      try
      {
         v.data = v.alloc.allocate(4);
         v.alloc.construct(&v.data[0], Spy(26));
         v.alloc.construct(&v.data[1], Spy(49));
         v.alloc.construct(&v.data[2], Spy(67));
         v.alloc.construct(&v.data[3], Spy(89));
         v.numElements = 4;
         v.numCapacity = 4;
      }
      catch (...)
      {
         assert(false);
      }
   }

   /*************************************************************
    * VERIFY STANDARD FIXTURE PARAMETERS
    *      0    1    2    3
    *    +----+----+----+----+
    *    | 26 | 49 | 67 | 89 |
    *    +----+----+----+----+
    *************************************************************/
   void assertStandardFixtureParameters(const custom::vector<Spy>& v, int line, const char* function)
   {
      assertIndirect(v.data != nullptr);
      assertIndirect(v.numCapacity == 4);
      assertIndirect(v.numElements == 4);

      if (v.data != nullptr && v.numElements >= 4)
      {
         assertIndirect(v.data[0] == Spy(26));
         assertIndirect(v.data[1] == Spy(49));
         assertIndirect(v.data[2] == Spy(67));
         assertIndirect(v.data[3] == Spy(89));
      }
   }
   
   /*************************************************************
    * VERIFY EMPTY FIXTURE PARAMETERS
    *************************************************************/
   void assertEmptyFixtureParameters(const custom::vector<Spy>& v, int line, const char* function)
   {
      assertIndirect(v.data == nullptr);
      assertIndirect(v.numCapacity == 0);
      assertIndirect(v.numElements == 0);
   }
   
   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *************************************************************/
   void teardownStandardFixture(custom::vector<Spy>&v)
   {
      if (v.data != nullptr)
      {
         for (size_t i = 0; i < v.numElements; i++)
            v.alloc.destroy(&v.data[i]);
         v.alloc.deallocate(v.data, v.numCapacity);

      }
      v.data = nullptr;
      v.numElements = v.numCapacity = 0;
   }



};

#endif // DEBUG
