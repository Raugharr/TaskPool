/*
 * Author: David Brotz
 * File: main.c
 */

#include "TaskTest.h"

#include <stdlib.h>
#include <check.h>

int main(void) {
	Suite* _Suite = TaskSuite();
	SRunner* _Runner = NULL;	
	int _FailedCt = 0;

	_Runner = srunner_create(_Suite);
	srunner_set_fork_status(_Runner, CK_NOFORK);
	srunner_run_all(_Runner, CK_NORMAL);
	_FailedCt = srunner_ntests_failed(_Runner);
	srunner_free(_Runner);
	return (_FailedCt == 0) ? (EXIT_SUCCESS) : (EXIT_FAILURE);
}

