#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <png.h>

void grayscaleHelper(png_bytep* image, int width, int start_height, int end_height, int channels) {
    for (int i = start_height; i < end_height; i++) {
        png_bytep row = image[i];

        for (int j = 0; j < width; j++) {
            png_bytep px = &(row[j * channels]);
            uint8_t gray = static_cast<uint8_t>(0.3 * px[0] + 0.59 * px[1] + 0.11 * px[2]);
            px[0] = px[1] = px[2] = gray;
        }
    }
}

//threaded solution - dummy code
void grayscaleThreaded(png_bytep* image, int width, int height, int channels, int numThreads) {
    if (numThreads > height) {
        numThreads = height; // Cant have more threads than height - otherwise a thread would be doing nothing
    }

    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    int rows_per_thread = height / numThreads;
    int remainder_rows = height % numThreads;

    int current_start = 0;
    for (int i = 0; i < numThreads; i++) {
        int thread_rows = rows_per_thread;
        if (remainder_rows > 0) {
            thread_rows ++;
            remainder_rows --;
        }
        int current_end = current_start + thread_rows;

        threads.emplace_back(grayscaleHelper, image, width, current_start, current_end, channels);
        current_start = current_end;
    }

    for (auto &t : threads) {
        t.join();
    }
}


