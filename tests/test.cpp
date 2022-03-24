#include "../serdes.hpp"
#include "test_class.hpp"

#include <array>
#include <vector>
#include <string>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include <iostream>

#include <gtest/gtest.h>

using namespace serdes;
using namespace serdes::test;
using namespace std;

struct pod_type
{
    int i;
    char f;
};

static int i=10;
static float f=20.0;
static double d=30.0;
static int8_t i8=40;
static int16_t i16=50;
static int32_t i32=60;
static int64_t i64=70;
static uint8_t ui8=80;
static uint16_t ui16=90;
static uint32_t ui32=100;
static uint64_t ui64=110;
string str_gt("this is test message");
vector<int> vec_i{ 1, 2, 3, 4, 5 };
vector<float> vec_f{ 1, 2, 3, 4, 5 };
vector<double> vec_d{ 1, 2, 3, 4, 5 };

vector<uint8_t> raw1{ 1, 2, 3, 4, 5 };
vector<uint8_t> raw2{ 6, 7, 8, 9 };
vector<uint8_t> raw3{ 10, 11 };
vector<vector<uint8_t> > vecvec{ raw1, raw2, raw3 };

using array100int = array<int, 100>;
using map_test = map<std::string, int>;
using unordered_map_test = unordered_map<std::string, int>;
using set_test = set<std::string>;
using unordered_set_test = unordered_set<std::string>;

static auto t_tuple = make_tuple(i, f, d, str_gt);
using tuple_test_t = decltype (t_tuple);
static auto t_tuple1 = make_tuple(i, f, d, i8, i16, i32, i64, ui8, ui16, ui32, ui64);
using tuple_test_t1 = decltype (t_tuple1);
static auto t_tuple2 = make_tuple(ui64, ui32, ui16, ui8, i64, i32, i16, i8, d, f, i);
using tuple_test_t2 = decltype (t_tuple2);

pod_type pt{};

TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(TraitsTests, IsContainer) {
    EXPECT_TRUE(is_std_container<vector<int> >::value);
    EXPECT_TRUE(is_std_container<array100int>::value);
    EXPECT_TRUE(is_std_container<string>::value);
    EXPECT_TRUE(is_std_container<list<int> >::value);
    EXPECT_TRUE(is_std_container<set_test>::value);
    EXPECT_TRUE(is_std_container<map_test>::value);
    EXPECT_TRUE(is_std_container<unordered_set_test>::value);
    EXPECT_TRUE(is_std_container<unordered_map_test>::value);
    EXPECT_FALSE(is_std_container<int>::value);
    EXPECT_FALSE(is_std_container<float>::value);
    EXPECT_FALSE(is_std_container<pod_type>::value);
    EXPECT_FALSE(is_std_container<tuple_test_t1>::value);
}

TEST(TraitsTests, IsContiguousContainer) {
    EXPECT_TRUE(is_contiguous_container<vector<int> >::value);
    EXPECT_FALSE(is_contiguous_container<array100int>::value);
    EXPECT_TRUE(is_contiguous_container<string>::value);
    EXPECT_FALSE(is_contiguous_container<list<int> >::value);
    EXPECT_FALSE(is_contiguous_container<set_test>::value);
    EXPECT_FALSE(is_contiguous_container<map_test>::value);
    EXPECT_FALSE(is_contiguous_container<unordered_set_test>::value);
    EXPECT_FALSE(is_contiguous_container<unordered_map_test>::value);
    EXPECT_FALSE(is_contiguous_container<int>::value);
    EXPECT_FALSE(is_contiguous_container<float>::value);
    EXPECT_FALSE(is_contiguous_container<pod_type>::value);
    EXPECT_FALSE(is_contiguous_container<tuple_test_t1>::value);
}

