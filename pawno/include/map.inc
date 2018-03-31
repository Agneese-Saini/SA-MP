#include <memory>

#if !defined DEFAULT_MAP_SIZE
	#define DEFAULT_MAP_SIZE 100
#endif

#define MAP_TYPE_int 1
#define MAP_TYPE_float 2
#define MAP_TYPE_string 3

#define map<%0,%1>%2[] \
	Pointer:%2 = MEM_NULLPTR, %2@next = 0, %2@first_type = MAP_TYPE_%0, %2@second_type = MAP_TYPE_%1

#define map_insert(%0,%1) \
	internal_insert(%0, %0@next, %0@first_type, %0@second_type, %1)

#define map_find(%0,%1) \
	internal_find(%0, %0@next, %0@first_type, %1)

#define map_delete(%0,%1) \
	internal_delete(%0, %0@next, %0@first_type, %0@second_type, %1)

#define map_first(%0,%1) \
	internal_first(%0, %0@next, %0@first_type, %1)

#define map_second(%0,%1) \
	internal_second(%0, %0@next, %0@second_type, %1)
	
#define map_set_first(%0,%1,%2) \
	internal_set_first(%0, %0@next, %0@first_type, %1, %2)

#define map_set_second(%0,%1,%2) \
	internal_set_second(%0, %0@next, %0@second_type, %1, %2)

#define map_clear(%0) \
	internal_clear(%0, %0@next, %0@first_type, %0@second_type)

#define map_size(%0) \
	%0@next

#define mapfind:%0[%1] \
	map_find(%0,%1)

#define mapget:%0[%1] \
	internal_second(%0, %0@next, %0@second_type, (map_find(%0,%1)))
	
#define mapset:%0[%1](%2) \
	internal_set_second(%0, %0@next, %0@second_type, (map_find(%0,%1)), %2)
    
forward @__map_amxfix(); @__map_amxfix() {
    strcat("", "", 0);
}

static stock setstringarg(arg, const src[], maxlength = sizeof(src)) {
	new address;

	#emit LOAD.S.pri arg
    #emit SMUL.C     4
    #emit LOAD.S.alt 0
	#emit ADD
    #emit ADD.C      12
    #emit LOAD.I
	#emit STOR.S.PRI address
	#emit PUSH.S     maxlength
	#emit PUSH.S     src
	#emit CONST.pri  0
	#emit SREF.S.pri address
	#emit PUSH.S     address
	#emit PUSH.C     12
	#emit SYSREQ.C   strcat
	#emit STACK      16
}

static stock getstringarg(arg, dest[], maxlength = sizeof(dest)) {
    dest[0] = EOS;

    #emit LOAD.S.pri arg
    #emit SMUL.C     4
    #emit LOAD.S.alt 0
    #emit ADD
    #emit ADD.C      12
    #emit LOAD.I
    #emit PUSH.S     maxlength
    #emit PUSH.pri
    #emit LOAD.S.alt dest
    #emit CONST.pri  0
    #emit STOR.I
    #emit PUSH.alt
    #emit PUSH.C     12
    #emit SYSREQ.C   strcat
    #emit STACK      16
}

static stock internal_insert(&Pointer:addr, &next, first_type, second_type, {Float, _}:...) {
    if (next == 0) {
        addr = MEM_new(DEFAULT_MAP_SIZE);
        if (addr == MEM_NULLPTR)
            return;
	}
	else {
		new capacity = MEM_get_size(addr);
		if (next == capacity) {
			new Pointer:new_addr = MEM_new(capacity + DEFAULT_MAP_SIZE);
	        if (new_addr == MEM_NULLPTR)
	            return;

			for (new i = 0; i < capacity; i++) {
				MEM_set_val(new_addr, i, MEM_get_val(addr, i));
			}

			MEM_delete(addr);
			addr = new_addr;
		}
	}

	static dest[2048];

	new Pointer:double_addr = MEM_new(2);

	if (first_type == MAP_TYPE_int || first_type == MAP_TYPE_float) {
		MEM_set_val(double_addr, 0, getarg(4));
	}
	else if (first_type == MAP_TYPE_string) {
	    getstringarg(4, dest);

	    new Pointer:string_addr = MEM_new_arr(dest);
		MEM_set_val(double_addr, 0, _:string_addr);
	}

	if (second_type == MAP_TYPE_int || second_type == MAP_TYPE_float) {
		MEM_set_val(double_addr, 1, getarg(5));
	}
	else {
	    getstringarg(5, dest);

	    new Pointer:string_addr = MEM_new_arr(dest);
		MEM_set_val(double_addr, 1, _:string_addr);
	}

	MEM_set_val(addr, next++, _:double_addr);
}

