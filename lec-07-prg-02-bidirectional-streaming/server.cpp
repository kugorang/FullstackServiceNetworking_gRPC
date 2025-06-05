#include "server.hpp"
#include <iostream>

using namespace std;

Status BidirectionalServer::GetServerResponse(ServerContext* context,
    ServerReaderWriter<Message, Message>* stream)
{
    cout << "서버가 gRPC 양방향 스트리밍을 처리합니다." << endl;
    
    // 클라이언트로부터 메시지를 읽을 객체
    Message request;
    
    // 스트림에서 메시지를 읽는 루프
    while (stream->Read(&request))
    {
        cout << "[클라이언트 -> 서버] " << request.message() << endl;
        
        // 받은 메시지를 그대로 다시 전송 (에코)
        Message response;
        response.set_message(request.message());
        
        if (!stream->Write(response))
        {
            cerr << "클라이언트 연결이 끊어졌습니다." << endl;
            break;
        }
        
        cout << "[서버 -> 클라이언트] " << response.message() << endl;
    }
    
    cout << "양방향 스트리밍 완료" << endl;
    return Status::OK;
}

int main()
{
    EnableDefaultHealthCheckService(true);
 
    // 서버 설정 및 실행
    string server_address("0.0.0.0:50051");
    BidirectionalServer service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, InsecureServerCredentials());
    builder.RegisterService(&service);
    
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "서버 시작. 포트 50051에서 리스닝 중입니다." << endl;
    
    server->Wait();
    return 0;
}
