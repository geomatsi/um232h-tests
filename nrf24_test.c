#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <um232h.h>

#include <nRF24L01.h>
#include <RF24.h>

/* */

struct ftdi_context fc;

/* */

dump_data(char *b, int n)
{
    int p;

    for(p = 0; p < n; p++) {
        printf("0x%02x ", *(b + p));
        if ((p > 0) && ((p % 64) == 0))
            printf("\n");
    }

    printf("\n");
}

/* */

void f_csn(int level)
{
    um232h_gpiol_set(&fc, BIT_L2, level);
    return;
}

void f_ce(int level)
{
    um232h_gpiol_set(&fc, BIT_L1, level);
    return;
}

void f_spi_set_speed(int khz)
{
	/* not implemented */
}

uint8_t f_spi_xfer(uint8_t data)
{
	return um232h_spi_byte_xfer(&fc, data);
}

struct rf24 nrf = {
	.csn = f_csn,
	.ce = f_ce,
	.spi_set_speed = f_spi_set_speed,
	.spi_xfer = f_spi_xfer,
};

/* */

int main(int argc, char **argv)
{
	uint8_t addr0[] = {'E', 'F', 'C', 'L', 'I'};
	uint8_t addr1[] = {'E', 'F', 'S', 'N', '1'};

	struct rf24 *pnrf;
	uint32_t more_data;
	uint8_t pipe;

	uint8_t recv_buffer[32];
	bool dynamic_payload = true;
	int recv_length = 32;

	/* init FTDI chip */

    um232h_mpsse_simple_init(&fc);
    um232h_set_loopback(&fc, 0);
    um232h_set_speed(&fc, 1000000);

	/* init nRF24 module */

	rf24_init(&nrf);
	pnrf = &nrf;

	rf24_print_status(rf24_get_status(pnrf));

	if (dynamic_payload)
	    rf24_enable_dynamic_payloads(pnrf);
    else
	    rf24_set_payload_size(pnrf, recv_length);

	rf24_open_reading_pipe(pnrf, 0x0 /* pipe number */, addr0);
	rf24_open_reading_pipe(pnrf, 0x1 /* pipe number */, addr1);

	rf24_start_listening(pnrf);
	rf24_print_status(rf24_get_status(pnrf));

	printf("Data Rate\t = %d\n",rf24_get_data_rate(pnrf));
	printf("Model\t\t = %d\n", rf24_is_p_variant(pnrf));
	printf("CRC Length\t = %d\n", rf24_get_crc_length(pnrf));
	printf("PA Power\t = %d\n", rf24_get_pa_level(pnrf));

	rf24_print_byte_register(pnrf, "RF_CH", RF_CH, 1);
	rf24_print_byte_register(pnrf, "RF_SETUP", RF_SETUP, 1);
	rf24_print_byte_register(pnrf, "CONFIG", CONFIG, 1);

	while (1) {

		if (rf24_available(pnrf, &pipe)) {

			if ((pipe < 0) || (pipe > 5)) {
				printf("WARN: invalid pipe number 0x%02x\n", (int) pipe);
			} else {
				printf("INFO: data ready in pipe 0x%02x\n", pipe);
				memset(recv_buffer, 0x0, sizeof(recv_buffer));

                if (dynamic_payload)
				    recv_length = rf24_get_dynamic_payload_size(pnrf);

				more_data = rf24_read(pnrf, recv_buffer, recv_length);
				printf("INFO: dump received %d bytes\n", recv_length);

				dump_data((char *)recv_buffer, recv_length);

				if (!more_data)
					printf("WARN: RX_FIFO not empty: %d\n", more_data);
			}
		}

		usleep(10000);
	}

	/* */

    um232h_simple_close(&fc);

	return 0;
}
