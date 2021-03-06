# Simple Component
set(srcs ${srcs}
  src/simple_source/parser/TestAssign.cpp
  src/simple_source/parser/TestCall.cpp
  src/simple_source/parser/TestCondExpr.cpp
  src/simple_source/parser/TestExpr.cpp
  src/simple_source/parser/TestIf.cpp
  src/simple_source/parser/TestMultiProcedure.cpp
  src/simple_source/parser/TestPrint.cpp
  src/simple_source/parser/TestProcedure.cpp
  src/simple_source/parser/TestRead.cpp
  src/simple_source/parser/TestStatementList.cpp
  src/simple_source/parser/TestWhile.cpp
  src/simple_source/parser/TestInterface.cpp
  src/simple_source/TestSimpleLexer.cpp
  )

# Query Executor
set(srcs ${srcs}
  src/query_executor/TestConstraintDatabase.cpp
  src/query_executor/TestClausePrioritizer.cpp
  src/query_executor/TestQueryExecutor.cpp
  )

# Query Builder
set(srcs ${srcs}
  src/query_builder/TestQueryValidator.cpp
  src/query_builder/TestSynonym.cpp
  src/query_builder/TestQueryLexer.cpp
  src/query_builder/TestQueryParser.cpp
  src/query_builder/TestRelCond.cpp
  )

# PKB
set(srcs ${srcs}
  src/pkb/TestPKB.cpp
  src/pkb/TestPKBExtension.cpp
  )

set(srcs ${srcs}
  src/main.cpp
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs})

target_link_libraries(unit_testing spa)

