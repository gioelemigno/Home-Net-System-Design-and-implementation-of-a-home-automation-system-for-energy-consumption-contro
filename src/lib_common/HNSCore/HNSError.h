#ifndef HNS_ERROR_H_
#define HNS_ERROR_H_
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
//  ERROR FLAGS to set error variable    :
#define CORRECT                               0

#define ERR_DATA                            254
#define ERR_RESOURCE_UNAVAILABLE            253
#define ERR_CRC                             252
#define ERR_RESPONSIVE                      251
#define ERR_RECIPIENT                       250
#define ERR_TIME                            249
#define ERR_SET                             248

#define ERR_READ                            247
#define ERR_WRITE                           246

#define ERR_START                           245
#define ERR_ADDRESS_NOT_FOUND_EEPROM        244
#define ERR_INTERNAL_ERROR                  243


#define ERR_NO_PACKET                       242

#define ERR_ACK_SYNC_FAILED                 241
#define ERR_TYPE                            240
#define ERR_NO_FREE_ADDRESS                 239
#define ERR_UNKNOWN_MODEL                   238
#define ERR_INVALID_ADDRESS                 237
#define ERR_DELETED_ADDRESS                 236

#define ERR_UNEXPECTED_DATA_SIZE            235
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

#define LEN_STRING_ERROR_FUNC_HAPPENED    64

typedef int8_t res_t;
typedef uint8_t err_t;



extern err_t error;
//extern char* error_func_happened[LEN_STRING_ERROR_FUNC_HAPPENED];
extern const char* error_func_happened;
extern bool error_printed;

extern void info_error(const char* msg);  //like to perror()

//use info_error, msg = name_function;
#define HNSError_DEBUG() \
   info_error(__func__)

//extern void debug(const char* name_function_called);

#ifdef __cplusplus
}
#endif

#endif /*HNS_ERROR_H_*/
