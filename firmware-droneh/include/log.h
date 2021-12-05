#ifndef LOG_H
#define LOG_H

#include "string.h"
#include "stdint.h"


#define LOG_MAX_PARAM_NAME_LENGTH 25
#define LOG_MAX_PARAMS 100

typedef uint8_t log_param_id_t;

typedef enum {
    U8  = 'B',  // Unsigned Byte
    I8  = 'b',  // Byte
    U16 = 'H',  // Unsigned Integer 16-bit
    I16 = 'h',  // Integer 16-bit
    U32 = 'I',  // Unsigned Integer 32-bit
    I32 = 'i',  // Integer 32-bit
    F32 = 'f',  // Float 32-bit
    F64 = 'd'   // Double 64-bit
} log_type_e;

typedef struct {
    log_param_id_t id;
    log_type_e     type;
    void*          ptr;
    uint8_t        name_length;
    char           name[LOG_MAX_PARAM_NAME_LENGTH];
} log_param_t;


class Log {
    public:
        static size_t log_param_id;
        static log_param_t log_params[LOG_MAX_PARAMS];
        static const size_t availableLogParams();
        static const size_t get_size(log_type_e type);
};


// Macro for adding a log parameter.
#define LOG_ADD_PARAM(_type, _name, _param_address)                             \
    Log::log_params[Log::log_param_id].id = (log_param_id_t) Log::log_param_id; \
    Log::log_params[Log::log_param_id].type = (log_type_e) _type;               \
    Log::log_params[Log::log_param_id].ptr = _param_address;                    \
    Log::log_params[Log::log_param_id].name_length = strlen(_name);             \
    strcpy(Log::log_params[Log::log_param_id].name, (const char*) _name);       \
    Log::log_param_id++;



#endif /* LOG_H */
