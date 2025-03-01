#include <iostream>
#include <random>
#include <cstdlib>
#include <string>

int main(int argc, char *argv[]) {
        int n = std::stoi(argv[1]);
        double square = 0;
        double circle = 0;
        std::random_device seed;
        std::mt19937 engine(seed());
        std::uniform_real_distribution<> dist(0.0, 1.0);

        for(int i = 0; i < n; i++){
                double x = dist(engine);
                double y = dist(engine);
                square += 1;
                if(x*x + y*y <= 1) {
                        circle += 1;
                }
        }
        double pi = (circle / square) * 4;
        std::cout << "pi:" << pi << std::endl;

        return 0;
}
