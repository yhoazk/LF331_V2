#ifndef _COMMON_H_
#define _COMMON_H_

#define QUEUE_NAME "/test_queue"
#define MAX_SIZE (1024)
#define MSG_STOP "exit"

#define CHECK(x) \
  do { \
    if (!(x)) { \
      fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
      perror(#x); \
      exit(-1); \
    } \
  }while(0)\
// need to leave this empty line here or the #endif gets concatenated adn 
// theres an error about # not followed by a macro parameter
#endif // _COMMON_H_
