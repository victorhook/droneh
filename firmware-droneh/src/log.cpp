#include "log.h"

int asd;

void init() {
    //uint8_t type = 0;
    //char* name =  "";
    //int param = 2;

}

size_t Log::log_param_id = 0;

log_param_t Log::log_params[LOG_MAX_PARAMS];


const size_t Log::availableLogParams()
{
    return Log::log_param_id;
}

const size_t Log::get_size(log_type_e type)
{
    switch (type) {
        case U8:
        case I8: return 1;
        case U16:
        case I16: return 2;
        case U32:
        case I32:
        case F32: return 4;
        case F64: return 8;
    }
    return -1;
}