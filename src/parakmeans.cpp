#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <omp.h>
#include "utils/parse_args.h"
#include "utils/parse_input.h"
#include "utils/random.h"
#include "utils/kmeans_helper.h"


static inline void pick_random_centers(std::vector<std::vector<double>> &global_centers,std::vector<std::vector<double>> &data, int k) {
    std::unordered_set<int> picked_centers;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int size = (int) data.size();
    int start_index = floor((size/(double)thread_count)*my_rank);
    int end_index = floor((size/(double)thread_count)*(my_rank+1));
    for (int i = 0; i < k; i++) {
        while (true) {
            int index = (int)(start_index) + (int) (random_double(0, 1) * (double) (end_index-start_index));
            if (index == (int) (end_index-start_index)) {
                index--;
            }
            if (picked_centers.find(index) == picked_centers.end()) {
                picked_centers.insert(index);
                global_centers.push_back(data[index]);
                break;
            }
        }
    }
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

static inline int
assign_points_to_centers(int k,std::vector<std::vector<double>> &data, std::vector<int> &assignments, std::vector <std::vector<double>> &global_center) {
    int changed = 0;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int size = (int) data.size();
    int start_index = floor((size/(double)thread_count)*my_rank);
    int end_index = floor((size/(double)thread_count)*(my_rank+1));
    for (int i = start_index; i < end_index; i++) {
        int new_assignment = get_closest_center(global_center, data[i]);
        if (new_assignment != assignments[i]) {
            changed++;
            assignments[i] = new_assignment;
        }
    }
    return changed;
}
/*
static inline void
recompute_centers(std::vector <std::vector<double>> &global_centers, std::vector<std::vector<double>> &data,
                  std::vector<int> &assignments) {
    int my_rank = omp_get_thread_num();
    std::vector<std::vector<double>> &centers = global_centers[my_rank];
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
    }\
}
 */
static inline std::vector<double> first_moment(std::vector<std::vector<double>> &data, int start_index, int end_index, int dimension){
    std::vector<double> FM;
    for(int i=0; i<dimension; i++){
        FM.push_back(0.0);
    }

    for(int i=start_index; i<end_index; i++){
        for(int j=0;j<dimension;j++){
            FM[j] += data[i][j];
        }

    }
    return FM;
}
static inline std::vector<double> second_moment(std::vector<std::vector<double>> &data, int start_index, int end_index, int dimension){
    std::vector<double> SM;
    for(int i=0; i<dimension; i++){
        SM.push_back(0.0);
    }
    for(int i=start_index; i<end_index; i++){
        for(int j=0;j<dimension;j++){
            SM[j] += pow(data[i][j],2);
        }

    }
    return SM;
}
static inline void
parallel_recompute(std::vector<std::vector<double>> *global_centers, std::vector<std::vector<double>> &data,
                 std::vector<int> &assignments){
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    std::vector<std::vector<double>> &centers = global_centers[my_rank];
    int size = (int) data.size();
    int start_index = floor((size/(double)thread_count)*my_rank);
    int end_index = floor((size/(double)thread_count)*(my_rank+1));
    std::vector<int> center_counts = std::vector<int>(centers.size(), 0);
    for (auto &center: centers) {
        for (double &i: center) {
            i = 0;
        }
    }
    for (int i = start_index; i < end_index; i++) {
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
static inline std::vector<double> gCC (std::vector<std::vector<double>> &data, int start_index, int end_index, int dimension) {
    int num_of_elements = end_index-start_index+1;
    std::vector<double> FM = first_moment(data, start_index, end_index, dimension);
    std::vector<double> global_center;
    for (int i =0; i< dimension;i++){
        global_center.push_back(0.0);
    }
    for(int i=0; i<dimension;i++){
        global_center[i] = FM[i]/(double)num_of_elements;
    }
    return global_center;
}

static inline std::vector<double> perf (std::vector<std::vector<double>> &data, int start_index, int end_index, int dimension){
    std::vector<double> performance;
    std::vector<double> FM = first_moment(data, start_index, end_index, dimension);
    std::vector<double> SM = second_moment(data, start_index, end_index, dimension);
    int number_of_elements = end_index-start_index+1;
    for(int i=0;i<dimension;i++){
        performance.push_back(0.0);
    }

    for (int i=0; i<dimension;i++){
        performance[i]=SM[i]-pow((FM[i]),2)/(double)number_of_elements;
    }
    return performance;
}
static inline std::vector<double> compute(std::vector <std::vector<double>> &global_centers, std::vector<std::vector<double>> &data,
                           std::vector<int> &assignments, int k, int dimension) {
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int size = (int) data.size();
    int start_index = floor((size/(double)thread_count)*my_rank);
    int end_index = floor((size/(double)thread_count)*(my_rank+1));
    int number = end_index-start_index+1;
    //pick_random_centers(global_centers,data, k);
    std::vector<double> globalCC = gCC(data, start_index, end_index, dimension);

    return globalCC;


}
int main(int argc, char **argv) {
    ParsedArgs args = parse_args(argc, argv, "A basic sequential version of K-Means clustering");
    auto data = parse_input(args.input_filename);
    auto reference_assignments = parse_labels_input(args.labels_filename);
    std::vector<int> assignments(data->size(), -1);
    int thread_count = args.thread_count;
    omp_set_num_threads(thread_count);
    std::vector <std::vector<double>> global_centers;
    int cycle_no = 0;
    double start_time, finish_time;
    int k = args.k;
    int changed =0;
    int dimension = data->at(0).size();
    start_time = omp_get_wtime(); // record start time

    #pragma omp parallel
    {
        auto globalCC = compute(global_centers, *data, assignments, k, dimension);
        #pragma omp critical
        global_centers.push_back(globalCC);
    }

/*
    while (true) {
        cycle_no++;
        changed += assign_points_to_centers(k,*data, assignments, global_center);
        std::cout << "Cycle #" << cycle_no << ": changed = " << changed << std::endl;
        if (changed == 0) {
            break;
        }
        parallel_recompute(global_center, *data, assignments);
    }

    finish_time = omp_get_wtime(); // record finish time

    double nmi = compute_assignments_nmi(assignments, *reference_assignments, args.k);
    std::cout << "NMI: " << nmi << std::endl;

    std::cout << "Algorithm finished in: " << finish_time - start_time << " sec" << std::endl;

    delete data;
    delete reference_assignments;
*/
    return 0;
}