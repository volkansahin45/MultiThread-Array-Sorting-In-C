#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define array_length 1000

int numberArray[array_length];

void merge(int arr[], int left, int middle, int right) {
    int i, j, k;
    int half1 = middle - left + 1;
    int half2 = right - middle;
    int first[half1], second[half2]; // temp arrays
    for (i = 0; i < half1; i++) {
        first[i] = arr[left + i];
    }
    for (j = 0; j < half2; j++) {
        second[j] = arr[middle + 1 + j];
    }
    i = 0;
    j = 0;
    k = left;

    while (i < half1 && j < half2) {
        if (first[i] <= second[j]) {
            arr[k] = first[i];
            ++i;
        } else {
            arr[k] = second[j];
            j++;
        }
        k++;
    }

    while (i < half1) {
        arr[k] = first[i];
        i++;
        k++;
    }

    while (j < half2) {
        arr[k] = second[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int middle = (l + (r-1))/ 2;
        merge_sort(arr, l, middle);
        merge_sort(arr, middle+1, r);
        merge(arr, l, middle, r);
    }
}

void fillArray()
{
    int i;
    for(i=0; i<1000; i++)
    {
        numberArray[i] = i;
    }
}

void shuffleArray()
{
    int i, n = 1000;
    for (i = 0; i < n - 1; i++)
    {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = numberArray[j];
      numberArray[j] = numberArray[i];
      numberArray[i] = t;
    }

}

void *runner(void *param)
{
    int midpoint = array_length/2;
    int whichThread = (int *)param;

    switch(whichThread)
    {
        case 0:
        merge_sort(numberArray, 0, midpoint);
        break;

        case 1:
        merge_sort(numberArray, midpoint + 1, array_length - 1);
        break;

        case 2:
        merge(numberArray, 0, (array_length/2), array_length-1);
        break;
    }
    pthread_exit(0);

}

int main()
{
    int i;
	pthread_t tids[3];
    
    fillArray();
    shuffleArray();

    pthread_create(&tids[0],NULL,runner, 0);
    pthread_create(&tids[1],NULL,runner, 1);
    pthread_join(tids[0],NULL);
    pthread_join(tids[1],NULL);
    
    pthread_create(&tids[2],NULL,runner, 2);
    pthread_join(tids[2],NULL);

	printf("Main Thread\n");

    FILE *fp;
    if((fp = fopen ("array.txt", "w+")) == NULL)
    {
        printf("Can't open the file\n");
        return;
    }

    if((fwrite(numberArray, sizeof(numberArray), 1, fp)) != 1)
    {
        printf("Write Error!!\n");
        return;
    }
    printf("Write Success\n");
    fclose(fp);


    if((fp = fopen ("array.txt", "r")) == NULL)
    {
        printf("Can't open the file\n");
        return;
    }

    int readArray[array_length];

    if((fread(readArray, sizeof(numberArray), 1, fp)) != 1)
    {
        printf("Read Error!!\n");
        return;
    }

    fclose(fp);

    printf("Numbers that read from file\n");
    for(i=0; i<1000; i++)
    {
        printf("%d ", readArray[i]);
    }

	return;
}

