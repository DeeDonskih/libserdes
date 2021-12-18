#include "../serdes.hpp"

#include <array>
#include <vector>
#include <string>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include <gtest/gtest.h>

using namespace serdes;
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
vector<int> vec_i{1,2,3,4,5};
vector<float> vec_f{1,2,3,4,5};
vector<double> vec_d{1,2,3,4,5};


using array100int = array<int,100>;
using map_test = map<std::string,int>;
using unordered_map_test = unordered_map<std::string,int>;
using set_test = set<std::string>;
using unordered_set_test = unordered_set<std::string>;

static auto t_tuple = make_tuple(i,f,d,str_gt);
using tuple_test_t = decltype (t_tuple);
static auto t_tuple1 = make_tuple(i,f,d,i8,i16,i32,i64,ui8,ui16,ui32,ui64);
using tuple_test_t1 = decltype (t_tuple1);
static auto t_tuple2 = make_tuple(ui64,ui32,ui16,ui8,i64,i32,i16,i8,d,f,i);
using tuple_test_t2 = decltype (t_tuple2);

pod_type pt{};


TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(TraitsTests,IsContainer){
    EXPECT_TRUE(is_std_container<vector<int>>::value);
    EXPECT_TRUE(is_std_container<array100int>::value);
    EXPECT_TRUE(is_std_container<string>::value);
    EXPECT_TRUE(is_std_container<list<int>>::value);
    EXPECT_TRUE(is_std_container<set_test>::value);
    EXPECT_TRUE(is_std_container<map_test>::value);
    EXPECT_TRUE(is_std_container<unordered_set_test>::value);
    EXPECT_TRUE(is_std_container<unordered_map_test>::value);
    EXPECT_FALSE(is_std_container<int>::value);
    EXPECT_FALSE(is_std_container<float>::value);
    EXPECT_FALSE(is_std_container<pod_type>::value);
    EXPECT_FALSE(is_std_container<tuple_test_t1>::value);
}

TEST(TraitsTests,IsContiguousContainer){
    EXPECT_TRUE(is_contiguous_container<vector<int>>::value);
    EXPECT_FALSE(is_contiguous_container<array100int>::value);
    EXPECT_TRUE(is_contiguous_container<string>::value);
    EXPECT_FALSE(is_contiguous_container<list<int>>::value);
    EXPECT_FALSE(is_contiguous_container<set_test>::value);
    EXPECT_FALSE(is_contiguous_container<map_test>::value);
    EXPECT_FALSE(is_contiguous_container<unordered_set_test>::value);
    EXPECT_FALSE(is_contiguous_container<unordered_map_test>::value);
    EXPECT_FALSE(is_contiguous_container<int>::value);
    EXPECT_FALSE(is_contiguous_container<float>::value);
    EXPECT_FALSE(is_contiguous_container<pod_type>::value);
    EXPECT_FALSE(is_contiguous_container<tuple_test_t1>::value);
}

TEST(TraitsTests,IsForwardIteratableContainer){
    EXPECT_FALSE(is_fw_iterable_container<vector<int>>::value);
    EXPECT_FALSE(is_fw_iterable_container<array100int>::value);
    EXPECT_FALSE(is_fw_iterable_container<string>::value);
    EXPECT_FALSE(is_fw_iterable_container<list<int>>::value);
    EXPECT_TRUE(is_fw_iterable_container<forward_list<int>>::value);
    EXPECT_FALSE(is_fw_iterable_container<set_test>::value);
    EXPECT_FALSE(is_fw_iterable_container<map_test>::value);
    EXPECT_TRUE(is_fw_iterable_container<unordered_set_test>::value);
    EXPECT_TRUE(is_fw_iterable_container<unordered_map_test>::value);
    EXPECT_FALSE(is_fw_iterable_container<int>::value);
    EXPECT_FALSE(is_fw_iterable_container<float>::value);
    EXPECT_FALSE(is_fw_iterable_container<pod_type>::value);
    EXPECT_FALSE(is_fw_iterable_container<tuple_test_t1>::value);
}

TEST(HelperTests,SizeOfTuple)
{
  size_t realsize_temp = sizeof (i)+sizeof (f)+sizeof(d)+str_gt.size()+sizeof(ArraySize);
    EXPECT_EQ(argumentSize(t_tuple),realsize_temp);
}

