#include <gmp.h>
#include <stdlib.h>
#include <stdio.h> 

void next_seed(mpz_t res, mpz_t seed, mpz_t power, mpz_t mod) {
	mpz_powm(res, seed, power, mod);
}
int main() {
	int magicBytes[] = {0x52, 0x49, 0x46, 0x46};
	int data[] = {0x46, 0x83, 0x49, 0x44};

	mpz_t pow, seed2, res, N, offset, powered_offset, temp, N_minus_one, seed3, euler_totient_n;
	mpz_init(pow);
	mpz_init(res);
	mpz_init(temp);
	mpz_init(euler_totient_n);
	mpz_init(seed2);
	mpz_init(seed3);
	mpz_init(powered_offset);
	mpz_init_set_str(N, "73882271767225067628282422302548400069735434660808564149855701357643617388546710750167259617348302921921308565033188464703728096338599435641707966042336489111486646451993143053931136428343207443971020988133537590763029595507074481482058514976650388077801669364711731111970923913958083815347282871017052613916475670517", 10);

	mpz_set_str(euler_totient_n, "73518206147262443676837361159127776505334808540008666455558114391523228702493449359589995596913129043911224919282022228135566170964663692657377830877081156633758852294230067854975206472518946297562604445162853306974953289714803969929459936619828333107726452353102277743999390152590440267776000000000000000000000000000", 10);

	mpz_init_set_ui(offset, (unsigned long) 31337);

	for(unsigned long power = 2; power <= 16; ++power){
		mpz_set_ui(pow, power);
		mpz_powm(powered_offset, pow, offset, euler_totient_n);
		for(unsigned long seed = 2 ; seed <= (1 << 16)+2; ++seed) {
			mpz_set_ui(seed2, seed);

			next_seed(temp, seed2, powered_offset, N);
			mpz_set(seed2, temp);

			for(int i = 0; i < 4; ++i) {
				next_seed(temp, seed2, pow, N);
				mpz_set(seed2, temp);
				int d = mpz_get_ui(seed2) & 255;
				if(d != (magicBytes[i] ^ data[i])) {
					goto continue_power_loop;
				}
			}
			printf("Found %lu %lu.\n", seed, power);
continue_power_loop:;
			if(seed % 10000 == 0){
				printf("Seed %lu, power %lu finished.\n", seed, power);
			}
		}
	}
}	

