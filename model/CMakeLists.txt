set(model_SRCS
    userManager.cc
    groupManager.cc
    groupUserManager.cc
    friendManager.cc
    offlineMsgManager.cc
  )

add_library(chat_model ${model_SRCS})
target_link_libraries(chat_model burger_db)

# install(TARGETS burger_db DESTINATION lib)

add_subdirectory(tests)
