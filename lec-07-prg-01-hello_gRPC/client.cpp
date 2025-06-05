#include "client.hpp"
#include <iostream>

using namespace std;
using namespace grpc;

/**
 * HelloClient 생성자
 * 
 * 채널을 받아 stub을 초기화합니다.
 */
HelloClient::HelloClient(shared_ptr<Channel> channel)
{
    stub = MyService::NewStub(channel);
}

/**
 * Execute 메서드 구현
 * 
 * 서버에 숫자를 전송하고 제곱값을 받아옵니다.
 */
int32_t HelloClient::Execute(int32_t number)
{
    // 요청 메시지 생성
    MyNumber request;
    request.set_value(number);
    
    // 응답 메시지 준비
    MyNumber response;
    
    // RPC 컨텍스트 생성
    ClientContext context;
    
    // 동기식 RPC 호출
    Status status = stub->MyFunction(&context, request, &response);
    
    // 호출 결과 확인
    if (!status.ok())
    {
        cerr << "RPC 실패: " << status.error_code() 
                  << " - " << status.error_message() << endl;
        return -1;
    }
    
    return response.value();
}

int main()
{
    // 서버 연결 설정
    string server_address = "localhost:50051";
    shared_ptr<Channel> channel = CreateChannel(server_address, InsecureChannelCredentials());
    
    // 클라이언트 생성
    HelloClient client(channel);
    
    // 테스트 값
    int32_t input = 4;
    cout << "서버에 전송할 값: " << input << endl;
    
    // RPC 호출
    int32_t result = client.Execute(input);

    if (result >= 0)
    {
        cout << "gRPC 결과: " << result << endl
            << input << "의 제곱은 " << result << "입니다." << endl;
    }
    
    return 0;
}
