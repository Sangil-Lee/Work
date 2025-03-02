#include "Optional.h"

#include <string>

#include <gtest/gtest.h>

namespace s7nodave {
namespace {

TEST(OptionalTest, testAssignmentOperator) {
    Optional<int> o1;
    EXPECT_FALSE(o1);
    o1 = 3;
    EXPECT_TRUE(o1);
    EXPECT_EQ(*o1, 3);
    o1 = 5;
    EXPECT_EQ(*o1, 5);
    int i = 7;
    o1 = i;
    EXPECT_EQ(*o1, 7);
    i = 9;
    o1 = std::move(i);
    EXPECT_EQ(*o1, 9);
    Optional<int> o2;
    EXPECT_FALSE(o2);
    o2 = o1;
    EXPECT_TRUE(o2);
    EXPECT_EQ(*o2, 9);
    o1 = 11;
    o2 = std::move(o1);
    EXPECT_EQ(*o2, 11);
}

TEST(OptionalTest, testCopyAndMoveConstructor) {
    Optional<int> o1;
    // Copy constructor.
    Optional<int> o2{o1};
    EXPECT_FALSE(o2);
    o1 = 1;
    Optional<int> o3{o1};
    ASSERT_TRUE(o3);
    EXPECT_EQ(*o3, 1);
    // Move constructor.
    o1 = Optional<int>();
    Optional<int> o4{std::move(o1)};
    EXPECT_FALSE(o4);
    o1 = 2;
    Optional<int> o5{std::move(o1)};
    ASSERT_TRUE(o5);
    EXPECT_EQ(*o5, 2);
}

TEST(OptionalTest, testDefaultConstructor) {
    Optional<int> o;
    EXPECT_FALSE(o);
}

// This mock class for counting the number of instances is not thread-safe, but
// this should be okay for our test.
class DestructorMock {
public:
    DestructorMock() {
        ++numberOfInstances;
    }

    DestructorMock(const DestructorMock&) : DestructorMock() {
    }

    DestructorMock(DestructorMock&&) : DestructorMock() {
    }

    ~DestructorMock() {
        --numberOfInstances;
    }

    DestructorMock& operator=(const DestructorMock&) {
        return *this;
    }

    static int numberOfInstances;
};

int DestructorMock::numberOfInstances = 0;

TEST(OptionalTest, testDestructorCalls) {
    // If there are any instances initially, something is really wrong and there
    // is no sense in continuing with this test.
    ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    // When we create an empty optional, now instance of the actual object class
    // should be created.
    {
        Optional<DestructorMock> o;
        ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    }
    // As no instance has been created, the destructor should not be called.
    ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    // When we create an engaged optional, there should be one instance of the
    // mock class.
    {
        DestructorMock mock;
        ASSERT_EQ(DestructorMock::numberOfInstances, 1);
        Optional<DestructorMock> o1{mock};
        ASSERT_EQ(DestructorMock::numberOfInstances, 2);
        // The same should be true when using move semantics.
        Optional<DestructorMock> o2{std::move(mock)};
        ASSERT_EQ(DestructorMock::numberOfInstances, 3);
    }
    // When destroying the optional, the wrapped object should be destroyed as
    // well.
    ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    // When assigning an object to an empty optional, the count should increase.
    // When assigning to an already engaged optional, it should not increase.
    {
        Optional<DestructorMock> o;
        ASSERT_EQ(DestructorMock::numberOfInstances, 0);
        DestructorMock mock;
        ASSERT_EQ(DestructorMock::numberOfInstances, 1);
        o = mock;
        ASSERT_EQ(DestructorMock::numberOfInstances, 2);
        o = mock;
        ASSERT_EQ(DestructorMock::numberOfInstances, 2);
    }
    ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    // The same should apply when using move semantics.
    {
        Optional<DestructorMock> o{DestructorMock()};
        ASSERT_EQ(DestructorMock::numberOfInstances, 1);
        DestructorMock mock;
        ASSERT_EQ(DestructorMock::numberOfInstances, 2);
        o = std::move(mock);
        ASSERT_EQ(DestructorMock::numberOfInstances, 2);
        o = std::move(mock);
        ASSERT_EQ(DestructorMock::numberOfInstances, 2);
    }
    ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    // When assigning an empty optional to a previously engaged optional, the
    // number of mock instances should decrease.
    {
        Optional<DestructorMock> o{DestructorMock()};
        ASSERT_EQ(DestructorMock::numberOfInstances, 1);
        o = Optional<DestructorMock>();
        ASSERT_EQ(DestructorMock::numberOfInstances, 0);
    }
}

TEST(OptionalTest, testEqualityOperator) {
    EXPECT_TRUE(Optional<int>(5) == Optional<int>(5));
    EXPECT_TRUE(Optional<int>() == Optional<int>());
    EXPECT_FALSE(Optional<int>(5) == Optional<int>(3));
    EXPECT_FALSE(Optional<int>(3) == Optional<int>(5));
    EXPECT_FALSE(Optional<int>(5) == Optional<int>());
    EXPECT_FALSE(Optional<int>() == Optional<int>(5));
}

TEST(OptionalTest, testInitializingConstructor) {
    std::string s = "abc";
    // Copying the parameter.
    Optional<std::string> o1{s};
    ASSERT_TRUE(o1);
    EXPECT_EQ(*o1, s);
    Optional<std::string> o2{std::move(s)};
    ASSERT_TRUE(o2);
    // We cannot compare with the original string here because it might not be
    // valid any longer due to moving it.
    EXPECT_EQ(*o2, "abc");
}

TEST(OptionalTest, testPointerToMemberOperator) {
    std::string s1 = "abc";
    Optional<std::string> o1 = s1;
    EXPECT_EQ(o1->length(), s1.length());
    std::string s2 = "abcde";
    Optional<std::string> o2 = s2;
    EXPECT_EQ(o2->length(), s2.length());
}

} // anonymous namespace
} // namespace s7nodave
