#include <unistd.h>

#include <um232h.h>

int main(int argc, char **argv)
{
	struct ftdi_context fc;
	int i;

    um232h_mpsse_simple_init(&fc);
    um232h_set_loopback(&fc, 0);
    um232h_set_speed(&fc, 1000000);

	for (i = 0; i < 3; i++)
    {
        um232h_gpiol_set(&fc, BIT_G2, 1);
        um232h_gpiol_set(&fc, BIT_G3, 0);
		sleep(1);
        um232h_gpiol_set(&fc, BIT_G2, 0);
        um232h_gpiol_set(&fc, BIT_G3, 1);
        sleep(1);
	}

    um232h_gpiol_set(&fc, BIT_G2, 0);
    um232h_gpiol_set(&fc, BIT_G3, 0);

    um232h_simple_close(&fc);

	return 0;
}
