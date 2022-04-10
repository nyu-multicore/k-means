#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <omp.h>
#include "utils/parse_args.h"
#include "utils/parse_input.h"
#include "utils/random.h"

#define MAX_THREAD_NUM 100

static inline double pairwise_distance(std::vector<double> &v1, std::vector<double> &v2) {
    double sum = 0;
    for (int i = 0; i < (int) v1.size(); i++) {
        sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(sum);
}

static inline std::vector<std::vector<double>> pick_random_centers(std::vector<std::vector<double>> &data, int k) {
    std::vector<std::vector<double>> centers;
    std::unordered_set<int> picked_centers;
    for (int i = 0; i < k; i++) {
        while (true) {
            int index = (int) (random_double(0, 1) * (double) data.size());
            if (index == (int) data.size()) {
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

static inline int get_closest_center(std::vector<std::vector<double>> &centers, std::vector<double> &vec) {
    int closest_center = 0;
    double min_distance = pairwise_distance(centers[0], vec);
    for (int i = 1; i < (int) centers.size(); i++) {
        double distance = pairwise_distance(centers[i], vec);
        if (distance < min_distance) {
            min_distance = distance;
            closest_center = i;
        }
    }
    return closest_center;
}

static inline bool
assign_points_to_centers(std::vector<std::vector<double>> &centers, std::vector<std::vector<double>> &data,
                         std::vector<int> &assignments) {
    bool changed = false;
    for (int i = 0; i < (int) data.size(); i++) {
        int new_assignment = get_closest_center(centers, data[i]);
        if (new_assignment != assignments[i]) {
            changed = true;
            assignments[i] = new_assignment;
        }
    }
    return changed;
}

static inline void
recompute_centers(std::vector<std::vector<double>> &centers, std::vector<std::vector<double>> &data,
                  std::vector<int> &assignments) {
    std::vector<int> center_counts = std::vector<int>(centers.size(), 0);
    for (auto &center: centers) {
        for (double &i: center) {
            i = 0;
        }
    }
    for (int i = 0; i < (int) data.size(); i++) {
        center_counts[assignments[i]]++;
        for (int j = 0; j < (int) centers[assignments[i]].size(); j++) {
            centers[assignments[i]][j] += data[i][j];
        }
    }
    for (int i = 0; i < (int) centers.size(); i++) {
        for (int j = 0; j < (int) centers[i].size(); j++) {
            centers[i][j] /= center_counts[i];
        }
    }
}

int main(int argc, char **argv) {
    ParsedArgs args = parse_args(argc, argv, "A sequential version of K-Means clustering");
    auto data = parse_input(args.input_filename);

    int cycle_no = 0;

    double start_time, finish_time;
    start_time = omp_get_wtime(); // record start time

    std::vector<std::vector<double>> centers = pick_random_centers(*data, args.k);
    std::vector<int> assignments(data->size(), -1);

    while (true) {
        cycle_no++;
        bool changed = assign_points_to_centers(centers, *data, assignments);
        std::cout << "Cycle #" << cycle_no << ": changed = " << changed << std::endl;
        if (!changed) {
            break;
        }
        recompute_centers(centers, *data, assignments);
    }

    finish_time = omp_get_wtime(); // record finish time
    std::cout << "Algorithm finished in: " << finish_time - start_time << " sec" << std::endl;

    delete data;
    return 0;
}
