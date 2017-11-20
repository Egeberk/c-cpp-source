/*Egeberk Özberk 
Visual Studio 2013
22.2.2017*/
/*egeberk@gmail.com*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10000
// HELPER FUNCTIONS
int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}
void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;

}
void fill_Array(int *arr, int *size)
{
	int i = 0;
	for (i = 0; i < MAX; i++)
	{
		arr[i] = rand() % 100 + 1;
		(*size)++;
	}
}
void print_Array(int *arr, int size)
{
	int i = 0;
	for (i = 0; i < MAX; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\nSize=%d \n",size);
}
void check(int *arr)
{
	int i = 0;
	for (i = 0; i < MAX-1; i++)
	{
		if (arr[i]>arr[i + 1])
		{
			printf("Not sorted.\n");
			return;
		}
	}
	printf("Sorted.\n");
}

// BUBBLE SORT
void bubble_Sort(int *arr)
{
	int	i, j;
	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX-1; j++)
		{
			if (arr[j]>arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
		}
	}
}

// SELECTION SORT
void selection_Sort(int *arr)
{
	int	i, j,min_index;
	for (i = 0; i < MAX-1; i++)
	{
		min_index = i;
		for (j = i+1; j < MAX; j++)
		{
			if (arr[j] <= arr[min_index])
				min_index = j;
		}
		swap(&arr[i], &arr[min_index]);
	}
}

// INSERTION SORT
void insertion_Sort(int *arr)
{
	int i, j;
	for (i = 0; i < MAX; i++)
	{
		for (j = i; j > 0; j--)
		{
			if (arr[j] < arr[j-1])
				swap(&arr[j], &arr[j-1]);
		}
	}
}

// COUNTING SORT
void counting_Sort(int *arr)
{
	int i=0, j=0,k=0,max=arr[i],*temp_arr;
	for (i = 0; i < MAX; i++)
		if (arr[i]>max)
			max = arr[i];
	temp_arr = (int*)malloc(sizeof(int)*(max+1));
	if (temp_arr == NULL)
	{
		printf("Error allocating memory!");
		exit(0);
	}
	memset(temp_arr, 0, (max+1)*sizeof(int));
	for (i = 0; i < MAX; i++)
	{
		(temp_arr[arr[i]])++;
	}
	for (i = 0; i < max+1; i++)
		if (temp_arr[i]>0)
			for (k = 0; k < temp_arr[i]; k++)
				arr[j++] = i;
	free(temp_arr);
}

// QUICK SORT
int partition(int *arr, int left, int right)
{
	int pivot = arr[left];
	int left_index = left;
	int right_index = right+1;
	while (1)
	{
		do{
			left_index++;
		} while (arr[left_index] < pivot && left_index<right_index);
		do{
			right_index--;
		} while (arr[right_index] > pivot);
		if (left_index > right_index)
			break;
		swap(&arr[left_index], &arr[right_index]);
	}
	swap(&arr[left], &arr[right_index]);
	return right_index;
}
void quick_Sort(int *arr, int start, int end)
{
	int division_index;
	if (start < end)
	{	
		division_index = partition(arr, start, end);
		quick_Sort(arr, start, division_index - 1);
		quick_Sort(arr, division_index + 1, end);
	}
}

// MERGE SORT
void merge(int *arr, int left, int middle, int right)
{
	int i, j, k;
	int l_size = middle - left + 1;
	int r_size = right - middle;
	int *left_temp = (int*)malloc(l_size*sizeof(int));
	int *right_temp = (int*)malloc(r_size*sizeof(int));
	for (i = 0; i < l_size; i++)
		left_temp[i] = arr[left + i];
	for (j = 0; j < r_size; j++)
		right_temp[j] = arr[(middle + 1) + j];
	i = 0; j = 0; k = left;
	while (i < l_size && j < r_size)
	{
		if (left_temp[i] >= right_temp[j])
			arr[k++] = right_temp[j++];
		else
			arr[k++] = left_temp[i++];
	}
	while (i < l_size)
		arr[k++] = left_temp[i++];
	while (j < r_size)
		arr[k++] = right_temp[j++];
}
void merge_Sort(int *arr, int start, int end)
{
	int division_index;
	if (start < end)
	{
		division_index =  (end + start) / 2;
		merge_Sort(arr, start, division_index);
		merge_Sort(arr, division_index + 1, end);
		merge(arr, start, division_index, end);
	}
}

// SHELL SORT
void shell_Sort(int *arr)
{
	int i, j, gap;
	gap = MAX / 2;
	while (gap > 0)
	{
		for (i = gap; i < MAX; ++i)
		{
			int temp = arr[i];
			for (j = i; j >= gap && temp < arr[j - gap]; j -= gap)
			{
				swap(&arr[j], &arr[j - gap]);
			}
		}
		gap /= 2;
	}
}

// SHAKER SORT
void shaker_sort(int *arr)
{
	int i, j, k=0;
	int start = 0;
	int end = MAX - 1;
	while (k <= MAX/2)
	{
		for (i = 0; i < end; i++)
			if (arr[i]>arr[i + 1])
				swap(&arr[i], &arr[i + 1]);
		start++;
		for (j = end; j>start; j--)
			if (arr[j] < arr[j - 1])
				swap(&arr[j], &arr[j - 1]);
		end--;
		k++;
	}
}

// GCC SORT
void gcc_sort(int *arr)
{
	qsort(arr, MAX,sizeof(int),compare);
}

// TREE SORT
typedef struct BST_NODE
{
	struct BST_NODE *left, *right;
	int data;
	int count;
} node;
int NEW_ARRAY_SIZE = 0;
node* generate_node(int value)
{
	node *n = (node*)malloc(sizeof(node));
	n->right = n->left = NULL;
	n->data = value;
	n->count = 1;
	return n;
}
node* insert_node(node *n, int value)
{
	if (n == NULL)
		return generate_node(value);
	if (value < n->data)
		n->left = insert_node(n->left, value);
	else if (value > n->data)
		n->right = insert_node(n->right, value);
	else
		n->count++;
	return n;
}
node* construct_BST(int *arr)
{
	int i;
	node *n = NULL;
	for (i = 0; i < MAX; i++)
		n = insert_node(n, arr[i]);
	return n;
}
void LVR(int *arr, node *n)
{
	if (n != NULL){
		int i;
		LVR(arr, n->left);
		for (i = 0; i < n->count; ++i)
			arr[NEW_ARRAY_SIZE++] = n->data;
		LVR(arr, n->right);
	}
}
void tree_sort(int *arr)
{
	node *root = construct_BST(arr);
	NEW_ARRAY_SIZE = 0;
	LVR(arr, root);
	free(root->left);
	free(root->right);
	free(root);
}

// DRIVER PROGRAM
int main()
{
	// HERE YOU CAN USE PRINT / FILL / CHECK FUNCTIONS 
	// ALL SORT FUNCTIONS EXPECT JUST AN ARRAY FOR PARAMETER
	// YOU CAN SET YOUR OWN SIZE VALUE BY CHANGING MAX VALUE DECLARED AT LINE 4
	int *Array;
	int Size = 0, i = 0;
	Array = (int*)malloc(sizeof(int)*MAX);
	if (Array == NULL)
	{
		printf("Error allocating memory!");
		exit(0);
	}
	fill_Array(Array,&Size);
	/*Insert any sort function here
	eg. shaker_sort(Array);
	*/
	check(Array);
	free(Array);
	getchar();
	return 0;
}

