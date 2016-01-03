/**
    Author: Ciprian Baetu
    Date: 03 Jan 2016
*/

#pragma once
class ThreadInformation {
    int threadId;
    int client;

public:
    int getThreadId() {return threadId;}
    int getClient() {return client;}
    void setThreadId(int threadId) {this->threadId = threadId;}
    void setClient(int client) {this->client = client;}
};
