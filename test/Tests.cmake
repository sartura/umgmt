add_executable(
    test_lib

    test/test_lib.c
)

target_link_libraries(
    test_lib

    ${CMOCKA_LIBRARIES}
    ${SYSREPO_LIBRARIES}
    ${LIBYANG_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)

add_test(NAME test_lib COMMAND test_lib)