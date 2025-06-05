#include "server.hpp"
#include <iostream>
#include <vector>
#include <thread>

using namespace std;
using namespace grpc;
using namespace serverstreaming;

Status ServerStreamingServer::GetServerResponse(ServerContext* context,
    const Number* request, ServerWriter<Message>* writer)
{
    // 받은 요청값 출력
    cout << "서버가 gRPC 서버 스트리밍을 처리합니다 {" 
              << request->value() << "}." << endl;
    
    // request의 value만큼 메시지 전송
    int32_t count = request->value();
    
    for (int i = 1; i <= count; ++i)
    {
        // 메시지 생성
        Message message;
        message.set_message("message #" + to_string(i));
        
        cout << "[서버 -> 클라이언트] " << message.message() << endl;
        
        if (!writer->Write(message))
        {
            cerr << "클라이언트 연결이 끊어졌습니다." << endl;
            return Status::CANCELLED;
        }
        
        // 메시지 간 지연
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    cout << "서버 스트리밍 완료" << endl;
    return Status::OK;
}

int main()
{   
    EnableDefaultHealthCheckService(true);

    // 서버 설정 및 실행
    string server_address("0.0.0.0:50051");
    ServerStreamingServer service;
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, InsecureServerCredentials());
    builder.RegisterService(&service);
    
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "서버 시작. 포트 50051에서 리스닝 중입니다." << endl;
    
    server->Wait();
    return 0;
}
