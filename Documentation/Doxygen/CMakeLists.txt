
# Build the documentation
#

INCLUDE (${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)

IF (BUILD_DOCUMENTATION)
  #
  # Configure the script and the doxyfile, then add target
  #
  IF(NOT DOT_PATH)
    GET_FILENAME_COMPONENT(DOT_PATH ${DOT} PATH)
  ENDIF(NOT DOT_PATH)

  CONFIGURE_FILE(
    ${PROJECT_SOURCE_DIR}/Documentation/Doxygen/doxyfile.in
    ${PROJECT_BINARY_DIR}/Documentation/Doxygen/doxyfile)

  CONFIGURE_FILE(
    ${PROJECT_SOURCE_DIR}/Documentation/Doxygen/doc_makeall.sh.in
    ${PROJECT_BINARY_DIR}/Documentation/Doxygen/doc_makeall.sh)

  ADD_CUSTOM_TARGET(doc
    ${BASH}
    ${PROJECT_BINARY_DIR}/Documentation/Doxygen/doc_makeall.sh)

ENDIF (BUILD_DOCUMENTATION)
