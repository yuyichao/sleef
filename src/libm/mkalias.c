//          Copyright Naoki Shibata 2010 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "funcproto.h"

int main(int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "0") == 0) exit(0);

  if (argc < 7) {
    fprintf(stderr, "Usage : %s <vector width> <vector FP type> <vector int type> <mangled ISA> <extension> <atr prefix>\n", argv[0]);
    exit(-1);
  }

  int vw = atoi(argv[1]);
  int fptype = vw >= 0 ? 0 : 1;
  vw = vw < 0 ? -vw : vw;
  char *isaname = argc == 7 ? argv[5] : "";
  char *atrPrefix = argv[6];

  static char *argType2[] = {
    "a0", "a0, a1", "a0", "a0, a1",
    "a0", "a0, a1, a2", "a0", "a0", "a0"
  };
  static char *typeSpec[] = { "d", "f" };

  static char returnType[9][1000];
  static char argType0[9][1000];
  static char argType1[9][1000];
  
  sprintf(returnType[0], "%s", argv[2]);
  sprintf(returnType[1], "%s", argv[2]);
  sprintf(returnType[2], "%s", fptype ? "vfloat2" : "vdouble2");
  sprintf(returnType[3], "%s", argv[2]);
  sprintf(returnType[4], "%s", argv[3]);
  sprintf(returnType[5], "%s", argv[2]);
  sprintf(returnType[6], "%s", fptype ? "vfloat2" : "vdouble2");
  sprintf(returnType[7], "int");
  sprintf(returnType[8], "void *");

  sprintf(argType0[0], "%s", argv[2]);
  sprintf(argType0[1], "%s, %s", argv[2], argv[2]);
  sprintf(argType0[2], "%s", argv[2]);
  sprintf(argType0[3], "%s, %s", argv[2], argv[3]);
  sprintf(argType0[4], "%s", argv[2]);
  sprintf(argType0[5], "%s, %s, %s", argv[2], argv[2], argv[2]);
  sprintf(argType0[6], "%s", argv[2]);
  sprintf(argType0[7], "int");
  sprintf(argType0[8], "int");

  sprintf(argType1[0], "%s a0", argv[2]);
  sprintf(argType1[1], "%s a0, %s a1", argv[2], argv[2]);
  sprintf(argType1[2], "%s a0", argv[2]);
  sprintf(argType1[3], "%s a0, %s a1", argv[2], argv[3]);
  sprintf(argType1[4], "%s a0", argv[2]);
  sprintf(argType1[5], "%s a0, %s a1, %s a2", argv[2], argv[2], argv[2]);
  sprintf(argType1[6], "%s a0", argv[2]);
  sprintf(argType1[7], "int a0");
  sprintf(argType1[8], "int a0");

  //

  if (fptype == 0) {
    printf("#ifdef __SLEEFSIMDDP_C__\n");
  } else {
    printf("#ifdef __SLEEFSIMDSP_C__\n");
  }
  printf("#ifdef ENABLE_ALIAS\n");

  if (argc == 7) {
    for(int i=0;funcList[i].name != NULL;i++) {
      if (funcList[i].ulp >= 0) {
	printf("EXPORT CONST %s Sleef_%s%s%s%d_u%02d(%s) __attribute__((alias(\"Sleef_%s%s%s%d_u%02d%s\")));\n",
	       returnType[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw, funcList[i].ulp,
	       argType0[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw, funcList[i].ulp, isaname
	       );
      } else {
	printf("EXPORT CONST %s Sleef_%s%s%s%d(%s) __attribute__((alias(\"Sleef_%s%s%s%d_%s\")));\n",
	       returnType[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw,
	       argType0[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw, isaname
	       );
      }
    }

    printf("\n");
  }
  
  printf("#else // #ifdef ENABLE_ALIAS\n");

  if (argc == 7) {
    for(int i=0;funcList[i].name != NULL;i++) {
      if (funcList[i].ulp >= 0) {
	printf("EXPORT CONST %s Sleef_%s%s%s%d_u%02d(%s) { return Sleef_%s%s%s%d_u%02d%s(%s); }\n",
	       returnType[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw, funcList[i].ulp,
	       argType1[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw, funcList[i].ulp, isaname,
	       argType2[funcList[i].funcType]
	       );
      } else {
	printf("EXPORT CONST %s Sleef_%s%s%s%d(%s) { return Sleef_%s%s%s%d_%s(%s); }\n",
	       returnType[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw,
	       argType1[funcList[i].funcType],
	       (funcList[i].flags & DET) == 0 ? "" : atrPrefix,
	       funcList[i].name, typeSpec[fptype], vw, isaname,
	       argType2[funcList[i].funcType]
	       );
      }
    }

    printf("\n");
  }

  printf("#endif // #ifdef ENABLE_ALIAS\n");
  if (fptype == 0) {
    printf("#endif // #ifdef __SLEEFSIMDDP_C__\n");
  } else {
    printf("#endif // #ifdef __SLEEFSIMDSP_C__\n");
  }

  exit(0);
}
