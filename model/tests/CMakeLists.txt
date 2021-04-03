add_executable(group_unittest group_unittest.cc)
target_link_libraries(group_unittest chat_model)

add_executable(friend_test friend_test.cc)
target_link_libraries(friend_test chat_model)

add_executable(test_user test_user.cc)
target_link_libraries(test_user chat_model)
