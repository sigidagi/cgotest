#include "demo.hpp"
#include <iostream>
Demo::Demo() {}

Demo::~Demo() {}

void Demo::demo_do() const { std::cout << "Hello from Demo" << std::endl; }

std::unique_ptr<Demo> new_demo() { return std::make_unique<Demo>(); }
