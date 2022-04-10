#include <iostream>
#include "utils/parse_args.h"
#include "utils/parse_input.h"
#include "utils/kmeans_helper.h"

#define MAX_THREAD_NUM 100

int main(int argc, char **argv)
{
  ParsedArgs args = parse_args(argc, argv, "A sequential version of K-Means clustering");
  auto data = parse_input(args.input_filename);

  std::vector<std::vector<double>> centers = pick_random_centers(*data, args.k);

  for (auto &center : centers)
  {
    for (auto &point : center)
    {
      std::cout << point << " ";
    }
    std::cout << std::endl;
  }

  delete data;
  return 0;
}
