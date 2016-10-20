// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

// --------
// includes
// --------


// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include "gtest/gtest.h"
#include "Allocator.h"
#include <vector> // for tests

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

    /* catch bad_alloc exception */ 

TEST(TestAllocator2, my_allocator_1) {
  
  try {
    my_allocator<double, 500> x;
    ASSERT_EQ(false, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TEST(TestAllocator2, my_allocator_) {

  try {
    my_allocator<int, 4> x;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TEST(TestAllocator2, my_allocator_3) {

  try {
    my_allocator<const int, 4> x;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

    
// --------------
// Test allocate()
// --------------


TEST(TestAllocator2, allocate_1) {

    my_allocator<double, 100> x;
    x.allocate(2);
    x.allocate(2);
    x.allocate(1);
    ASSERT_EQ(x[0 ], -16);
    ASSERT_EQ(x[24], -16);
    ASSERT_EQ(x[48], -8);
    ASSERT_EQ(x[60], -8);
    ASSERT_EQ(x[64], 28);
}

TEST(TestAllocator2, allocate_2) {
    my_allocator<double, 100> x;
    x.allocate(5);
    const my_allocator<double,100> y = x;
    ASSERT_EQ(y[0],-40);}

TEST(TestAllocator2,allocate_3) {
    my_allocator<int, 100> x;
    x.allocate(15);
    x.allocate(1);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(y[0], -60);
    ASSERT_EQ(y[68], -4);}


// --------------
// Test deallocate()
// --------------

/* allocate 2 then deallocate 1 pointer coalesce 3 blocks*/
TEST(TestAllocator2,deallocate_1) {
    my_allocator<int, 100> x;
    x.allocate(15);
    auto ptr = x.allocate(1);
    x.deallocate(ptr,1);
    ASSERT_EQ(x[0], -60);
    ASSERT_EQ(x[68], 24);}

/*allocate then deallocate 2 pointers coalesce 2 blocks*/
TEST(TestAllocator2,deallocate_2) {
    my_allocator<int, 100> x;
    auto ptr = x.allocate(10);
    auto ptr2 = x.allocate(1);
    x.deallocate(ptr,10);
    x.deallocate(ptr2,1);
    ASSERT_EQ(x[0],92);}

/*allocate then deallocate 3 pointers coalesce 3 blocks*/
TEST(TestAllocator2, deallocate_3) {
    my_allocator<int, 100> x;
    auto ptr = x.allocate(1);
    auto ptr2 = x.allocate(5);
    auto ptr3 = x.allocate(10);
    x.deallocate(ptr, 1);
    x.deallocate(ptr3, 5);
    x.deallocate(ptr2, 10);
    ASSERT_EQ(x[0],92);}

// --------------
// Test valid()
// --------------

/* Test valid after construction */
TEST(TestAllocator2, valid_1) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x.valid(), true);
}

/* Test valid after allocation */
TEST(TestAllocator2, valid_2) {
  my_allocator<double, 100> x;
  x.allocate(5);
  ASSERT_EQ(x.valid(), true);
}

/* Test valid after deallocation */
TEST(TestAllocator2, valid_3) {
  my_allocator<int, 100> x;
  int *p = x.allocate(5);
  x.deallocate(p, 5);
  ASSERT_EQ(x.valid(), true);
}




// --------------
// TestAllocator3
// --------------
/* Downing */
template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    const value_type     v = 2;
    const pointer        b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
x.deallocate(b, s);}}


/* my tests again */

/*Coalesc next block */
TYPED_TEST(TestAllocator3, mytest_1) {
  
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p1 = x.allocate(3);
    pointer p2 = x.allocate(3);
    pointer p3 = x.allocate(1);
    x.deallocate(p2, sizeof(double));
    x.deallocate(p1, sizeof(double));

    pointer p4 = x.allocate(7);
    ASSERT_NE(p4, nullptr);
}

  /* Coalescing with previous block */
TYPED_TEST(TestAllocator3, mytest_2) {
  
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p1 = x.allocate(4);
    pointer p2 = x.allocate(4);
    pointer p3 = x.allocate(1);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
    
    pointer p4 = x.allocate(8);
    ASSERT_NE(p4, nullptr);
}

  /* Coalescing with block next and previous block */
TYPED_TEST(TestAllocator3, test_3) {

  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p1 = x.allocate(3);
    pointer p2 = x.allocate(3);
    pointer p3 = x.allocate(1);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
    x.deallocate(p3, sizeof(double));
    
    pointer p4 = x.allocate(9);
    ASSERT_NE(p4, nullptr);
}

  /* Allocate one block */
TYPED_TEST(TestAllocator3, mytest_4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 5;

}





