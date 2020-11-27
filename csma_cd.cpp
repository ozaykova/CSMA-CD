#include "csma_cd.h"

double getPause(int retry) {
    if (retry > 10) {
        retry = 10; // do not increase after 10th retry
    }
    auto res = (rand() % ((1 << retry) + 1)) * BIT_INTERVAL_512;
    return res;
}

bool cmp(const Node& a, const Node& b) {
    return a.TimeToSendFrame < b.TimeToSendFrame;
}

void CSMA_CD::Process() {
    while (Nodes.size() > 0) {
        sort(Nodes.begin(), Nodes.end(), cmp);
        if (Nodes[0].TimeToSendFrame >= TIME_NOW) {
            TIME_NOW = Nodes[0].TimeToSendFrame;
        } else {
            for (size_t i = 0; i < Nodes.size(); ++i) {
                if (Nodes[i].TimeToSendFrame < TIME_NOW) {
                    Nodes[i].TimeToSendFrame = TIME_NOW; // This nodes try to send frame in inter package gap
                }
            }
        }
        if (!CheckCollision()) {
            TIME_NOW += TIME_TO_TRANSFER_ONE_FRAME;
            std::cout << "Successfully transferred frame from node " << Nodes[0].NodeNum << " in "
                      << TIME_NOW << " mks" << std::endl;
            Nodes.erase(Nodes.begin());
            TIME_NOW += INTER_PACKET_GAP;
        }
    }
}

bool CSMA_CD::NeedSendFrame(int i) {
    if (Nodes[i].Retry >= 16) {
        Nodes.erase(Nodes.begin() + i);
        std::cout << "Fail to transfer frame from node " << i << " retry limit exceeded" << std::endl;
        return false;
    }
    return true;
}

bool CSMA_CD::CheckCollision() {
    int collisionsCount = 0;
    for (size_t i = 1; i < Nodes.size(); ++i) {
        if (Nodes[i].TimeToSendFrame < Nodes[0].TimeToSendFrame + TIME_TO_TRANSFER_ONE_FRAME) {
            ++collisionsCount;
            ++Nodes[i].Retry;
            if (NeedSendFrame(i)) {
                Nodes[i].TimeToSendFrame += getPause(Nodes[i].Retry);
            }
        }
    }
    if (collisionsCount > 0) {
        ++Nodes[0].Retry;
        if (NeedSendFrame(0)) {
            Nodes[0].TimeToSendFrame += getPause(Nodes[0].Retry);
        }
        return true;
    }
    return false;
}

int main() {
    srand(time(0));
    bool success = false;
    int N; // count of nodes
    while (!success) {
        std::cout << "Enter the number of nodes from 1 to 100" << std::endl;
        std::string s;
        std::cin >> s;
        N = atoi(s.c_str());
        if (N > 0 && N < 101) {
            success = true;
        }
    }
    std::vector<Node> nodes;
    for (size_t i = 0; i < N; ++i) {
        nodes.push_back(Node(i));
    }
    CSMA_CD ethernet(std::move(nodes));
    ethernet.Process();
}