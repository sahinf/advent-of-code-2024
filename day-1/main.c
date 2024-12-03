// https://adventofcode.com/2024/day/1
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static inline int compare_ascending(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

static int padding = 25;

void part1(size_t size, int *list1, int *list2);
void part2(size_t size, int *list1, int *list2);

int main() {

	// Read input
	const char *file_name = "input.txt";
	FILE *file = fopen(file_name, "r");
	if (!file)  {
		perror("Error opening file"); return 1;
	}

	char line[1 << 4];
	int list1[1 << 10];
	int list2[1 << 10];
	size_t size;

	// Read LHS into list1 and RHS into list2
	for (size = 0; fgets(line, sizeof(line), file) != NULL; size++) { 
		if (ferror(file)) {
			perror("Error reading from file");
			fclose(file); return 1;
		}

		if (!(sscanf(line, "%d %d", &list1[size], &list2[size]) == 2)) {
			fprintf(stderr, "Could not parse two integers from line %s: %s", line, strerror(errno));
			return 1;
		}
	}
	fclose(file);

	// sorting required for O(nlogn) vs O(n^2)
	qsort(list1, size, sizeof(int), compare_ascending);
	qsort(list2, size, sizeof(int), compare_ascending);

	part1(size, list1, list2);
	part2(size, list1, list2);
	return 0;
}

void part1(size_t size, int *list1, int *list2) {
	size_t diffs = 0;
	for (int i=0; i < size; i++) {
		/*printf("%d - %d = %d\n", list1[i], list2[i], abs(list1[i] - list2[i]));*/
		diffs += abs(list1[i] - list2[i]);
	}

	printf("%-*s = %lu\n", padding,"Total diffs", diffs);
}

void part2(size_t size, int *list1, int *list2){
	unsigned long int similarity_i, similarity_total, i, j;
	similarity_i = similarity_total = i = j = 0;

	for (; i < size; i++, similarity_i = 0) {
		// catch up RHS to LHS
		while (j < size && list2[j] < list1[i])
			j++;

		// count similarity of LHS[i]
		for (; j < size && list2[j] == list1[i]; j++) 
			similarity_i++;

		similarity_total += similarity_i * list1[i];
	}
	printf("%-*s = %lu\n", padding, "Similarity score O(nlogn)", similarity_total);


	// O(n^2) solution
	i = j = similarity_i = similarity_total = 0;
	for (; i < size; i++, similarity_i = 0) {
		for (j = 0; j < size; j++) {
			if (list1[i] == list2[j]) {
				similarity_i += 1;
			}
		}
		similarity_total += similarity_i * list1[i];
	}
	printf("%-*s = %lu\n", padding, "Similarity score O(n^2)", similarity_total);

}
