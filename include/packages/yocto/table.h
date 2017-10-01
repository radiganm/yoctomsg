/* table.h
 */

#include "../common/status.h"

#ifndef yo_table_h
#define yo_table_h

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct __attribute__((packed)) Yo_Table_s
  {
    int              size;
    int              capacity;
    pthread_mutex_t  lock;
  } Yo_Table_t;

  status_t yo_table_open(Yo_Table_t *table);

  status_t yo_table_close(Yo_Table_t *table);

#ifdef __cplusplus
}
#endif

#endif

/* *EOF* */
