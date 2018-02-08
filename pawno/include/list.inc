#if defined list_included
	#endinput
#endif
#define list_included

#define List:%0<%1> __list@%0[%1], __list_count@%0, __list_size@%0 = %1
#define List2D:%0[%1]<%2> __list@%0[%1][%2], __list_count@%0[%1], __list_size@%0[%1] = {%2, ...}

#define List_Add(%0,%1) Internal_List_Add(__list@%0, __list_count@%0, __list_size@%0, %1)
stock Internal_List_Add(array[], &count, size, value) {
	if (count == size)
		return -1;

	array[count] = value;
	return count++;
}

#define List_AddArray(%0,%1,%2) Internal_List_AddArray(__list@%0, __list_count@%0, __list_size@%0, %1, %2)
stock Internal_List_AddArray(array[], &count, arraySize, values[], valuesSize) {
	if (valuesSize > arraySize)
		return 0;

	for (new i = 0; i < valuesSize; i++)
		array[i] = values[i];

	count = valuesSize;
	return count;
}

#define List_Delete(%0,%1) Internal_List_Delete(__list@%0, __list_count@%0, %1)
stock Internal_List_Delete(array[], &count, value) {
	for (new i = 0; i < count; i++) {
		if (array[i] == value) {

		    for (; i < (count - 1); i++)
				array[i] = array[i + 1];

		    array[count - 1] = EOS;

		    count--;
			return 1;
		}
	}

	return 0;
}

#define List_Size(%0) __list_count@%0

#define List_Clear(%0) __list_count@%0 = 0

#define List_IndexOf(%0,%1) Internal_List_IndexOf(__list@%0, __list_count@%0, %1)
stock Internal_List_IndexOf(array[], count, value) {
	for (new i = (count - 1); i != -1; i--) {
		if (array[i] == value)
			return i;
	}

	return -1;
}

#define List_Random(%0) __list@%0[random(__list_count@%0)]

#define List_RandomEx(%0,%1,%2) __list@%0[random((%2 < %1 || %2 >= %0) ? (@LIST_COUNT) : (%2)) + %1]

#define List_Sort(%0,%1) Internal_List_Sort(__list@%0, __list_count@%0, %1)
stock Internal_List_Sort(array[], count, type) { // credits to Ryder
	new tempLeft = left;
	new tempRight = right;
	new pivot = array[(left + right) / 2];
	new tempVar;

 	while (tempLeft <= tempRight) {
    	while (array[tempLeft] < pivot) ++tempLeft;
    	while (array[tempRight] > pivot) --tempRight;

    	if(tempLeft <= tempRight) {
	        tempVar = array[tempLeft];
			array[tempLeft] = array[tempRight];
 			array[tempRight] = tempVar;

	        ++tempLeft;
			--tempRight;
		}
    }

	if(left < tempRight) quickSort(array, left, tempRight);
	if(tempLeft < right) quickSort(array, tempLeft, right);

	return 1;
}

#define List_Shuffle(%0) Internal_List_Shuffle(__list@%0, __list_count@%0)
stock Internal_List_Shuffle(array[], count) {
	new r;
	new swap;

	for (new i = 0; i < count; i++) {
		r = (i + random((count - i) + 1));

		swap = array[r];
		array[r] = array[i];
		array[i] = swap;
	}

	return 1;
}