TEST(SerializerTests,SizeOfSer)
{
    size_t realsize_temp = sizeof (i)+sizeof (f)+sizeof(d)+str_gt.size()+sizeof(uint32_t);
    auto serie = serialize(t_tuple);
    EXPECT_EQ(serie.size(),realsize_temp);
}
/*
int main(int argc, char *argv[])
{
  using namespace std::string_literals;
    int i=100;
    float f=100.0;
    double d=100.0;

    pod_type pt{};

//        std::cout<< "template pod "<< ((serdes::is_pod_new<pod_type>::value)) <<std::endl;
//        std::cout<< "template trivialy copyable "<< ((std::is_trivially_copyable<pod_type>::value)) <<std::endl;
//        std::cout<< "template trivial "<< ((std::is_trivial<pod_type>::value)) <<std::endl;
//        std::cout<< "template std layout "<< ((std::is_standard_layout<pod_type>::value)) <<std::endl;

//        std::cout<< "Vector has ra "<< ((serdes::has_random_access_iteratior<std::vector<float>>::value)) <<std::endl;
//        std::cout<< "Float has ra "<< ((serdes::has_random_access_iteratior<float>::value)) <<std::endl;
//        std::cout<< "Vector has fw "<< ((serdes::has_forward_iteratior<std::vector<float>>::value)) <<std::endl;
//        std::cout<< "Vector is pod "<< ((serdes::is_pod_new<std::vector<float>>::value)) <<std::endl;
//        std::cout<< "============================================================================="<<std::endl<<std::endl;

    std::vector<int> i_vec={1,2,3,4,5,6};
    std::vector<float> f_vec={1.0,0.1,0.01,0,001,0.0001,0.00001};

    std::string cstr = "std::move(i_vec)"s;

    using test_tuple = std::tuple<int,float,std::string>;
    test_tuple tuple_ser = std::make_tuple(1,2.0,"34"s);
    std::cout << std::endl << "size of std::make_tuple(1,2.0,s): " << serdes::argumentSize(tuple_ser) << std::endl;

    serdes::ByteArray ser_pack{};
    size_t off=0;
    serdes::ByteArray ser_str_rv = serdes::serialize("std::move(i_vec)"s);
    std::string str_deser = serdes::deserialize<std::string>(ser_str_rv,off);
    std::cout << std::endl << "Deser string: " << str_deser << std::endl;

    serdes::serialize_tuple(ser_pack,tuple_ser);
    test_tuple tuple_deser{};
    off=0;
    serdes::deserialize_tuple(tuple_deser,ser_pack,off);
    off=0;
    test_tuple tuple_ready = serdes::deserialize<test_tuple>(ser_pack,off);

    std::cout << std::endl << "Par1: " << std::get<0>(tuple_deser) << "Par2: " << std::get<1>(tuple_deser) << "Par3: " << std::get<2>(tuple_deser) << std::endl;
    std::cout << std::endl << "Par1: " << std::get<0>(tuple_ready) << "Par2: " << std::get<1>(tuple_ready) << "Par3: " << std::get<2>(tuple_ready) << std::endl;

//    auto des_pack = serdes::deserialize<int,float,std::string>(ser_pack,0);

//    test_tuple tuple_deser = serdes::deserialize<std::tuple<int,float,std::string>>(ser_pack,0);
//    serdes::serialize(ser_pack,1,2,2.5,"3"s);
//    serdes::ByteArray ser_i = serdes::serialize(i);
//    serdes::ByteArray ser_pt = serdes::serialize(pt);

//    serdes::ByteArray ser_v = serdes::serialize(i_vec);
//    serdes::ByteArray ser_v_lv = serdes::serialize<std::vector<float>>(fv);
//    serdes::ByteArray ser_I_rv = serdes::serialize(1);
//    serdes::ByteArray ser_I_rv2 = serdes::serialize(2);
//    serdes::ByteArray ser_f_lv = serdes::serialize(f);
//    serdes::ByteArray ser_vec_rv = serdes::serialize(std::vector<float>{});
//    serdes::ByteArray ser_vec_rv2 = serdes::serialize(std::move(i_vec));
//    serdes::ByteArray ser_str_lv = serdes::serialize(cstr_l);
//    serdes::ByteArray ser_str_rv = serdes::serialize("std::move(i_vec)"s);

    return 0;
}
*/
