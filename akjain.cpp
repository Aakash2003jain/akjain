#include <iostream>
#include <chrono>
#include "taskflow/taskflow.hpp"

// Define the function to be executed in parallel
void task(int id) {
  std::cout << "Task " << id << " is running on thread " << std::this_thread::get_id() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
  // Create a taskflow object
  tf::Taskflow tf;

  // Create the nodes of the DAG
  auto node1 = tf.emplace([](){ task(1); }).name("Node 1");
  auto node2 = tf.emplace([](){ task(2); }).name("Node 2");
  auto node3 = tf.emplace([](){ task(3); }).name("Node 3");
  auto node4 = tf.emplace([](){ task(4); }).name("Node 4");
  auto node5 = tf.emplace([](){ task(5); }).name("Node 5");

  // Define the dependencies between the nodes
  node1.precede(node2);
  node2.precede(node3, node4);
  node3.precede(node5);
  node4.precede(node5);

  // Execute the DAG in parallel
  tf::Executor executor;
  executor.run(tf).wait();

  return 0;
}
