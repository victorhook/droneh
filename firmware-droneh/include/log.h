#ifndef LOG_H
#define LOG_H

#include "string.h"
#include "stdint.h"


#define LOG_MAX_PARAM_NAME_LENGTH 10
#define LOG_MAX_PARAMS 100

typedef enum {
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    F32,
    F64
} log_type_e;


typedef struct {
    log_type_e type;
    char name[LOG_MAX_PARAM_NAME_LENGTH];
    void* param;
} log_param_t;


class Log {
    public:
        static size_t log_param_id;
        static log_param_t log_params[LOG_MAX_PARAMS];
};


// Macro for adding a log parameter.
#define LOG_ADD_PARAM(_type, _name, _param)                         \
    Log::log_params[Log::log_param_id].type = (log_type_e) _type;         \
    strcpy(Log::log_params[Log::log_param_id].name, (const char*) _name); \
    Log::log_params[Log::log_param_id].param = _param;                    \
    Log::log_param_id++;



#endif /* LOG_H */
