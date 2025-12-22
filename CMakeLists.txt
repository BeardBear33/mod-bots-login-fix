set(scripts_STAT_SRCS
  ${CMAKE_CURRENT_LIST_DIR}/src/mod_bots_login_fix_loader.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/mod_bots_login_fix_config.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/mod_bots_login_fix_variation_1.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/mod_bots_login_fix_variation_2.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/mod_bots_login_fix_variation_3.cpp
)

AC_ADD_SCRIPT(${scripts_STAT_SRCS})

message(STATUS "-> module 'mod-bots-login-fix' loaded.")
