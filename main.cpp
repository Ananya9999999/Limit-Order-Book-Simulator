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
        }
}