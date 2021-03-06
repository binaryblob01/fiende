
/*  A Bison parser, made from parser.yy
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	CLASS	257
#define	WRAPCLASSDECL	258
#define	WRAPMETHODDECL	259
#define	SIGNALDECL	260
#define	ENDCLASSDEF	261
#define	CLASSSECTION	262
#define	DOCSECTION	263
#define	PRIVATESECTION	264
#define	IMPLSECTION	265
#define	CONST	266
#define	VOLATILE	267
#define	STATIC	268
#define	FIXMEGTKCONST	269
#define	WRAPCTORCAST	270
#define	WRAPCTORDEFAULT	271
#define	WRAPDTOR	272
#define	WRAPMEMBERDECL	273
#define	SYMNAME	274
#define	NUMBER	275
#define	TYPESPECIFIER	276
#define	DUMMY	277

#line 3 "parser.yy"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include "gtkmmproc.h"

namespace std {}
using namespace std;  

WidgetDef noWidget;
WidgetDef *currentWidget=&noWidget;
list<WidgetDef*> widgets;

OutputChannel *channel;

extern "C"
{
int yylex();
int yyparse();
}

extern FILE *yyout, *yyin;


#line 30 "parser.yy"
typedef union {
  int ival;
  string* sval;
  Argument* vval;
  ArgList* aval;
  FunctionDef* fval;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		132
#define	YYFLAG		-32768
#define	YYNTBASE	34

#define YYTRANSLATE(x) ((unsigned)(x) <= 277 ? yytranslate[x] : 57)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    33,     2,     2,     2,    28,     2,    24,
    26,    27,     2,    25,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    29,
    32,    30,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    31,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     5,     7,     9,    11,    13,    15,    17,    19,
    36,    55,    57,    60,    62,    64,    66,    68,    70,    72,
    75,    78,    81,    84,    86,    91,    95,    99,   101,   103,
   105,   107,   110,   112,   116,   118,   121,   126,   130,   132,
   137,   143,   149,   151,   154,   161,   169,   182,   184,   188,
   190,   194,   196,   207,   220,   233,   238,   240,   242,   244,
   246,   248,   250
};

static const short yyrhs[] = {    35,
    34,     0,    35,     0,    36,     0,    51,     0,    49,     0,
    54,     0,    50,     0,    55,     0,    56,     0,     4,    24,
    20,    25,    20,    25,    20,    25,    20,    25,    20,    25,
    20,    25,    20,    26,     0,     4,    24,    20,    25,    20,
    25,    20,    25,    20,    25,    20,    25,    20,    25,    20,
    25,    20,    26,     0,    22,     0,    37,    22,     0,    12,
     0,    13,     0,    15,     0,    20,     0,    41,     0,    37,
     0,    40,    27,     0,    40,    28,     0,    40,    38,     0,
    38,    39,     0,    39,     0,    20,    29,    42,    30,     0,
    20,    29,    30,     0,    42,    25,    40,     0,    40,     0,
    20,     0,    21,     0,    31,     0,    44,    43,     0,    43,
     0,    24,    44,    26,     0,    40,     0,    40,    20,     0,
    40,    20,    32,    44,     0,    46,    25,    45,     0,    45,
     0,    40,    20,    24,    26,     0,    40,    20,    24,    46,
    26,     0,    40,    41,    24,    46,    26,     0,    47,     0,
    47,    12,     0,     5,    24,    48,    25,    48,    26,     0,
     5,    24,    48,    25,    14,    48,    26,     0,    19,    24,
    53,    25,    20,    25,    20,    25,    40,    25,    40,    26,
     0,     7,     0,    52,    31,    20,     0,    20,     0,    53,
    31,    20,     0,    20,     0,     6,    24,    33,    20,    33,
    25,    52,    25,    48,    26,     0,     6,    24,    33,    20,
    33,    25,    52,    25,    48,    25,    20,    26,     0,     6,
    24,    33,    20,    33,    25,    52,    25,    48,    25,    48,
    26,     0,     8,    24,    20,    26,     0,     8,     0,    11,
     0,     9,     0,    10,     0,    16,     0,    17,     0,    18,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    51,    52,    55,    56,    57,    58,    59,    60,    61,    87,
   104,   124,   128,   137,   141,   145,   155,   159,   163,   169,
   174,   179,   188,   197,   207,   215,   222,   229,   235,   237,
   239,   246,   252,   256,   263,   267,   271,   283,   288,   296,
   300,   304,   310,   311,   318,   329,   342,   360,   371,   376,
   383,   388,   436,   445,   455,   467,   472,   476,   480,   484,
   490,   494,   498
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","CLASS",
"WRAPCLASSDECL","WRAPMETHODDECL","SIGNALDECL","ENDCLASSDEF","CLASSSECTION","DOCSECTION",
"PRIVATESECTION","IMPLSECTION","CONST","VOLATILE","STATIC","FIXMEGTKCONST","WRAPCTORCAST",
"WRAPCTORDEFAULT","WRAPDTOR","WRAPMEMBERDECL","SYMNAME","NUMBER","TYPESPECIFIER",
"DUMMY","'('","','","')'","'*'","'&'","'<'","'>'","'|'","'='","'\\\"'","input",
"classwrap","wrapdef","typespecifiers","cv_qualifier","identifier","typename",
"template_id","template_argument_list","value_sym","value_list","cpp_argument",
"cpp_arglist","cpp_func_id","cpp_func","method_decl","member_decl","endclassdef",
"sig_attrib_list","member_attrib_list","signal_decl","section_decl","ctor_decl", NULL
};
#endif

static const short yyr1[] = {     0,
    34,    34,    35,    35,    35,    35,    35,    35,    35,    36,
    36,    37,    37,    38,    38,    38,    39,    39,    39,    40,
    40,    40,    40,    40,    41,    41,    42,    42,    43,    43,
    43,    44,    44,    44,    45,    45,    45,    46,    46,    47,
    47,    47,    48,    48,    49,    49,    50,    51,    52,    52,
    53,    53,    54,    54,    54,    55,    55,    55,    55,    55,
    56,    56,    56
};

static const short yyr2[] = {     0,
     2,     1,     1,     1,     1,     1,     1,     1,     1,    16,
    18,     1,     2,     1,     1,     1,     1,     1,     1,     2,
     2,     2,     2,     1,     4,     3,     3,     1,     1,     1,
     1,     2,     1,     3,     1,     2,     4,     3,     1,     4,
     5,     5,     1,     2,     6,     7,    12,     1,     3,     1,
     3,     1,    10,    12,    12,     4,     1,     1,     1,     1,
     1,     1,     1
};

static const short yydefact[] = {     0,
     0,     0,     0,    48,    57,    59,    60,    58,    61,    62,
    63,     0,     2,     3,     5,     7,     4,     6,     8,     9,
     0,     0,     0,     0,     0,     1,     0,    14,    15,    16,
    17,    12,    19,     0,    24,     0,    18,    43,     0,     0,
     0,    52,     0,     0,     0,    13,    23,     0,    20,    21,
    22,     0,    44,     0,     0,    56,     0,     0,     0,    26,
    28,     0,     0,     0,     0,     0,     0,     0,    51,     0,
     0,    25,    40,    35,    39,     0,     0,     0,    45,     0,
     0,     0,    27,    36,     0,    41,    42,    46,    50,     0,
     0,     0,     0,    38,     0,     0,     0,     0,    29,    30,
     0,    31,    33,    37,     0,    49,     0,     0,     0,    32,
     0,    53,     0,     0,    34,    17,     0,     0,     0,    54,
    55,    47,     0,     0,     0,     0,    10,     0,    11,     0,
     0,     0
};

static const short yydefgoto[] = {    26,
    13,    14,    33,    34,    35,    36,    37,    62,   103,   104,
    75,    76,    38,    39,    15,    16,    17,    90,    43,    18,
    19,    20
};

static const short yypact[] = {    70,
    11,    18,    40,-32768,    46,-32768,-32768,-32768,-32768,-32768,
-32768,    60,    70,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    72,    99,    61,    79,    85,-32768,    88,-32768,-32768,-32768,
    68,-32768,    94,    63,-32768,    -4,-32768,   106,   111,   100,
    98,-32768,    30,   115,    -8,-32768,-32768,   -14,-32768,-32768,
-32768,   107,-32768,    95,   104,-32768,   118,   119,   116,-32768,
    45,     1,    78,    99,    99,   114,   117,   120,-32768,   123,
    99,-32768,-32768,    17,-32768,    77,   101,   121,-32768,   124,
   126,   125,    45,   122,    99,-32768,-32768,-32768,-32768,    31,
   127,   128,    -3,-32768,    99,   129,    99,   130,-32768,-32768,
    -3,-32768,-32768,    19,   103,-32768,    21,   131,    75,-32768,
   110,-32768,    99,   132,-32768,   -23,   133,    39,   136,-32768,
-32768,-32768,   135,   138,   108,   141,-32768,   137,-32768,   153,
   162,-32768
};

static const short yypgoto[] = {   164,
-32768,-32768,-32768,   -36,   134,   -44,   139,-32768,   -41,    64,
    81,   105,-32768,   -52,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768
};


#define	YYLAST		175


static const short yytable[] = {    51,
    61,    66,   120,    28,    29,    45,    30,    28,    29,    63,
    30,    31,    78,    32,    45,    48,    99,   100,    74,    74,
   101,    60,    49,    50,    51,    71,    83,   102,    28,    29,
    72,    30,    28,    29,    21,    30,    84,    51,    99,   100,
    74,    22,   105,    49,    50,   113,    51,    49,    50,   102,
    28,    29,   107,    30,    57,    95,    28,    29,   117,    30,
    58,    96,   110,    23,   122,    49,    50,   110,   118,    24,
    51,    49,    50,     1,     2,     3,     4,     5,     6,     7,
     8,    51,    31,    25,    32,     9,    10,    11,    12,    28,
    29,    27,    30,    40,    99,   100,    45,    31,    41,    32,
   115,    85,    86,    73,    42,   102,    28,    29,    65,    30,
    28,    29,    44,    30,    31,    46,    32,    53,    31,    55,
    32,    28,    29,    56,    30,    85,    87,   111,   112,   116,
    64,    32,   126,   127,    59,    54,    67,    68,    69,    79,
    70,    80,    82,    89,    81,    91,    88,    98,   106,    92,
   114,    97,   131,    93,   108,   123,   119,   125,   121,   124,
   128,   132,   129,   130,   109,    94,     0,    47,    77,     0,
     0,     0,     0,     0,    52
};

static const short yycheck[] = {    36,
    45,    54,    26,    12,    13,    29,    15,    12,    13,    24,
    15,    20,    65,    22,    29,    20,    20,    21,    63,    64,
    24,    30,    27,    28,    61,    25,    71,    31,    12,    13,
    30,    15,    12,    13,    24,    15,    20,    74,    20,    21,
    85,    24,    95,    27,    28,    25,    83,    27,    28,    31,
    12,    13,    97,    15,    25,    25,    12,    13,   111,    15,
    31,    31,   104,    24,    26,    27,    28,   109,   113,    24,
   107,    27,    28,     4,     5,     6,     7,     8,     9,    10,
    11,   118,    20,    24,    22,    16,    17,    18,    19,    12,
    13,    20,    15,    33,    20,    21,    29,    20,    20,    22,
    26,    25,    26,    26,    20,    31,    12,    13,    14,    15,
    12,    13,    25,    15,    20,    22,    22,    12,    20,    20,
    22,    12,    13,    26,    15,    25,    26,    25,    26,    20,
    24,    22,    25,    26,    20,    25,    33,    20,    20,    26,
    25,    25,    20,    20,    25,    20,    26,    20,    20,    25,
    20,    25,     0,    32,    25,    20,    25,    20,    26,    25,
    20,     0,    26,     0,   101,    85,    -1,    34,    64,    -1,
    -1,    -1,    -1,    -1,    36
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 10:
#line 88 "parser.yy"
{
#ifdef DEBUG
  cout << "WRAPCLASSDECL" << endl;
  cout << "1: " << *(yyvsp[-13].sval) <<endl;
  cout << "2: " << *(yyvsp[-11].sval) <<endl;
  cout << "3: " << *(yyvsp[-9].sval) <<endl;
  cout << "4: " << *(yyvsp[-7].sval) <<endl;
  cout << "5: " << *(yyvsp[-5].sval) <<endl;
  cout << "6: " << *(yyvsp[-3].sval) <<endl;
  cout << "7: " << *(yyvsp[-1].sval) <<endl;
#endif
  WidgetDef *w=new WidgetDef(yyvsp[-13].sval,yyvsp[-11].sval,yyvsp[-9].sval,yyvsp[-7].sval,yyvsp[-5].sval,yyvsp[-3].sval,yyvsp[-1].sval);
  currentWidget=w;
  widgets.insert(widgets.end(),w);
  w->print_decl_begin(yyout);
;
    break;}
case 11:
#line 105 "parser.yy"
{
#ifdef DEBUG
  cout << "WRAPCLASSDECL" << endl;
  cout << "1: " << *(yyvsp[-15].sval) <<endl;
  cout << "2: " << *(yyvsp[-13].sval) <<endl;
  cout << "3: " << *(yyvsp[-11].sval) <<endl;
  cout << "4: " << *(yyvsp[-9].sval) <<endl;
  cout << "5: " << *(yyvsp[-7].sval) <<endl;
  cout << "6: " << *(yyvsp[-5].sval) <<endl;
  cout << "7: " << *(yyvsp[-3].sval) <<endl;
  cout << "8: " << *(yyvsp[-1].sval) <<endl;
#endif
  WidgetDef *w=new WidgetDef(yyvsp[-15].sval,yyvsp[-13].sval,yyvsp[-11].sval,yyvsp[-9].sval,yyvsp[-7].sval,yyvsp[-5].sval,yyvsp[-3].sval,yyvsp[-1].sval);
  currentWidget=w;
  widgets.insert(widgets.end(),w);
  w->print_decl_begin(yyout);
;
    break;}
case 12:
#line 125 "parser.yy"
{
  yyval.sval = yyvsp[0].sval;
;
    break;}
case 13:
#line 129 "parser.yy"
{
  *(yyvsp[-1].sval)+=" ";
  *(yyvsp[-1].sval)+=*(yyvsp[0].sval);
  delete (yyvsp[0].sval);
  yyval.sval = yyvsp[-1].sval;
;
    break;}
case 14:
#line 138 "parser.yy"
{
  yyval.sval = new string("const");
;
    break;}
case 15:
#line 142 "parser.yy"
{
  yyval.sval = new string("volatile");
;
    break;}
case 16:
#line 146 "parser.yy"
{
#ifdef GTKMM_FIXME_GTK_CONST
 yyval.sval = new string("const");
#else
 yyval.sval = new string("");
#endif
;
    break;}
case 17:
#line 156 "parser.yy"
{
  yyval.sval = yyvsp[0].sval;
;
    break;}
case 18:
#line 160 "parser.yy"
{
  yyval.sval = yyvsp[0].sval;
;
    break;}
case 19:
#line 164 "parser.yy"
{
  yyval.sval = yyvsp[0].sval;
;
    break;}
case 20:
#line 170 "parser.yy"
{
  *(yyvsp[-1].sval)+="*";
  yyval.sval = yyvsp[-1].sval;
;
    break;}
case 21:
#line 175 "parser.yy"
{
  *(yyvsp[-1].sval)+="&";
  yyval.sval = yyvsp[-1].sval;
;
    break;}
case 22:
#line 180 "parser.yy"
{
  if ((yyvsp[0].sval)->length ()) {
    *(yyvsp[-1].sval)+=" ";
  }
  *(yyvsp[-1].sval)+=*(yyvsp[0].sval);
  delete (yyvsp[0].sval);
  yyval.sval = yyvsp[-1].sval;
;
    break;}
case 23:
#line 189 "parser.yy"
{
  if ((yyvsp[-1].sval)->length ()) {
    *(yyvsp[-1].sval)+=" ";
  }
  *(yyvsp[-1].sval)+=*(yyvsp[0].sval);
  delete (yyvsp[0].sval);
  yyval.sval = yyvsp[-1].sval;
;
    break;}
case 24:
#line 198 "parser.yy"
{
  yyval.sval = yyvsp[0].sval;
;
    break;}
case 25:
#line 208 "parser.yy"
{
  *(yyvsp[-3].sval)+="<";
  *(yyvsp[-3].sval)+=*(yyvsp[-1].sval);
  *(yyvsp[-3].sval)+=">";
  delete (yyvsp[-1].sval);
  yyval.sval = yyvsp[-3].sval;
;
    break;}
case 26:
#line 216 "parser.yy"
{
  *(yyvsp[-2].sval)+="<>";
  yyval.sval = yyvsp[-2].sval;
;
    break;}
case 27:
#line 223 "parser.yy"
{
  *(yyvsp[-2].sval)+=",";
  *(yyvsp[-2].sval)+=*(yyvsp[0].sval);
  delete (yyvsp[0].sval);
  yyval.sval = yyvsp[-2].sval;
;
    break;}
case 28:
#line 230 "parser.yy"
{
  yyval.sval = yyvsp[0].sval;
;
    break;}
case 29:
#line 236 "parser.yy"
{ yyval.sval=yyvsp[0].sval; ;
    break;}
case 30:
#line 238 "parser.yy"
{ yyval.sval=yyvsp[0].sval; ;
    break;}
case 31:
#line 240 "parser.yy"
{ yyval.sval=new string("|"); ;
    break;}
case 32:
#line 247 "parser.yy"
{ 
  *(yyvsp[-1].sval)+=*(yyvsp[0].sval);
  delete yyvsp[0].sval;
  yyval.sval=yyvsp[-1].sval;
;
    break;}
case 33:
#line 253 "parser.yy"
{
  yyval.sval=yyvsp[0].sval;
;
    break;}
case 34:
#line 257 "parser.yy"
{
  *(yyvsp[-1].sval)="("+*(yyvsp[-1].sval)+")";
  yyval.sval=yyvsp[-1].sval;
;
    break;}
case 35:
#line 264 "parser.yy"
{
  yyval.vval = new Argument(yyvsp[0].sval);
;
    break;}
case 36:
#line 268 "parser.yy"
{
  yyval.vval = new Argument(yyvsp[-1].sval,yyvsp[0].sval);
;
    break;}
case 37:
#line 272 "parser.yy"
{
  yyval.vval = new Argument(yyvsp[-3].sval,yyvsp[-2].sval,yyvsp[0].sval);
//  cout << "value = " << *($<sval>4) <<endl;
;
    break;}
case 38:
#line 284 "parser.yy"
{
  (yyvsp[-2].aval)->push_arg(yyvsp[0].vval);
  yyval.aval = yyvsp[-2].aval;
;
    break;}
case 39:
#line 289 "parser.yy"
{
  ArgList* l = new ArgList();
  l->push_arg(yyvsp[0].vval);
  yyval.aval = l;
;
    break;}
case 40:
#line 297 "parser.yy"
{
  yyval.fval = new FunctionDef(yyvsp[-3].sval,yyvsp[-2].sval);
;
    break;}
case 41:
#line 301 "parser.yy"
{
  yyval.fval = new FunctionDef(yyvsp[-4].sval,yyvsp[-3].sval,yyvsp[-1].aval);
;
    break;}
case 42:
#line 305 "parser.yy"
{
  yyval.fval = new FunctionDef(yyvsp[-4].sval,yyvsp[-3].sval,yyvsp[-1].aval);
;
    break;}
case 44:
#line 312 "parser.yy"
{
  (yyvsp[-1].fval)->mark_const();
  yyval.fval = yyvsp[-1].fval;
;
    break;}
case 45:
#line 319 "parser.yy"
{
#ifdef DEBUG
  cout<< "WRAP_METHOD 1:" 
      << (yyvsp[-3].fval)->get_function() << "  2:"
      << (yyvsp[-1].fval)->get_function() <<endl;
#endif
  MethodDef *method=new MethodDef(yyvsp[-3].fval,yyvsp[-1].fval);
  currentWidget->push_method(method);
  method->print_decl(yyout);
;
    break;}
case 46:
#line 330 "parser.yy"
{
#ifdef DEBUG
  cout<< "WRAP_STATIC_METHOD 1:" 
      << (yyvsp[-4].fval)->get_function() << "  2:"
      << (yyvsp[-2].fval)->get_function() <<endl;
#endif
  MethodDef *method=new StaticMethodDef(yyvsp[-4].fval,yyvsp[-1].fval);
  currentWidget->push_method(method);
  method->print_decl(yyout);
;
    break;}
case 47:
#line 343 "parser.yy"
{
#ifdef DEBUG
  cout << "WRAP_MEMBER" 
       << "  Attrib:  "  << *(yyvsp[-9].ival) 
       << "  CppName: "  << *(yyvsp[-7].sval) 
       << "  CName:   "  << *(yyvsp[-5].sval) 
       << "  CppType: "  << *(yyvsp[-3].sval) 
       << "  CType:   "  << *(yyvsp[-1].sval) 
       << endl ;
#endif
  MemberDef *member=new MemberDef(yyvsp[-9].ival,yyvsp[-7].sval,yyvsp[-5].sval,yyvsp[-3].sval,yyvsp[-1].sval);
  currentWidget->push_member(member);
  member->print_decl(yyout);
;
    break;}
case 48:
#line 361 "parser.yy"
{
#ifdef DEBUG
  cout << "ENDCLASS"<<endl;
#endif
  currentWidget->print_decl_end(yyout);
  currentWidget=&noWidget;
  fputc('}',yyout);
;
    break;}
case 49:
#line 372 "parser.yy"
{
  yyval.ival=(yyvsp[-2].ival) | sig_attribute(yyvsp[0].sval);
  delete yyvsp[0].sval;
;
    break;}
case 50:
#line 377 "parser.yy"
{
  yyval.ival= sig_attribute(yyvsp[0].sval);
  delete yyvsp[0].sval;
;
    break;}
case 51:
#line 384 "parser.yy"
{
  yyval.ival=(yyvsp[-2].ival) | member_attribute(yyvsp[0].sval);
  delete yyvsp[0].sval;
;
    break;}
case 52:
#line 389 "parser.yy"
{
  yyval.ival= member_attribute(yyvsp[0].sval);
  delete yyvsp[0].sval;
;
    break;}
case 53:
#line 437 "parser.yy"
{
#ifdef DEBUG
  cout << " SIGNALDECL (\""<< *(yyvsp[-6].sval) <<"\", "<< (yyvsp[-3].ival) <<","<< (yyvsp[-1].fval)->get_function() << ")" <<endl;
#endif
  SignalDef *sigDef = new SignalDef(yyvsp[-3].ival,yyvsp[-6].sval,yyvsp[-1].fval);
  currentWidget->push_signal(sigDef);
  sigDef->print_decl(yyout);
;
    break;}
case 54:
#line 446 "parser.yy"
{
  yyvsp[-5].ival|=ATTR_EMIT;
#ifdef DEBUG
  cout << " SIGNALDECL (\""<< *(yyvsp[-8].sval) <<"\", "<< (yyvsp[-5].ival) <<","<< (yyvsp[-3].fval)->get_function() << "=>" << *(yyvsp[-1].sval) << ")" <<endl;
#endif
  SignalDef *sigDef = new SignalDef(yyvsp[-5].ival,yyvsp[-8].sval,yyvsp[-3].fval,yyvsp[-1].sval);
  currentWidget->push_signal(sigDef);
  sigDef->print_decl(yyout);
;
    break;}
case 55:
#line 456 "parser.yy"
{
  yyvsp[-5].ival|=ATTR_TRANSLATE|ATTR_EMIT;
#ifdef DEBUG
  cout << " SIGNALDECL (\""<< *(yyvsp[-8].sval) <<"\", "<< (yyvsp[-5].ival) <<","<< (yyvsp[-3].fval)->get_function() << "=>" << (yyvsp[-1].fval)->get_function() << ")" <<endl;
#endif
  SignalDef *sigDef = new SignalDef(yyvsp[-5].ival,yyvsp[-8].sval,yyvsp[-3].fval,yyvsp[-1].fval);
  currentWidget->push_signal(sigDef);
  sigDef->print_decl(yyout);
;
    break;}
case 56:
#line 468 "parser.yy"
{
  channel->section_class(*(yyvsp[-1].sval)); 
  delete yyvsp[-1].sval;
;
    break;}
case 57:
#line 473 "parser.yy"
{
  channel->section_class();
;
    break;}
case 58:
#line 477 "parser.yy"
{
  channel->section_impl(); 
;
    break;}
case 59:
#line 481 "parser.yy"
{
  channel->section_doc(); 
;
    break;}
case 60:
#line 485 "parser.yy"
{
  channel->section_private(); 
;
    break;}
case 61:
#line 491 "parser.yy"
{
  currentWidget->set_cast_ctor(yyout);
;
    break;}
case 62:
#line 495 "parser.yy"
{
  currentWidget->set_default_ctor(yyout);
;
    break;}
case 63:
#line 499 "parser.yy"
{
  currentWidget->set_dtor(yyout);
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 505 "parser.yy"


int sig_attribute(string *s)
{
  if (!s) 
    yyerror("NULL in attribute");

  if ((*s)=="vfunc")     return ATTR_VFUNC;
  if ((*s)=="sig")       return ATTR_SIG;
  if ((*s)=="impl")      return ATTR_IMPL;
  if ((*s)=="emit")      return ATTR_EMIT;
  if ((*s)=="noemit")    return ATTR_NOEMIT;
  if ((*s)=="translate") return ATTR_TRANSLATE;
  if ((*s)=="both")      return ATTR_BOTH;
  if ((*s)=="marsh")     return ATTR_MARSH;
  if ((*s)=="fake")      return ATTR_GTK_EMIT;

  cerr << "Warning "<<yylineno<<": Unknown attribute. ("<<*s<<")"<<endl;
  return 0;
}

int member_attribute(string *s)
{
  if (!s)
    yyerror("NULL in attribute");
  if ((*s)=="inline")    return ATTR_INLINE;
  if ((*s)=="value")     return ATTR_VALUE;
  if ((*s)=="ref")       return ATTR_REF;

  cerr << "Warning "<<yylineno<<": Unknown attribute. ("<<*s<<")"<<endl;
  return 0;
}


int main(int argc, char *argv[])
{
  int rc;
  env.init(argc,argv);

  yyin=fopen(env.input_name().c_str(),"r");
  if (!yyin) 
    {
     cerr << "Failed to open file: "<< env.input_name().c_str() <<endl;
     exit (-1);
    }

//  headerfile =new HeaderChannel(env.header_name());
//  privatefile=new PHeaderChannel(env.private_name());
//  implfile   =new SourceChannel(env.impl_name());
  channel=new OutputChannel();
  yyout=channel->out();
  rc=yyparse();
  channel->process();
  return rc;
}

