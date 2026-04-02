<h1>C++ Limit Order Book Simulator 🚀</h1>

Production-grade HFT matching engine modeling market microstructure with price-time priority, partial fills, and microsecond-latency design patterns. Built for Graviton Research Capital quant interviews.

<h2>Features</h2>

-Price-time priority matching (best bid/ask, FIFO queues per level)

-Market/Limit orders with GTC/IOC support

-Partial fills & aggressive order crossing

-NSE/BSE tick data replay ready

-C++20 + cache-optimized data structures

<h2>Architecture</h2>

```
text
std::map<double, std::deque<Order>>  →  Price levels → FIFO queues
rbegin()/begin()                     →  O(1) best bid/ask access
custom allocators planned            →  Zero-copy low-latency
```

<h2>Quick Demo</h2>

```cpp
LimitOrderBook lob;
lob.addOrder(true, 100.0, 50);   // BID@100: 50
lob.addOrder(false, 99.5, 30);   // ASK@99.5: 30
lob.addOrder(true, 99.8, 40);    // Aggressive BID → TRADE 30@99.5
```

<h2>Output:</h2>

```text
TRADE: 30 @ 99.5 (ID: 2)
BID 100: 20
BID 99.8: 40
```

<h2>Tech Stack</h2>

```text
C++20 • STL • OpenMP • Linux
g++ -std=c++20 -O3 -march=native
```

<h2>Build & Run</h2>

```bash
g++ -std=c++20 -O3 main.cpp -o lob
./lob    # Linux/Mac
.\lob.exe # Windows
```

<h2>Roadmap</h2>

-Core matching engine

-Custom allocators (arena/pool)

-Lock-free queues (Disruptor)

-Multi-threaded tick replay

-HFT strategies (market making, sniping)

-LOB depth visualization
