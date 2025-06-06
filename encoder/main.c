#include <stdio.h>
#include <stdint.h>

int main() {
	int c = 0;
  uint64_t i = 0;
  uint64_t v = 0;
	while((c = getchar()) != EOF) {
    v |= ((uint64_t)c) << (8 * i);
    i++;
    if (i == 8) {
        printf("\t c64 %ld\n", v);
        i = 0;
        v = 0;
    }
	}

  printf("\t c64 %ld\n", v);
}
