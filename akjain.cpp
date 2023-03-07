#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <taskflow/taskflow.hpp>

// Helper function to print the content of an unordered map
void PrintUnorderedMap(std::unordered_map<std::string, int> hash_map) {
    std::cout << "Printing unordered map: " << std::endl;
    for (const auto& element : hash_map) {
        std::cout << element.first << " " << element.second << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    // Setup the taskflow 
    tf::Taskflow tf;

    // Hash tables to combine
    std::unordered_map<std::string, int> hash_table1{{"foo", 1}, {"bar", 2}, {"baz", 3}};
    std::unordered_map<std::string, int> hash_table2{{"bar", 4}, {"baz", 5}, {"qux", 6}};
    std::unordered_map<std::string, int> hash_table3{{"foo", 7}, {"baz", 8}, {"qux", 9}};
    std::unordered_map<std::string, int> result_hash_table;

    // Print the hash tables
    PrintUnorderedMap(hash_table1);
    PrintUnorderedMap(hash_table2);
    PrintUnorderedMap(hash_table3);

    // Create the tasks
    auto task1 = tf.emplace([&]() {
        // Combine the first two hash tables
        for (const auto& element : hash_table2) {
            if (hash_table1.find(element.first) != hash_table1.end()) {
                result_hash_table[element.first] = hash_table1[element.first] + element.second;
            }
            else {
                result_hash_table[element.first] = element.second;
            }
        }
    });

    auto task2 = tf.emplace([&]() {
        // Combine the third hash table
        for (const auto& element : hash_table3) {
            if (result_hash_table.find(element.first) != result_hash_table.end()) {
                result_hash_table[element.first] += element.second;
            }
            else {
                result_hash_table[element.first] = element.second;
            }
        }
    });

    // Connect the tasks
    task1.precede(task2);

    // Create a taskflow executor
    tf::Executor executor;

    // Execute the taskflow
    executor.run(tf).get();

    // Print the result
    PrintUnorderedMap(result_hash_table);

    return 0;
}
