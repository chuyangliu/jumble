#include "catch2/catch_amalgamated.hpp"
#include "jumble/unique_ptr.hpp"
#include <string>
#include <functional>

class DeleterWrapper {
public:
    using DeleterType = std::function<void(std::string*)>;

    std::size_t delCnt;
    DeleterType deleter;

    DeleterWrapper() : delCnt(0) {
        deleter = [&](std::string* p) {
            ++delCnt;
            delete p;
        };
    }
};

TEST_CASE("Basic") {
    jumble::UniquePtr<std::string> p1;
    REQUIRE(!static_cast<bool>(p1));
    REQUIRE(p1.get() == nullptr);

    jumble::UniquePtr<std::string> p2(new std::string("hello world"));
    REQUIRE(static_cast<bool>(p2));
    REQUIRE(*p2 == *p2.get());
    REQUIRE(p2->substr(6) == "world");

    jumble::swap<std::string>(p1, p2);
    REQUIRE(!static_cast<bool>(p2));
    REQUIRE(p2.get() == nullptr);
    REQUIRE(static_cast<bool>(p1));
    REQUIRE(*p1 == *p1.get());
    REQUIRE(p1->substr(6) == "world");
}

TEST_CASE("Reset") {
    DeleterWrapper deleterWrapper;

    auto s = new std::string("hello world");
    jumble::UniquePtr<std::string, DeleterWrapper::DeleterType> p1(s, deleterWrapper.deleter);
    REQUIRE(*p1.get() == "hello world");

    p1.reset();
    REQUIRE(p1.get() == nullptr);
    REQUIRE(deleterWrapper.delCnt == 1);

    p1.reset(new std::string("quick fox"));
    REQUIRE(*p1.get() == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 1);

    std::string *p2 = p1.release();
    REQUIRE(p1.get() == nullptr);
    REQUIRE(*p2 == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 1);

    delete p2;
}

TEST_CASE("CopyControl1") {
    jumble::UniquePtr<std::string> p1(new std::string("hello world"));
    REQUIRE(*p1.get() == "hello world");

    jumble::UniquePtr<std::string> p2(std::move(p1));
    REQUIRE(p1.get() == nullptr);
    REQUIRE(*p2.get() == "hello world");

    jumble::UniquePtr<std::string> p3;
    p3 = std::move(p2);
    REQUIRE(p1.get() == nullptr);
    REQUIRE(p2.get() == nullptr);
    REQUIRE(*p3.get() == "hello world");

    p3 = nullptr;
    REQUIRE(p1.get() == nullptr);
    REQUIRE(p2.get() == nullptr);
    REQUIRE(p3.get() == nullptr);
}

TEST_CASE("CopyControl2") {
    DeleterWrapper deleterWrapper;

    jumble::UniquePtr<std::string, DeleterWrapper::DeleterType> p1(deleterWrapper.deleter);
    REQUIRE(p1.get() == nullptr);
    REQUIRE(deleterWrapper.delCnt == 0);

    auto s = new std::string("quick fox");
    jumble::UniquePtr<std::string, DeleterWrapper::DeleterType> p2(s, deleterWrapper.deleter);
    REQUIRE(*p2.get() == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 0);

    p1 = std::move(p2);
    REQUIRE(*p1.get() == "quick fox");
    REQUIRE(p2.get() == nullptr);
    REQUIRE(deleterWrapper.delCnt == 0);

    jumble::UniquePtr<std::string, DeleterWrapper::DeleterType> p3(std::move(p1));
    REQUIRE(p1.get() == nullptr);
    REQUIRE(p2.get() == nullptr);
    REQUIRE(*p3.get() == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 0);

    p3.reset();
    REQUIRE(p1.get() == nullptr);
    REQUIRE(p2.get() == nullptr);
    REQUIRE(p3.get() == nullptr);
    REQUIRE(deleterWrapper.delCnt == 1);
}
