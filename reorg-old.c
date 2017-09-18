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

createBuckets(Bucket buckets[]){
	Person* person;
	unsigned int bucket_offset;

	person = malloc(sizeof(Person));
	for(person_offset = 0; person_offset <  person_length/sizeof(Person); person_offset++){
		person = &person_map[person_offset];
		found_location = 0;

		//Search for the right bucket to put the person in
		for(bucket_offset = 0; bucket_offset < number_of_buckets; bucket_offset++){
			if(buckets[bucket_offset].location_id == person->location){
				printf("%u\n",buckets[bucket_offset].number_of_items);
				buckets[bucket_offset].ids[buckets[bucket_offset].number_of_items] = person->person_id;
				buckets[bucket_offset].number_of_items++;
				found_location = 1;
				break;
			}
		}

		// If not yet in structure
		if(found_location == 0){
			Bucket new_bucket;
			new_bucket.number_of_items = 0;
			new_bucket.location_id = person->location;
			new_bucket.ids[0] = person->person_id;
			new_bucket.number_of_items++;
			buckets[number_of_buckets] = new_bucket;
			number_of_buckets++;
		}
	}
	//free(person);
}

int main(int argc, char *argv[]) {
	clock_t begin = clock();
	// Create output files
	FILE *knows_new_out;
	FILE *person_new_out;

	knows_new_out = fopen("knows_new.bin", "w+");
	person_new_out = fopen("person_new.bin", "w+");

	number_of_buckets = 0;
	person_map = (Person *) mmapr(makepath(argv[1], "person",   "bin"), &person_length);
	knows_map = (unsigned int *) mmapr(makepath(argv[1], "knows",    "bin"), &knows_length);

	unsigned int maxNumberOfBuckets = person_length/sizeof(Person);

	Bucket* buckets = malloc((maxNumberOfBuckets * sizeof(Bucket)) + (1000000 * sizeof(unsigned long)));

	createBuckets(buckets);

	unsigned int *deletedKnows, *tempDeletedKnows;
	deletedKnows = (int *)malloc(sizeof(int));
	tempDeletedKnows = (int *)malloc(sizeof(int));
	*deletedKnows = 0;

	Person* person;
	person = malloc(sizeof(Person));
	// Loop through persons
	/*for(person_offset = 0; person_offset < maxNumberOfBuckets; person_offset++){
		*tempDeletedKnows = 0;
		person = &person_map[person_offset];

		Bucket* person_bucket;
		long y;
		short found = 0;
		unsigned int bucket_offset;

		for(bucket_offset = 0; bucket_offset < number_of_buckets; bucket_offset++){
			for(y = 0; y < buckets[bucket_offset].number_of_items; y++){
				if(person->person_id == buckets[bucket_offset].ids[y]){
					person_bucket = &buckets[bucket_offset];
					found = 1;
					break;
				}
			}
			if(found == 1){
				break;
			}
		}

		int x = person->knows_first + person->knows_n;
		// Loop through knows
		for(knows_offset = person->knows_first; knows_offset < x; knows_offset++){
			Person* knows = malloc(sizeof(Person));
			knows = &person_map[knows_map[knows_offset]];
			found = 0;
			for(y = 0; y < person_bucket->number_of_items; y++){
				printf("%u\n", person_bucket->ids[y]);
				if(person_bucket->ids[y] == knows->person_id){
					found = 1;
					break;
				}
			}

			if(found == 1){
				fwrite(&knows_offset,  1, sizeof(unsigned int), knows_new_out);
			} else {
				*deletedKnows++;
				*tempDeletedKnows++;
			}
		}

		Person new_person = createPerson(person->person_id, person->birthday, person->location, person->knows_first - *deletedKnows, person->knows_n - *tempDeletedKnows, person->interests_first, person->interest_n);
		fwrite(&new_person, 1, sizeof(Person), person_new_out);
	}

	fclose(knows_new_out);
	fclose(person_new_out);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f",time_spent);*/
	return 0;
}
