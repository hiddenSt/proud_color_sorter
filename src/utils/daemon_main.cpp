#include <utils/daemon_main.hpp>

#include <csignal>
#include <cstdlib>
#include <thread>
#include <mutex>

#include <CLI/App.hpp>

namespace proud_color_sorter::utils {

int DaemonMain(int argc, char* argv[]) {

  
  CLI::App app{"App for sorting random generated colors."};
  
  std::size_t num_threads = 1;
  app.add_option("--num-threads", num_threads, "Number of threads to ");
  // 
  // while (thread_pool)
  return EXIT_FAILURE;
}

}  // namespace proud_color_sorter
