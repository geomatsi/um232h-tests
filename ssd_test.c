/*
 * Example: control seven-segment display using UM232H gpio lines
 */

#include <unistd.h>

#include <um232h.h>

uint8_t lled[] = { BIT_L0, BIT_L1, BIT_L2, BIT_L3 };
uint8_t hled[] = { BIT_H0, BIT_H1, BIT_H2, BIT_H3 };

uint8_t num[10][8] = {
    {1, 1, 0, 1, 1, 1, 0, 1},
    {0, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 0, 0},
    {1, 0, 1, 0, 1, 1, 0, 1},
    {0, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {0, 0, 0, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1},
};

void clear(struct ftdi_context *ftdi)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        um232h_gpiol_set(ftdi, lled[i], 0);
        um232h_gpioh_set(ftdi, hled[i], 0);
    }
}

void set(struct ftdi_context *ftdi, uint8_t *dat)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        um232h_gpiol_set(ftdi, lled[i], dat[i]);
        um232h_gpioh_set(ftdi, hled[i], dat[i+4]);
    }
}

int main(int argc, char **argv)
{
	struct ftdi_context fc;
	int i, n;

    um232h_mpsse_simple_init(&fc);
    um232h_set_loopback(&fc, 0);
    um232h_set_speed(&fc, 1000000);

    for(n = 0; n < 10; n++)
    {
        for(i = 0; i < 10; i++)
        {
            clear(&fc);
            set(&fc, num[i]);
            usleep(500000);
        }
    }

    clear(&fc);

    um232h_simple_close(&fc);

	return 0;
}
