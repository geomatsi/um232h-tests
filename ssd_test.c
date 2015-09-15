/*
 * Example: control seven-segment display using UM232H gpio lines
 *
 * 7-segment display in use: 3ЛС321А (RUS)
 *
 * More about curcuitry
 *
 *
 *       -- SLD           GND --
 *       -- VIO           5V0 --
 *       -- 3V3           USB --
 *       -- PU2           RST --
 *       -- PU1           AC9 --
 *       -- GND           AC8 --
 *       -- AD0           AC7 --
 *       -- AD1   UM232H  AC6 --
 *       -- AD2           AC5 --
 *       -- AD3           AC4 --
 *       -- AD4           AC3 --
 *       -- AD5           AC2 --
 *       -- AD6           AC1 --
 *       -- AD7           AC0 --
 *
 *
 *
 *       -- I01  3ЛС321А  I14 --
 *       -- I02    --     I13 --
 *       -- I03   |  |    I12 --
 *       -- I04    --     I11 --
 *       -- I05   |  |    I10 --
 *       -- I06    --  /  I09 --
 *       -- I07           I08 --
 *
 *
 * Connections:
 *   VIO - 3V3
 *   USB - 5V0
 *   GND - I04
 *   GND - I10
 *   AD4 - I07
 *   AD5 - I06
 *   AD6 - I02
 *   AD7 - I01
 *   AC0 - I14
 *   AC1 - I13
 *   AC2 - I09
 *   AC3 - I08
 *
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
