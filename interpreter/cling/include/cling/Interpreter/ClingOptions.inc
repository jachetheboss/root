#ifndef PREFIX
#error "Define PREFIX prior to including this file!"
#endif

/////////
// Prefixes

#define COMMA ,
PREFIX(prefix_0, {llvm::StringLiteral("")})
PREFIX(prefix_1, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("")})
PREFIX(prefix_3, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_2, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
#undef COMMA

#ifndef OPTION
#error "Define OPTION prior to including this file!"
#endif

OPTION(prefix_0, "<input>", INPUT, Input, INVALID, INVALID, 0, 0, 0, 0, 0, 0)
OPTION(prefix_0, "<unknown>", UNKNOWN, Unknown, INVALID, INVALID, 0, 0, 0, 0, 0, 0)
#ifndef NDEBUG
OPTION(prefix_2, "debug-only=", _debugFlags_EQ, Joined, INVALID, INVALID, 0, 0, 0,
       "Debug flags to enable", 0, 0)
OPTION(prefix_2, "debug-only", _debugFlags, Separate, INVALID, INVALID, 0, 0, 0,
       "Debug flags to enable", 0, 0)
#endif
OPTION(prefix_2, "errorout", _errorout, Flag, INVALID, INVALID, 0, 0, 0,
       "Do not recover from input errors", 0, 0)
// Re-implement to forward to our help
OPTION(prefix_3, "help", help, Flag, INVALID, INVALID, 0, 0, 0,
       "Print this help text", 0, 0)
OPTION(prefix_1, "L", L, JoinedOrSeparate, INVALID, INVALID, 0, 0, 0,
       "Add directory to library search path", "<directory>", 0)
OPTION(prefix_1, "l", l, JoinedOrSeparate, INVALID, INVALID, 0, 0, 0,
       "Load a library before prompt", "<library>", 0)
OPTION(prefix_2, "metastr=", _metastr_EQ, Joined, INVALID, INVALID, 0, 0, 0,
       "Set the meta command tag, default '.'", 0, 0)
OPTION(prefix_2, "metastr", _metastr, Separate, INVALID, INVALID, 0, 0, 0,
       "Set the meta command tag, default '.'", 0, 0)
OPTION(prefix_2, "nologo", _nologo, Flag, INVALID, INVALID, 0, 0, 0,
       "Do not show startup-banner", 0, 0)
OPTION(prefix_3, "noruntime", noruntime, Flag, INVALID, INVALID, 0, 0, 0,
       "Disable runtime support (no null checking, no value printing)", 0, 0)
OPTION(prefix_2, "ptrcheck", _ptrcheck, Flag, INVALID, INVALID, 0, 0, 0,
       "Enable injection of pointer validity checks", 0, 0)
OPTION(prefix_3, "version", version, Flag, INVALID, INVALID, 0, 0, 0,
       "Print the compiler version", 0, 0)
OPTION(prefix_1, "v", v, Flag, INVALID, INVALID, 0, 0, 0,
       "Enable verbose output", 0, 0)
