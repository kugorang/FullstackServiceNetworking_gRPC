#include "client.hpp"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace grpc;
using namespace bidirectional;

BidirectionalClient::BidirectionalClient(shared_ptr<Channel> channel)
{
    stub = Bidirectional::NewStub(channel);
}

void BidirectionalClient::SendAndReceiveMessages()
{
    ClientContext context;
    
    // 양방향 스트림 생성
    shared_ptr<ClientReaderWriter<Message, Message>> stream(stub->GetServerResponse(&context));
    
    // 메시지 전송용 스레드
    thread writer_thread([stream]() {
        vector<string> messages = { "message #1", "message #2", "message #3", 
            "message #4", "message #5" };
        
        for (const auto& text : messages) 
        {
            Message message;
            message.set_message(text);
            
            cout << "[클라이언트 -> 서버] " << message.message() << endl;
            
            if (!stream->Write(message))
            {
                cerr << "메시지 전송 실패" << endl;
                break;
            }
            
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        
        stream->WritesDone();
    });
    
    // 서버로부터 응답 받기
    Message response;

    while (stream->Read(&response))
        cout << "[서버 -> 클라이언트] " << response.message() << endl;
    
    writer_thread.join();
    
    Status status = stream->Finish();

    if (!status.ok())
        cerr << "RPC 실패: " << status.error_message() << std::endl;
    else
        cout << "양방향 스트리밍 완료" << std::endl;
}

int main()
{
    // 서버 연결 및 클라이언트 실행
    string server_address = "localhost:50051";
    shared_ptr<Channel> channel = CreateChannel(server_address, InsecureChannelCredentials());
    
    BidirectionalClient client(channel);
    client.SendAndReceiveMessages();
    
    return 0;
}
