/*
    Copyright (C) 1995-2013, The AROS Development Team. All rights reserved.

    Desc: piglatin.language description file.
*/

/*
    This is a rather silly language, but it is more of an example than
    then english language file, as this one actually does something...

    In fact you may even wish to copy this one instead of copying
    english_language.c and english_data.c
*/

#define LANGSTR     "piglatin"          /* String version of above      */
#define NLANGSTR    "pig-latin"         /* Native version of LANGSTR    */
#define LANGVER     41                  /* Version number of language   */
#define LANGREV     3                   /* Revision number of language  */
#define LANGTAG     "\0$VER: " LANGSTR ".language 41.3 (30.09.2014)"
#define NLANGTAG    "$NLANG:" NLANGSTR

#include "lang_openclose.inc"
#include "lang_getlangstring.inc"

/* -----------------------------------------------------------------------
   Library function table - you will need to alter this
   I have this right here at the end of the library so that I do not
   have to have prototypes for the functions. Although you could do that.
   ----------------------------------------------------------------------- */

void *const functable[] =
{
    &AROS_SLIB_ENTRY(open,language,1),
    &AROS_SLIB_ENTRY(close,language,2),
    &AROS_SLIB_ENTRY(expunge,language,3),
    &AROS_SLIB_ENTRY(null,language,0),
    &AROS_SLIB_ENTRY(mask,language,5),

    /* Note, shorter function table, as only getlangstring is used */

    /* 0 - 3 */
    &AROS_SLIB_ENTRY(null, language, 0),
    &AROS_SLIB_ENTRY(null, language, 0),
    &AROS_SLIB_ENTRY(null, language, 0),
    &AROS_SLIB_ENTRY(getlangstring, language, 9),
    (void *)-1
};

/*
    Note how only the required data structures are kept...

    This is the list of strings. It is an array of pointers to strings,
    although how it is laid out is implementation dependant.
*/
const STRPTR ___strings[] =
{
    /* A blank string */
    "",

    /*  The days of the week. Starts with the first day of the week.
        In English this would be Sunday, this depends upon the settings
        of Locale->CalendarType.
    */
    "Undaysay",   "Ondaymay",   "Uesdaytay",  "Ednesdayway",
    "Hursdaytay", "Ridayfay",   "Aturdaysay",

    /* Abbreviated days of the week */
    "Unsay", "Onmay", "Uetay", "Edway", "Hutay", "Rifay", "Atsay",

    /* Months of the year */
    "Anuaryjay",  "Ebruaryfay", "Archmay",
    "Aprilway",    "Aymay",      "Unejay",
    "Ulyjay",     "Augustway",   "Eptembersay",
    "Octoberway",  "Ovembernay", "Ecemberday",

    /* Abbreviated months of the year */
    "Anjay", "Ebfay", "Armay", "Aprway", "Aymay", "Unjay",
    "Uljay", "Augway", "Epsay", "Octway", "Ovnay", "Ecday",

    "Yesay", /* Yes, affirmative response */
    "Onay", /* No/negative response */

    /* AM/PM strings AM 0000 -> 1159, PM 1200 -> 2359 */
    "am", "pm",

    /* Soft and hard hyphens */
    "-", "-",

    /* Open and close quotes */
    "\"", "\"",

    /* Days: But not actual day names
       Yesterday - the day before the current
       Today - the current day
       Tomorrow - the next day
       Future.
    */
    "Esterdayyay", "Odaytay", "Omorrowtay", "Uturefay",

    /* Native language name */
    NLANGSTR
};

/* This is the end of ROMtag marker. */
const char end=0;