static stock internal_find(Pointer:addr, next, first_type, {Float, _}:...) {
	static dest[2048];
	static argdest[2048];

    new Pointer:double_addr;

	if (first_type == MAP_TYPE_int || first_type == MAP_TYPE_float) {
	    new argval = getarg(3);

		for (new i = 0; i < next; i++) {
			double_addr = Pointer:MEM_get_val(addr, i);
	    	if (MEM_get_val(double_addr, 0) == argval) {
				return i;
			}
		}
	}
	else {
	    getstringarg(3, argdest);
	    if (argdest[0] == EOS)
	        return -1;

		new Pointer:string_addr;
		for (new i = 0; i < next; i++) {
			double_addr = Pointer:MEM_get_val(addr, i);
			string_addr = Pointer:MEM_get_val(double_addr, 0);
            dest[0] = EOS;
			MEM_get_arr(string_addr, _, dest);

	    	if (dest[0] != EOS && !strcmp(dest, argdest)) {
				return i;
			}
	    }
	}

	return -1;
}

static stock internal_delete(&Pointer:addr, next, first_type, second_type, index) {
    if (index >= next)
        return;

	new Pointer:double_addr = Pointer:MEM_get_val(addr, index);

	if (first_type == MAP_TYPE_string) {
	    new Pointer:string_addr = Pointer:MEM_get_val(double_addr, 0);
		MEM_delete(string_addr);
	}

	if (second_type == MAP_TYPE_string) {
	    new Pointer:string_addr = Pointer:MEM_get_val(double_addr, 1);
		MEM_delete(string_addr);
	}

	MEM_delete(double_addr);
}

static stock internal_first(Pointer:addr, next, first_type, index, {Float, _}:...) {
	static dest[2048];
	new ret = 0;

    if (index >= next)
        return ret;

  	new Pointer:double_addr = Pointer:MEM_get_val(addr, index);

	if (first_type == MAP_TYPE_int || first_type == MAP_TYPE_float) {
 		ret = MEM_get_val(double_addr, 0);
 		
		if (numargs() == 4)
			setarg(4, _, ret);
	}
	else {
	    new Pointer:string_addr = Pointer:MEM_get_val(double_addr, 0);
	    MEM_get_arr(string_addr, _, dest);
		ret = dest[0];

    	if (numargs() == 4)
			setstringarg(4, dest);
	}

	return ret;
}

static stock internal_second(Pointer:addr, next, second_type, index, {Float, _}:...) {
	static dest[2048];
	new ret = 0;

    if (index >= next)
        return ret;

 	new Pointer:double_addr = Pointer:MEM_get_val(addr, index);

	if (second_type == MAP_TYPE_int || second_type == MAP_TYPE_float) {
	    ret = MEM_get_val(double_addr, 1);
	    
		if (numargs() == 4)
			setarg(4, _, ret);
	}
	else {
	    new Pointer:string_addr = Pointer:MEM_get_val(double_addr, 1);
	    MEM_get_arr(string_addr, _, dest);
		ret = dest[0];

	   	if (numargs() == 4)
			setstringarg(4, dest);
	}

	return ret;
}

static stock internal_set_first(Pointer:addr, next, first_type, index, {Float, _}:...) {
	static dest[2048];
	
    if (index >= next)
        return;

   	new Pointer:double_addr = Pointer:MEM_get_val(addr, index);

	if (first_type == MAP_TYPE_int || first_type == MAP_TYPE_float) {
	    MEM_set_val(double_addr, 0, getarg(4));
	}
	else {
	    getstringarg(4, dest);

		new Pointer:string_addr = Pointer:MEM_get_val(double_addr, 0);
	    MEM_set_arr(string_addr, _, dest);
	}
}

static stock internal_set_second(Pointer:addr, next, second_type, index, {Float, _}:...) {
	static dest[2048];
	
    if (index >= next)
        return;

	new Pointer:double_addr = Pointer:MEM_get_val(addr, index);

	if (second_type == MAP_TYPE_int || second_type == MAP_TYPE_float) {
	    MEM_set_val(double_addr, 1, getarg(4));
	}
	else {
	    getstringarg(4, dest);

		new Pointer:string_addr = Pointer:MEM_get_val(double_addr, 1);
	    MEM_set_arr(string_addr, _, dest);
	}
}

static stock internal_clear(&Pointer:addr, &next, first_type, second_type) {
	for (new i = 0; i < next; i++) {
        internal_delete(addr, next, first_type, second_type, i);
	}

	next = 0;
}
