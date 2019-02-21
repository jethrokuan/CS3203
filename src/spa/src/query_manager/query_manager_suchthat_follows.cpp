#include <cassert>
#include <string>
#include <vector>
#include "query_evaluator/pql/declaration.h"
#include "query_manager/query_manager.h"
#include "utils/utils.h"

using namespace QE;

std::vector<std::string> QueryManager::handleFollowsTSuchThat(
    Query* query, std::optional<Synonym> arg1AsSynonym,
    std::optional<Synonym> arg2AsSynonym, bool arg1InSelect, bool arg2InSelect,
    bool arg1IsUnderscore, bool arg2IsUnderscore,
    std::optional<std::string> arg1AsBasic,
    std::optional<std::string> arg2AsBasic) {
  if (arg1AsSynonym && arg1InSelect && arg2AsBasic) {
    // Case 1: Selected variable is in this such_that, left argument
    // Follows(s, 3)
    return pkb->getBeforeLineS(*arg2AsBasic);
  } else if (arg2AsSynonym && arg2InSelect && arg1AsBasic) {
    // Case 2: Selected variable is in this such_that, left argument
    // Follows(3, s)
    return pkb->getFollowingLineS(*arg1AsBasic);
  } else if (arg1AsSynonym && arg1InSelect && arg2IsUnderscore) {
    // Case 3: Selected variable is in this such_that, left argument, right arg
    // underscore Follows(s, _)
    // Need to find all selected things and then run the correct follows fx
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (!pkb->getFollowingLineS(de).empty()) {
        results.push_back(de);
      }
    }
    return results;
  } else if (arg2AsSynonym && arg2InSelect && arg1IsUnderscore) {
    // Case 4: Selected variable is in this such_that, right argument, left arg
    // underscore Follows(_, s)
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (!pkb->getBeforeLineS(de).empty()) {
        results.push_back(de);
      }
    }
    return results;
  } else if (arg1AsSynonym && arg2AsSynonym && arg1InSelect) {
    // Case 5: Selected variable is in this such_that, left argument, right arg
    // also is a variable, Follows(s, p)
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg2AsSynonym)
                            ->getDesignEntity();
    std::cout << "Right arg DE: " << getDesignEntityString(right_arg_de)
              << "\n";
    auto all_unselected_designentities = getSelect(right_arg_de);
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLineS(de, unselect_de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  } else if (arg1AsSynonym && arg2AsSynonym && arg2InSelect) {
    // Case 6: Selected variable is in this such_that, right argument, right arg
    // also is a variable, Follows(p, s)
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();

    std::cout << "Left arg DE: " << getDesignEntityString(left_arg_de) << "\n";
    auto all_unselected_designentities = getSelect(left_arg_de);
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLineS(unselect_de, de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }
  std::cout << "No cases matched - this is a problem\n";
  return std::vector<std::string>();
}