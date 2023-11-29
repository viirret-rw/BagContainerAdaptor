#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>
#include <BagContainerAdaptor/linked_list.hpp>

#include <list>

// Testing various functions of the BagContainerAdaptor class.
template <typename Container>
class BagContainerAdaptorTest : public ::testing::Test
{
protected:
    void insertTest()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.size(), 3);
    }

    void eraseTest1()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.size(), 3);

        adapter.erase(adapter.begin());

        EXPECT_EQ(adapter.size(), 2);
    }

    void eraseTest2()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.size(), 3);

        adapter.erase(2);

        EXPECT_EQ(adapter.size(), 2);
    }

    void eraseTestMultiple()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(2);
        adapter.insert(2);
        adapter.insert(2);
        adapter.insert(5);
        adapter.insert(6);

        EXPECT_EQ(adapter.size(), 5);

        adapter.erase(2);

        EXPECT_EQ(adapter.size(), 2);
    }

    void findTest()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        auto it = adapter.find(2);

        EXPECT_TRUE(it != adapter.end());
    }

    void sizeTest()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        EXPECT_EQ(adapter.size(), 1);
        adapter.insert(2);
        EXPECT_EQ(adapter.size(), 2);
        adapter.insert(3);
        EXPECT_EQ(adapter.size(), 3);
    }

    void emptyTest()
    {
        BagContainerAdaptor<int, Container> adapter;
        EXPECT_TRUE(adapter.empty());
        adapter.insert(1);
        EXPECT_FALSE(adapter.empty());
    }

    void swapTest()
    {
        BagContainerAdaptor<int, Container> adapter1;
        adapter1.insert(1);
        adapter1.insert(2);
        adapter1.insert(3);
        adapter1.insert(4);

        BagContainerAdaptor<int, Container> adapter2;
        adapter2.insert(1);

        EXPECT_EQ(adapter1.size(), 4);
        EXPECT_EQ(adapter2.size(), 1);

        adapter1.swap(adapter2);

        EXPECT_EQ(adapter1.size(), 1);
        EXPECT_EQ(adapter2.size(), 4);
    }

    void moveConstructorTest()
    {
        Container container{1, 2, 3};

        BagContainerAdaptor<int, Container> adapter(std::move(container));

        EXPECT_EQ(adapter.size(), 3);
    }

    void moveAssingmentTest()
    {
        {
            BagContainerAdaptor<int, Container> adapter;

            EXPECT_EQ(adapter.size(), 0);

            adapter = Container{1, 2, 3};

            EXPECT_EQ(adapter.size(), 3);
        }

        {
            BagContainerAdaptor<int, Container> adapter = Container{1, 2, 3};
            EXPECT_EQ(adapter.size(), 3);
        }
    }

    void nonConstIterationTest()
    {
        BagContainerAdaptor<int, Container> adapter = Container{5, 10, 15};

        int counter = 0;
        for (auto i = adapter.begin(); i != adapter.end(); i++)
        {
            counter++;
        }

        EXPECT_EQ(counter, adapter.size());
    }

    void constIterationTest()
    {
        BagContainerAdaptor<int, Container> adapter = Container{1, 2, 3, 4, 5};

        int counter = 0;
        for (auto i = adapter.cbegin(); i != adapter.cend(); i++)
        {
            counter++;
        }
        EXPECT_EQ(counter, adapter.size());
    }
};

using MainContainerTypes = ::testing::Types<
    std::list<int>,
    std::vector<int>,
    std::deque<int>,
    std::forward_list<int>,
    std::multiset<int>,
    std::unordered_multiset<int>>;

TYPED_TEST_SUITE(BagContainerAdaptorTest, MainContainerTypes);

TYPED_TEST(BagContainerAdaptorTest, insertTest)
{
    this->insertTest();
}

TYPED_TEST(BagContainerAdaptorTest, eraseTest1)
{
    this->eraseTest1();
}

TYPED_TEST(BagContainerAdaptorTest, eraseTest2)
{
    this->eraseTest2();
}

TYPED_TEST(BagContainerAdaptorTest, eraseTestMultiple)
{
    this->eraseTestMultiple();
}

TYPED_TEST(BagContainerAdaptorTest, findTest)
{
    this->findTest();
}

TYPED_TEST(BagContainerAdaptorTest, sizeTest)
{
    this->sizeTest();
}

TYPED_TEST(BagContainerAdaptorTest, emptyTest)
{
    this->emptyTest();
}

TYPED_TEST(BagContainerAdaptorTest, swapTest)
{
    this->swapTest();
}

TYPED_TEST(BagContainerAdaptorTest, moveConstructorTest)
{
    this->moveConstructorTest();
}

TYPED_TEST(BagContainerAdaptorTest, moveAssingmentTest)
{
    this->moveAssingmentTest();
}

TYPED_TEST(BagContainerAdaptorTest, nonConstIterationTest)
{
    this->nonConstIterationTest();
}

TYPED_TEST(BagContainerAdaptorTest, constIterationTest)
{
    this->constIterationTest();
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
