/* grey.h
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include <inttypes.h>

#ifndef grey_h
#define grey_h

#ifndef __cplusplus
extern "C" {
#endif

  typedef struct __attribute__ ((__packed__)) grey_header_s
  {
    char    version[4];
    char    head_rep[4];
    char    data_rep[4];
    int32_t detached;
    int32_t locked;
    int32_t piped;
    int32_t ext_start;
    int32_t ext_size;
    char    format[2];
    int16_t flag_mask;
    float   time_code;
    int16_t inlet;
    int16_t outlets;
    int16_t outmask;
    int16_t pipeloc;
    int16_t pipesize;
    double  n_in;
    double  n_out;
    double  ns_out[8];
    int32_t key_length;
    char    key_words[92];
  } grey_header_t;

  typedef struct __attribute__ ((__packed__)) grey_adjunct_1000_s
  {
    double  xstart;
    double  xdelta;
    int32_t xunits;
    double  pad[236];
  } grey_adjunct_1000_t;

  typedef struct __attribute__ ((__packed__)) grey_file_1000_s
  {
    grey_header_t       header;
    grey_adjunct_1000_t adjunct;
    size_t              N;
    void                *data;
  } grey_file_1000_t;

#ifndef __cplusplus
}
#endif

#endif

/* EOF */
