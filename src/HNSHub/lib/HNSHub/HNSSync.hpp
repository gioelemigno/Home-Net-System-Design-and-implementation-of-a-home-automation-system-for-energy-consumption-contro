#ifndef HNS_SYNC_H_
#define HNS_SYNC_H_
#ifdef __cplusplus
extern "C" {
#endif
//---------------FLAGS----------------------------------------------------------
//------------------------------------------------------------------------------

//-------------------INCLUDE and DEFINE for __linux__ --------------------------
#ifdef __linux__
#include "../HNSCore/HNSError.h"
#include "../HNSCore/HNSRouting.h"
#include "../HNSCore/HNSMacro.h"
#endif
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for __linux__-------------------------------
#ifdef __linux__
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for __linux__-------------------------------------
#ifdef __linux__

  // on success return 0
  res_t HNSSync_sync_bak();
  res_t HNSSync_sync(address_t* address);
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
//------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif
#endif /* HNS_SYNC_H_ */
