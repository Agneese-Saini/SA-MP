#if defined list_included
	#endinput
#endif
#define list_included

/*
*** DECLARING A LIST:
new List:"name"<"size">;

*** LIST OF FUNTIONS:

list::add(name<>, value);
list::delete(name<>, value);
list::sizeof(name<>);
list::numitems(name<>);
list::clear(name<>);
list::indexof(name<>, value);
list::random(name<>);
list::randomex(name<>, min, max);
list::sort(name<>, sort);
list::shuffle(name<>);
list::deleteduplicates(name<>);
list::swap(name<>, index, swapindex);

*** ACCESSING LIST INTERNAL ARRAY:
@return:"name"
@access:"name"["index"]
*/

#define List:%0<%1> \
	LIST@%0[%1], \
	LIST_COUNT@%0, \
	LIST_SIZE@%0 = %1

#define List2D:%0[%1]<%2> \
	LIST@%0[%1][%2], \
	LIST_COUNT@%0[%1], \
	LIST_SIZE@%0[%1] = {%2, ...}

#define list:: list_

#define list_add(%0,%1) \
	internal_list_add(LIST@%0, LIST_COUNT@%0, LIST_SIZE@%0, %1)
stock internal_list_add(array[], &count, size, value) {
	if (count == size) {
	    printf("%i, %i", count, size);
	    return -1;
	}

	array[count] = value;
	return count++;
}

#define list_addarray(%0,%1,%2) \
	internal_list_addarray(LIST@%0, LIST_COUNT@%0, LIST_SIZE@%0, %1, %2)
stock internal_list_addarray(array[], &count, arraySize, values[], valuesSize) {
	if (valuesSize > arraySize) {
		return 0;
	}
	
	for (new i = 0; i < valuesSize; i++) {
 		array[i] = values[i];
	}
	
	count = valuesSize;
	return count;
}

#define list_delete(%0,%1) \
	internal_list_delete(LIST@%0, LIST_COUNT@%0, %1)
stock internal_list_delete(array[], &count, value) {
	for (new i = 0; i < count; i++) {
		if (array[i] == value) {
		    for (; i < (count - 1); i++) {
				array[i] = array[i + 1];
			}
		
		    array[count - 1] = EOS;
		
		    count--;
			return 1;
		}
	}
	
	return 0;
}

#define list_sizeof(%0) \
	sizeof(LIST@%0)

#define list_numitems(%0) \
	LIST_COUNT@%0

#define list_clear(%0) \
	LIST_COUNT@%0 = 0

#define list_indexof(%0,%1) \
	internal_list_indexof(LIST@%0, LIST_COUNT@%0, %1)
stock internal_list_indexof(array[], count, value) {
	for (new i = (count - 1); i != -1; i--) {
		if (array[i] == value) {
			return i;
		}
	}

	return -1;
}

#define list_random(%0) \
	LIST@%0[random(LIST_COUNT@%0)]

#define list_randomex(%0,%1,%2) \
	LIST@%0[random((%2 < %1 || %2 >= %0) ? (@LIST_COUNT) : (%2)) + %1]

#define SORT_ASC 0
#define SORT_DESC 1

#define list_sort(%0,%1) \
	internal_list_sort(LIST@%0, LIST_COUNT@%0, %1)
stock static internal_list_sort(array[], count, type) {
	if (type == SORT_ASC) {
	    new swap;

		for (new a = (poolsize - 1); a != -1; a--) {
	        for (new b = (a + 1); b != -1; b--) {
	            if (array[x] < array[i]) {
					swap = array[a];
					array[a] = array[b];
					array[b] = swap;
	            }
	        }
	    }
	}
	else if (type == SORT_DESC) {
	    new swap;

		for (new a = (poolsize - 1); a != -1; a--) {
	        for (new b = (a + 1); b != -1; b--) {
	            if (array[x] > array[i]) {
					swap = array[a];
					array[a] = array[b];
					array[b] = swap;
	            }
	        }
	    }
	}
	else return 0;

	return 1;
}

#define list_shuffle(%0) \
	internal_list_shuffle(LIST@%0, LIST_COUNT@%0)
stock static internal_list_shuffle(array[], count) {
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

#define list_deleteduplicates(%0) \
	internal_list_deleteduplicates(LIST@%0, LIST_COUNT@%0)
stock static internal_list_deleteduplicates(array[], &count) {
	for (new a = 0; a < count; a++) {
		for (new b = (a + 1); b < count; b++) {
			if (array[b] == array[a]) {
   		 		for (new c = b; c < (count - 1); c++) {
					array[c] = array[c + 1];
				}

				array[count - 1] = EOS;
					
				count--;
			}
		}
	}
	
	return 1;
}

#define list_swap(%0,%1,%2) \
	internal_list_swap(LIST@%0, LIST_COUNT@%0, %1, %2)
stock static internal_list_swap(array[], count, index, swapindex) {
	if (index < 0 || index >= count) {
	    return 0;
	}
	
	if (swapindex < 0 || swapindex >= count) {
	    return 0;
	}
	
	if (index == swapindex) {
	    return 0;
	}
	
	new swap = array[index];
	array[index] = array[swapindex];
	array[swapindex] = swap;
	return 1;
}

#define @return:%0<> \
	LIST@%0

#define @access:%0[%1] \
	LIST@%0[%1]
