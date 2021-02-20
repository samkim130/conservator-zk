include_directories(.)

add_executable(ConservatorFrameworkFactoryTest ConservatorFrameworkFactoryTest.cpp)
add_test(NAME ConservatorFrameworkFactoryTest 
	COMMAND ${CMAKE_SOURCE_DIR}/build/ConservatorFrameworkFactoryTest)
target_link_libraries(ConservatorFrameworkFactoryTest conservator-framework ${ZOOKEEPER_LIBRARY} ${CHECK_LDFLAGS} ${CHECK_CFLAGS})
