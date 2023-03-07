#include <iostream>
#include "taskflow/taskflow.hpp"

int main() {
  tf::Taskflow taskflow;

  auto task1 = taskflow.emplace([] () {
    std::cout << "Task 1\n";
  });

  auto task2 = taskflow.emplace([] () {
    std::cout << "Task 2\n";
  });

  auto task3 = taskflow.emplace([] () {
    std::cout << "Task 3\n";
  });

  task1.precede(task2);
  task2.precede(task3);

  tf::Executor executor;
  executor.run(taskflow).wait();

  return 0;
}
