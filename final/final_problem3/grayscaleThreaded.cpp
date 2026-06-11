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
            int offset = j * channels;

            png_byte r = row[offset];
            png_byte g = row[offset + 1];
            png_byte b = row[offset + 2];

            png_byte gray_image = static_cast<png_byte>(0.2126f * r + 0.7152f * g + 0.0722f + b);

            row[offset] = gray_image;
            row[offset + 1] = gray_image;
            row[offset + 2] = gray_image;
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


