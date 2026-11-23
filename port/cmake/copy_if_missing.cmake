# Copies the folder SRC to DST only if DST isn't there yet: a build's own copy that a person may have changed since, as the hero tool's
# heroes (Bobby, beside the tool: its source, which its Save changes), unlike the game's mods/bobby, copied fresh each build.
#   cmake -DSRC=FROM -DDST=TO -P copy_if_missing.cmake
if(NOT EXISTS "${DST}")
  file(COPY "${SRC}/" DESTINATION "${DST}")
endif()
