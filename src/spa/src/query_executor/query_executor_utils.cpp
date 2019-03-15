#include <cassert>
#include <string>
#include <vector>
#include "query_executor/query_executor.h"
#include "utils/utils.h"

using namespace QE;

std::string QueryExecutor::suchThatArgToString(StmtOrEntRef arg) {
  return std::visit(overload{[](StmtRef& s) { return stmtRefToString(s); },
                             [](EntRef& e) { return entRefToString(e); }},
                    arg);
}

std::string QueryExecutor::stmtRefToString(StmtRef arg) {
  return std::visit(
      overload{[](Synonym& s) { return s.synonym; },
               [](Underscore&) { return std::string("_"); },
               [](StatementNumber& s) { return std::to_string(s); }},
      arg);
}

std::string QueryExecutor::entRefToString(EntRef arg) {
  return std::visit(overload{[](Synonym& s) { return s.synonym; },
                             [](Underscore&) { return std::string("_"); },
                             [](QuoteIdent& s) {
                               // Remove quotes
                               return s.quote_ident.substr(
                                   1, s.quote_ident.size() - 2);
                             }},
                    arg);
}

std::optional<Synonym> QueryExecutor::getRefAsSynonym(Ref arg) {
  if (auto syn = std::get_if<Synonym>(&arg)) {
    return *syn;
  } else {
    return std::nullopt;
  }
}

bool QueryExecutor::isRefUnderscore(Ref arg) {
  return std::holds_alternative<Underscore>(arg);
}

//! Returns argument as a basic statementref or quoteident as string
std::optional<std::string> QueryExecutor::getRefAsBasic(Ref arg) {
  return std::visit(
      overload{[](Synonym& s) { return s.synonym; },
               [](Underscore&) { return std::string("_"); },
               [](StatementNumber& s) { return std::to_string(s); },
               [](QuoteIdent& s) {
                 // Remove quotes
                 return s.quote_ident.substr(1, s.quote_ident.size() - 2);
               }},
      arg);
}

std::vector<std::string> QueryExecutor::getSynonymsFromSelect(
    std::vector<Declaration*>* decls) {
  std::vector<std::string> result;
  for (const auto& decl : *decls) {
    result.push_back(decl->getSynonym().synonym);
  }
  return result;
}
