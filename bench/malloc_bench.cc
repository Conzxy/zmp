#include "zmp/zmp.h"

#include <benchmark/benchmark.h>
#include <cstdio>

#include <vector>

using namespace benchmark;

#define prepare_block_parameter                                                \
  int block_num = state.range(0);                                              \
  int block_size = state.range(1)

static void BM_malloc(State &state)
{
  prepare_block_parameter;
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<void *> ptrs(block_num);
    state.ResumeTiming();
    for (int i = 0; i < block_num; ++i) {
      auto p = malloc(block_size);
      ptrs[i] = p;
    }

    for (int i = 0; i < block_num; ++i)
      free(ptrs[i]);
  }
}

static void BM_zmp_malloc(State &state)
{
  prepare_block_parameter;

  for (auto _ : state) {
    zmp_mem_pool_t zmp;
    zmp_mem_pool_init(&zmp, block_size, block_num);

    state.PauseTiming();
    std::vector<void *> ptrs(block_num);
    state.ResumeTiming();

    for (int i = 0; i < block_num; ++i) {
      auto p = zmp_mem_pool_alloc(&zmp);
      if (!p) {
        abort();
      }
      ptrs[i] = p;
    }

    for (int i = 0; i < block_num; ++i)
      zmp_mem_pool_free(&zmp, ptrs[i]);

    zmp_mem_pool_destroy(&zmp);
  }
}

#define register_benchmark_case(_func, _name)                                  \
  BENCHMARK(_func)                                                             \
      ->Ranges({{100, 1000}, {1000, 100000}})                                  \
      ->RangeMultiplier(4)                                                     \
      ->Name(_name)

register_benchmark_case(BM_zmp_malloc, "zmp malloc");
register_benchmark_case(BM_malloc, "libc malloc");