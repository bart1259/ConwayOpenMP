#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <chrono>

#define WIDTH 100
#define HEIGHT 100
#define DEFAULT_ITERATIONS 100

//#define USE_OPENMP

inline int get_index(int x, int y, int width) {
    return x + (y * width);
}

struct board
{
public:
    const int width;
    const int height;
    bool* data;
    bool* buffer;

    board(int width, int height);
    void print();
    void randomize();
    void next_step();
    void next_n_steps(int n);
    int get_neighbors(int x, int y);
};

board::board(int width, int height) : width(width), height(height) {
    data = new bool[width * height];
    buffer = new bool[width * height];
}

void board::print(){
    std::stringstream ss;
    ss << std::string(width, '=') << std::endl;
    
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            ss << (data[get_index(x, y, width)] ? '*' : ' ');
        }

        ss << '\n';
    }
    
    std::cout << ss.str();
}


void board::randomize() {
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            data[get_index(x, y, width)] = rand() % 2;
        }
    }
}

int board::get_neighbors(int x, int y) {

    int count = 0;

    for (size_t xx = std::max(0, x-1); xx <= std::min(width - 1, x + 1); xx++)
    {
        for (size_t yy = std::max(0, y-1); yy <= std::min(width - 1, y + 1); yy++)
        {
            count += data[get_index(xx, yy, width)];
        }
        
    }

    return count;
}

void board::next_step() {
    #ifdef USE_OPENMP
    #pragma omp parallel for
    #endif
    for (size_t y = 0; y < height; y++)
    {
        #ifdef USE_OPENMP
        #pragma omp parallel for
        #endif
        for (size_t x = 0; x < width; x++)
        {
            int neighbors = get_neighbors(x, y);
            if(neighbors <= 1){
                buffer[get_index(x, y, width)] = 0; // Solitude
            } else if(neighbors >= 4) {
                buffer[get_index(x, y, width)] = 0; // Overpopulation
            } else if (neighbors == 3){
                buffer[get_index(x, y, width)] = 1; // Breeding 
            }
        }
    }
    std::swap(buffer, data);
}

void board::next_n_steps(int n){
    for (size_t i = 0; i < n; i++)
    {
        next_step();
    }
    
}


uint64_t get_ms() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(int argc, char **argv) {

    int iterations = DEFAULT_ITERATIONS;

    try
    {
        if(argc > 1)
            iterations = std::stoi(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cout << "Invalid iteration count" << std::endl;
    }


    #ifdef USE_OPENMP
    std::cout << "Compiled with openmp" << std::endl;
    #else
    std::cout << "Compiled without openmp" << std::endl;
    #endif

    std::cout << "Performing " << iterations << " iterations" << std::endl;

    board* map = new board(WIDTH, HEIGHT);

    map->randomize();

    long now = get_ms();

    map->next_n_steps(iterations);

    std::cout << "Done in " << (get_ms() - now) / 1000.0f << " seconds" << std::endl;

    return 0;
}