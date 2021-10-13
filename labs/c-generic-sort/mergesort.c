
void mergesort(void *lineptr[], int left, int right,
	       int (*comp)(void *, void *)) {
	
	void merge(void *lineptr[], int, int,int,int (*comp)(void *, void *));
    
    if (left<right){
		int mid = (right+left)/2;//punto medio
		mergesort(lineptr,left,mid,comp);//mitad izquierda
		mergesort(lineptr,mid+1,right,comp);//mitad derecha
        merge(lineptr,left,mid,right,comp);
	}
}


void merge(void *lineptr[], int left, int mid,int right, int (*comp)(void *, void *)){
    if(left<right){
        int i=0, j=0, k=left;
        void *izq[mid - left + 1];
        void *der[right - mid];
        for (i = 0; i < mid-left+1; i++) {
                izq[i] = lineptr[left + i];
        }
        for (j = 0; j < right-mid; j++) {
                der[j] = lineptr[mid + 1 + j];
        }
        i = 0;
        j = 0; 
        k = left; 
        while(i < mid - left + 1 && j < right-mid) {
            if((*comp)(izq[i], der[j]) <= 0) {
                lineptr[k] = izq[i];
                i++;
            }
            else{
                lineptr[k] = der[j];
                j++;
            }
            k++;
        }
        while(i < mid - left + 1){
            lineptr[k] = izq[i];
            i++;
            k++;
        }
    
        while(j < right-mid){
            lineptr[k] = der[j];
            j++;
            k++;
        }
    }
}
