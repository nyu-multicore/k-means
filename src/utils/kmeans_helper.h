#ifndef KMEANS_KMEANS_HELPER_H
#define KMEANS_KMEANS_HELPER_H

#include <vector>
#include <random>
#include <math.h>
#include <unordered_set>

static inline double pairwise_distance(std::vector<double> &v1, std::vector<double> &v2) {
    double sum = 0;
    for (int i = 0; i < (int)v1.size(); i++) {
        sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(sum);
}

static inline std::vector<std::vector<double>> pick_random_centers(std::vector<std::vector<double>> &data, int k) {
    std::vector<std::vector<double>> centers;
    std::unordered_set<int> picked_centers;
    for (int i = 0; i < k; i++) {
        while (true) {
            int index = ((double)(rand()) / RAND_MAX) * data.size();
            if (index == (int)data.size()) {
                index--;
            }
            if (picked_centers.find(index) == picked_centers.end()) {
                picked_centers.insert(index);
                centers.push_back(data[index]);
                break;
            }
        }
    }
    return centers;
}

static inline double center_shift(std::vector<std::vector<double>> &centers, std::vector<std::vector<double>> &new_centers) {
    double sum = 0;
    for (int i = 0; i < (int)centers.size(); i++) {
        sum += pairwise_distance(centers[i], new_centers[i]);
    }
    return sum;
}

static inline int get_closest_center(std::vector<std::vector<double>> &centers, std::vector<double> &vec) {
    int closest_center = 0;
    double min_distance = pairwise_distance(centers[0], vec);
    for (int i = 1; i < (int)centers.size(); i++) {
        double distance = pairwise_distance(centers[i], vec);
        if (distance < min_distance) {
            min_distance = distance;
            closest_center = i;
        }
    }
    return closest_center;
}

#endif
