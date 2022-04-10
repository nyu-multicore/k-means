#include <iostream>
#include "utils/parse_args.h"
#include "utils/parse_input.h"

#define MAX_THREAD_NUM 100

int main(int argc, char **argv)
{
  ParsedArgs args = parse_args(argc, argv, "A sequential version of K-Means clustering");
  auto data = parse_input(args.input_filename);
  std::cout << data->size() << std::endl;
  int i = 0;
  for (auto &vec : *data)
  {
    i++;
    if (i > 10)
      break;
    for (auto &val : vec)
    {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }
  delete data;
  return 0;
}
