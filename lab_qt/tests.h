#pragma once
#include "domain.h"
#include "repo.h"
#include "service.h"
#include "basket.h"
#include "exceptions.h"
#include <memory>

void test_repo_operations(Repo* repo);
void test_service_operations(Repo* repo);
void test_undo_operations(Repo* repo);
void test_domain();
void test_repo_interface();
void test_file_repo_specific();
void test_in_memory_repo_specific();
void test_service();
void test_basket();
void test_service_basket_operations();
void test_export_basket_failure_simple();
void test_file_exception_class();
void test_undo_functionality();
void test_load_from_file();
//void run_tests();