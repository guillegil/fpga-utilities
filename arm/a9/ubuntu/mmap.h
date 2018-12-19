#ifndef __MMAP_H__
#define __MMAP_H__

#include <sys/mman.h>
#include <stdint.h>

#define MEM_DIR "/dev/mem"

struct
{
    int fd;
    uint32_t *cfg;
} mmconf;

typedef struct mmconf MemConfig;


inline int8_t map_start(MemConfig *mmcfg);
inline void map_stop(MemConfig *mmcfg);

inline uint32_t read(uint32_t *cfg, uint32_t dir_offset);
inline void write(uint32_t *cfg, uint32_t dir_offset, uint32_t data);


#endif //__MMAP_H__