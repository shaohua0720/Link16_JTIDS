#pragma once
#include "core/utils/tools.h"
#include "core/config/global.h"
#include "protocol/message/STDPMsg.h"

int32_t encoder_Link16(int32_t n, int32_t m, string& inputs);
int32_t decoder_Link16(string& raw_data, int32_t& n, int32_t& m);
