#ifndef KMEANS_PARSE_ARGS_H
#define KMEANS_PARSE_ARGS_H

#include <string>

#define MAX_THREAD_NUM 100

struct ParsedArgs
{
  int thread_count;
  std::string input_filename;
};

void print_help(std::string executable, std::string description);

ParsedArgs parse_args(int argc, char **argv, std::string description);

#endif
