#include "zmp.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void zmp_perror(char const *msg)
{
  fputs(msg, stderr);
  fflush(stderr);
  abort();
}

bool zmp_mem_pool_init(zmp_mem_pool_t *mp, size_t block_size, size_t block_num)
{
  assert(mp);

  mp->start_addr = malloc(block_size * block_num);
  mp->header = mp->start_addr;
  mp->free_num = block_num;
  mp->block_num = block_num;
  mp->block_size = block_size;
  mp->init_idx = 0;
  if (!mp->start_addr) {
    return false;
  }
  return true;
}

void zmp_mem_pool_destroy(zmp_mem_pool_t *mp)
{
  if (mp->free_num != mp->block_num) {
    zmp_perror("zmp_mem_pool_destroy: There are some blocks are not freed");
  }
  free(mp->start_addr);
}

void *zmp_mem_pool_alloc(zmp_mem_pool_t *mp)
{
  if (mp->init_idx < mp->block_num) {
    size_t *p_curr_addr =
        (size_t *)(mp->start_addr + mp->init_idx * mp->block_size);
    *p_curr_addr = (size_t)(p_curr_addr) + mp->block_size;
    ++mp->init_idx;
  }

  if (mp->free_num > 0) {
    size_t *p_next_addr = (size_t *)mp->header;
    void *ret = mp->header;

    // Pop the header and set the next free block as the new header
    mp->free_num--;
    mp->header = (mp->free_num == 0) ? (unsigned char *)NULL
                                     : (unsigned char *)(*p_next_addr);
    return ret;
  }

  return NULL;
}

void zmp_mem_pool_free(zmp_mem_pool_t *mp, void *p_block)
{
  if (p_block < mp->start_addr ||
      p_block > mp->start_addr + mp->block_size * (mp->block_num - 1))
  {
    zmp_perror("zmp_mem_pool_free(): Invalid pointer");
  }

  size_t *p_next_addr = (size_t *)p_block;
  if (!mp->header) {
    // There is no avaliable free block
    *p_next_addr = (size_t)(mp->start_addr + mp->block_num * mp->block_size);
  } else {
    *p_next_addr = *(size_t *)mp->header;
  }
  mp->header = (unsigned char *)p_next_addr;
  ++mp->free_num;
}