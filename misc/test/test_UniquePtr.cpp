#include "gtest/gtest.h"
#include "tastylib/UniquePtr.h"
#include <string>
#include <functional>

using tastylib::UniquePtr;


class UniquePtrTest: public ::testing::Test {
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

std::size_t *UniquePtrTest::deleteCnt = nullptr;
std::function<void(std::string*)> *UniquePtrTest::deleter = nullptr;


TEST_F(UniquePtrTest, Basic) {
    UniquePtr<std::string> p1;
    EXPECT_EQ(static_cast<bool>(p1), false);
    EXPECT_EQ(p1.get(), nullptr);

    UniquePtr<std::string> p2(new std::string("hello world"));
    EXPECT_EQ(static_cast<bool>(p2), true);
    EXPECT_EQ(*p2, *p2.get());
    EXPECT_EQ(p2->substr(6), "world");

    tastylib::swap<std::string>(p1, p2);
    EXPECT_EQ(static_cast<bool>(p2), false);
    EXPECT_EQ(p2.get(), nullptr);
    EXPECT_EQ(static_cast<bool>(p1), true);
    EXPECT_EQ(*p1, *p1.get());
    EXPECT_EQ(p1->substr(6), "world");
}

TEST_F(UniquePtrTest, Reset) {
    *deleteCnt = 0;

    UniquePtr<std::string, std::function<void(std::string*)>> p1(new std::string("hello world"), *deleter);
    EXPECT_EQ(*p1.get(), "hello world");

    p1.reset();
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*deleteCnt, 1);

    p1.reset(new std::string("quick fox"));
    EXPECT_EQ(*p1.get(), "quick fox");
    EXPECT_EQ(*deleteCnt, 1);

    std::string *p2 = p1.release();
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*p2, "quick fox");
    EXPECT_EQ(*deleteCnt, 1);
    delete p2;
}

TEST_F(UniquePtrTest, CopyControl1) {
    UniquePtr<std::string> p1(new std::string("hello world"));
    EXPECT_EQ(*p1.get(), "hello world");

    UniquePtr<std::string> p2(std::move(p1));
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*p2.get(), "hello world");

    UniquePtr<std::string> p3;
    p3 = std::move(p2);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(p2.get(), nullptr);
    EXPECT_EQ(*p3.get(), "hello world");

    p3 = nullptr;
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(p2.get(), nullptr);
    EXPECT_EQ(p3.get(), nullptr);
}

TEST_F(UniquePtrTest, CopyControl2) {
    *deleteCnt = 0;

    UniquePtr<std::string, std::function<void(std::string*)>> p1(*deleter);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*deleteCnt, 0);

    UniquePtr<std::string, std::function<void(std::string*)>> p2(new std::string("quick fox"), *deleter);
    EXPECT_EQ(*p2.get(), "quick fox");
    EXPECT_EQ(*deleteCnt, 0);

    p1 = std::move(p2);
    EXPECT_EQ(*p1.get(), "quick fox");
    EXPECT_EQ(p2.get(), nullptr);
    EXPECT_EQ(*deleteCnt, 0);

    UniquePtr<std::string, std::function<void(std::string*)>> p3(std::move(p1));
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(p2.get(), nullptr);
    EXPECT_EQ(*p3.get(), "quick fox");
    EXPECT_EQ(*deleteCnt, 0);

    p3.reset();
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(p2.get(), nullptr);
    EXPECT_EQ(p3.get(), nullptr);
    EXPECT_EQ(*deleteCnt, 1);
}
