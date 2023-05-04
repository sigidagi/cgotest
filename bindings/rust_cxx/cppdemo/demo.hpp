#include <string>
#include <memory>

class Demo{
public:
    Demo();
    Demo(Demo &&) = default;
    Demo(const Demo &) = default;
    Demo &operator=(Demo &&) = default;
    Demo &operator=(const Demo &) = default;
    ~Demo();

    void demo_do() const;

private:
    
};


std::unique_ptr<Demo> new_demo();
