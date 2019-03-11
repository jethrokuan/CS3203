#include "query_builder/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(nullptr),
      selected_declarations(nullptr),
      such_that(nullptr),
      pattern(nullptr){};

Query::~Query() {
  delete declarations;
  delete selected_declarations;
  delete such_that;
  delete pattern;
}
