set(client_SRCS
    # interface.cc
    chatClient.cc
    main.cc
)

add_executable(chatClient ${client_SRCS})

target_link_libraries(chatClient burger_base burger_net chat_model pthread rt)


# add_subdirectory(tests)
