#include "HNSError.h"
#include <stdio.h>
#include <stdbool.h>

err_t error = 0;
const char *error_func_happened = " ";
bool error_printed = false;

void info_error(const char *msg)
{

  printf("*** %s: %s occured error: ", msg, error_func_happened);

#ifdef __linux__
  printf("\033[1;31m"); //bold red
#endif

  switch (error)
  {
  case ERR_CRC:
    printf("Wrong CRC received");
    break;

  case ERR_DATA:
    printf("Size of data above limit"); // of %d", MAX_DATA_SIZE);
    break;

  case ERR_RECIPIENT:
    printf("The packet is not addressed to this device");
    break;

  case ERR_START:
    printf("No starter bytes");
    break;

  case ERR_TIME:
    printf("Timeout");
    break;

  case ERR_ADDRESS_NOT_FOUND_EEPROM:
    printf("No address saved in eeprom");
    break;

  case ERR_ACK_SYNC_FAILED:
    printf("Error during check new address");
    break;

  case ERR_INTERNAL_ERROR:
    printf("Ocuurred an unrecoverable error !!!");
    break;

  case ERR_NO_PACKET:
    printf("No packet available");
    break;

  case ERR_TYPE:
    printf("Received unexpected packet type");
    break;

  case ERR_NO_FREE_ADDRESS:
    printf("No free address available");
    break;

  case ERR_UNKNOWN_MODEL:
    printf("Unknown device model");
    break;

  case ERR_INVALID_ADDRESS:
    printf("Unassigned address");
    break;

  case ERR_DELETED_ADDRESS:
    printf("Device deleted");
    break;

  case ERR_UNEXPECTED_DATA_SIZE:
    printf("Unexpected data size");
    break;

  default:
    printf("UNKNOWN ERROR!!!");
    break;
  }
  printf(" ***\n");

#ifdef __linux__
  printf("\033[0m"); //remove color
#endif

  error_printed = true;
}
