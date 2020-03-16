// C++ program to find groups of unknown
// Points using K nearest neighbour algorithm.
#include <bits/stdc++.h>
using namespace std;

struct Point
{
	int val;	 // Group of point
	double x, y;	 // Co-ordinate of point
	double distance; // Distance from test point
};

//mergeSort
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	int L[n1], R[n2];

	for (i = 0; i < n1; i++)
	L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
	R[j] = arr[m + 1+ j];
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
	if (L[i] <= R[j])
		{
		arr[k] = L[i];
		i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}
while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		int m = l+(r-l)/2;
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);
		merge(arr, l, m, r);
	}
}


//quickSort
void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}
int partition (int arr[], int low, int high)
	{
		int pivot = arr[high]; // pivot
		int i = (low - 1); // Index of smaller element
		for (int j = low; j <= high- 1; j++)
		{
			if (arr[j] <= pivot)
			{
				i++; // increment index of smaller element
				swap(&arr[i], &arr[j]);
			}
		}
		swap(&arr[i + 1], &arr[high]);
		return (i + 1);
	}
void quickSort(int arr[], int low, int high)
	{
		if (low < high)
		{
			int pi = partition(arr, low, high);
			quickSort(arr, low, pi - 1);
			quickSort(arr, pi + 1, high);
		}
	}


//insertionSort
void insertionSort(int arr[], int n)
	{
	    int i, key, j;
	    for (i = 1; i < n; i++)
	    {
	        key = arr[i];
	        j = i - 1;
	  			while (j >= 0 && arr[j] > key)
	        {
	            arr[j + 1] = arr[j];
	            j = j - 1;
	        }
	        arr[j + 1] = key;
	    }
	}

// Used to sort an array of points by increasing
// order of distance
bool comparison(Point a, Point b)
{
	return (a.distance < b.distance);
}
// This function finds classification of point p using
// k nearest neighbour algorithm. It assumes only two
// groups and returns 0 if p belongs to group 0, else
// 1 (belongs to group 1).
int classifyAPoint(Point arr[], int n, int k, Point p)
{
// Fill distances of all points from p
	for (int i = 0; i < n; i++)
		arr[i].distance =	sqrt((arr[i].x - p.x) * (arr[i].x - p.x) +
(arr[i].y - p.y) * (arr[i].y - p.y));
// Sort the Points by distance from p
	sort(arr, arr+n, comparison);
// Now consider the first k elements and only
	// two groups
	int freq1 = 0;	 // Frequency of group 0
	int freq2 = 0;	 // Frequency of group 1
	for (int i = 0; i < k; i++)
	{
		if (arr[i].val == 0)
			freq1++;
		else if (arr[i].val == 1)
			freq2++;
	}

	return (freq1 > freq2 ? 0 : 1);
}

// Driver code
int main()
{
	int n = 17; // Number of data points
	Point arr[n];

	arr[0].x = 1;
	arr[0].y = 12;
	arr[0].val = 0;

	arr[1].x = 2;
	arr[1].y = 5;
	arr[1].val = 0;

	arr[2].x = 5;
	arr[2].y = 3;
	arr[2].val = 1;

	arr[3].x = 3;
	arr[3].y = 2;
	arr[3].val = 1;

	arr[4].x = 3;
	arr[4].y = 6;
	arr[4].val = 0;

	arr[5].x = 1.5;
	arr[5].y = 9;
	arr[5].val = 1;

	arr[6].x = 7;
	arr[6].y = 2;
	arr[6].val = 1;

	arr[7].x = 6;
	arr[7].y = 1;
	arr[7].val = 1;

	arr[8].x = 3.8;
	arr[8].y = 3;
	arr[8].val = 1;

	arr[9].x = 3;
	arr[9].y = 10;
	arr[9].val = 0;

	arr[10].x = 5.6;
	arr[10].y = 4;
	arr[10].val = 1;

	arr[11].x = 4;
	arr[11].y = 2;
	arr[11].val = 1;

	arr[12].x = 3.5;
	arr[12].y = 8;
	arr[12].val = 0;

	arr[13].x = 2;
	arr[13].y = 11;
	arr[13].val = 0;

	arr[14].x = 2;
	arr[14].y = 5;
	arr[14].val = 1;

	arr[15].x = 2;
	arr[15].y = 9;
	arr[15].val = 0;

	arr[16].x = 1;
	arr[16].y = 7;
	arr[16].val = 0;

	/*Testing Point*/
	Point p;
	p.x = 2.5;
	p.y = 7;

	// Parameter to decide group of the testing point
	int k = 3;
	printf ("The value classified to unknown point"
			" is %d.\n", classifyAPoint(arr, n, k, p));
	return 0;
}
