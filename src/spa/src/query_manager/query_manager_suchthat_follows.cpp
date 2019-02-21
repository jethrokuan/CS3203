#include <cassert>
#include <string>
#include <variant>
#include <vector>
#include "query_evaluator/pql/declaration.h"
#include "query_manager/query_manager.h"
#include "utils/utils.h"

using namespace QE;

std::variant<bool, std::vector<std::string>>
QueryManager::handleFollowsTSuchThat(Query* query,
                                     std::optional<Synonym> arg1AsSynonym,
                                     std::optional<Synonym> arg2AsSynonym,
                                     bool arg1InSelect, bool arg2InSelect,
                                     bool arg1IsUnderscore,
                                     bool arg2IsUnderscore,
                                     std::optional<std::string> arg1AsBasic,
                                     std::optional<std::string> arg2AsBasic) {
  if (arg1InSelect || arg2InSelect) {
    // At least something in such_that is selected
    return handleFollowsTSuchThatSelected(
        query, arg1AsSynonym, arg2AsSynonym, arg1InSelect, arg2InSelect,
        arg1IsUnderscore, arg2IsUnderscore, arg1AsBasic, arg2AsBasic);
  } else {
    // Handle cases where nothing in such that is selected
    return handleFollowsTSuchThatNotSelected(
        query, arg1AsSynonym, arg2AsSynonym, arg1IsUnderscore, arg2IsUnderscore,
        arg1AsBasic, arg2AsBasic);
  }

  std::cout << "No cases matched - this is a problem\n";
  assert(false);
}

std::variant<bool, std::vector<std::string>>
QueryManager::handleFollowsTSuchThatSelected(
    Query* query, std::optional<Synonym> arg1AsSynonym,
    std::optional<Synonym> arg2AsSynonym, bool arg1InSelect, bool arg2InSelect,
    bool arg1IsUnderscore, bool arg2IsUnderscore,
    std::optional<std::string> arg1AsBasic,
    std::optional<std::string> arg2AsBasic) {
  if (arg1AsSynonym && arg1InSelect && arg2AsBasic) {
    // Case 1: Selected variable is in this such_that, left argument
    // Follows*(s, 3)
    return pkb->getBeforeLineS(*arg2AsBasic);
  } else if (arg2AsSynonym && arg2InSelect && arg1AsBasic) {
    // Case 2: Selected variable is in this such_that, left argument
    // Follows*(3, s)
    return pkb->getFollowingLineS(*arg1AsBasic);
  } else if (arg1AsSynonym && arg1InSelect && arg2IsUnderscore) {
    // Case 3: Selected variable is in this such_that, left argument, right arg
    // underscore Follows*(s, _)
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
    // underscore Follows*(_, s)
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
    // also is a variable, Follows*(s, p)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return std::vector<std::string>();
    }
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg2AsSynonym)
                            ->getDesignEntity();
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
    // also is a variable, Follows*(p, s)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return std::vector<std::string>();
    }
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
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
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}

std::variant<bool, std::vector<std::string>>
QueryManager::handleFollowsTSuchThatNotSelected(
    Query* query, std::optional<Synonym> arg1AsSynonym,
    std::optional<Synonym> arg2AsSynonym, bool arg1IsUnderscore,
    bool arg2IsUnderscore, std::optional<std::string> arg1AsBasic,
    std::optional<std::string> arg2AsBasic) {
  if (arg1IsUnderscore && arg2IsUnderscore) {
    // Case 7: Follows*(_, _)
    // TODO: Need to check PKB for existence of any follows relationships
    assert(false);
  } else if (arg1AsSynonym && arg2AsSynonym) {
    // Case 8: Selected variable is NOT in this such_that, need to check for
    // truth/falsity overall
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return false;
    }
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg1AsSynonym)
                            ->getDesignEntity();

    auto all_left_designentities = getSelect(left_arg_de);
    auto all_right_designentities = getSelect(right_arg_de);
    std::vector<std::string> results;
    for (auto left_de : all_left_designentities) {
      for (auto right_de : all_right_designentities) {
        // Any satisfied relation would mean this clause is true overall
        if (pkb->isLineFollowLineS(left_de, right_de)) {
          return true;
        }
      }
    }
    return false;
  } else if (arg1AsSynonym && arg2AsBasic) {
    // Case 9: Selected variable is NOT in this such_that, left argument
    // Follows(s1, 3)
    return !(pkb->getBeforeLineS(*arg2AsBasic).empty());
  } else if (arg2AsSynonym && arg1AsBasic) {
    // Case 10: Selected variable is NOT in this such_that, right argument
    // Follows(3, s1)
    return !(pkb->getFollowingLineS(*arg1AsBasic).empty());
  } else if (arg1AsBasic && arg2IsUnderscore) {
    // Case 11: Selected variable is NOT in this such_that, left basic, right
    // underscore: Follows(3, _)
    return !(pkb->getFollowingLineS(*arg1AsBasic).empty());
  } else if (arg2AsBasic && arg1IsUnderscore) {
    // Case 12: Selected variable is NOT in this such_that, right basic, left
    // underscore: Follows(_, 3)
    return !(pkb->getBeforeLineS(*arg2AsBasic).empty());
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}