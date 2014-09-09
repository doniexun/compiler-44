/*
 * Will Korteland
 * 10th September 2014
 *
 * Second phase of the translation process. From the spec:
 * Each instance of a backslash character (\) immediately followed by
 * a new-line character is deleted, splicing physical source lines to
 * form logical source lines. Only the last backslash on any physical
 * source line shall be eligible for being part of such a splice. A
 * source file that is not empty shall end in a new-line character,
 * which shall not be immediately preceded by a backslash character
 * before any such splicing takes place.
 */
