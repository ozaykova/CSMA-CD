#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

double INTER_PACKET_GAP = 9.6;
double TIME_NOW = 0;
double BIT_INTERVAL_512 = 51.2; // in mks, using 10MBit/sec Ethernet
double TIME_TO_TRANSFER_ONE_FRAME = 1210.8;

class Node {
public:
    double TimeToSendFrame = 0;
    int Retry = 0;
    int NodeNum = 0;

    Node(int n) : NodeNum(n) {}
};

class CSMA_CD {
public:
    CSMA_CD(std::vector<Node> &&N) : Nodes(N) {}
    void Process();

private:
    std::vector<Node> Nodes;
    bool NeedSendFrame(int i);
    bool CheckCollision();
};