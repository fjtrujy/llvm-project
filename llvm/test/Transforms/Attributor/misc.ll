; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --function-signature --check-attributes
; RUN: opt -attributor -enable-new-pm=0 -attributor-manifest-internal  -attributor-max-iterations-verify -attributor-annotate-decl-cs -attributor-max-iterations=4 -S < %s | FileCheck %s --check-prefixes=CHECK,NOT_CGSCC_NPM,NOT_CGSCC_OPM,NOT_TUNIT_NPM,IS__TUNIT____,IS________OPM,IS__TUNIT_OPM
; RUN: opt -aa-pipeline=basic-aa -passes=attributor -attributor-manifest-internal  -attributor-max-iterations-verify -attributor-annotate-decl-cs -attributor-max-iterations=4 -S < %s | FileCheck %s --check-prefixes=CHECK,NOT_CGSCC_OPM,NOT_CGSCC_NPM,NOT_TUNIT_OPM,IS__TUNIT____,IS________NPM,IS__TUNIT_NPM
; RUN: opt -attributor-cgscc -enable-new-pm=0 -attributor-manifest-internal  -attributor-annotate-decl-cs -S < %s | FileCheck %s --check-prefixes=CHECK,NOT_TUNIT_NPM,NOT_TUNIT_OPM,NOT_CGSCC_NPM,IS__CGSCC____,IS________OPM,IS__CGSCC_OPM
; RUN: opt -aa-pipeline=basic-aa -passes=attributor-cgscc -attributor-manifest-internal  -attributor-annotate-decl-cs -S < %s | FileCheck %s --check-prefixes=CHECK,NOT_TUNIT_NPM,NOT_TUNIT_OPM,NOT_CGSCC_OPM,IS__CGSCC____,IS________NPM,IS__CGSCC_NPM
;
; Mostly check we do not crash on these uses

define internal void @internal(void (i8*)* %fp) {
;
;
; CHECK-LABEL: define {{[^@]+}}@internal
; CHECK-SAME: (void (i8*)* nonnull [[FP:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[A:%.*]] = alloca i32, align 4
; CHECK-NEXT:    call void @foo(i32* noalias nocapture nofree noundef nonnull writeonly align 4 dereferenceable(4) [[A]]) [[ATTR1:#.*]]
; CHECK-NEXT:    call void [[FP]](i8* bitcast (void (i32*)* @foo to i8*))
; CHECK-NEXT:    call void @callback1(void (i32*)* noundef nonnull @foo)
; CHECK-NEXT:    call void @callback2(void (i8*)* noundef bitcast (void (i32*)* @foo to void (i8*)*))
; CHECK-NEXT:    call void @callback2(void (i8*)* nonnull [[FP]])
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[A]] to i8*
; CHECK-NEXT:    call void [[FP]](i8* [[TMP1]])
; CHECK-NEXT:    ret void
;
entry:
  %a = alloca i32, align 4
  %tmp = bitcast i32* %a to i8*
  call void @foo(i32* nonnull %a)
  call void %fp(i8* bitcast (void (i32*)* @foo to i8*))
  call void @callback1(void (i32*)* nonnull @foo)
  call void @callback2(void (i8*)* bitcast (void (i32*)* @foo to void (i8*)*))
  call void @callback2(void (i8*)* %fp)
  %tmp1 = bitcast i32* %a to i8*
  call void %fp(i8* %tmp1)
  ret void
}

define void @external(void (i8*)* %fp) {
;
;
; CHECK-LABEL: define {{[^@]+}}@external
; CHECK-SAME: (void (i8*)* [[FP:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[A:%.*]] = alloca i32, align 4
; CHECK-NEXT:    call void @foo(i32* noalias nocapture nofree noundef nonnull writeonly align 4 dereferenceable(4) [[A]]) [[ATTR1]]
; CHECK-NEXT:    call void @callback1(void (i32*)* noundef nonnull @foo)
; CHECK-NEXT:    call void @callback2(void (i8*)* noundef bitcast (void (i32*)* @foo to void (i8*)*))
; CHECK-NEXT:    call void @callback2(void (i8*)* [[FP]])
; CHECK-NEXT:    call void [[FP]](i8* bitcast (void (i32*)* @foo to i8*))
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[A]] to i8*
; CHECK-NEXT:    call void [[FP]](i8* [[TMP1]])
; CHECK-NEXT:    call void @internal(void (i8*)* nonnull [[FP]])
; CHECK-NEXT:    ret void
;
entry:
  %a = alloca i32, align 4
  %tmp = bitcast i32* %a to i8*
  call void @foo(i32* nonnull %a)
  call void @callback1(void (i32*)* nonnull @foo)
  call void @callback2(void (i8*)* bitcast (void (i32*)* @foo to void (i8*)*))
  call void @callback2(void (i8*)* %fp)
  call void %fp(i8* bitcast (void (i32*)* @foo to i8*))
  %tmp1 = bitcast i32* %a to i8*
  call void %fp(i8* %tmp1)
  call void @internal(void (i8*)* %fp)
  ret void
}

define internal void @foo(i32* %a) {
;
; IS__TUNIT____: Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
; IS__TUNIT____-LABEL: define {{[^@]+}}@foo
; IS__TUNIT____-SAME: (i32* nocapture nofree nonnull writeonly align 4 dereferenceable(4) [[A:%.*]]) [[ATTR0:#.*]] {
; IS__TUNIT____-NEXT:  entry:
; IS__TUNIT____-NEXT:    store i32 0, i32* [[A]], align 4
; IS__TUNIT____-NEXT:    ret void
;
; IS__CGSCC____: Function Attrs: argmemonly nofree norecurse nosync nounwind willreturn writeonly
; IS__CGSCC____-LABEL: define {{[^@]+}}@foo
; IS__CGSCC____-SAME: (i32* nocapture nofree nonnull writeonly align 4 dereferenceable(4) [[A:%.*]]) [[ATTR0:#.*]] {
; IS__CGSCC____-NEXT:  entry:
; IS__CGSCC____-NEXT:    store i32 0, i32* [[A]], align 4
; IS__CGSCC____-NEXT:    ret void
;
entry:
  store i32 0, i32* %a
  ret void
}

declare void @callback1(void (i32*)*)
declare void @callback2(void (i8*)*)
