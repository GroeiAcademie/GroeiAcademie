#ifndef EXAMPLES_H
#define EXAMPLES_H

#if defined(LANGUAGE_NL)
  #if defined(__has_include)
    #if __has_include(<Language/UserExample_NL.h>)
      #include <Language/UserExample_NL.h>
    #endif
  #endif
  #include <Language/Examples_NL.h>
#elif defined(LANGUAGE_DE)
  #if defined(__has_include)
    #if __has_include(<Language/UserExample_DE.h>)
      #include <Language/UserExample_DE.h>
    #endif
  #endif
  #include <Language/Examples_DE.h>
#elif defined(LANGUAGE_EN)
  #if defined(__has_include)
    #if __has_include(<Language/UserExample_EN.h>)
      #include <Language/UserExample_EN.h>
    #endif
  #endif
  #include <Language/Examples_EN.h>
#elif defined(LANGUAGE_FR)
  #if defined(__has_include)
    #if __has_include(<Language/UserExample_FR.h>)
      #include <Language/UserExample_FR.h>
    #endif
  #endif
  #include <Language/Examples_FR.h>
#endif

#endif
