#include <gtest/gtest.h>
#include "AVLSet.hpp"
#include <iostream>
#include <string>


void visitI(const int& e)
{
    std::cout << e << std::endl;
}


void visitS(const std::string& e)
{
    std::cout << e << std::endl;
}


// unbalance!!
TEST(AVLSetTests, constructEmptyBSTSizeIsZero)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));
}


TEST(AVLSetTests, addSomeNodesToBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    EXPECT_EQ(6, a.size());
    EXPECT_TRUE(a.contains(6));
    EXPECT_FALSE(a.contains(20));
}


TEST(AVLSetTests, addSixNodesToBST_heightIsThree)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    EXPECT_EQ(0, a.height());
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    EXPECT_EQ(6, a.size());
    EXPECT_EQ(3, a.height());
}


TEST(AVLSetTests, heightOfEmptyBSTIsNegativeOne)
{
    AVLSet<std::string> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_EQ(-1, a.height());
}


TEST(AVLSetTests, preorderBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    a.preorder(visitI);
}


TEST(AVLSetTests, postorderBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    a.postorder(visitI);
}


TEST(AVLSetTests, inorderBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    a.inorder(visitI);
}


TEST(AVLSetTests, copyExistingBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    AVLSet<int> b = a;
    
    EXPECT_EQ(a.size(), b.size());
    EXPECT_EQ(a.height(), b.height());
    a.preorder(visitI);
    b.preorder(visitI);
}


TEST(AVLSetTests, copyExistingEmptyBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());

    AVLSet<int> b = a;
    
    EXPECT_EQ(a.size(), b.size());
    EXPECT_EQ(a.height(), b.height());
    a.preorder(visitI);
    b.preorder(visitI);
}


TEST(AVLSetTests, copyExpiringBST)
{
    AVLSet<int> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());
    EXPECT_FALSE(a.contains(10));

    a.add(10);
    a.add(5);
    a.add(13);
    a.add(14);
    a.add(7);
    a.add(6);

    AVLSet<int> b = std::move(a);
    
    EXPECT_EQ(6, b.size());
    EXPECT_EQ(0, a.size());
    EXPECT_EQ(3, b.height());
    EXPECT_EQ(-1, a.height());
}


TEST(AVLSetTests, assignExistingBST)
{
    AVLSet<std::string> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());

    a.add("g");
    a.add("a");
    a.add("r");
    a.add("w");

    AVLSet<std::string> b{false};
    b.add("r");
    EXPECT_EQ(1, b.size());

    b = a;
    
    EXPECT_EQ(4, b.size());
    EXPECT_EQ(4, a.size());
    EXPECT_EQ(a.height(), b.height());
}


TEST(AVLSetTests, assignExpiringBST)
{
    AVLSet<std::string> a{false};
    EXPECT_TRUE(a.isImplemented());
    EXPECT_EQ(0, a.size());

    a.add("g");
    a.add("a");
    a.add("r");
    a.add("w");

    AVLSet<std::string> b{false};
    b.add("r");
    EXPECT_EQ(1, b.size());

    b = std::move(a);
    
    EXPECT_EQ(4, b.size());
    EXPECT_EQ(1, a.size());
}


// balance!!
TEST(AVLSetTests, simpleLLRotationBalance)
{
    AVLSet<int> a{true};
    a.add(30);
    a.add(20);
    a.add(10);

    a.inorder(visitI);    
    a.add(5);
    a.add(1);
    EXPECT_EQ(2, a.height());
}


TEST(AVLSetTests, simpleRRRotationBalance)
{
    AVLSet<int> a;
    a.add(10);
    a.add(20);
    a.add(30);
    
    std::vector<int> inElements;
    a.inorder([&](const int& element) { inElements.push_back(element); });
    std::vector<int> expectedInElements{10, 20, 30};

    ASSERT_EQ(3, inElements.size());

    for (unsigned int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(inElements[i], expectedInElements[i]);
    }
}


TEST(AVLSetTests, simpleLRRotationBalance)
{
    AVLSet<int> a;
    a.add(40);
    a.add(20);
    a.add(60);
    a.add(10);
    a.add(30);
    a.add(50);
    a.add(5);
    a.add(25);
    a.add(35);

    EXPECT_EQ(3, a.height());

    a.add(23);
    
    std::vector<int> inElements;
    a.inorder([&](const int& element) { inElements.push_back(element); });
    std::vector<int> expectedInElements{5, 10, 20, 23, 25, 30, 35, 40, 50, 60};

    ASSERT_EQ(10, inElements.size());

    for (unsigned int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(inElements[i], expectedInElements[i]);
    }

    EXPECT_EQ(3, a.height());
}


TEST(AVLSetTests, simpleRLRotationBalance)
{
    AVLSet<int> a;
    a.add(10);
    a.add(30);
    a.add(20);
    
    std::vector<int> inElements;
    a.inorder([&](const int& element) { inElements.push_back(element); });
    std::vector<int> expectedInElements{10, 20, 30};

    ASSERT_EQ(3, inElements.size());

    for (unsigned int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(inElements[i], expectedInElements[i]);
    }
    EXPECT_EQ(1, a.height());
}




