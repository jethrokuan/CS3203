#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

namespace QE {

//! Represents an error that occurs during parsing of PQL tokens into a Query
class PQLParseException : public std::runtime_error {
 public:
  PQLParseException(const std::string& msg = "") : std::runtime_error(msg) {}
};

//! Represents an error that occurs during the creation of PQL tokens
class PQLTokenizeException : public std::runtime_error {
 public:
  PQLTokenizeException(const std::string& msg = "") : std::runtime_error(msg) {}
};

//! Represents an error that occurs during the validation of a PQL query
class PQLValidationException : public std::runtime_error {
 public:
  PQLValidationException(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

}  // namespace QE
