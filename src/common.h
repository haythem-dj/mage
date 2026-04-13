#ifndef COMMON_H_
#define COMMON_H_

#define ERROR(fmt, ...) fprintf(stderr, "[ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)

#endif // COMMON_H_