TEST(TraitsTests, IsForwardIteratableContainer) {
    EXPECT_FALSE(is_fw_iterable_container<vector<int> >::value);
    EXPECT_FALSE(is_fw_iterable_container<array100int>::value);
    EXPECT_FALSE(is_fw_iterable_container<string>::value);
    EXPECT_FALSE(is_fw_iterable_container<list<int> >::value);
    EXPECT_TRUE(is_fw_iterable_container<forward_list<int> >::value);
    EXPECT_FALSE(is_fw_iterable_container<set_test>::value);
    EXPECT_FALSE(is_fw_iterable_container<map_test>::value);
    EXPECT_TRUE(is_fw_iterable_container<unordered_set_test>::value);
    EXPECT_TRUE(is_fw_iterable_container<unordered_map_test>::value);
    EXPECT_FALSE(is_fw_iterable_container<int>::value);
    EXPECT_FALSE(is_fw_iterable_container<float>::value);
    EXPECT_FALSE(is_fw_iterable_container<pod_type>::value);
    EXPECT_FALSE(is_fw_iterable_container<tuple_test_t1>::value);
}

TEST(HelperTests, SizeOfTuple)
{
    size_t realsize_temp = sizeof(i) + sizeof(f) + sizeof(d) + str_gt.size() + sizeof(ArraySize);
    EXPECT_EQ(argumentSize(t_tuple), realsize_temp);
}

TEST(SerializerTests, SizeOfSer)
{
    size_t realsize_temp = sizeof(i) + sizeof(f) + sizeof(d) + str_gt.size() + sizeof(uint32_t);
    auto serie = serialize(t_tuple);
    EXPECT_EQ(serie.size(), realsize_temp);
}

TEST(SerializerTests, SizeOfVecVec)
{
    size_t realsize = raw1.size() + raw2.size() + raw3.size() + sizeof(uint32_t) * 3 + sizeof(uint32_t);
    auto serialized = serialize(vecvec);
    EXPECT_EQ(serialized.size(), realsize);
}

TEST(DeserializerTests, DeserVector)
{
    using ret_type = decltype(raw1);
    size_t os=0;
    auto seialized = serialize<ret_type>(raw1);
    auto deserialized = deserialize<ret_type>(seialized, os);
    EXPECT_EQ(raw1, deserialized);
}

TEST(DeserializerTests, DeserVecVec)
{
    using ret_type = decltype(vecvec);
    size_t os=0;
    auto seialized = serialize<ret_type>(vecvec);
    auto deserialized = deserialize<ret_type>(seialized, os);
    EXPECT_EQ(vecvec, deserialized);
}

TEST(TraitsTests, isSerializableTest)
{
    EXPECT_TRUE(serdes::is_serializable_object<serdes::test::TestClass>::value);
    EXPECT_FALSE(serdes::is_serializable_object<int>::value);
    EXPECT_FALSE(serdes::is_serializable_object<float>::value);
    EXPECT_FALSE(serdes::is_serializable_object<std::vector<int> >::value);
}

TEST(SerDeser, SerializeClassTest)
{
    TestClass default_tc;
    TestClass testclass_obj{ "Test", 123, Count::WRONG };
    cout << default_tc.toString() << std::endl << testclass_obj.toString() << std::endl;
    TestClass uno("Uno", 1, Count::FIRST);
    auto serialized_obj = uno.Serialize();
    TestClass deserialized_obj;
    EXPECT_NE(uno, deserialized_obj);
    cout << "Deserialized object size: " << deserialized_obj.Deserialize(serialized_obj) << std::endl;
    cout << "Deserialized object data:\n" << deserialized_obj.toString() << std::endl;
    EXPECT_EQ(uno, deserialized_obj);
    TestClass duos("Duos", 0xDEADBEEF, Count::SECOND);
    serialized_obj = serdes::serialize(duos);
    size_t os = 0;
    TestClass deserialized_duos = serdes::deserialize<TestClass>(serialized_obj, os);
    EXPECT_EQ(duos, deserialized_duos);
}
