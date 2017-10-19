/*
Example: (a complicated operation performed lol)

	new ArrayList:myList = ArrayList(10);
	ArrayList_Add(myList, _:ArrayList(5));
	
	ArrayList_Add(ArrayList:ArrayList:ArrayList_Get(myList, 0), 5);
	
	printf("%i", ArrayList#ArrayList:ArrayList_Get(myList, 0)[0]);
	
*/

/*
native ArrayList(size = MAX_ARRAY_LIST_SIZE);
native ArrayList_Delete(ArrayList:list);
native ArrayList_IsValid(ArrayList:list);
native ArrayList_Add(ArrayList:list, value);
native ArrayList_Remove(ArrayList:list, index);
native ArrayList_RemoveValue(ArrayList:list, value);
native ArrayList_Set(ArrayList:list, index, value);
native ArrayList_Get(ArrayList:list, index);
native ArrayList_MaxSize(ArrayList:list);
native ArrayList_Size(ArrayList:list);
native ArrayList_IndexOf(ArrayList:list, value);
native ArrayList_Shuffle(ArrayList:list);
native ArrayList_Sort(ArrayList:list);
native ArrayList_ElimateDuplicates(ArrayList:list);
*/

#if !defined MAX_ARRAY_LISTS
	#define MAX_ARRAY_LISTS \
		512
#endif

#if !defined MAX_ARRAY_LIST_SIZE
	#define MAX_ARRAY_LIST_SIZE \
		2048
#endif

#define INVALID_ARRAY_LIST \
	ArrayList:-1

#define ArrayList#%0[%1] \
    arrayListsValues[%0][%1]

enum E_ARRAY_LIST {
	bool:E_ARRAY_LIST_VALID,
	E_ARRAY_LIST_SIZE,
	E_ARRAY_LIST_COUNT
};

static arrayLists[ArrayList:MAX_ARRAY_LISTS][E_ARRAY_LIST];
static arrayListsValues[ArrayList:MAX_ARRAY_LISTS][MAX_ARRAY_LIST_SIZE];

stock ArrayList:ArrayList(size = MAX_ARRAY_LIST_SIZE) {
	if (size < 0 || size > MAX_ARRAY_LIST_SIZE) {
		return INVALID_ARRAY_LIST;
	}
	
	new ArrayList:index = INVALID_ARRAY_LIST;
	for (new ArrayList:i; i < ArrayList:MAX_ARRAY_LISTS; i++) {
		if (!arrayLists[i][E_ARRAY_LIST_VALID]) {
			index = i;
			break;
		}
	}
	
	if (index == INVALID_ARRAY_LIST) {
		return INVALID_ARRAY_LIST;
	}

	arrayLists[index][E_ARRAY_LIST_VALID] = true;
	arrayLists[index][E_ARRAY_LIST_SIZE] = size;
	arrayLists[index][E_ARRAY_LIST_COUNT] = 0;
	return index;
}

stock ArrayList_Delete(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	arrayLists[list][E_ARRAY_LIST_VALID] = false;
	return 1;
}

stock ArrayList_IsValid(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	return arrayLists[list][E_ARRAY_LIST_VALID];
}

stock ArrayList_Add(ArrayList:list, value) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	if (arrayLists[list][E_ARRAY_LIST_COUNT] == arrayLists[list][E_ARRAY_LIST_SIZE]) {
		return -3;
	}

	new index = arrayLists[list][E_ARRAY_LIST_COUNT]++;
	arrayListsValues[list][index] = value;
	return index;
}

stock ArrayList_Remove(ArrayList:list, index) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	if (index < 0 || index >= arrayLists[list][E_ARRAY_LIST_COUNT]) {
		return 0;
	}
	
	for (new i = index; i < (arrayLists[list][E_ARRAY_LIST_COUNT] - 1); i++) {
		arrayListsValues[list][i] = arrayListsValues[list][i + 1];
	}
	arrayLists[list][E_ARRAY_LIST_COUNT]--;
	return 1;
}

