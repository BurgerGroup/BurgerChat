set(model_SRCS
    userManager.cc
  )

add_library(chat_model ${model_SRCS})
target_link_libraries(chat_model burger_db)

# install(TARGETS burger_db DESTINATION lib)

add_subdirectory(tests)
