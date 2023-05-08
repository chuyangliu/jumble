#include "gtest/gtest.h"
#include "tastylib/SharedPtr.h"
#include <string>

using tastylib::SharedPtr;
using tastylib::makeShared;


class SharedPtrTest: public ::testing::Test {
protected:

    static void SetUpTestCase() {
        deleteCnt = new std::size_t(0);
        deleter = new std::function<void(std::string*)>(
            [](std::string* p) {
                ++*deleteCnt;
                delete p;
            }
        );
    }

    static void TearDownTestCase() {
        delete deleteCnt; deleteCnt = nullptr;
        delete deleter; deleter = nullptr;
    }

protected:
    static std::size_t *deleteCnt;
    static std::function<void(std::string*)> *deleter;
};

std::size_t *SharedPtrTest::deleteCnt = nullptr;
std::function<void(std::string*)> *SharedPtrTest::deleter = nullptr;


TEST_F(SharedPtrTest, Basic) {
    SharedPtr<std::string> p1;
    EXPECT_EQ(static_cast<bool>(p1), false);
    EXPECT_EQ(p1.unique(), false);
    EXPECT_EQ(p1.useCount(), 0);

    auto p2 = makeShared<std::string>("hello world");
    EXPECT_EQ(static_cast<bool>(p2), true);
    EXPECT_EQ(p2.unique(), true);
    EXPECT_EQ(p2.useCount(), 1);
    EXPECT_EQ(*p2, *p2.get());
    EXPECT_EQ(p2->substr(6), "world");

    tastylib::swap<std::string>(p1, p2);
    EXPECT_EQ(static_cast<bool>(p2), false);
    EXPECT_EQ(p2.unique(), false);
    EXPECT_EQ(p2.useCount(), 0);
    EXPECT_EQ(static_cast<bool>(p1), true);
    EXPECT_EQ(p1.unique(), true);
    EXPECT_EQ(p1.useCount(), 1);
    EXPECT_EQ(*p1, *p1.get());
    EXPECT_EQ(p1->substr(6), "world");
}

TEST_F(SharedPtrTest, Reset) {
    *deleteCnt = 0;

    auto p1 = makeShared<std::string>("hello world");
    EXPECT_EQ(p1.useCount(), 1);
    EXPECT_EQ(*p1.get(), "hello world");

    p1.reset();
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*deleteCnt, 0);

    p1.reset(new std::string("quick fox"), *deleter);
    EXPECT_EQ(p1.useCount(), 1);
    EXPECT_EQ(*p1.get(), "quick fox");
    EXPECT_EQ(*deleteCnt, 0);

    p1.reset();
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*deleteCnt, 1);
}

TEST_F(SharedPtrTest, CopyControl1) {
    SharedPtr<std::string> p1;
    EXPECT_EQ(p1.useCount(), 0);

    p1 = makeShared<std::string>("hello world");
    EXPECT_EQ(p1.useCount(), 1);

    SharedPtr<std::string> p2 = p1;
    EXPECT_EQ(p1.useCount(), 2);
    EXPECT_EQ(p2.useCount(), 2);

    p1.reset();
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p2.useCount(), 1);

    SharedPtr<std::string> p3 = std::move(p2);
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p2.useCount(), 0);
    EXPECT_EQ(p3.useCount(), 1);

    p2 = p3;
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p2.useCount(), 2);
    EXPECT_EQ(p3.useCount(), 2);

    p1 = std::move(p2);
    EXPECT_EQ(p1.useCount(), 2);
    EXPECT_EQ(p2.useCount(), 0);
    EXPECT_EQ(p3.useCount(), 2);

    p1.reset();
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p2.useCount(), 0);
    EXPECT_EQ(p3.useCount(), 1);

    p3.reset();
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_EQ(p2.useCount(), 0);
    EXPECT_EQ(p3.useCount(), 0);
}

TEST_F(SharedPtrTest, CopyControl2) {
    *deleteCnt = 0;

    SharedPtr<std::string> p1(new std::string("hello world"), *deleter);
    // p1 = p1;  // disable for clang warns
    EXPECT_TRUE(p1.unique());
    EXPECT_EQ(*deleteCnt, 0);

    SharedPtr<std::string> p2 = p1;
    EXPECT_FALSE(p1.unique());
    EXPECT_FALSE(p2.unique());
    EXPECT_EQ(p1.useCount(), 2);
    EXPECT_EQ(p2.useCount(), 2);

    p1 = SharedPtr<std::string>(new std::string("quick fox"), *deleter);
    EXPECT_TRUE(p1.unique());
    EXPECT_TRUE(p2.unique());
    EXPECT_EQ(*p1, "quick fox");
    EXPECT_EQ(*p2, "hello world");
    EXPECT_EQ(*deleteCnt, 0);

    SharedPtr<std::string> p3(std::move(p2));
    EXPECT_TRUE(p1.unique());
    EXPECT_EQ(p2.useCount(), 0);
    EXPECT_TRUE(p3.unique());
    EXPECT_EQ(*deleteCnt, 0);

    p2 = std::move(p1);
    EXPECT_EQ(p1.useCount(), 0);
    EXPECT_TRUE(p2.unique());
    EXPECT_TRUE(p3.unique());
    EXPECT_EQ(*p2, "quick fox");
    EXPECT_EQ(*p3, "hello world");
    EXPECT_EQ(*deleteCnt, 0);

    p3 = p2;
    EXPECT_EQ(p3.useCount(), 2);
    EXPECT_EQ(*p3, "quick fox");
    EXPECT_EQ(*deleteCnt, 1);

    p2.reset();
    EXPECT_TRUE(p3.unique());
    EXPECT_EQ(*p3, "quick fox");
    EXPECT_EQ(*deleteCnt, 1);

    p3.reset();
    EXPECT_EQ(p3.useCount(), 0);
    EXPECT_EQ(*deleteCnt, 2);
}
