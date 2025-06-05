#include "client.hpp"
#include <iostream>
#include <vector>

ClientStreamingClient::ClientStreamingClient(shared_ptr<Channel> channel)
{
    stub = ClientStreaming::NewStub(channel);
}

void ClientStreamingClient::SendMessages()
{
    // 서버로부터 받을 응답 객체
    Number response;
    ClientContext context;
    
    // 클라이언트 스트리밍을 위한 Writer 생성
    unique_ptr<ClientWriter<Message>> writer(stub->GetServerResponse(&context, &response));
    
    // 5개의 메시지를 생성하여 전송
    int message_count = 5;
    
    for (int i = 1; i <= message_count; ++i)
    {
        // 메시지 생성
        Message message;
        message.set_message("message #" + to_string(i));
        
        cout << "[클라이언트 -> 서버] " << message.message() << endl;
        
        if (!writer->Write(message))
        {
            cerr << "메시지 전송 실패" << endl;
            break;
        }
    }
    
    writer->WritesDone();
    
    Status status = writer->Finish();
    
    if (status.ok())
        cout << "[서버 -> 클라이언트] " << response.value() << endl 
            << "서버가 " << response.value() << "개의 메시지를 받았습니다." << endl;
    else
        cerr << "RPC 실패: " << status.error_message() << endl;
}

int main()
{
    // 서버 연결 및 클라이언트 실행
    string server_address = "localhost:50051";
    shared_ptr<Channel> channel = CreateChannel(server_address, InsecureChannelCredentials());
    
    ClientStreamingClient client(channel);
    client.SendMessages();
    
    return 0;
}
