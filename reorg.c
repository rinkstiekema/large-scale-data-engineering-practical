#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>

#include "utils.h"

#define ARRAY_SIZE( array ) ( sizeof( array ) / sizeof( array[0] )

typedef struct {
	unsigned int number_of_items;
	unsigned short location_id;
	unsigned long ids[];
} Bucket;

Person *person_map;
unsigned int *knows_map;
unsigned long person_length;
unsigned long knows_length;
unsigned short number_of_buckets;
short found_person;
short found_location;
unsigned int person_offset;
unsigned int knows_offset;

Person createPerson( 	unsigned long  person_id,	unsigned short birthday, unsigned short location, unsigned long  knows_first, unsigned short knows_n, unsigned long  interests_first,	unsigned short interest_n) {
	Person p;

	p.person_id = person_id;
	p.birthday = birthday;
	p.location = location;
	p.knows_first = knows_first;
	p.knows_n = knows_n;
	p.interests_first = interests_first;
	p.interest_n = interest_n;

	return p;
}

int main(int argc, char *argv[]) {
	clock_t begin = clock();
	// Create output files
	FILE *knows_new_out;
	FILE *person_new_out;

	knows_new_out = fopen("knows_new.bin", "w+");
	person_new_out = fopen("person_new.bin", "w+");

	person_map = (Person *) mmapr(makepath(argv[1], "person",   "bin"), &person_length);
	knows_map = (unsigned int *) mmapr(makepath(argv[1], "knows",    "bin"), &knows_length);

	int i,j,deletedTotal;
	deletedTotal = 0;
	for(i = 0; i < person_length/sizeof(Person); i++){
		printf("Starts on new person %d out of %d\n", i, person_length/sizeof(Person));
		Person* person = &person_map[i];
		int x = person->knows_first + person->knows_n;
		int deleted = 0;
		for(j = 0; j < x; j++) {
			Person* knows = &person_map[knows_map[j]];

			if(knows->location == person->location){
				fwrite(&j,  1, sizeof(unsigned int), knows_new_out);
				deleted++;
				deletedTotal++;
			}
		}

		Person new_person = createPerson(person->person_id, person->birthday, person->location, person->knows_first - deletedTotal, person->knows_n - deleted, person->interests_first, person->interest_n);
		fwrite(&new_person, 1, sizeof(Person), person_new_out);
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f",time_spent);
	return 0;
}
