#include <stdlib.h>
#include <stdio.h>
#include <ffi.h>

typedef struct {
	int a;
} dummy;

dummy *testffi (int a, unsigned char b )
{
	dummy *obj = (dummy *)malloc (sizeof (dummy));
	printf ("called with %d, %c\n", a, b);
	obj->a = a;
	return obj;
}

int main (void)
{
	ffi_cif cif;
	ffi_status s;
	ffi_type *arg_types[2];
	void *arg_values[2];
	ffi_arg result;
	/* init a cif */
	arg_types[0] = &ffi_type_sint;
	arg_types[1] = &ffi_type_uchar;
	if ((s = ffi_prep_cif (&cif, FFI_DEFAULT_ABI, 2, &ffi_type_pointer, arg_types) != FFI_OK))
	{
		printf (" could not ffi_prep_cif() \n");
		return 1;
	}
	/* init a call */
	int arg1 = 100;
	unsigned char arg2 = 'a';
	arg_values[0] = &arg1;
	arg_values[1] = &arg2;
	ffi_call (&cif, FFI_FN (testffi), &result, arg_values);
	dummy *obj = (dummy *)result;
	printf ("called and function returned : %d\n", obj->a);
	free (obj);
	return 0;
}

