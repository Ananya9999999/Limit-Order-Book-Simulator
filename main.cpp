#include<iostream>
#include<map>
#include<deque>
#include<chrono>

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
            while (!bids.empty() && !asks.empty() && bids.rbegin()-> first >= asks.begin()->first){
                auto& bestBid= bids.rbegin()->second;
                auto& bestAsk= asks.begin()->second;

                double tradePrice= bestAsk.front().price;
                uint64_t tradedVol= std::min(bestBid.front().volume, bestAsk.front().volume);

                std::cout << "TRADE: " << tradedVol << " @ " << tradePrice << " (ID: " << bestAsk.front().id << ")" << std::endl;
                std::cout.flush();

                bestBid.front().volume-= tradedVol;
                bestAsk.front().volume-= tradedVol;

                if (bestBid.front().volume==0) bestBid.pop_front();
                if (bestAsk.front().volume==0) bestAsk.pop_front();

                if (bids.rbegin()->second.empty()) bids.erase(bids.rbegin()->first);
                if (asks.begin()->second.empty()) asks.erase(asks.begin()->first);
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
    return 0;
}