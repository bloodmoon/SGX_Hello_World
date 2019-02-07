#include "VaultE_u.h"
#include <errno.h>

typedef struct ms_say_hello_t {
	int ms_retval;
} ms_say_hello_t;

typedef struct ms_ocall_print_t {
	const char* ms_str;
} ms_ocall_print_t;

static sgx_status_t SGX_CDECL VaultE_ocall_print(void* pms)
{
	ms_ocall_print_t* ms = SGX_CAST(ms_ocall_print_t*, pms);
	ocall_print(ms->ms_str);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[1];
} ocall_table_VaultE = {
	1,
	{
		(void*)VaultE_ocall_print,
	}
};
sgx_status_t say_hello(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_say_hello_t ms;
	status = sgx_ecall(eid, 0, &ocall_table_VaultE, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

