// RUN: %clang_cc1 -mllvm -emptyline-comment-coverage=false -fprofile-instrument=clang -fcoverage-mapping -dump-coverage-mapping -emit-llvm-only -std=c++1z -triple %itanium_abi_triple -main-file-name switch.cpp %s | FileCheck %s

// CHECK: foo
void foo(int i) {   // CHECK-NEXT: File 0, [[@LINE]]:17 -> [[@LINE+8]]:2 = #0
  switch(i) {       // CHECK-NEXT: Gap,File 0, [[@LINE]]:13 -> [[@LINE+4]]:10 = 0
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:11 = #2
    return;
  case 2:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:10 = #3
    break;          // CHECK-NEXT: Gap,File 0, [[@LINE]]:10 -> [[@LINE+2]]:3 = #1
  }
  int x = 0;        // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:2 = #1
}

int nop() { return 0; }

                    // CHECK: bar
void bar(int i) {   // CHECK-NEXT: File 0, [[@LINE]]:17 -> [[@LINE+20]]:2 = #0
  switch (i)
    ;               // CHECK-NEXT: File 0, [[@LINE]]:5 -> [[@LINE]]:6 = 0

  switch (i) {      // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+16]]:2 = #1
  }

  switch (i)        // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+13]]:2 = #2
    nop();          // CHECK-NEXT: File 0, [[@LINE]]:5 -> [[@LINE]]:10 = 0

  switch (i)        // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+10]]:2 = #3
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:10 = #5
    nop();

  switch (i) {      // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+6]]:2 = #4
    nop();          // CHECK-NEXT: Gap,File 0, [[@LINE-1]]:14 -> [[@LINE+2]]:10 = 0
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:10 = #7
    nop();
  }
  nop();            // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:2 = #6
}

                    // CHECK: baz
void baz() {        // CHECK-NEXT: File 0, [[@LINE]]:12 -> [[@LINE+5]]:2 = #0
  switch (int i = true ? nop()  // CHECK: [[@LINE]]:26 -> [[@LINE]]:31 = #2
                       : nop(); // CHECK-NEXT: [[@LINE]]:26 -> [[@LINE]]:31 = (#0 - #2)
          i) {}
  nop();            // CHECK-NEXT: [[@LINE]]:3 -> [[@LINE+1]]:2 = #1
}

                    // CHECK-NEXT: main
int main() {        // CHECK-NEXT: File 0, [[@LINE]]:12 -> [[@LINE+35]]:2 = #0
  int i = 0;
  switch(i) {       // CHECK-NEXT: Gap,File 0, [[@LINE]]:13 -> [[@LINE+8]]:10 = 0
  case 0:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+2]]:10 = #2
    i = 1;
    break;
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+2]]:10 = #3
    i = 2;
    break;
  default:          // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:10 = #4
    break;          // CHECK-NEXT: File 0, [[@LINE]]:10 -> [[@LINE+2]]:3 = #1
  }
  switch(i) {       // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+23]]:2 = #1
  case 0:           // CHECK-NEXT: Gap,File 0, [[@LINE-1]]:13 -> [[@LINE+6]]:10 = 0
    i = 1;          // CHECK-NEXT: File 0, [[@LINE-1]]:3 -> [[@LINE+1]]:10 = #6
    break;
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+3]]:10 = #7
    i = 2;
  default:          // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:10 = (#7 + #8)
    break;          // CHECK-NEXT: File 0, [[@LINE]]:10 -> [[@LINE+3]]:3 = #5
  }
                    // CHECK-NEXT: File 0, [[@LINE+1]]:3 -> [[@LINE+14]]:2 = #5
  switch(i) {       // CHECK-NEXT: Gap,File 0, [[@LINE]]:13 -> [[@LINE+6]]:11 = 0
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+5]]:11 = #10
  case 2:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+4]]:11 = (#10 + #11)
    i = 11;
  case 3:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+2]]:11 = ((#10 + #11) + #12)
  case 4:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:11 = (((#10 + #11) + #12) + #13)
    i = 99;
  }

  foo(1);           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+3]]:11 = #9
  bar(1);
  baz();
  return 0;
}

                     // CHECK: pr44011
int pr44011(int i) { // CHECK-NEXT: File 0, [[@LINE]]:20 -> {{.*}}:2 = #0
  switch (i) {       // CHECK-NEXT: Gap,File 0, [[@LINE]]:14 -> [[@LINE+6]]:13 = 0

  case 1:            // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:13 = #2
    return 0;

  default:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:13 = #3
    return 1;
  }
} // A region for counter #1 is missing due to the missing return.


// FIXME: End location for "case 1" shouldn't point at the end of the switch.
                         // CHECK: fallthrough
int fallthrough(int i) { // CHECK-NEXT: File 0, [[@LINE]]:24 -> [[@LINE+12]]:2 = #0
  switch(i) {       // CHECK-NEXT: Gap,File 0, [[@LINE]]:13 -> [[@LINE+9]]:10 = 0
  case 1:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+8]]:10 = #2
    i = 23;
  case 2:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+2]]:10 = (#2 + #3)
    i = 11;
    break;
  case 3:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+3]]:10 = #4
  case 4:           // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+2]]:10 = (#4 + #5)
    i = 99;
    break;
  }
}

void abort(void) __attribute((noreturn));
                   // CHECK: noret
int noret(int x) { // CHECK-NEXT: File 0, [[@LINE]]:18 -> [[@LINE+9]]:2
  switch (x) {     // CHECK-NEXT: Gap,File 0, [[@LINE]]:14 -> [[@LINE+6]]:14 = 0
  default:         // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:12
    abort();
  case 1:         // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:13
    return 5;
  case 2:         // CHECK-NEXT: File 0, [[@LINE]]:3 -> [[@LINE+1]]:14
    return 10;
  }
}
