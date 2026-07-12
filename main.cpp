#include<iostream>
#include<map>
#include<deque>
#include<chrono>
#include<algorithm>

struct Order{
    uint64_t id, volume;
    double price;
    bool isBuy;
    std::chrono::steady_clock::time_point timestamp;
};

class LimitOrderBook{
    private:
        std::map<double, std::deque<Order>, std::greater<double>> bids;
        std::map<double, std::deque<Order>> asks;

        uint64_t nextId= 1;
    public:
        void addOrder(bool isBuy, double price, uint64_t volume){
            Order order{nextId++, volume, price, isBuy, std::chrono::steady_clock::now()};

            if (isBuy){
                bids[price].emplace_back(order);
            }
            else{
                asks[price].emplace_back(order);
            }

            matchOrder();
        }

        void matchOrder(){
            while (!bids.empty() && !asks.empty() && bids.begin()->first >= asks.begin()->first){
                auto& bestBid= bids.begin()->second;
                auto& bestAsk= asks.begin()->second;

                double tradePrice= bestAsk.front().price;
                uint64_t tradedVol= std::min(bestBid.front().volume, bestAsk.front().volume);

                std::cout << "TRADE: " << tradedVol << " @ " << tradePrice << " (ID: " << bestAsk.front().id << ")" << std::endl;
                std::cout.flush();

                bestBid.front().volume-= tradedVol;
                bestAsk.front().volume-= tradedVol;

                if (bestBid.front().volume==0) bestBid.pop_front();
                if (bestAsk.front().volume==0) bestAsk.pop_front();

                if (bids.begin()->second.empty()) bids.erase(bids.begin());
                if (asks.begin()->second.empty()) asks.erase(asks.begin());
            }
        }

        void printBook() {
            std::cout << "\n=== LOB ===\n";
            for (const auto& [price, orders] : bids) {
                std::cout << "BID " << price << ": " << orders.front().volume << std::endl;
            }
            for (const auto& [price, orders] : asks) {
                std::cout << "ASK " << price << ": " << orders.front().volume << std::endl;
            }
        }
};

int main(){
    LimitOrderBook lob;
    lob.addOrder(true, 100.0, 50);
    lob.addOrder(false, 99.5, 30);
    lob.printBook();

    lob.addOrder(true,99.8, 40);
    lob.printBook();

    std::cout << "\n--- Multi-level regression check ---\n";
    LimitOrderBook lob2;
    lob2.addOrder(true, 99.0, 10);
    lob2.addOrder(true, 101.0, 10);  
    lob2.addOrder(true, 100.0, 10);
    lob2.printBook();

    std::cout << "\nIncoming ask should hit the 101.0 bid first:\n";
    lob2.addOrder(false, 98.0, 5);
    lob2.printBook();

    return 0;
}
