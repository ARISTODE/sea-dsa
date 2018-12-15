#pragma once

#include "llvm/IR/CallSite.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>

namespace sea_dsa {

class Graph;

class DSAllocSite {
public:
  enum StepKind { Local, BottomUp, TopDown };
  using Step = std::pair<StepKind, const llvm::Function *>;
  using CallEdge = std::pair<StepKind, llvm::ImmutableCallSite>;
  using CallPath = std::vector<Step>;
  using CallPaths = std::vector<CallPath>;

private:
  /// llvm::Value corresponding described by this allocation site
  const llvm::Value &m_value;

  /// the graph that owns this allocation site descriptor.
  /// Not necessarily the graph of a function in which m_value appears.
  const sea_dsa::Graph &m_owner;

  /// Optional call paths describing current evolution of this allocation site
  CallPaths m_callPaths;

public:
  /// \brief Return the llmv::Value corresponding to this DSAllocSite
  const llvm::Value &getValue() const {
    return m_value;
  }

  /// \brief Return the owning graph
  const Graph &getOwner() const {
    return m_owner;
  }

  const CallPaths& getCallPaths() const {return m_callPaths;}

  bool hasCallPaths() const { return !m_callPaths.empty(); }

  void print(llvm::raw_ostream &os = llvm::errs()) const;
  void printCallPaths(llvm::raw_ostream &os = llvm::errs()) const;

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream& os,
                                       const DSAllocSite& AS) {
    AS.print(os);
    return os;
  }

  bool operator<(const DSAllocSite &other) const {
    assert(m_owner == other.m_owner);
    return &m_value < &other.m_value;
  }

  bool operator==(const DSAllocSite &other) const {
    assert(m_owner == other.m_owner);
    return &m_value == &other.m_value;
  }

  bool operator!=(const DSAllocSite &other) const {
    return !(*this == other);
  }

  bool operator>(const DSAllocSite &other) const {
    return (other != *this) && !(other < *this);
  }

private:
  friend sea_dsa::Graph;

  /// private constructor exposed only to sea_dsa::Graph
  DSAllocSite(const sea_dsa::Graph &g, const llvm::Value &v)
      : m_value(v), m_owner(g) {}
};

}