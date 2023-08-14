#pragma once
#include <stdlib.h>
#include <stddef.h>

void* aligned_malloc(size_t required_bytes, size_t alignment);
void aligned_free(void *p);