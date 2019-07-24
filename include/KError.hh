#ifndef _KERROR_HH
#define _KERROR_HH

typedef struct {
  enum Error_Code
    { 
     PARTICLE_DATA_NOT_FOUND=1,
     ZERO_DIVISION,
     INVALID_ARGUMENT,
     NULLPTR_FOUND,
     
     ERR_CODE_SIZE,
    };
} KError;
#endif
