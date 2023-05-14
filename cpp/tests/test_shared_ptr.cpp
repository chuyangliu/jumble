#include "catch2/catch_amalgamated.hpp"
#include "jumble/shared_ptr.hpp"
#include <string>

class DeleterWrapper {
public:
    std::size_t delCnt;
    std::function<void(std::string*)> deleter;

    DeleterWrapper() : delCnt(0) {
        deleter = [&](std::string* p) {
            ++delCnt;
            delete p;
        };
    }
};

TEST_CASE("Basic") {
    jumble::SharedPtr<std::string> p1;
    REQUIRE(!static_cast<bool>(p1));
    REQUIRE(!p1.unique());
    REQUIRE(p1.useCount() == 0);

    auto p2 = jumble::makeShared<std::string>("hello world");
    REQUIRE(static_cast<bool>(p2));
    REQUIRE(p2.unique());
    REQUIRE(p2.useCount() == 1);
    REQUIRE(*p2 == *p2.get());
    REQUIRE(p2->substr(6) == "world");

    jumble::swap<std::string>(p1, p2);
    REQUIRE(!static_cast<bool>(p2));
    REQUIRE(!p2.unique());
    REQUIRE(p2.useCount() == 0);
    REQUIRE(static_cast<bool>(p1));
    REQUIRE(p1.unique());
    REQUIRE(p1.useCount() == 1);
    REQUIRE(*p1 == *p1.get());
    REQUIRE(p1->substr(6) == "world");
}

TEST_CASE("Reset") {
    DeleterWrapper deleterWrapper;

    auto p1 = jumble::makeShared<std::string>("hello world");
    REQUIRE(p1.useCount() == 1);
    REQUIRE(*p1.get() == "hello world");

    p1.reset();
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p1.get() == nullptr);
    REQUIRE(deleterWrapper.delCnt == 0);

    p1.reset(new std::string("quick fox"), deleterWrapper.deleter);
    REQUIRE(p1.useCount() == 1);
    REQUIRE(*p1.get() == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 0);

    p1.reset();
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p1.get() == nullptr);
    REQUIRE(deleterWrapper.delCnt == 1);
}

TEST_CASE("CopyControl1") {
    jumble::SharedPtr<std::string> p1;
    REQUIRE(p1.useCount() == 0);

    p1 = jumble::makeShared<std::string>("hello world");
    REQUIRE(p1.useCount() == 1);

    jumble::SharedPtr<std::string> p2 = p1;
    REQUIRE(p1.useCount() == 2);
    REQUIRE(p2.useCount() == 2);

    p1.reset();
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p2.useCount() == 1);

    jumble::SharedPtr<std::string> p3 = std::move(p2);
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p2.useCount() == 0);
    REQUIRE(p3.useCount() == 1);

    p2 = p3;
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p2.useCount() == 2);
    REQUIRE(p3.useCount() == 2);

    p1 = std::move(p2);
    REQUIRE(p1.useCount() == 2);
    REQUIRE(p2.useCount() == 0);
    REQUIRE(p3.useCount() == 2);

    p1.reset();
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p2.useCount() == 0);
    REQUIRE(p3.useCount() == 1);

    p3.reset();
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p2.useCount() == 0);
    REQUIRE(p3.useCount() == 0);
}

TEST_CASE("CopyControl2") {
    DeleterWrapper deleterWrapper;

    jumble::SharedPtr<std::string> p1(new std::string("hello world"), deleterWrapper.deleter);
    // p1 = p1;  // disable for clang warns
    REQUIRE(p1.unique());
    REQUIRE(deleterWrapper.delCnt == 0);

    jumble::SharedPtr<std::string> p2 = p1;
    REQUIRE(!p1.unique());
    REQUIRE(!p2.unique());
    REQUIRE(p1.useCount() == 2);
    REQUIRE(p2.useCount() == 2);

    p1 = jumble::SharedPtr<std::string>(new std::string("quick fox"), deleterWrapper.deleter);
    REQUIRE(p1.unique());
    REQUIRE(p2.unique());
    REQUIRE(*p1 == "quick fox");
    REQUIRE(*p2 == "hello world");
    REQUIRE(deleterWrapper.delCnt == 0);

    jumble::SharedPtr<std::string> p3(std::move(p2));
    REQUIRE(p1.unique());
    REQUIRE(p2.useCount() == 0);
    REQUIRE(p3.unique());
    REQUIRE(deleterWrapper.delCnt == 0);

    p2 = std::move(p1);
    REQUIRE(p1.useCount() == 0);
    REQUIRE(p2.unique());
    REQUIRE(p3.unique());
    REQUIRE(*p2 == "quick fox");
    REQUIRE(*p3 == "hello world");
    REQUIRE(deleterWrapper.delCnt == 0);

    p3 = p2;
    REQUIRE(p3.useCount() == 2);
    REQUIRE(*p3 == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 1);

    p2.reset();
    REQUIRE(p3.unique());
    REQUIRE(*p3 == "quick fox");
    REQUIRE(deleterWrapper.delCnt == 1);

    p3.reset();
    REQUIRE(p3.useCount() == 0);
    REQUIRE(deleterWrapper.delCnt == 2);
}
