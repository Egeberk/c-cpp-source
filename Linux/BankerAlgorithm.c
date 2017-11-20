// BANKER'S ALGORITHM
// Program requires an input .txt file of # of system/resources map.
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_PROCESSES 5
#define NUMBER_OF_RESOURCES 3
typedef struct RESOURCE
{
	int allocation;
	int max;
	int need;
}RESOURCE;
typedef struct PROCESS
{
	int process_id;
	RESOURCE resources[NUMBER_OF_RESOURCES];
	int done; // 0=Undone 1=Done
}PROCESS;
int AVAILABLE[NUMBER_OF_RESOURCES];

// INIT PROCESSES
void read_system(PROCESS* processes, char *FILE_NAME)
{
	FILE *f = fopen(FILE_NAME, "r");
	int i, j ,k=0,l=0;
	// Allocation & Max Matrix
	for (i = 0; i < NUMBER_OF_PROCESSES * 2; i++)
	{
		if (i < NUMBER_OF_PROCESSES){
			processes[k].process_id = k;
			processes[k].done = 0;
			for (j = 0; j < NUMBER_OF_RESOURCES; j++)
				fscanf(f, "%d", &processes[k].resources[j].allocation);
			k++;
		}
		else{
			processes[l].process_id = l;
			processes[l].done = 0;
			for (j = 0; j < NUMBER_OF_RESOURCES; j++)
				fscanf(f, "%d", &processes[l].resources[j].max);
			l++;
		}
	}

	// Available Vector
	for (i = 0; i < NUMBER_OF_RESOURCES; i++)
		fscanf(f, "%d", &AVAILABLE[i]);

	// Need Matrix
	for (i = 0; i < NUMBER_OF_PROCESSES; i++)
		for (j = 0; j < NUMBER_OF_RESOURCES; j++)
			processes[i].resources[j].need = processes[i].resources[j].max - processes[i].resources[j].allocation;
	fclose(f);
}

// CHECKS IF THERE ANY PROCESSES LEFT
int all_done(PROCESS *processes)
{
	int i;
	for (i = 0; i < NUMBER_OF_PROCESSES; i++)
	if (!processes[i].done)
		return 0;
	return 1;
}

// UPDATES AVAILABLE VECTOR
void update_available(RESOURCE resources[])
{
	int i = 0;
	for (i = 0; i < NUMBER_OF_RESOURCES; i++)
		AVAILABLE[i] += resources[i].allocation;
}

// BANKER'S ALGORITHM
int is_safe(PROCESS* processes)
{
	printf("SEQUENCE: ");
	int i, j, fulfilled=0,found=0;
	while (!all_done(processes))
	{
		found = 0;
		fulfilled = 0;
		for (i = 0; i < NUMBER_OF_PROCESSES; i++){
			for (j = 0; j < NUMBER_OF_RESOURCES; j++){
				if (processes[i].done){
					fulfilled = 0;
					break;
				}
				if (processes[i].resources[j].need > AVAILABLE[j]){
					fulfilled = 0;
					break;
				}
				else
					fulfilled = 1;
			}
			if (fulfilled){
				found = 1;
				processes[i].done = 1;
				printf("P%d ", processes[i].process_id);
				update_available(processes[i].resources);
			}
		}
		if (!found){
			printf("X (FAILURE)");
			return 0;
		}
	}
	return 1;
}

// DRIVER PROGRAM
int main(int argc, char **argv)
{
	PROCESS *processes = (PROCESS*)malloc(NUMBER_OF_PROCESSES*sizeof(PROCESS));
	read_system(processes,argv[1]);

	if (is_safe(processes))
		printf("\nSystem is safe..\n");
	else
		printf("\nSystem is not safe..\n");

	free(processes);
	system("pause");
	return 0;
}
