#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

typedef uint8_t colour[3];

static const uint8_t led[3] = {
	PB0,
	PB4,
	PB2
};

static colour current = {0,0,0};
static colour target = {0,0,0};

#define COLOURS 7
static colour cs[COLOURS] = {
	{ 255, 0, 0, },
	{ 0, 255, 0, },
	{ 255, 255, 0, },
	{ 0, 0, 255, },
	{ 255, 0, 255, },
	{ 0, 255, 255, },
	{ 255, 255, 255, },
};

static void retarget(void) {
	uint8_t ci = random()%COLOURS;
	for (int i=0; i<3; i++) {
		target[i] = cs[ci][i];
	}
}

static uint8_t reached(void) {
	for (int i=0; i<3; i++) {
		if (target[i] != current[i]) {
			return 0;
		}
	}
	return 1;
}

static void approach(void) {
	for (int i=0; i<3; i++) {
		if (target[i] < current[i]) {
			current[i]--;
		}
		if (target[i] > current[i]) {
			current[i]++;
		}
	}

}

int main(void) {
	for (int i=0; i<3; i++) {
		set_output(DDRB, led[i]);
	}

	uint8_t tick = 0;
	while(1) {
		for (int i=0; i<3; i++) {
			if (tick < current[i]) {
				output_high(PORTB, led[i]);
			} else {
				output_low(PORTB, led[i]);
			}
		}
		tick = (tick+1)%256;
		if (tick == 0) {
			if (reached()) {
				retarget();
			} else {
				approach();
			}
		}
	}
	return 0;
}
