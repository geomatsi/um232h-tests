#ifndef __UM232H_H__
#define __UM232H_H__

#include <inttypes.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <ftdi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* */

#define BIT_SK 0x01 /* AD0 pin */
#define BIT_DO 0x02 /* AD1 pin */
#define BIT_DI 0x04 /* AD2 pin */
#define BIT_CS 0x08 /* AD3 pin */
#define BIT_G0 0x10 /* AD4 pin */
#define BIT_G1 0x20 /* AD5 pin */
#define BIT_G2 0x40 /* AD6 pin */
#define BIT_G3 0x80 /* AD7 pin */

#define BITS_GPO (BIT_G0|BIT_G1|BIT_G2|BIT_G3)
#define BITS_OUT (BIT_SK|BIT_DO|BIT_CS|BIT_G0|BIT_G1|BIT_G2|BIT_G3)

#define CLOCK_MAX_SPEED 6000000

/* */

#define FTDI_CHECK(FUN, MSG, CTX) do { \
		if ((FUN) < 0) \
		{ \
			fprintf(stderr, \
				"%s: %s\n", \
				MSG, \
				ftdi_get_error_string(CTX)); \
			assert(0); \
		} \
	} while (0)


/* */

void um232h_gpiol_set(struct ftdi_context *fc, int bit, int level);
uint8_t um232h_spi_byte_xfer(struct ftdi_context *fc, uint8_t data);
void um232h_mpsse_simple_init(struct ftdi_context *fc);
void um232h_set_loopback(struct ftdi_context *fc, int loopback);
void um232h_set_speed(struct ftdi_context *fc, uint32_t speed);
void um232h_simple_close(struct ftdi_context *fc);

#ifdef __cplusplus
}
#endif

#endif /* __UM232H_H__ */
