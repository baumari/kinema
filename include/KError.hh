#ifndef K_ERROR_HH
#define K_ERROR_HH

namespace KError{
enum Error_Code
  {
   SUCCESS = 0,
   PARTICLE_DATA_NOT_FOUND,
   ZERO_DIVISION,
   INVALID_ARGUMENT,
   NULLPTR_FOUND,
   
   ERR_CODE_SIZE,
  };
}
#endif
