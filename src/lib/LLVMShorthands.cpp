/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * LLVMShorthands.cpp
 *
 *  Created on: 15.05.2017
 *      Author: philipp
 */

#include "LLVMShorthands.h"

bool isFunctionPointer(const llvm::Value *V) noexcept {
  if (V) {
    if (V->getType()->isPointerTy() &&
        V->getType()->getPointerElementType()->isFunctionTy()) {
      return true;
    }
    return false;
  }
  return false;
}

bool matchesSignature(const llvm::Function *F,
                      const llvm::FunctionType *FType) {
  FType->dump();
  if (F->getArgumentList().size() == FType->getNumParams() &&
      F->getReturnType() == FType->getReturnType()) {
    auto &arglist = F->getArgumentList();
    unsigned i = 0;
    for (auto &arg : arglist) {
      if (arg.getType() != FType->getParamType(i)) {
        return false;
      }
      ++i;
    }
    return true;
  }
  return false;
}

string llvmIRToString(const llvm::Value *V) {
  string IRBuffer;
  llvm::raw_string_ostream RSO(IRBuffer);
  V->print(RSO);
  if (auto Inst = llvm::dyn_cast<llvm::Instruction>(V)) {
    RSO << "=>ID:" << getMetaDataID(Inst);
  }
  RSO.flush();
  return IRBuffer;
}

vector<const llvm::Value *>
globalValuesUsedinFunction(const llvm::Function *F) {
  vector<const llvm::Value *> globals_used;
  for (auto &BB : *F) {
    for (auto &I : BB) {
      for (auto &Op : I.operands()) {
        if (const llvm::GlobalValue *G =
                llvm::dyn_cast<llvm::GlobalValue>(Op)) {
          globals_used.push_back(G);
        }
      }
    }
  }
  return globals_used;
}

string getMetaDataID(const llvm::Instruction *I) {
  return llvm::cast<llvm::MDString>(I->getMetadata(MetaDataKind)->getOperand(0))
      ->getString()
      .str();
}

const llvm::Argument *getNthFunctionArgument(const llvm::Function *F,
                                             unsigned argNo) {
  if (argNo < F->arg_size()) {
    unsigned current = 0;
    for (auto &A : F->args()) {
      if (argNo == current) {
        return &A;
      }
      ++current;
    }
  }
  return nullptr;
}

const llvm::Instruction *getNthInstruction(const llvm::Function *F,
                                           unsigned idx) {
  unsigned i = 0;
  for (auto &BB : *F) {
    for (auto &I : BB) {
      if (i == idx) {
        return &I;
      } else {
        ++i;
      }
    }
  }
  return nullptr;
}

const llvm::Module *getModuleFromVal(const llvm::Value *V) {
  if (const llvm::Argument *MA = llvm::dyn_cast<llvm::Argument>(V))
    return MA->getParent() ? MA->getParent()->getParent() : nullptr;

  if (const llvm::BasicBlock *BB = llvm::dyn_cast<llvm::BasicBlock>(V))
    return BB->getParent() ? BB->getParent()->getParent() : nullptr;

  if (const llvm::Instruction *I = llvm::dyn_cast<llvm::Instruction>(V)) {
    const llvm::Function *M =
        I->getParent() ? I->getParent()->getParent() : nullptr;
    return M ? M->getParent() : nullptr;
  }
  if (const llvm::GlobalValue *GV = llvm::dyn_cast<llvm::GlobalValue>(V))
    return GV->getParent();
  if (const auto *MAV = llvm::dyn_cast<llvm::MetadataAsValue>(V)) {
    for (const llvm::User *U : MAV->users())
      if (llvm::isa<llvm::Instruction>(U))
        if (const llvm::Module *M = getModuleFromVal(U))
          return M;
    return nullptr;
  }
  return nullptr;
}

size_t computeModuleHash(llvm::Module *M, bool considerIdentifier) {
  string SourceCode;
  if (considerIdentifier) {
    llvm::raw_string_ostream RSO(SourceCode);
    llvm::WriteBitcodeToFile(M, RSO);
    RSO.flush();
  } else {
    string Identifier = M->getModuleIdentifier();
    M->setModuleIdentifier("");
    llvm::raw_string_ostream RSO(SourceCode);
    llvm::WriteBitcodeToFile(M, RSO);
    RSO.flush();
    M->setModuleIdentifier(Identifier);
  }
  return hash<string>{}(SourceCode);
}