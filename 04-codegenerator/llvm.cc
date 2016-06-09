#include <cstdlib>
#include <iostream>
#include <string>

#define MODULE_NAME test

int main (int argc, char const *argv[]) {

  char *error = nullptr;
  LLVMLinkInJIT();
  LLVMInitializeNativeTarget();
  LLVMModuleRef mod = LLVMModuleCreateWithName(MODULE_NAME);

  LLVMTypeRef fac_args[] = { LLVMInt32Type() };
  LLVMValueRef fac = LLVMAddFunction(mod, "main", LLVMFunctionType(LLVMVoidType(), nullptr, 0, 0));
  LLVMSetFunctionCallConv(fac, LLVMCCallConv);
  LLVMValueRef n = LLVMGetParam(fac, 0);

  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(fac, "entry");
  LLVMBasicBlockRef iftrue = LLVMAppendBasicBlock(fac, "iftrue");
  LLVMBasicBlockRef iffalse = LLVMAppendBasicBlock(fac, "iffalse");
  LLVMBasicBlockRef end = LLVMAppendBasicBlock(fac, "end");
  LLVMBuilderRef builder = LLVMCreateBuilder();

  LLVMPositionBuilderAtEnd(builder, entry);
  LLVMValueRef If = LLVMBuildICmp(builder, LLVMIntEQ, n, LLVMConstInt(LLVMInt32Type(), 0, 0), "n == 0");
  LLVMBuildCondBr(builder, If, iftrue, iffalse);

  LLVMPositionBuilderAtEnd(builder, iftrue);
  LLVMValueRef res_iftrue = LLVMConstInt(LLVMInt32Type(), 1, 0);
  LLVMBuildBr(builder, end);

  LLVMPositionBuilderAtEnd(builder, iffalse);
  LLVMValueRef n_minus = LLVMBuildSub(builder, n, LLVMConstInt(LLVMInt32Type(), 1, 0), "n - 1");
  LLVMValueRef call_fac_args[] = {n_minus};
  LLVMValueRef call_fac = LLVMBuildCall(builder, fac, call_fac_args, 1, "fac(n - 1)");
  LLVMValueRef res_iffalse = LLVMBuildMul(builder, n, call_fac, "n * fac(n - 1)");
  LLVMBuildBr(builder, end);

  LLVMPositionBuilderAtEnd(builder, end);
  LLVMValueRef res = LLVMBuildPhi(builder, LLVMInt32Type(), "result");
  LLVMValueRef phi_vals[] = {res_iftrue, res_iffalse};
  LLVMBasicBlockRef phi_blocks[] = {iftrue, iffalse};
  LLVMAddIncoming(res, phi_vals, phi_blocks, 2);
  LLVMBuildRet(builder, res);

  LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
  LLVMDisposeMessage(error);
}	