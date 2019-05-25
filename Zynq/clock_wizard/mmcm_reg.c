#include "mmcm_reg.h"

void set_mmcm_id(struct __mmcm_info *info, uint32_t addr)
{
    info -> addr = addr;
}

void set_mmcm_primitive(struct __mmcm_info *info, bool primitive)
{
    info -> primitive = primitive;
}

void set_mmcm_fin(struct __mmcm_info *info, uint32_t fin)
{
    info -> fin = fin;
}

void set_mmcm_active_outputs(struct __mmcm_info *info, uint8_t active_outputs)
{
    info -> active_outputs = active_outputs;
}

void set_mmcm_features(struct __mmcm_info *info, __mmcm_features features)
{
    info -> features = features;
}