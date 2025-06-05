#include "hello_grpc.hpp"

namespace HelloGrpc
{
    /**
     * my_func 구현
     * 
     * Python의 hello_grpc.py에 있던 함수와 동일
     * def my_func(input_number):
     *     return input_number * input_number
     */
    int32_t my_func(int32_t input_number)
    {
        return input_number * input_number;
    }
}
