/*
 * Will Korteland
 * 10th September 2014
 *
 * Fourth phase of the translation process. From the spec:
 * Preprocessing directives are executed, macro invocations are
 * expanded, and _Pragma unary operator expressions are executed. If a
 * character sequence that matches the syntax of a universal character
 * name is produced by token concatenation, the behavior is
 * undefined. A #include preprocessing directive causes the named
 * header or source file to be processed from phase 1 through phase 4,
 * recursively. All preprocessing directives are then deleted.
 */
