#include "server.hpp"
#include <iostream>

using namespace std;
using namespace grpc;
using namespace clientstreaming;

Status ClientStreamingServer::GetServerResponse(ServerContext* context,
    ServerReader<Message>* reader, Number* response)
{    
    cout << "서버가 gRPC 클라이언트 스트리밍을 처리합니다." << endl;
    
    // 메시지 카운터
    int32_t count = 0;
    
    // 클라이언트로부터 메시지 읽기
    Message request;

    while (reader->Read(&request))
    {
        cout << "[클라이언트 -> 서버] " << request.message() << endl;
        ++count;
    }
    
    // 총 메시지 개수를 응답으로 설정
    response->set_value(count);
    cout << "총 " << count << "개의 메시지를 받았습니다." << endl;
    
    return Status::OK;
}

int main()
{
    EnableDefaultHealthCheckService(true);

    // 서버 설정 및 실행
    string server_address("0.0.0.0:50051");
    ClientStreamingServer service;
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, InsecureServerCredentials());
    builder.RegisterService(&service);
    
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "서버 시작. 포트 50051에서 리스닝 중입니다." << endl;
    
    server->Wait();
    return 0;
}
