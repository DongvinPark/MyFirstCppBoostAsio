#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <thread>
#include <vector>

boost::lockfree::queue<int> queue(100);  // Capacity: 100

void producer() {
  for (int i = 1; i <= 10; ++i) {
    while (!queue.push(i)) {}  // Keep trying until successful
    std::cout << "Produced: " << i << std::endl;
  }
}

void consumer() {
  int value;
  while (true) {
    while (queue.pop(value)) {  // Try to pop elements
      std::cout << "Consumed: " << value << std::endl;
    }
  }
}

int main() {
  std::thread t1(producer);
  std::thread t2(consumer);

  t1.join();
  t2.join();
}
