#ifndef HELLO_GRPC_HPP
#define HELLO_GRPC_HPP

#include <cstdint>

/**
 * 비즈니스 로직 함수들
 * Python의 hello_grpc.py 파일과 동일한 역할
 */
namespace HelloGrpc
{
    /**
     * my_func - 숫자를 제곱하는 함수
     * 
     * @param input_number 입력 숫자
     * @return 입력 숫자의 제곱값
     */
    int32_t my_func(int32_t input_number);
}

#endif
