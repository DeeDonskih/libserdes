#include <serdes.hpp>

#include <vector>
#include <string>

struct pod_type
{
    int i;
    char f;
};

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
