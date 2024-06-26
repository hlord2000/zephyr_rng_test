#include <zephyr/kernel.h>
#include <zephyr/random/random.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

// Ideally some more error checking should take place
int rand_range(uint32_t *num, uint32_t min, uint32_t max) {
	if (min > max) {
		return -EINVAL;
	}

	// This finds the closest power of 2 to our maximum value
	// by continually doubling the value of upper until it is greater than max
	uint32_t upper = 1;
	uint32_t lower = 1;
	while (upper < max) {
		lower = upper;
		upper <<= 1;
	}

	// Upper and lower are then compared. The closest to our max value is chosen
	uint32_t mask;
	if (max - lower < upper - max) {
		mask = lower - 1;
	} else {
		mask = upper - 1;
	}

	// Generate random numbers, mask them, and check if they are within the range
	// If they are, return the number
	while (true) {
		sys_rand_get(num, sizeof(*num));
		*num = *num & mask;
		if (*num >= min && *num <= max) {
			return 0;
		}
	}
}

int main(void)
{
	uint32_t random_number;
	while (true) {
		rand_range(&random_number, 1, 1000);
		LOG_INF("Random number: %u", random_number);
		k_sleep(K_MSEC(1000));
	}
	return 0;
}
