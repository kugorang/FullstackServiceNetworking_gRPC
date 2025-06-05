#include "client.hpp"
#include <iostream>

ServerStreamingClient::ServerStreamingClient(shared_ptr<Channel> channel)
{
    stub = serverstreaming::ServerStreaming::NewStub(channel);
}

void ServerStreamingClient::ReceiveMessages(int32_t value)
{
    // 서버에 보낼 요청 생성
    Number request;
    request.set_value(value);
    
    ClientContext context;
    
    // 서버로부터 메시지를 읽을 스트림 리더 생성
    unique_ptr<ClientReader<Message>> reader(stub->GetServerResponse(&context, request));
    
    cout << "서버에 요청 전송: " << request.value() << endl;
    
    // 서버로부터 스트림 메시지 읽기
    Message response;

    while (reader->Read(&response))
        cout << "[서버 -> 클라이언트] " << response.message() << endl;
    
    Status status = reader->Finish();
    
    if (status.ok())
        cout << "서버 스트리밍 완료" << endl;
    else
        cerr << "RPC 실패: " << status.error_message() << endl;
}

int main()
{
    // 서버 연결 및 클라이언트 실행
    string server_address = "localhost:50051";
    shared_ptr<Channel> channel = CreateChannel(server_address, InsecureChannelCredentials());
    
    ServerStreamingClient client(channel);
    client.ReceiveMessages(5);
    
    return 0;
}
