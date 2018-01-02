void swap(int *x, int *y){
	int tmp;
	tmp = *x;
	*x = *y;
	*y = tmp;
}

void qsort2Ways(int a[], int left, int right){
	int i = left - 1;
	int j = right;
	int pivot = a[right];
	while(1) {
		while(i <= right && a[++i] < pivot); //stop when a[i] >= pivot
		while(j >= left && a[--j] > pivot); //stop when a[j] <= pivot
		if(i >= j)
			break;
		//when pointer cross

		swap(&a[i], &a[j]);
	}

	swap(&a[i], &a[right]);
	// swap pivot into the middle
	j = i - 1;
	i = i + 1;
	qsort2Ways(a, left, j);
	qsort2Ways(a, i, right);
}

void qsort3Ways(int a[], int left, int right){
	int i = left - 1;
	int j = right;
	int p = left - 1;
	int q = right;
	int pivot = a[right];
	if(right <= 1)
		return;
	while(1) {
		while(i <= right && a[++i] < pivot);
		while(j >= left && a[j] > pivot)
			swap(&a[i], &a[j]);

		if(a[i] == pivot) {
			p++;
			swap(&a[p], &a[i]); // if a[i] = pivot then swap it to the left
		}
		if(a[j] == pivot) {
			q--;
			swap(&a[q], &a[j]); // if a[j] = pivot then swap it the right;
		}

		swap(&a[i], &a[right]); // swap pivot to the middle

		if(i >= j)
			break;

		j = i - 1;
		i = i + 1;
		//move cursor to adjecent to a[i], from left to p and from q to right, thre are all pivot-equal elements

		for(int k = left; k < p; k++)
			swap(&a[k], &a[j--]);
		for(int k = right - 1; k > q; k--)
			swap(&a[k], &a[i++]);
		qsort3Ways(a, left, j);
		qsort3Ways(a, i, right);
	}
}