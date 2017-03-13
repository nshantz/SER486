#include <stdint.h>
#include <stdbool.h>
//This is to wrap the C++ functions from the library into C code

#ifdef __cplusplus
	extern "C" {
#endif

void* call_CAP1188_I2C(int8_t i);
uint8_t call_CAP1188_touched(void* p);
bool call_CAP1188_begin(void* p, uint8_t i);

#ifdef __cplusplus
	}
#endif
