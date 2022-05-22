#include <gtest/gtest.h>
#include "HashSet.hpp"
#include "StringHashing.hpp"


unsigned int hashZero(const int& a) {return 0;}

TEST(HashSetTests, ConstructEmptyHashSet_SizeIsZero)
{
    std::function<unsigned int(const int&)> f = hashZero;
    HashSet<int> h{f};
    ASSERT_TRUE(h.isImplemented());
    ASSERT_EQ(0, h.size());
}


TEST(HashSetTests, HashSetContainsTheGivenElements)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    EXPECT_EQ(9, h.size());
    EXPECT_TRUE(h.contains("g"));
}


TEST(HashSetTests, emptyHashSetContainsNothing)
{
    HashSet<std::string> h{hashStringAsProduct};
    ASSERT_FALSE(h.contains("HELLO"));
}


TEST(HashSetTests, HashSetDoesNotContainTheGivenElements)
{
    HashSet<std::string> h{hashStringAsZero};
    h.add("hello");
    h.add("kaylee");
    EXPECT_FALSE(h.contains("stan"));
}


TEST(HashSetTests, addExistElement_NoEffect)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("g");
    EXPECT_EQ(9, h.size());
    EXPECT_TRUE(h.contains("g"));
}


TEST(HashSetTests, addElementsThatTriggerRehashing)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("stan");
    h.add("nihap");
    EXPECT_EQ(11, h.size());
    EXPECT_TRUE(h.contains("stan"));
}


TEST(HashSetTests, indexOutOfBoundariesReturnZero)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_EQ(0, h.elementsAtIndex(2));
}


TEST(HashSetTests, emptyHashSetReturnZeroAtAnyIndex)
{
    HashSet<std::string> h{hashStringAsProduct};
    for (unsigned int i = 0; i < 10; i++)
    {
        EXPECT_EQ(0, h.elementsAtIndex(i));
    }
}


TEST(HashSetTests, elementsAtIndexReturnCorrectNumber)
{
    HashSet<std::string> h{hashStringAsZero};
    h.add("hello");
    h.add("kaylee");
    EXPECT_EQ(2, h.elementsAtIndex(0));
    for (unsigned int i = 1; i < 10; i++)
    {
        EXPECT_EQ(0, h.elementsAtIndex(i));
    }
}


TEST(HashSetTests, elementsAtIndexReturnTrue)
{
    HashSet<std::string> h{hashStringAsZero};
    EXPECT_FALSE(h.isElementAtIndex("a", 11));
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.isElementAtIndex("hello", 0));
    EXPECT_FALSE(h.isElementAtIndex("kaylee", 3));
}


TEST(HashSetTests, afterRehashing_ElementAtIndex)
{
    HashSet<std::string> h{hashStringAsSum};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("qwert");
    unsigned int index = hashStringAsSum("qwert") % 10;
    EXPECT_TRUE(h.isElementAtIndex("qwert", index));
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("stan");
    h.add("nihap");
    EXPECT_EQ(11, h.size());
    EXPECT_TRUE(h.contains("stan"));
    unsigned int index2 = hashStringAsSum("qwert") % 21;
    EXPECT_TRUE(h.isElementAtIndex("qwert", index2));
    EXPECT_FALSE(h.isElementAtIndex("qwert", index));
}


TEST(HashSetTests, copyExistingHashSet)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("stan");
    h.add("nihap");
    EXPECT_EQ(11, h.size());
    EXPECT_TRUE(h.contains("stan"));

    HashSet<std::string> h2 = h;
    EXPECT_EQ(h.size(), h2.size());
    for (unsigned int i = 0; i < 21; i++)
    {
        EXPECT_EQ(h.elementsAtIndex(i), h2.elementsAtIndex(i));
    }
}


TEST(HashSetTests, copyExpiringHashSet)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("stan");
    h.add("nihap");
    EXPECT_EQ(11, h.size());
    EXPECT_TRUE(h.contains("stan"));
    
    HashSet<std::string> h2 = std::move(h);
    EXPECT_EQ(11, h2.size());
    EXPECT_EQ(0, h.size());
    for (unsigned int i = 0; i < 10; i++)
    {
        EXPECT_EQ(0, h.elementsAtIndex(i));
    }
}


TEST(HashSetTests, assignExistingHashSet)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("stan");
    h.add("nihap");
    EXPECT_EQ(11, h.size());
    EXPECT_TRUE(h.contains("stan"));

    HashSet<std::string> h2{hashStringAsZero};
    h2.add("boo");
    h2.add("is");
    h2.add("happy");
    EXPECT_EQ(3, h2.size());

    h2 = h;
    EXPECT_EQ(h.size(), h2.size());
    for (unsigned int i = 0; i < 21; i++)
    {
        EXPECT_EQ(h.elementsAtIndex(i), h2.elementsAtIndex(i));
    }
}


TEST(HashSetTests, assignExpiringHashSet)
{
    HashSet<std::string> h{hashStringAsProduct};
    h.add("hello");
    h.add("kaylee");
    EXPECT_TRUE(h.contains("kaylee"));

    h.add("a");
    h.add("b");
    h.add("c");
    h.add("d");
    h.add("e");
    h.add("f");
    h.add("g");
    h.add("stan");
    h.add("nihap");
    EXPECT_EQ(11, h.size());
    EXPECT_TRUE(h.contains("stan"));

    HashSet<std::string> h2{hashStringAsZero};
    h2.add("boo");
    h2.add("is");
    h2.add("happy");
    EXPECT_EQ(3, h2.size());

    h2 = std::move(h);
    EXPECT_EQ(11, h2.size());
    EXPECT_EQ(3, h.size());
}