stock ArrayList_RemoveValue(ArrayList:list, value) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	for (new i; i < arrayLists[list][E_ARRAY_LIST_COUNT]; i++) {
		if (arrayListsValues[list][i] == value) {
			for (; i < (arrayLists[list][E_ARRAY_LIST_COUNT] - 1); i++) {
                arrayListsValues[list][i] = arrayListsValues[list][i + 1];
			}
			arrayLists[list][E_ARRAY_LIST_COUNT]--;

			return 1;
		}
	}

	return 0;
}

stock ArrayList_Set(ArrayList:list, index, value) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	if (index < 0 || index >= arrayLists[list][E_ARRAY_LIST_COUNT]) {
		return 0;
	}

	arrayListsValues[list][index] = value;
	return 1;
}

stock ArrayList_Get(ArrayList:list, index) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	if (index < 0 || index >= arrayLists[list][E_ARRAY_LIST_COUNT]) {
		return 0;
	}

	return arrayListsValues[list][index];
}

stock ArrayList_Size(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	return arrayLists[list][E_ARRAY_LIST_COUNT];
}

stock ArrayList_MaxSize(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	return arrayLists[list][E_ARRAY_LIST_SIZE];
}

stock ArrayList_IndexOf(ArrayList:list, value) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	for (new i; i < arrayLists[list][E_ARRAY_LIST_COUNT]; i++) {
		if (arrayListsValues[list][i] == value) {
			return i;
		}
	}
	
	return -3;
}

stock ArrayList_Shuffle(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}
	
	new size = arrayLists[list][E_ARRAY_LIST_COUNT],
		temp,
		r;
	while (size > 1) {
		r = random(size);
		size--;

		temp = arrayListsValues[list][size];
		arrayListsValues[list][size] = arrayListsValues[list][r];
		arrayListsValues[list][r] = temp;
	}
	return 1;
}

static Internal@ArrayList_Partition(array[], start, end) {
    new pivot = array[end];
	new i = (start - 1);
	new temp;
	
    for (new j = start; j <= (end- 1); j++) {
        if (array[j] <= pivot) {
            i++;
            
            temp = array[i];
			array[i] = array[j];
			array[j] = temp;
        }
    }
    
    temp = array[i + 1];
	array[i + 1] = array[end];
	array[end] = temp;
    return (i + 1);
}

static Internal@ArrayList_QuickSort(array[], start, end) {
	if (start >= end) {
		return;
	}
	
	new i = Internal@ArrayList_Partition(array, start, end);

	Internal@ArrayList_QuickSort(array, start, (i - 1));
	Internal@ArrayList_QuickSort(array, (i + 1), end);
}

stock ArrayList_Sort(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}
	
	Internal@ArrayList_QuickSort(arrayListsValues[list], 0, (arrayLists[list][E_ARRAY_LIST_COUNT] - 1));
	return 1;
}

stock ArrayList_ElimateDuplicates(ArrayList:list) {
	if (list < ArrayList:0 || list >= ArrayList:MAX_ARRAY_LISTS) {
		return -1;
	}

	if (!arrayLists[list][E_ARRAY_LIST_VALID]) {
		return -2;
	}

	new size = arrayLists[list][E_ARRAY_LIST_COUNT];
	for (new i = 0; i < size; i++) {
	    for (new j = (i + 1); j < size;) {
	        if (arrayListsValues[list][j] == arrayListsValues[list][i]) {
	            for (new k = j; k < size; k++) {
             		arrayListsValues[list][k] = arrayListsValues[list][k + 1];
				}
				
				size--;
			}
			else {
				j++;
			}
		}
	}
	
	if (size == arrayLists[list][E_ARRAY_LIST_COUNT]) {
		return 0;
	}
	
	arrayLists[list][E_ARRAY_LIST_COUNT] = size;
	return 1;
}
