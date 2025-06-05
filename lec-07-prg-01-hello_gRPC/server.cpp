#include "server.hpp"
#include <iostream>

using namespace std;
using namespace grpc;

/**
 * MyFunction 구현
 * 
 * 클라이언트로부터 받은 숫자를 제곱하여 반환합니다.
 * Python의 hello_grpc.my_func() 함수와 동일한 로직입니다.
 */
Status MyServiceImplement::MyFunction(ServerContext* context, const MyNumber* request, 
    MyNumber* response)
{
    // 클라이언트로부터 받은 값 로깅
    cout << "서버가 받은 값: " << request->value() << endl;
    
    // 제곱 계산
    // Python: input_number * input_number
    int32_t input_value = request->value();
    int32_t result = input_value * input_value;
    
    // 계산 결과를 응답 메시지에 설정
    response->set_value(result);
    
    // 전송할 값 로깅
    cout << "서버가 보낼 값: " << response->value() << endl;
    
    // 성공 상태 반환
    return Status::OK;
}

/**
 * gRPC 서버 실행 함수
 * 
 * 서버를 초기화하고 클라이언트 연결을 대기합니다.
 * Python의 serve() 함수에 대응합니다.
 */
static void StartGrpcServer()
{   
    // gRPC 기본 기능 활성화
    EnableDefaultHealthCheckService(true);
    
    // 서버 빌더 구성
    ServerBuilder builder;

    // 서버 주소 설정 (모든 인터페이스, 포트 50051)
    string server_address = "0.0.0.0:50051";
    
    // 리스닝 포트 추가 (보안 없는 연결)
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // 서비스 인스턴스 생성
    MyServiceImplement service;

    // 서비스 등록
    builder.RegisterService(&service);
    
    // 서버 빌드 및 시작
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "서버가 " << server_address << " 에서 시작되었습니다." << endl;
    
    // 종료 신호 대기
    server->Wait();
}

int main()
{
    try
    {
        StartGrpcServer();
    }
    catch (const exception& e) 
    {
        cerr << "서버 오류: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
