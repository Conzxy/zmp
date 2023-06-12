#ifndef _CONZXY_ZMP_H__
#define _CONZXY_ZMP_H__

#ifdef __cplusplus
extern "C" {
#endif //!__cplusplus

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct zmp_mem_pool_ {
  unsigned char *start_addr;
  unsigned char *header;
  size_t free_num;
  size_t block_num;
  size_t block_size;
  size_t init_idx;
} zmp_mem_pool_t;

bool zmp_mem_pool_init(zmp_mem_pool_t *mp, size_t block_size, size_t block_num);

void *zmp_mem_pool_alloc(zmp_mem_pool_t *mp);

void zmp_mem_pool_free(zmp_mem_pool_t *mp, void *p_block);

void zmp_mem_pool_destroy(zmp_mem_pool_t *mp);

#ifdef __cplusplus
}
#endif //!__cplusplus

#endif //!_ZMP_H__