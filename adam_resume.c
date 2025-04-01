/**
    Resume-As-Code
    Copyright (c) 2025, Adam Rosenberg
    All rights reserved.
*/
#define RESUME_FULL_NAME     "Adam Rosenberg"
#define RESUME_EMAIL         "adam@sirspot.com"
#define RESUME_CITY          "Orlando"
#define RESUME_STATE         "FL"
#define RESUME_MOBILE_NUMBER ""
#define RESUME_WEB_URL       "http://www.sirspot.com"

// 
// MARK: ABOUT
//

/** \file adam_resume.c
 
  ----------------------------------------------------------
    Use one of the following commands to
    build this resume from source:
    - gcc adam_resume.c -o adam_resume.exe
    - clang adam_resume.c -o adam_resume.exe
    - cl.exe /Fe:adam_resume.exe adam_resume.c

  ----------------------------------------------------------
    To get help with this resume:
    - run the command: adam_resume.exe -h
    - see MainPrintHelp()

  ----------------------------------------------------------
    To modify this resume:
    - update the contact information at the top
    - using hard-coded data
        - add sections in enum Section_e
        - update ResumeFillSectionsFromHardCoded()
        - update ResumeFillEntriesFromHardCoded()
    - using JSON data at runtime
        - update RESUME_JSON_DATA

  ----------------------------------------------------------
    How the resume source code is organized:

    This is a very long file. The following sections are
    marked on the minimap when using VS Code as your editor.
    Consider adding the "Mark Jump" extension. It will make
    reading this file markedly less difficult, pun intended.

    - RESUME STRINGS
        defines keys and values for resume data
        stored as JSON

    - JSON MACROS
        these macros are used to format JSON data within
        the source code in a "readable" way that doesn't
        require the user to escape quotes manually

    - RESUME DATA
        resume data stored as strings within the source
        is added and updated here

    - IMPORTANT NOTES
        information and assumptions made when writing
        the code for this resume

    - VERSION
        version history of the resume source code.
        this will be updated any time this file changes.

    - SYSTEM INCLUDES
        includes header files provided by the
        development environment and defines exactly
        what is used from each

    - DEFINES
        various preprocess definitions. roughly organized
        by use. this mostly exists to prevent the use of
        any "magic" numbers

    - ENUMS
        any time there is a possibility for more than
        two unique choices, an enumeration is defined
        to assign a name and value to each. this is also
        where resume sections can be hard-coded
        in enum Section_e

    - FLAGS
        similar to enums but used when there is a
        possibility for more than two choices that are
        not mutually exclusive. these are implemented
        as bit fields to save memory.

    - STRUCT TYPES
        predefined types for each struct defined in
        this code. this helps to future-proof the resume
        source. if it ever needs to be split into multiple
        files, the data structures can be easily shared
        without necessarily providing direct access to
        their members.

    - STRUCTS
        complete definition of data structures used to
        store and process the resume data

    - CALLBACK DEFINITIONS
        applies a name to function pointers making them
        easy to document and use throughout the code

    - PROTOTYPES
        predefines all functions. roughly organized by
        the data structures they access. normally this
        would be done in one or more separate header files.

    - GLOBALS
        just a few global variables primarily used to store
        all of the resume data itself instead of loading it
        from external files

    - HELPER MACROS
        any general-purpose macros that are useful
        throughout the code are placed here. macros
        that are specific to a single function are
        placed just above its function to make it clear
        where the macro is being used.

    - SOURCE
        implementation of all predefined functions
        in the same order they were predefined.

    - PROGRAM
        the main entry point function is here.

*/

// 
// MARK: RESUME STRINGS
//
/*
   Note: additional strings are defined in the 
         following lobal variables:
         - g_JsonKeywordTrue
         - g_JsonKeywordFalse
         - g_JsonKeywordNull
         - g_EntryStateLookup
*/

#define RESUME_STRING_DATE_END       "date_end"
#define RESUME_STRING_DATE_OPTION    "date_option"
#define RESUME_STRING_DATE_ORDER     "date_order"
#define RESUME_STRING_DATE_START     "date_start"
#define RESUME_STRING_DISPLAY_MAX    "display_max"
#define RESUME_STRING_ENTRIES        "entries"
#define RESUME_STRING_NAME           "name"
#define RESUME_STRING_NEWEST_FIRST   "newest_first"
#define RESUME_STRING_OLDEST_FIRST   "oldest_first"
#define RESUME_STRING_ORDER          "order"
#define RESUME_STRING_RANDOM         "random"
#define RESUME_STRING_SHOW_YEAR_ONLY "show_year_only"
#define RESUME_STRING_TEXT           "text"

//
// MARK: JSON MACROS
//

/** place a comma for json data and additional spacer
    such as " " or "\n"
*/
#define JSON_MORE(spacer) "," spacer

/** joins key and value into quoted strings separated
    by a ':' for a json value
    \param lkey a quoted key such as "name"
    \param value can be a quoted value such as "value" or a
                 number such as 100 or the start of a JSON
                 array or object such as [ or {
*/
#define JSON_LITERAL_KEY_VALUE(lkey, value) #lkey ": " #value

/** joins key and value into quoted strings separated by
    a ':' for a json value
    \param dkey a pre-defined key such as RESUME_STRING_NAME
    \param value can be a quoted value such as "value", a
                 number such as 100, or the start of a JSON
                 array or object such as [ or {
*/
#define JSON_DEFINED_KEY_VALUE(dkey, value) "\"" dkey "\": " #value

/** starts a resume section. this must be followed by one
    or more JSON_OBJECT_ENTRY() and finished with
    JSON_OBJECT_END_SECTION()
    Note: if there is more than one entry,
            each must be separated by JSON_MORE()
    \param name the section name such as "Education"
    \param options one or more JSON_LITERAL_KEY_VALUE() or
                   JSON_DEFINED_KEY_VALUE()
                   Note: if there is more than one value,
                   each must be separated by JSON_MORE()
*/
#define JSON_OBJECT_START_SECTION(name, options) "{ " JSON_DEFINED_KEY_VALUE(RESUME_STRING_NAME, name) JSON_MORE(" ") options JSON_MORE(" ") JSON_DEFINED_KEY_VALUE(RESUME_STRING_ENTRIES, \n[\n)

/** ends a resume section. see JSON_OBJECT_START_SECTION()
*/
#define JSON_OBJECT_END_SECTION() "\n]\n}"

/** creates an entry object
    \param text required quoted text such as "text"
    \param start optional start date.
                 can be an empty string "" or a date
                 such as "2000-01-31"
    \param start optional end date.
                 can be an empty string "" or a date
                 such as "2000-01-31"
*/
#define JSON_OBJECT_ENTRY(text, start, end) "{ " JSON_DEFINED_KEY_VALUE(RESUME_STRING_TEXT, text) JSON_MORE(" ") JSON_DEFINED_KEY_VALUE(RESUME_STRING_DATE_START, start) JSON_MORE(" ") JSON_DEFINED_KEY_VALUE(RESUME_STRING_DATE_END, end) " }"

//
// MARK: RESUME DATA
//

/** Languages section as a JSON object
*/
#define RESUME_SECTION_LANGUAGES() \
\
JSON_OBJECT_START_SECTION("Languages and Libraries",\
JSON_DEFINED_KEY_VALUE(RESUME_STRING_DATE_ORDER,   "date_end"      ) JSON_MORE(" ")\
JSON_DEFINED_KEY_VALUE(RESUME_STRING_DATE_OPTION,  "show_year_only") JSON_MORE(" ")\
JSON_DEFINED_KEY_VALUE(RESUME_STRING_DISPLAY_MAX,  6               ))\
    JSON_OBJECT_ENTRY("basic",          "1993-01-01", "1994-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("pascal",         "1995-01-01", "1999-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("delphi",         "1998-01-01", "1999-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("bash",           "2019-01-01", "")           JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("php",            "2003-01-01", "2021-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("sql",            "2002-01-01", "2023-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("java",           "2000-01-01", "2001-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("c#",             "2002-01-01", "2003-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("qt/c++",         "2005-01-01", "")           JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("c++",            "2001-01-01", "")           JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("c",              "1999-01-01", "")           JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("python",         "2019-01-01", "2021-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("zim/javascript", "2021-01-01", "2021-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("powershell",     "2024-01-01", "2024-01-01") JSON_MORE("\n")\
    JSON_OBJECT_ENTRY("mfc/c++",        "2006-01-01", "2015-01-01")\
JSON_OBJECT_END_SECTION()

/** resume data as a JSON array of objects
*/
#define RESUME_JSON_DATA "[\n" RESUME_SECTION_LANGUAGES() "\n]\n"

//
// MARK: IMPORTANT NOTES
//
/*

    1. all types of char* used to store a string are assumed
       to contain null-terminated characters defined by
       ISO/IEC 8859-1 "Latin-1".

    2. some system functions used in this code are not the
       latest and "safest". proper memory checks are made
       wherever possible to mitigate these issues. this
       allows for wider support of compilers without using
       any preprocessor directives.

*/

//
// MARK: VERSION
//
/*

    2025-01-31 v1.01.00
    - add macros for easy json formatted strings
    - add json parser for dynamic section data

    2025-01-13 v1.00.02
    - indicate defaults in help text
    - add projects section
    - add detailed entry state information
    - add entry display options -e and -a

    2025-01-03 v1.00.01
    - fixed a few typos

    2025-01-01 v1.00.00
    - includes basic resume data structure for
      text and html output formats

*/

#define MAJOR_VERSION 1
#define MINOR_VERSION 1
#define PATCH_VERSION 0

//
// MARK: SYSTEM INCLUDES
//

// included for printf(), snprintf(), sscanf()
#include <stdio.h>

// included for malloc(), realloc(), free(), rand(), srand()
#include <stdlib.h>

// included for struct tm, mktime(), struct timespec,
// clock_gettime(), time()
#include <time.h>

// included for memset(), memcpy(), strcmp(), strncmp()
#include <string.h>

// included for uint16_t
#include <stdint.h>

// included for tolower()
#include <ctype.h>

//
// MARK: DEFINES
//

/** number of nanoseconds in 1 millisecond
*/
#define NANO_PER_MILLI 1000000

/** an 8.5"x11" printed page can usually fit 60 lines.
    this value must be >= 4 or ResumeInit() will fail
*/
#define RESUME_MAX_LINES_PER_PAGE (60)

/** max number of pages for the resume.
    this is the absolute max so is much larger than
    what is actually needed
*/
#define RESUME_MAX_PAGES (5)

/** max number of entries for the entire resume.
    allow for 1 entry per line of the max number of pages
*/
#define RESUME_MAX_ENTRIES (RESUME_MAX_PAGES * RESUME_MAX_LINES_PER_PAGE)

/** display option for all sections
*/
#define DISPLAY_ALL (-1)

/** display option to hide an entire section
*/
#define DISPLAY_NONE (0)

/** invalid position when attempting to find an array index
    or traverse the entire array
*/
#define INVALID_POSITION (-1)

/** unknown year is most likely not a leap year so
    instead of 0 the value here is 1
*/
#define YEAR_UNKNOWN (1)

/** timestamp value when not set to a valid timestamp
*/
#define TIMESTAMP_UNSET (0)

/** full date string format.
    for example: "2038-01-18" would be the day before a
    32bit unix timestamp rolls over to 1901.
*/
#define DATE_STRING_FORMAT "%04d-%02d-%02d"

/** characters needed for date string in DATE_STRING_FORMAT
*/
#define DATE_STRING_LENGTH 10

/** max length of the following value keywords:
    - false
    - true
    - null
*/
#define JSON_VALUE_KEYWORD_MAX_LENGTH 5

/** this is the max value of a signed int32.
    INT_MAX on a 32bit system is 2GB so it should
    be enough for any text being added to this resume :)
*/
#define MAX_STRING_LENGTH 2147483647

/** max number of resume sections that can be hidden
    by resume options
*/
#define RESUME_OPTIONS_MAX_HIDDEN_SECTIONS 32

//
// MARK: ENUMS
//

/** result codes
*/
typedef enum Result_e
{
    RESULT_UNKNOWN = 0,
    RESULT_OK,
    RESULT_WARNING,
    RESULT_ERROR,
    RESULT_NEXT,
    RESULT_YES,
    RESULT_NO,
    RESULT_COUNT
} Result_t;

/** exit codes
*/
enum Exit_e
{
    EXIT_OK                         =  0,
    EXIT_FORMAT_UNKNOWN_ARG         = -1,
    EXIT_FORMAT_ARG_MISSING         = -2,
    EXIT_FORMAT_SELECTION_MISSING   = -3,
    EXIT_EXTENDED_COUNT_INVALID     = -4,
    EXIT_EXTENDED_COUNT_ARG_MISSING = -5,
    EXIT_EXTENDED_COUNT_MISSING     = -6,
    EXIT_UNKNOWN_ARG                = -7,
    EXIT_FILL_ERROR                 = -8,
    EXIT_RESUME_INIT_ERROR          = -9
};

/** months
*/
typedef enum Month_e
{
    MONTH_JAN = 0,
    MONTH_FEB,
    MONTH_MAR,
    MONTH_APR,
    MONTH_MAY,
    MONTH_JUN,
    MONTH_JUL,
    MONTH_AUG,
    MONTH_SEP,
    MONTH_OCT,
    MONTH_NOV,
    MONTH_DEC,
    MONTH_COUNT
} Month_t;

/** formatting options for program output
*/
typedef enum Format_e
{
    FORMAT_TEXT = 0,
    FORMAT_HTML,
    FORMAT_COUNT
} Format_t;

/** entry display order
*/
typedef enum Order_e
{
    ORDER_NEWEST_FIRST = 0,
    ORDER_OLDEST_FIRST,
    ORDER_RANDOM,
    ORDER_COUNT
} Order_t;

/** hard-coded resume sections used for filtering.
    Note: update ResumeFillSectionsFromHardCoded() and
          ResumeFillEntriesFromHardCoded() when making
          changes to this enum
*/
enum Section_e
{
    SECTION_ACCOMPLISHMENTS = 0,
    SECTION_EDUCATION,
    SECTION_WORK_HISTORY,
    SECTION_EXPERIENCE,
    SECTION_TOOLS,
    SECTION_PROJECTS,
    SECTION_INTERESTS,
    SECTION_COUNT,
    SECTION_AT_RUNTIME = SECTION_COUNT
};

/** set to INVALID_POSITION for no section.
    \see enum Section_e for hard-coded values.
    section numbers must be >= SECTION_AT_RUNTIME
    when loaded at runtime
*/
typedef unsigned int Section_t;

/** each entry can have these times associated with it
*/
typedef enum EntryTime_e
{
    ENTRY_TIME_START = 0,
    ENTRY_TIME_END,
    ENTRY_TIME_COUNT
} EntryTime_t;

/** entry state data stored by ManagedEntries_t
*/
typedef enum EntryState_e
{
    /** no entry has been accessed
    */
    ENTRY_STATE_UNKNOWN = 0,

    /** the entry or entries are ok
    */
    ENTRY_STATE_OK,

    /** there is no space for more entries
    */
    ENTRY_STATE_ERROR_FULL,

    /** entry text is an empty string
    */
    ENTRY_STATE_ERROR_TEXT_EMPTY,

    /** entry text is null
    */
    ENTRY_STATE_ERROR_TEXT_MISSING,

    /** start date is invalid
    */
    ENTRY_STATE_ERROR_START_DATE,

    /** end date is invalid
    */
    ENTRY_STATE_ERROR_END_DATE,

    /** an end date was specified without a start date
    */
    ENTRY_STATE_ERROR_START_DATE_MISSING,

    /** occurs when a new Entry_t cannot be
        allocated
    */
    ENTRY_STATE_ERROR_ALLOC,

    /** failed to set entry text.
        this can happen if a malloc in
        SetText() fails
    */
    ENTRY_STATE_ERROR_SET_TEXT,

    /** failed to resize an entry array
    */
    ENTRY_STATE_ERROR_RESIZE,

    /** the section for the entry is missing
    */
    ENTRY_STATE_ERROR_SECTION_MISSING,

    ENTRY_STATE_COUNT
} EntryState_t;

/** json state data
*/
typedef enum JsonState_e
{
    /** no json has been read
    */
    JSON_STATE_UNKNOWN = 0,

    /** the json data is ok
    */
    JSON_STATE_OK,

    /** numbers can only have one decimal point
    */
    JSON_STATE_ERROR_TOO_MANY_DECIMALS,

    /** json numbers must start with a number.
        leading decimal point is not allowed.
    */
    JSON_STATE_ERROR_LEADING_DECIMAL,

    /** the start of a string is missing the '"'
    */
    JSON_STATE_ERROR_STRING_MISSING_START,

    /** the end of a string is missing the '"'
    */
    JSON_STATE_ERROR_STRING_MISSING_END,

    /** an escape character '\\' was found at
        the end of a string with nothing to
        escape after it
    */
    JSON_STATE_ERROR_STRING_MISSING_ESCAPED_CHAR,

    /** a valid string was found but the next
        non-whitespace character was not one
        of the following: ',', ']', '}', ':', or '\0'
    */
    JSON_STATE_ERROR_STRING_INVALID_FOLLOWER,

    /** the start of an array is missing the '['
    */
    JSON_STATE_ERROR_ARRAY_MISSING_START,

    /** the end of an array is missing the ']'
    */
    JSON_STATE_ERROR_ARRAY_MISSING_END,

    /** an unexpected character was found while
        searching for the next array element
    */
    JSON_STATE_ERROR_ARRAY_MISSING_NEXT_OR_END_CHAR,

    /** the start of an object is missing the '{'
    */
    JSON_STATE_ERROR_OBJECT_MISSING_START,

    /** the end of an object is missing the '}'
    */
    JSON_STATE_ERROR_OBJECT_MISSING_END,

    /** could not find a quoted string key value
        within the current object
    */
    JSON_STATE_ERROR_OBJECT_MISSING_KEY,

    /** a key was found in the current object but
        was not followed by a ':' to start the value
    */
    JSON_STATE_ERROR_OBJECT_MISSING_VALUE_START,

    /** the value of a key was not a string or a number
        and contained an uppercase character.
        there are no json keywords that contain
        uppercase characters.
        see JSON_STATE_ERROR_VALUE_INVALID
    */
    JSON_STATE_ERROR_VALUE_CASE,

    /** the value of a key was not a string or a number
        and was not one of the following case-sensitive
        keywords: true, false, or null
    */
    JSON_STATE_ERROR_VALUE_INVALID,

    /** a valid value was found but the next
        non-whitespace character was not one
        of the following: ',', ']', '}', or '\0'
    */
    JSON_STATE_ERROR_VALUE_INVALID_FOLLOWER,

    /** a comma was found but the next
        non-whitespace character was not one
        of the following: '{', '[', '"', or '\0'
    */
    JSON_STATE_ERROR_COMMA_INVALID_FOLLOWER,

    /** an unexpected character was found while
        attempting to skip json data. for example,
        if an attempt is made to skip json data
        from within a string
    */
    JSON_STATE_ERROR_UNEXPECTED_CHAR,

    JSON_STATE_COUNT
} JsonState_t;

//
// MARK: FLAGS
//

/** date display options
*/
enum DateOption_e
{
    DATE_OPTION_SHOW_ALL         = 0x0000,
    DATE_OPTION_HIDE_START_DAY   = 0x0001,
    DATE_OPTION_HIDE_START_MONTH = 0x0002,
    DATE_OPTION_HIDE_START_YEAR  = 0x0004,
    DATE_OPTION_HIDE_END_DAY     = 0x0010,
    DATE_OPTION_HIDE_END_MONTH   = 0x0020,
    DATE_OPTION_HIDE_END_YEAR    = 0x0040,
    DATE_OPTION_ALL              = 0xFFFF
};

#define DATE_OPTION_HIDE_START (DATE_OPTION_HIDE_START_YEAR | DATE_OPTION_HIDE_START_MONTH | DATE_OPTION_HIDE_START_DAY)
#define DATE_OPTION_HIDE_END   (DATE_OPTION_HIDE_END_YEAR | DATE_OPTION_HIDE_END_MONTH | DATE_OPTION_HIDE_END_DAY)
#define DATE_OPTION_HIDE_DAY   (DATE_OPTION_HIDE_START_DAY | DATE_OPTION_HIDE_END_DAY)
#define DATE_OPTION_HIDE_MONTH (DATE_OPTION_HIDE_START_MONTH | DATE_OPTION_HIDE_END_MONTH)
#define DATE_OPTION_HIDE_YEAR  (DATE_OPTION_HIDE_START_YEAR | DATE_OPTION_HIDE_END_YEAR)
#define DATE_OPTION_HIDE_ALL   (DATE_OPTION_HIDE_START | DATE_OPTION_HIDE_END)

#define DATE_OPTION_SHOW_YEAR_ONLY (DATE_OPTION_HIDE_MONTH | DATE_OPTION_HIDE_DAY)
#define DATE_OPTION_SHOW_START_ONLY DATE_OPTION_HIDE_END
#define DATE_OPTION_SHOW_END_ONLY   DATE_OPTION_HIDE_START

/** see enum DateOption_e
*/
typedef uint16_t DateOption_t;

//
// MARK: STRUCT TYPES
//

typedef struct Entry_s           Entry_t;
typedef struct Entries_s         Entries_t;
typedef struct EntriesTraverse_s EntriesTraverse_t;
typedef struct ManagedEntries_s  ManagedEntries_t;
typedef struct SectionData_s     SectionData_t;
typedef struct ResumeOptions_s   ResumeOptions_t;
typedef struct Resume_s          Resume_t;

//
// MARK: STRUCTS
//

/** stores a single resume item with optional date
*/
struct Entry_s
{
    /** string
    */
    char* m_EntryText;

    /** UTC Unix timestamp or 0 if not used.
    */
    time_t m_EntryTimestamp[ENTRY_TIME_COUNT];

};

/** stores an array of Entry_t including
    the number of entries used
*/
struct Entries_s
{
    /** array of Entry_t* where all entries
        from index 0 to (m_EntryUsed - 1) are valid,
        but are in no partcular order
    */
    Entry_t** m_Entries;

    /** max number of array elements of m_Entry
    */
    int m_EntriesMax;

    /** number of array elements in use
    */
    int m_EntriesUsed;

};

/** used to traverse entries from Entries_t
*/
struct EntriesTraverse_s
{
    /** used by EntriesTraverseFirst() to change the order
        of an Entries_t object and by EntriesTraverseNext()
        to traverse them
    */
    Entries_t m_EntriesTraverse;

    /** entry number (1-indexed) used by
        EntriesTraverseFirst() and EntriesTraverseNext().
        this value is INVALID_POSITION until
        EntriesTraverseFirst() is called
    */
    int m_EntriesTraverseNumber;

};

/** stores an array of entries and manages
    the memory for each
*/
struct ManagedEntries_s
{
    /** array of entries
    */
    Entries_t m_ManagedEntries;

    /** set to the last error that occurred.
        this is set to ENTRY_STATE_UNKNOWN if an entry
        has not been accessed yet or ENTRY_STATE_OK to
        indicate no error.
    */
    EntryState_t m_ManagedEntriesError;

};

/** stores each section. \see SECTION_COUNT
*/
struct SectionData_s
{
    /** string
    */
    char* m_SectionTitle;

    /** max number of entries to display in this section or
        DISPLAY_ALL or DISPLAY_NONE
    */
    int m_SectionDisplayMax;

    /** the order to display entries in this section
    */
    Order_t m_SectionOrder;

    /** entry time used when m_SectionOrder is
        ORDER_NEWEST_FIRST or ORDER_OLDEST_FIRST
    */
    EntryTime_t m_SectionOrderEntryTime;

    /** determines how dates are displayed in this section
    */
    DateOption_t m_SectionDateOption;

    /** all entries for this section
    */
    Entries_t m_SectionEntries;

    /** pointer to the next section
    */
    SectionData_t* m_SectionNext;

};

/** resume options control how the entries are displayed
*/
struct ResumeOptions_s
{

    /** set to DISPLAY_ALL to always display all entries.
        set to DISPLAY_NONE to use defaults.
        set to 1 to RESUME_MAX_LINES_PER_PAGE to display
        up to the specified number of additional entries
    */
    int m_ExtendedDisplayCount;

    /** see enum Format_e
    */
    Format_t m_DisplayFormat;

    /** array of section names that 
        should be hidden.  
    */
    const char* m_HideSectionName[RESUME_OPTIONS_MAX_HIDDEN_SECTIONS];

    /** number of sections to hide in m_HideSectionName
    */
    int m_HideSectionCount;
};

/** resume data organizes entries into sections
*/
struct Resume_s
{
    /** all entries are stored here then organized
        into sections
    */
    ManagedEntries_t m_ResumeAllEntries;

    /** hard-coded sections of the resume
    */
    SectionData_t m_ResumeSection[SECTION_COUNT];

    /** section data loaded at run-time
    */
    SectionData_t* m_ResumeSectionRuntime;

    /** set to the section where an error last occurred.
        this is set to INVALID_POSITION to indicate
        no error.
    */
    Section_t m_ResumeErrorInSection;

    /** set to the last entry error that occurred.
        this is set to ENTRY_STATE_UNKNOWN if an entry
        has not been accessed yet or ENTRY_STATE_OK to
        indicate no error.
    */
    EntryState_t m_ResumeErrorInEntry;

    /** resume options
    */
    ResumeOptions_t m_ResumeOptions;

};

//
// MARK: CALLBACK DEFINITIONS
//

/** callback function definition used by EntriesFind()
    \param entry the entry within the entries being searched
    \param entryTime the entry time to use for comparison
    \param userData the data passed into EntriesFind()
    \returns RESULT_OK when the entry is correct,
             RESULT_ERROR to cancel the search, or
             RESULT_NEXT to continue searching
*/
typedef Result_t (*EntriesFindCallback_t)(
    Entry_t* entry,
    EntryTime_t entryTime,
    void* userData);

//
// MARK: PROTOTYPES
//

// --- MAIN ---

/** print help to standard out
*/
static void MainPrintHelp(
    const char* programName);

/** print version to standard out
*/
static void MainPrintVersion(void);

// --- STRING ---

/** set text by performing a deep copy of src or using
    a shared empty string for NULL or empty strings
    \param dest pointer to the string to update
    \param src the string source or NULL to clear the text
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t SetText(
    char** dest,
    const char* src);

/** set text by performing a deep copy of src or using
    a shared empty string for NULL or empty strings
    \param dest pointer to the string to update
    \param src the string source or NULL to clear the text
    \param srcLength the max number of bytes to copy, which
                     will not exceed those up to the
                     null-terminator of src
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t SetTextSubstr(
    char** dest,
    const char* src,
    int srcLength);

/** search for an uppercase character
    \param text
    \param maxChars max number of characters
           to search through
    \returns position of the uppercase character or
             INVALID_POSITION if not found
*/
static int FindUppercase(
    const char* text,
    int maxChars);

// --- TIME AND DATE ---

/** get the number of days in the specified month
    \param month \see Month_e
    \param year used to determine leap year.
           use YEAR_UNKNOWN if unsure.
    \returns the number of days 31, 30, 29, or 28
*/
static int GetDaysInMonth(
    int month,
    int year);

/** get a date string from the timestamp
    \param time the timestamp
    \param dest location to store the string
    \param destLength bytes available at dest
    \param dateOption control whether the year, month, and
           day are written to the string
*/
static int TimeToDateString(
    time_t time,
    char* dest,
    int destLength,
    DateOption_t dateOption);

/** get the timestamp from the date string
    \param time optional pointer to the time to update.
           use NULL and check for RESULT_ERROR if just
           checking if date is valid.
    \param date optional date in DATE_STRING_FORMAT
           use NULL or empty string for no date.
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t TimeFromDateString(
    time_t* time,
    const char* date);

// --- ENTRIES ---

/** init. call EntriesResize() after init to set size of
    this array.
    \param me
*/
static void EntriesInit(
    Entries_t* me);

/** deinit
    \param me
*/
static void EntriesDeInit(
    Entries_t* me);

/** resize the entries array
    \param me
    \param max the max number of entries this array
           should be able to store
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t EntriesResize(
    Entries_t* me,
    int max);

/** append an entry to the end of the array
    \param me
    \param entry a pointer to the entry to append
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t EntriesAppend(
    Entries_t* me,
    Entry_t* entry);

/** insert an entry at the specified index within the array
    \param me
    \param entry a pointer to the entry to append
    \param index the position to insert between 0 and the
           current number of entries
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t EntriesInsertAt(
    Entries_t* me,
    Entry_t* entry,
    int index);

/** an EntriesFindCallback_t to find an older entry.
    see EntriesFind()
    \param entry
    \param entryTime
    \param userData pointer to an Entry_t to compare to
    \returns RESULT_OK if entry is older than userData
*/
static Result_t CallbackEntryIsOlder(
    Entry_t* entry,
    EntryTime_t entryTime,
    void* userData);

/** an EntriesFindCallback_t to find a newer entry.
    see EntriesFind()
    \param entry
    \param userData pointer to an Entry_t to compare to
    \returns RESULT_OK if entry is newer than userData
*/
static Result_t CallbackEntryIsNewer(
    Entry_t* entry,
    EntryTime_t entryTime,
    void* userData);

/** find an entry
    \param me
    \param findCallback callback to define find conditions
    \param entryTime the time to compare while searching
    \param userData passed to the callback
    \returns the index of the entry that was found or
             INVALID_POSITION if not found
*/
static int EntriesFind(
    Entries_t* me,
    EntriesFindCallback_t findCallback,
    EntryTime_t entryTime,
    void* userData);

/** remove an entry from the end of the array
    \param me
    \returns a pointer to the entry or NULL if none exist
*/
static Entry_t* EntriesTakeLast(
    Entries_t* me);

// --- ENTRIES TRAVERSE ---

/** init. call EntriesTraverseFirst() after init to
    begin traversing an array
    \param me
*/
static void EntriesTraverseInit(
    EntriesTraverse_t* me);

/** deinit
    \param me
*/
static void EntriesTraverseDeInit(
    EntriesTraverse_t* me);

/** get the first entry based on the specified order
    \param me
    \param entries the entries to traverse
    \param order the order to traverse the entries with
            subsequent calls to EntriesTraverseNext()
    \param entryTime the time to compare when sorting
           by ORDER_NEWEST_FIRST or ORDER_OLDEST_FIRST
    \returns the first entry or NULL if none remain
*/
static Entry_t* EntriesTraverseFirst(
    EntriesTraverse_t* me,
    Entries_t* entries,
    Order_t order,
    EntryTime_t entryTime);

/** get the next entry based on the order started by
    the last call to EntriesTraverseFirst()
    \param me
    \returns the next entry or NULL if none remain
*/
static Entry_t* EntriesTraverseNext(
    EntriesTraverse_t* me);

// --- MANAGED ENTRIES ---

/** init
    \param me
    \param max the max entries allowed
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t ManagedEntriesInit(
    ManagedEntries_t* me,
    int max);

/** deinit
    \param me
*/
static void ManagedEntriesDeInit(
    ManagedEntries_t* me);

/** append an entry to the array
    \param me
    \param text the entry text. cannot be empty.
    \param start optional start date.
           see TimeFromDateString()
    \param end optional end date when start is specified.
           see TimeFromDateString()
    \returns the entry or NULL for error
*/
static Entry_t* ManagedEntriesAppend(
    ManagedEntries_t* me,
    const char* text,
    const char* start,
    const char* end);

/** append an entry to the array using part of a string
    \param me
    \param text the entry text
    \param textLength the entry text length
    \param start optional start date.
           see TimeFromDateString()
    \param end optional end date when start is specified.
           see TimeFromDateString()
    \returns the entry or NULL for error
*/
static Entry_t* ManagedEntriesAppendSubstr(
    ManagedEntries_t* me,
    const char* text,
    int textLength,
    const char* start,
    const char* end);

// --- ENTRY ---

/** init
    \param me
*/
static void EntryInit(
    Entry_t* me);

/** deinit
    \param me
*/
static void EntryDeInit(
    Entry_t* me);

/** set the entry text.
    this creates a deep copy of the string.
    \param me
    \param text optional pointer to a null-terminated
                string. use NULL to clear the text.
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t EntrySetText(
    Entry_t* me,
    const char* text);

/** set the entry text to a substring.
    this creates a deep copy of the string.
    \param me
    \param text optional pointer to a null-terminated
                string. use NULL to clear the text.
    \param textLength length of text
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t EntrySetTextSubstr(
    Entry_t* me,
    const char* text,
    int textLength);

/** get a date string from the entry time.
    this uses TimeToDateString() to get the string
    unless the timestamp value is TIMESTAMP_UNSET
    or the dateOption prevents the string from being
    written entirely
    \param me
    \param dest
    \param destLength
    \param entryTime
    \param dateOption
    \returns positing within dest where writing date
             string data was stopped.
             Note: the entire destLength will still be
             filled with either spaces or periods
             depending on whether the dateOption allowed
             anything to be written
*/
static int EntryTimeToString(
    Entry_t* me,
    char* dest,
    int destLength,
    EntryTime_t entryTime,
    DateOption_t dateOption);

// --- SECTION DATA ---

/** init
    \param me
*/
static void SectionDataInit(
    SectionData_t* me);

/** deinit
    \param me
*/
static void SectionDataDeInit( // recursive
    SectionData_t* me);

/** set the section title.
    this creates a deep copy of the string.
    \param me
    \param text optional pointer to a null-terminated
                string. use NULL to clear the text.
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t SectionDataSetTitle(
    SectionData_t* me,
    const char* title);

/** set the section title.
    this creates a deep copy of the string.
    \param me
    \param title optional pointer to a null-terminated
                 string. use NULL to clear the text.
    \param titleLength length of title
    \returns RESULT_OK, RESULT_WARNING, or RESULT_ERROR
*/
static Result_t SectionDataSetTitleSubstr(
    SectionData_t* me,
    const char* title,
    int titleLength);

/** include an entry in this section
    \param me
    \param entry pointer to the entry
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t SectionDataIncludeEntry(
    SectionData_t* me,
    Entry_t* entry);

// --- JSON ---

/** find the data length of a value
    \param json the json data. examples:
                - 1000 will return 4
                - "text" will return 6
                - true will return 4
    \param jsonState this will be updated only if an
                     error occurs. \see enum JsonState_e
    \returns the value data length or
             0 if json does not start with a value
*/
static int JsonValueDataLength( // recursive
    const char* json,
    JsonState_t* jsonState);

/** find the data length of a number
    \param json the json data. example: 5.1
                will return 3 as the data length
    \param jsonState this will be updated only if an
                     error occurs. \see enum JsonState_e
    \returns the value data length or
             0 if json does not start with a number
*/
static int JsonNumberDataLength(
    const char* json,
    JsonState_t* jsonState);

/** find the data length of a single quoted string
    \param json the json data. example: "string": "value"
                will return 8 as the data length
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns the string data length including the quotes or
             0 if json does not start with a '"' or
             1 if an end '"' was not found
*/
static int JsonStringDataLength(
    const char* json,
    JsonState_t* jsonState);

/** find the data length of an array
    \param json the json data. example: ["value1","value2"]
                will return 19 as the data length
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns the array data length including the brackets or
             0 if json does not start with a '[' or
             1 if an end ']' was not found
*/
static int JsonArrayDataLength( // recursive
    const char* json,
    JsonState_t* jsonState);

/** find the data length of an object
    \param json the json data. example: {"key":"value"}
                will return 15 as the data length
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns the object data length including the braces or
             0 if json does not start with a '{' or
             1 if an end '}' was not found
*/
static int JsonObjectDataLength( // recursive
    const char* json,
    JsonState_t* jsonState);

/** skip the current json data
    \param json current json data
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns pointer to the next, non-whitespace, json data.
             IMPORTANT: if the skip cannot occur the
                        original pointer is returned
             good examples:
             - if '[' then after the closing ']':
               ',', ']', '}', or '\0'
             - if '{' then after the closing '}':
               ',', ']', '}', or '\0'
             - if '"' then after the closing '"':
               ',', ']', '}', ':', or '\0'
             - if ':' then after the value:
               ',', '}', or '\0'
             - if a number then after it:
               ',', ']', '}', or '\0'
             - if a boolean then after it:
               ',', ']', '}', or '\0'
             - if a null then after it:
               ',', ']', '}', or '\0'
             - if ',' then after it:
               '{', '[', or '"'
             - if whitespace then a non-whitespace after it
*/
static const char* JsonSkip( // recursive
    const char* json,
    JsonState_t* jsonState);

/** uses JsonSkip() to find the start of the value for
    the specified key
    \param json current json data within an object
    \param key the key to search for within the object
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns a pointer to the start of the value for this
             key or the end of the object if the key is not
             found. the start of the value is any
             non-whitespace character following the ':'
             after the key
*/
static const char* JsonSkipToKeyValue( // recursive
    const char* json,
    const char* key,
    JsonState_t* jsonState);

/** uses JsonSkip() to find the start of the value for
    the next key within an object.
    \param json current json data within an object.
                the next non-whitespace character should
                be one of the following: '"', ',', '}', '\0'
                Note: when looping through object keys,
                      this pointer must be the return value
                      of the previous call to this function
                      plus the value length.
                      \see JsonValueDataLength()
    \param key this will be set to the next key or NULL
               if no key was found
    \param keyLength this is the length of the next key
                     or < 2 if no key was found
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns a pointer to the start of the value for the
             next key or the end of the object if there
             are no more keys. the start of the value is
             any non-whitespace character following the ':'
             after the key. use JsonValueDataLength() to
             find the lenth of the value.
*/
static const char* JsonSkipToNextValue( // recursive
    const char* json,
    const char** key,
    int* keyLength,
    JsonState_t* jsonState);

/** find the position in the json data where the next
    array index begins.
    \param json the json data at the first character within
                the array at the current index.
                - for currentIndex == INVALID_POSITION this
                  should be the first character after the
                  open bracket '['
                - for currentIndex >= 0 this should be the
                  first character of the data at that index
                  past the '[' or ','
    \param currentIndex set this to the current index or to
                        INVALID_POSITION when looking for
                        the start of index 0. this will be
                        updated to the next index value on
                        success or to INVALID_POSITION if
                        no array data remains.
    \param jsonState this will be updated only if an error
                     occurs. \see enum JsonState_e
    \returns a pointer to the first character of the
             json value at currentIndex.
             if currentIndex is INVALID_POSITION then a
             pointer to: ']' or '\0'
*/
static const char* JsonArrayNextIndex( // recursive
    const char* json,
    int* currentIndex,
    JsonState_t* jsonState);

// --- RESUME ---

/** init
    \param me
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t ResumeInit(
    Resume_t* me,
    ResumeOptions_t* resumeOptions);

/** check if the specified section should be hidden
    \param me
    \param sectionTitle null-terminated string
    \returns RESULT_YES if hidden or RESULT_NO
*/
static Result_t ResumeOptionsIsHiddenSection(
    ResumeOptions_t* me,
    const char* sectionTitle);

/** deinit
    \param me
*/
static void ResumeDeInit(
    Resume_t* me);

/** insert section data at runtime
    \param me
    \param section the section index to insert
            new section data. a value
            >= SECTION_AT_RUNTIME and
            <= ResumeGetSectionCount()
    \returns pointer to the initialized section data
             or NULL if it could not be inserted
*/
SectionData_t* ResumeInsertSection(
    Resume_t* me,
    Section_t section);

/** remove a section without deinit
*/
SectionData_t* ResumeRemoveSection(
    Resume_t* me,
    Section_t section);

/** get the total number of hard-coded
    and runtime sections
*/
Section_t ResumeGetSectionCount(
    Resume_t* me);

/** get section data from the resume
    \param me
    \param section see enum Section_e
    \returns section data or NULL if it does not exist
*/
SectionData_t* ResumeGetSectionData(
    Resume_t* me,
    Section_t section);

/** get section index from the resume
    \param me
    \param sectionData
    \returns see enum Section_e
*/
Section_t ResumeGetSectionIndex(
    Resume_t* me,
    SectionData_t* sectionData);

/** get a section title from the resume
    \param me
    \param section see enum Section_e
    \returns section title or empty string if there is none
*/
static const char* ResumeGetSectionTitle(
    Resume_t* me,
    Section_t section);

/** add entry to the resume under the specified section
    \param me
    \param section see enum Section_e
    \param text the entry text. cannot be empty.
    \param start optional start date.
           see TimeFromDateString()
    \param end optional end date when start is specified.
           see TimeFromDateString()
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t ResumeAddEntry(
    Resume_t* me,
    Section_t section,
    const char* text,
    const char* start,
    const char* end);

/** add entry to the specified section
    \param me
    \param resume
    \param text the entry text
    \param textLength the entry text length
    \param start optional start date.
           see TimeFromDateString()
    \param end optional end date when start is specified.
           see TimeFromDateString()
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t SectionAddEntry(
    SectionData_t* me,
    Resume_t* resume,
    const char* text,
    int textLength,
    const char* start,
    const char* end);

/** print html version of resume to standard out
*/
static void ResumePrintHtml(
    Resume_t* me);

/** print text version of resume to standard out
*/
static void ResumePrintPlainText(
    Resume_t* me);

/** fill resume sections at runtime from a json array
    \param me
    \param json a pointer to the start of a json array.
                Note: any whitespace before the '['
                      will be ignored
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t ResumeFillSectionsFromJsonArray(
    Resume_t* me,
    const char* json);

/** fill the section at runtime from a json object
    \param me
    \param sectionData
    \param json a pointer to the start of a json object.
                Note: any whitespace before the '{'
                      will be ignored
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t ResumeFillSectionFromJsonObject(
    Resume_t* me,
    SectionData_t* sectionData,
    const char* json);

/** fill resume entries at runtime from a json array
    \param me
    \param sectionData
    \param json a pointer to the start of a json array.
                Note: any whitespace before the '['
                      will be ignored
    \returns RESULT_OK or RESULT_ERROR
*/
static Result_t ResumeFillEntriesFromJsonArray(
    Resume_t* me, 
    SectionData_t* sectionData,
    const char* json);

/** fill the resume from hard-coded data.
    calls ResumeFillSectionsFromHardCoded() and ResumeFillEntriesFromHardCoded()
*/
static Result_t ResumeFillHardCoded(
    Resume_t* me);

/** fill the resume with section data
*/
static Result_t ResumeFillSectionsFromHardCoded(
    Resume_t* me);

/** fill the resume with entries
*/
static Result_t ResumeFillEntriesFromHardCoded(
    Resume_t* me);

//
// MARK: GLOBALS
//

static const char g_JsonKeywordTrue[]  = "true";
static const char g_JsonKeywordFalse[] = "false";
static const char g_JsonKeywordNull[]  = "null";

static const char g_ResumeFullName[]     = RESUME_FULL_NAME;
static const char g_ResumeEmail[]        = RESUME_EMAIL;
static const char g_ResumeCity[]         = RESUME_CITY;
static const char g_ResumeState[]        = RESUME_STATE;
static const char g_ResumeMobileNumber[] = RESUME_MOBILE_NUMBER;
static const char g_ResumeWebUrl[]       = RESUME_WEB_URL;
static const char g_ResumeJsonData[]     = RESUME_JSON_DATA;

static const char g_EmptyText[] = "";

/** lookup table to find text version of entry state
*/
static const char* g_EntryStateLookup[ENTRY_STATE_COUNT] =
{
    "unknown",               // ENTRY_STATE_UNKNOWN
    "ok",                    // ENTRY_STATE_OK
    "error-full",            // ENTRY_STATE_ERROR_FULL
    "error-text-empty",      // ENTRY_STATE_ERROR_TEXT_EMPTY
    "error-text-missing",    // ENTRY_STATE_ERROR_TEXT_MISSING
    "error-start-date",      // ENTRY_STATE_ERROR_START_DATE
    "error-end-date",        // ENTRY_STATE_ERROR_END_DATE
    "error-start-missing",   // ENTRY_STATE_ERROR_START_DATE_MISSING
    "error-alloc",           // ENTRY_STATE_ERROR_ALLOC
    "error-set-text",        // ENTRY_STATE_ERROR_SET_TEXT
    "error-resize",          // ENTRY_STATE_ERROR_RESIZE
    "error-section-missing"  // ENTRY_STATE_ERROR_SECTION_MISSING
};

//
// MARK: HELPER MACROS
//

/** indicate a param is not used
*/
#define PARAM_NOT_USED(var) (void)var;

/** check if the character is whitespace
*/
#define IS_WHITESPACE(c) ((c) <= ' ')

/** skip whitespace including most non-printable characters
*/
#define SKIP_WHITESPACE(ptr)\
while(IS_WHITESPACE(*ptr))\
{\
    if((*ptr) == '\0')\
    {\
        break;\
    }\
    ptr++;\
}

/** skip all characters until null-terminator is found
*/
#define SKIP_TO_NULL(ptr)\
while((*ptr) != '\0')\
{\
    ptr++;\
}

//
// MARK: SOURCE
//

// MARK: --- MAIN ---

/**********************************************************/
static void MainPrintHelp(
    const char* programName)
{
    printf("Usage: %s [options]\n", programName);
    printf("\n");
    printf("  options:\n");
    printf("\n");
    printf("    -a               display all entries. overrides -e\n");
    printf("\n");
    printf("    -e [count]       extend entry display\n");
    printf("        count:\n");
    printf("          1 - %d     this number of additional entries will\n", RESUME_MAX_LINES_PER_PAGE);
    printf("                     be shown in each section when available\n");
    printf("\n");
    printf("    -f [format]      format of the resume output\n");
    printf("        format:\n");
    printf("          html       single-file HTML\n");
    printf("          text       single-file plain text *default*\n");
    printf("\n");
    printf("    -h               this help text\n");
    printf("\n");
    printf("    -v               version\n");
    printf("\n");
}

/**********************************************************/
static void MainPrintVersion(void)
{
    printf("%s Resume v%d.%02d.%02d\n", g_ResumeFullName, MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
    printf("\n");
}

// MARK: --- STRING ---

/**********************************************************/
static Result_t SetText(
    char** dest,
    const char* src)
{
    // set the text with practically no upper bounds
    return SetTextSubstr(dest, src, MAX_STRING_LENGTH);
}

/**********************************************************/
static Result_t SetTextSubstr(
    char** dest,
    const char* src,
    int srcLength)
{
    Result_t result = RESULT_OK;

    if(dest)
    {
        unsigned long length = 0;

        // remove the current text
        if((*dest) != g_EmptyText)
        {
            free((*dest));
        }

        // get the string length
        if(src)
        {
            length = strlen(src);
            if(srcLength < length)
            {
                length = srcLength;
            }
        }

        if(length > 0)
        {
            // allocate memory for the text and null-terminater
            // then copy the text
            (*dest) = (char*)malloc(((length + 1) * sizeof(char)));
            if((*dest))
            {
                memcpy((*dest), src, length);
                (*dest)[length] = '\0';
            }
            else
            {
                // failed to allocated memory
                result = RESULT_ERROR;

                // safe to use shared empty text because
                // all functions check for this
                (*dest) = (char*)g_EmptyText;
            }
        }
        else
        {
            // set zero length clears the text

            // safe to use shared empty text because
            // all functions check for this
            (*dest) = (char*)g_EmptyText;
        }
    }
    else
    {
        // dest is invalid
    }

    return result;
}

/**********************************************************/
static int FindUppercase(
    const char* text,
    int maxChars)
{
    int pos = 0;
    while(pos < maxChars)
    {
        char current = text[pos];
        if(current == '\0')
        {
            // end of string.
            // uppercase character not found
            return INVALID_POSITION;
        }
        else if(current != (char)tolower(current))
        {
            // current is uppercase
            return pos;
        }
        pos++;
    }
    // finished max characters.
    // uppercase character not found
    return INVALID_POSITION;
}

// MARK: --- TIME AND DATE ---

/** used by GetDaysInMonth() to check if the
    year is a leap year.
    it's always a leap year every 400 years.
    it's also a leap year every 4 years except
    every 100 years.
*/
#define IS_LEAP_YEAR(year) (((year % 400) == 0) || (((year % 4) == 0) && ((year % 100) != 0)))

/**********************************************************/
static int GetDaysInMonth(
    int month,
    int year)
{
    int days;

    switch(month)
    {
    case MONTH_FEB:
    {
        if(IS_LEAP_YEAR(year))
        {
            days = 29;
        }
        else
        {
            days = 28;
        }
    }
    break;

    case MONTH_SEP: // fall-through
    case MONTH_APR: // fall-through
    case MONTH_JUN: // fall-through
    case MONTH_NOV:
    {
        days = 30;
    }
    break;

    default:
    {
        days = 31;
    }
    break;

    }

    return days;
}

/**********************************************************/
static int TimeToDateString(
    time_t time,
    char* dest,
    int destLength,
    DateOption_t dateOption)
{
    int pos = 0;
    struct tm* tm = gmtime(&time);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;

    // sanity checks to prevent string
    // format buffer overflow
    if((year < 1900) || (year > 9999))
    {
        year = 0;
    }
    if((month < 0) || (month > 99))
    {
        month = 0;
    }
    if((day < 0) || (day > 99))
    {
        day = 0;
    }

    if((dateOption & DATE_OPTION_HIDE_YEAR) != DATE_OPTION_HIDE_YEAR)
    {
        snprintf(&dest[pos], (destLength - pos), "%04d", year);
        pos += 4;
    }

    if((dateOption & DATE_OPTION_HIDE_MONTH) != DATE_OPTION_HIDE_MONTH)
    {
        if(pos > 0)
        {
            dest[pos] = '-';
            pos++;
        }
        snprintf(&dest[pos], (destLength - pos), "%02d", month);
        pos += 2;

        if((dateOption & DATE_OPTION_HIDE_DAY) != DATE_OPTION_HIDE_DAY)
        {
            if(pos > 0)
            {
                dest[pos] = '-';
                pos++;
            }
            snprintf(&dest[pos], (destLength - pos), "%02d", day);
            pos += 2;
        }
    }
    else
    {
        // month is hidden.
        // it is too confusing to allow the day to be shown
        // without the month so no additional date fields
        // are filled here
    }

    return pos;
}

/**********************************************************/
static Result_t TimeFromDateString(
    time_t* time,
    const char* date)
{
    Result_t result = RESULT_OK;

    if(time)
    {
        (*time) = TIMESTAMP_UNSET;
    }
    else
    {
        // time to update is missing
        result = RESULT_WARNING;
    }

    if(date)
    {
        if(date[0] != '\0')
        {
            struct tm tm  = { 0 };
            if(sscanf(date, DATE_STRING_FORMAT,
                       &tm.tm_year,
                       &tm.tm_mon,
                       &tm.tm_mday) == 3)
            {
                tm.tm_mon--;
                if((tm.tm_mon >= MONTH_JAN) &&
                    (tm.tm_mon <= MONTH_DEC))
                {
                    int daysInMonth = GetDaysInMonth(tm.tm_mon, tm.tm_year);
                    if((tm.tm_mday >= 1) &&
                        (tm.tm_mday <= daysInMonth))
                    {
                        if((tm.tm_year < 1984) ||
                            (tm.tm_year > 2084))
                        {
                            // I'm either in diapers or over
                            // 100 years old.
                            // consider this a warning :)
                            result = RESULT_WARNING;
                        }
                        tm.tm_year -= 1900;
                        if(time)
                        {
                            (*time) = mktime(&tm);
                        }
                        else
                        {
                            // time to update is missing
                            result = RESULT_WARNING;
                        }
                    }
                    else
                    {
                        // invalid day
                        result = RESULT_ERROR;
                    }
                }
                else
                {
                    // invalid month
                    result = RESULT_ERROR;
                }
            }
            else
            {
                // invalid date format
                result = RESULT_ERROR;
            }
        }
        else
        {
            // empty date.
            // this is ok.
        }
    }
    else
    {
        // no date.
        // this is ok.
    }

    return result;
}

// MARK: --- ENTRIES ---

/**********************************************************/
static void EntriesInit(
    Entries_t* me)
{
    me->m_Entries = NULL;
    me->m_EntriesMax = 0;
    me->m_EntriesUsed = 0;
}

/**********************************************************/
static void EntriesDeInit(
    Entries_t* me)
{
    if(me->m_Entries)
    {
        free(me->m_Entries);
        me->m_Entries = NULL;
        me->m_EntriesMax = 0;
    }
    me->m_EntriesUsed = 0;
}

/**********************************************************/
static Result_t EntriesResize(
    Entries_t* me,
    int max)
{
    Result_t result = RESULT_ERROR;

    if(me->m_EntriesMax >= max)
    {
        // resize not needed
        result =  RESULT_OK;
    }
    else if(max > 0)
    {
        Entry_t** entries = (Entry_t**)realloc(me->m_Entries, sizeof(Entry_t*) * max);
        if(entries)
        {
            me->m_Entries = entries;
            me->m_EntriesMax = max;
            result = RESULT_OK;
        }
        else
        {
            // error. failed to allocate memory
        }
    }
    else
    {
        // warning. invalid max value
        result = RESULT_WARNING;
    }

    return result;
}

/**********************************************************/
static Result_t EntriesAppend(
    Entries_t* me,
    Entry_t* entry)
{
    Result_t result = RESULT_ERROR;

    if(entry == NULL)
    {
        // warning. cannot append null entry
        result = RESULT_WARNING;
    }
    else if(EntriesResize(me, (me->m_EntriesUsed + 1)) == RESULT_OK)
    {
        me->m_Entries[me->m_EntriesUsed] = entry;
        me->m_EntriesUsed++;
        result = RESULT_OK;
    }
    else
    {
        // error. cannot resize
    }

    return result;
}

/**********************************************************/
static Result_t EntriesInsertAt(
    Entries_t* me,
    Entry_t* entry,
    int index)
{
    Result_t result = RESULT_ERROR;

    if(entry == NULL)
    {
        // warning. cannot insert null entry
        result = RESULT_WARNING;
    }
    else if((index < 0) || (index > me->m_EntriesUsed))
    {
        // error. cannot insert at this index
        result = RESULT_WARNING;
    }
    else if(EntriesResize(me, (me->m_EntriesUsed + 1)) == RESULT_OK)
    {
        // shift all entries >= index before inserting the entry
        int shiftIndex = (me->m_EntriesUsed - 1);
        while(shiftIndex >= index)
        {
            me->m_Entries[shiftIndex + 1] = me->m_Entries[shiftIndex];
            shiftIndex--;
        }
        me->m_Entries[index] = entry;
        me->m_EntriesUsed++;
        result = RESULT_OK;
    }
    else
    {
        // error. cannot resize
    }

    return result;
}

/**********************************************************/
static Result_t CallbackEntryIsOlder(
    Entry_t* entry,
    EntryTime_t entryTime,
    void* userData)
{
    Entry_t* compareToEntry = (Entry_t*)userData;
    if(compareToEntry->m_EntryTimestamp[entryTime] == TIMESTAMP_UNSET)
    {
        // the entry is always older when comparing
        // against an entry with no timestamp
        return RESULT_OK;
    }
    else if(entry->m_EntryTimestamp[entryTime] != TIMESTAMP_UNSET)
    {
        // both entries have a timestamp.
        // check if entry is older (smaller timestamp)
        if(entry->m_EntryTimestamp[entryTime] < compareToEntry->m_EntryTimestamp[entryTime])
        {
            // the entry is older
            return RESULT_OK;
        }
    }

    // allow search to continue
    return RESULT_NEXT;
}

/**********************************************************/
static Result_t CallbackEntryIsNewer(
    Entry_t* entry,
    EntryTime_t entryTime,
    void* userData)
{
    Entry_t* compareToEntry = (Entry_t*)userData;
    if(entry->m_EntryTimestamp[entryTime] == TIMESTAMP_UNSET)
    {
        // the entry is always newer when
        // it has no timestamp
        return RESULT_OK;
    }
    else if(compareToEntry->m_EntryTimestamp[entryTime] != TIMESTAMP_UNSET)
    {
        // both entries have a timestamp.
        // check if entry is newer (larger timestamp)
        if(entry->m_EntryTimestamp[entryTime] > compareToEntry->m_EntryTimestamp[entryTime])
        {
            // the entry is newer
            return RESULT_OK;
        }
    }

    // allow search to continue
    return RESULT_NEXT;
}

/**********************************************************/
static int EntriesFind(
    Entries_t* me,
    EntriesFindCallback_t findCallback,
    EntryTime_t entryTime,
    void* userData)
{
    int index = 0;
    while(index < me->m_EntriesUsed)
    {
        Result_t result = findCallback(
            me->m_Entries[index],
            entryTime,
            userData);
        if(result == RESULT_OK)
        {
            // callback found desired entry at this index
            return index;
        }
        else if(result == RESULT_ERROR)
        {
            // callback has canceled this search
            break;
        }
        index++;
    }

    return INVALID_POSITION;
}

/**********************************************************/
static Entry_t* EntriesTakeLast(
    Entries_t* me)
{
    Entry_t* entry = NULL;

    if(me->m_EntriesUsed > 0)
    {
        me->m_EntriesUsed--;
        entry = me->m_Entries[me->m_EntriesUsed];
    }

    return entry;
}

// MARK: --- ENTRIES TRAVERSE ---

/**********************************************************/
static void EntriesTraverseInit(
    EntriesTraverse_t* me)
{
    EntriesInit(&me->m_EntriesTraverse);
    me->m_EntriesTraverseNumber = INVALID_POSITION;
}

/**********************************************************/
static void EntriesTraverseDeInit(
    EntriesTraverse_t* me)
{
    EntriesDeInit(&me->m_EntriesTraverse);
    me->m_EntriesTraverseNumber = INVALID_POSITION;
}

/**********************************************************/
static Entry_t* EntriesTraverseFirst(
    EntriesTraverse_t* me,
    Entries_t* entries,
    Order_t order,
    EntryTime_t entryTime)
{
    Entry_t* firstEntry = NULL;

    if(entries == NULL)
    {
        // entries is missing
        me->m_EntriesTraverseNumber = INVALID_POSITION;
        return NULL;
    }

    me->m_EntriesTraverseNumber = 0;

    if(entries->m_EntriesUsed > 0)
    {
        if(EntriesResize(&me->m_EntriesTraverse, entries->m_EntriesUsed) == RESULT_OK)
        {
            int entryIndex;
            Entry_t* insertEntry;
            EntriesFindCallback_t findCallback = NULL;

            // select the callback to use for the
            // insertion sort
            switch(order)
            {
            case ORDER_RANDOM:
            {
                // no sorting necessary. just select
                // random positions to insert the entries.
                me->m_EntriesTraverse.m_EntriesUsed = 0;
                EntriesAppend(&me->m_EntriesTraverse, entries->m_Entries[0]);

                entryIndex = 1;
                while(entryIndex < entries->m_EntriesUsed)
                {
                    int insertIndex = rand() % (me->m_EntriesTraverse.m_EntriesUsed + 1);
                    insertEntry = entries->m_Entries[entryIndex];
                    EntriesInsertAt(&me->m_EntriesTraverse, insertEntry, insertIndex);
                    entryIndex++;
                }
            }
            break;

            case ORDER_OLDEST_FIRST:
            {
                // perform insertion sort by looking for
                // a newer entry and inserting the older
                // entry before it
                findCallback = CallbackEntryIsNewer;
            }
            break;

            case ORDER_NEWEST_FIRST: // fall-through
            default:
            {
                // perform insertion sort by looking for
                // an older entry and inserting the newer
                // entry before it
                findCallback = CallbackEntryIsOlder;
            }
            break;
            }

            if(findCallback)
            {
                // create the ordered entries by adding the
                // first entry to the empty array and then
                // continuing with a basic insertion sort
                me->m_EntriesTraverse.m_EntriesUsed = 0;
                EntriesAppend(&me->m_EntriesTraverse, entries->m_Entries[0]);

                entryIndex = 1;
                while(entryIndex < entries->m_EntriesUsed)
                {
                    int insertIndex;

                    // use the find callback to select
                    // a position in the traverse
                    // entries array
                    insertEntry = entries->m_Entries[entryIndex];
                    insertIndex = EntriesFind(&me->m_EntriesTraverse, findCallback, entryTime, insertEntry);
                    if(insertIndex != INVALID_POSITION)
                    {
                        // found entry at insertIndex.
                        // place this entry before it
                        EntriesInsertAt(&me->m_EntriesTraverse, insertEntry, insertIndex);
                    }
                    else
                    {
                        // entry can be placed at the end
                        EntriesAppend(&me->m_EntriesTraverse, insertEntry);
                    }
                    entryIndex++;
                }
            }
            else
            {
                // not using insertion sort
            }

            firstEntry = EntriesTraverseNext(me);
        }
        else
        {
            // error. failed to resize the entries
        }
    }
    else
    {
        // no entries
    }

    return firstEntry;
}

/**********************************************************/
static Entry_t* EntriesTraverseNext(
    EntriesTraverse_t* me)
{
    Entry_t* entry = NULL;

    if(me->m_EntriesTraverseNumber < 0)
    {
        // error. must first call EntriesTraverseFirst()
        return NULL;
    }

    if(me->m_EntriesTraverseNumber < me->m_EntriesTraverse.m_EntriesUsed)
    {
        entry = me->m_EntriesTraverse.m_Entries[me->m_EntriesTraverseNumber];
        me->m_EntriesTraverseNumber++;
    }
    else
    {
        // no more entries
    }

    return entry;
}

// MARK: --- MANAGED ENTRIES ---

/**********************************************************/
static Result_t ManagedEntriesInit(
    ManagedEntries_t* me,
    int max)
{
    EntriesInit(&me->m_ManagedEntries);
    me->m_ManagedEntriesError = ENTRY_STATE_UNKNOWN;
    return EntriesResize(&me->m_ManagedEntries, max);
}

/**********************************************************/
static void ManagedEntriesDeInit(
    ManagedEntries_t* me)
{
    Entry_t* entry = EntriesTakeLast(&me->m_ManagedEntries);
    while(entry)
    {
        EntryDeInit(entry);
        free(entry);
        entry = EntriesTakeLast(&me->m_ManagedEntries);
    }

    EntriesDeInit(&me->m_ManagedEntries);
}

/**********************************************************/
static Entry_t* ManagedEntriesAppend(
    ManagedEntries_t* me,
    const char* text,
    const char* start,
    const char* end)
{
    // set the text with practically no upper bounds
    return ManagedEntriesAppendSubstr(me, text, MAX_STRING_LENGTH, start, end);
}

/**********************************************************/
static Entry_t* ManagedEntriesAppendSubstr(
    ManagedEntries_t* me,
    const char* text,
    int textLength,
    const char* start,
    const char* end)
{
    time_t timeStart = TIMESTAMP_UNSET;
    time_t timeEnd = TIMESTAMP_UNSET;
    Entry_t* entry = NULL;

    if(me->m_ManagedEntries.m_EntriesUsed == me->m_ManagedEntries.m_EntriesMax)
    {
        // no entries available
        me->m_ManagedEntriesError = ENTRY_STATE_ERROR_FULL;
        return NULL;
    }

    // make sure text is valid
    if(text)
    {
        if(text[0] == '\0')
        {
            // empty text is not allowed
            me->m_ManagedEntriesError = ENTRY_STATE_ERROR_TEXT_EMPTY;
            return NULL;
        }
    }
    else
    {
        // text is missing
        me->m_ManagedEntriesError = ENTRY_STATE_ERROR_TEXT_MISSING;
        return NULL;
    }

    // make sure dates are valid
    if(start)
    {
        if(TimeFromDateString(&timeStart, start) == RESULT_ERROR)
        {
            // start date is optional, but this date
            // could not parsed
            me->m_ManagedEntriesError = ENTRY_STATE_ERROR_START_DATE;
            return NULL;
        }

        if(end)
        {
            if(TimeFromDateString(&timeEnd, end) == RESULT_ERROR)
            {
                // end date is optional, but this date
                // could not parsed
                me->m_ManagedEntriesError = ENTRY_STATE_ERROR_END_DATE;
                return NULL;
            }
        }
        else
        {
            // no end date. this is ok
        }
    }
    else
    {
        // no start date. this is probably ok
        if(end)
        {
            // cannot have end date without start date
            me->m_ManagedEntriesError = ENTRY_STATE_ERROR_START_DATE_MISSING;
            return NULL;
        }
    }

    // create the entry and place it in the array
    entry = (Entry_t*)malloc(sizeof(Entry_t));
    if(entry)
    {
        EntryInit(entry);
        if(EntrySetTextSubstr(entry, text, textLength) == RESULT_OK)
        {
            // save the previously parsed dates
            entry->m_EntryTimestamp[ENTRY_TIME_START] = timeStart;
            entry->m_EntryTimestamp[ENTRY_TIME_END] = timeEnd;

            // place the entry at the end of the array.
            // not using EntriesAppend() because managed
            // entries array is already sized during init
            me->m_ManagedEntries.m_Entries[me->m_ManagedEntries.m_EntriesUsed] = entry;
            me->m_ManagedEntries.m_EntriesUsed++;

            me->m_ManagedEntriesError = ENTRY_STATE_OK;
        }
        else
        {
            // could not set entry text
            me->m_ManagedEntriesError = ENTRY_STATE_ERROR_SET_TEXT;
            EntryDeInit(entry);
            free(entry);
            entry = NULL;
        }
    }
    else
    {
        // could not allocate entry
        me->m_ManagedEntriesError = ENTRY_STATE_ERROR_ALLOC;
    }

    return entry;
}

// MARK: --- ENTRY ---

/**********************************************************/
static void EntryInit(
    Entry_t* me)
{
    int index;
    // safe to use shared empty text because
    // all functions check for this
    me->m_EntryText = (char*)g_EmptyText;

    index = 0;
    do
    {
        me->m_EntryTimestamp[index] = TIMESTAMP_UNSET;
        index++;
    }
    while(index < ENTRY_TIME_COUNT);
}

/**********************************************************/
static void EntryDeInit(
    Entry_t* me)
{
    int index;
    SetText(&me->m_EntryText, NULL);

    index = 0;
    do
    {
        me->m_EntryTimestamp[index] = TIMESTAMP_UNSET;
        index++;
    }
    while(index < ENTRY_TIME_COUNT);
}

/**********************************************************/
static Result_t EntrySetText(
    Entry_t* me,
    const char* text)
{
    return SetText(&me->m_EntryText, text);
}

/**********************************************************/
static Result_t EntrySetTextSubstr(
    Entry_t* me,
    const char* text,
    int textLength)
{
    return SetTextSubstr(&me->m_EntryText, text, textLength);
}

/**********************************************************/
static int EntryTimeToString(
    Entry_t* me,
    char* dest,
    int destLength,
    EntryTime_t entryTime,
    DateOption_t dateOption)
{
    int pos = 0;
    char fillChar = ' ';

    if((dest == NULL) || (destLength <= 0))
    {
        // invalid destination
        return 0;
    }

    if(entryTime == ENTRY_TIME_START)
    {
        if((dateOption & DATE_OPTION_HIDE_START) != DATE_OPTION_HIDE_START)
        {
            // start time selected and not hidden.

            // select a fill character to make it easier
            // to follow a line when only partial data
            // info is displayed
            fillChar = '.';

            if(me->m_EntryTimestamp[ENTRY_TIME_START] != TIMESTAMP_UNSET)
            {
                // TimeToDateString() will check if both
                // start and end are hidden.
                // this entry is the start so force end
                // to be hidden.
                dateOption |= DATE_OPTION_HIDE_END;

                pos += TimeToDateString(
                    me->m_EntryTimestamp[ENTRY_TIME_START],
                    &dest[pos],
                    (destLength - pos),
                    dateOption);
            }
            else
            {
                // no start time
            }
        }
        else
        {
            // entire start date is hidden
        }
    }
    else if(entryTime == ENTRY_TIME_END)
    {
        if((dateOption & DATE_OPTION_HIDE_END) != DATE_OPTION_HIDE_END)
        {
            // end time selected and not hidden.

            // select a fill character to make it easier
            // to follow a line when only partial data
            // info is displayed
            fillChar = '.';

            if(me->m_EntryTimestamp[ENTRY_TIME_END] != TIMESTAMP_UNSET)
            {
                // TimeToDateString() will check if both
                // start and end are hidden.
                // this entry is the end so force start
                // to be hidden.
                dateOption |= DATE_OPTION_HIDE_START;

                pos += TimeToDateString(
                    me->m_EntryTimestamp[ENTRY_TIME_END],
                    &dest[pos],
                    (destLength - pos),
                    dateOption);
            }
            else
            {
                // no end time.
                // use "present"
                static const char presentString[] = "present";
                if((destLength - pos) >= ((int)sizeof(presentString) - 1))
                {
                    memcpy(&dest[pos], presentString, (sizeof(presentString) - 1));
                    pos += (sizeof(presentString) - 1);
                }
                else
                {
                    // not enough space for "present"
                }
            }
        }
        else
        {
            // entire end date is hidden
        }
    }

    // fill remaining length
    memset(&dest[pos], fillChar, (destLength - pos));

    return pos;
}

// MARK: --- SECTION DATA ---

/**********************************************************/
static void SectionDataInit(
    SectionData_t* me)
{
    // safe to use shared empty text because
    // all functions check for this
    me->m_SectionTitle = (char*)g_EmptyText;

    // default to display all entries from newest to oldest
    // (descending order)
    me->m_SectionDisplayMax = DISPLAY_ALL;
    me->m_SectionOrder = ORDER_NEWEST_FIRST;
    me->m_SectionOrderEntryTime = ENTRY_TIME_START;

    // default to display no date information
    me->m_SectionDateOption = DATE_OPTION_HIDE_ALL;

    EntriesInit(&me->m_SectionEntries);

    // default to no next section
    me->m_SectionNext = NULL;
}

/**********************************************************/
static void SectionDataDeInit( // recursive
    SectionData_t* me)
{
    if(me->m_SectionNext)
    {
        SectionDataDeInit(me->m_SectionNext); // recursive
        me->m_SectionNext = NULL;
    }
    SetText(&me->m_SectionTitle, NULL);
    EntriesDeInit(&me->m_SectionEntries);
}

/**********************************************************/
static Result_t SectionDataSetTitle(
    SectionData_t* me,
    const char* title)
{
    return SetText(&me->m_SectionTitle, title);
}

/**********************************************************/
static Result_t SectionDataSetTitleSubstr(
    SectionData_t* me,
    const char* title,
    int titleLength)
{
    return SetTextSubstr(&me->m_SectionTitle, title, titleLength);
}

/**********************************************************/
static Result_t SectionDataIncludeEntry(
    SectionData_t* me,
    Entry_t* entry)
{
    return EntriesAppend(&me->m_SectionEntries, entry);
}

// MARK: --- JSON ---

/**********************************************************/
static int JsonValueDataLength( // recursive
    const char* json,
    JsonState_t* jsonState)
{
    int length = 0;
    char current = (*json);
    if(current == '[')
    {
        length = JsonArrayDataLength(json, jsonState); // recursive
    }
    else if(current == '{')
    {
        length = JsonObjectDataLength(json, jsonState); // recursive
    }
    else if(current == '"')
    {
        length = JsonStringDataLength(json, jsonState);
    }
    else if(current >= '0' || current <= '9')
    {
        // decimal '.' is not checked here because 
        // json numbers cannot have a leading decimal.
        // see JSON_STATE_ERROR_LEADING_DECIMAL
        length = JsonNumberDataLength(json, jsonState);
    }
    else if(FindUppercase(json, JSON_VALUE_KEYWORD_MAX_LENGTH) == INVALID_POSITION)
    {
        // check for json keywords
        if(strncmp(json, g_JsonKeywordTrue, (sizeof(g_JsonKeywordTrue)-1)) == 0)
        {
            // true
            length = (sizeof(g_JsonKeywordTrue)-1);
        }
        else if(strncmp(json, g_JsonKeywordFalse, (sizeof(g_JsonKeywordFalse)-1)) == 0)
        {
            // false
            length = (sizeof(g_JsonKeywordFalse)-1);
        }
        else if(strncmp(json, g_JsonKeywordNull, (sizeof(g_JsonKeywordNull)-1)) == 0)
        {
            // null
            length = (sizeof(g_JsonKeywordNull)-1);
        }
        else
        {
            // not a json keyword
        }
    }
    else
    {
        // there are no json keywords
        // that contain uppercase characters.
        (*jsonState) = JSON_STATE_ERROR_VALUE_CASE;
    }

    if(length == 0)
    {
        (*jsonState) = JSON_STATE_ERROR_VALUE_INVALID;
    }

    return length;
}

/**********************************************************/
static int JsonNumberDataLength(
    const char* json,
    JsonState_t* jsonState)
{
    int decimalPos = INVALID_POSITION;
    int pos = 0;
    char current;
    current = json[pos];
    while (current != '\0')
    {
        if(current == '.')
        {
            // found decimal point
            if(pos > 0)
            {
                if(decimalPos == INVALID_POSITION)
                {
                    decimalPos = pos;
                }
                else
                {
                    // cannot have more than one decimal point.
                    (*jsonState) = JSON_STATE_ERROR_TOO_MANY_DECIMALS;
                    // invalid number
                    return 0;
                }
            }
            else
            {
                // must have a leading number
                (*jsonState) = JSON_STATE_ERROR_LEADING_DECIMAL;
                return 0;
            }
        }
        else if((current < '0') || (current > '9'))
        {
            // not a number.
            // let the caller determine if the character(s)
            // following the number are valid.
            return pos;
        }
        pos++;
        current = json[pos];
    }

    // end of json data
    return pos;
}

/**********************************************************/
static int JsonStringDataLength(
    const char* json,
    JsonState_t* jsonState)
{
    int pos;
    char current;

    if(json[0] != '"')
    {
        // quoted string must start with a quote
        (*jsonState) = JSON_STATE_ERROR_STRING_MISSING_START;
        return 0;
    }

    pos = 1;
    current = json[pos];
    while (current != '\0')
    {
        if(current == '"')
        {
            // end of quoted string
            return (pos + 1);
        }
        else if(current == '\\')
        {
            // escape next character
            pos++;
            current = json[pos];
            if(current == '\0')
            {
                // missing escaped character
                (*jsonState) = JSON_STATE_ERROR_STRING_MISSING_ESCAPED_CHAR;
                break;
            }
        }
        pos++;
        current = json[pos];
    }

    // end of quoted string not found
    (*jsonState) = JSON_STATE_ERROR_STRING_MISSING_END;

    return 1;
}

/**********************************************************/
static int JsonArrayDataLength( // recursive
    const char* json,
    JsonState_t* jsonState)
{
    int pos;
    char current;

    if(json[0] != '[')
    {
        // array must start with a bracket
        (*jsonState) = JSON_STATE_ERROR_ARRAY_MISSING_START;
        return 0;
    }

    pos = 1;
    current = json[pos];
    while (current != '\0')
    {
        if(current == ']')
        {
            // end of array
            return (pos + 1);
        }
        else
        {
            const char* currentPtr = &json[pos];
            const char* skipPtr = JsonSkip(currentPtr, jsonState); // recursive
            if(skipPtr != currentPtr)
            {
                pos += (int)(skipPtr - currentPtr);
                current = json[pos];
            }
            else
            {
                // failed to skip json data.
                // check jsonState for details.
                // cannot find the end of the array
                break;
            }
        }
    }

    // end of array not found
    (*jsonState) = JSON_STATE_ERROR_ARRAY_MISSING_END;

    return 1;
}

/**********************************************************/
static int JsonObjectDataLength( // recursive
    const char* json,
    JsonState_t* jsonState)
{
    int pos;
    char current;

    if(json[0] != '{')
    {
        // object must start with a brace
        (*jsonState) = JSON_STATE_ERROR_OBJECT_MISSING_START;
        return 0;
    }

    pos = 1;
    current = json[pos];
    while (current != '\0')
    {
        if(current == '}')
        {
            // end of object
            return (pos + 1);
        }
        else
        {
            const char* currentPtr = &json[pos];
            const char* skipPtr = JsonSkip(currentPtr, jsonState); // recursive
            if(skipPtr != currentPtr)
            {
                pos += (int)(skipPtr - currentPtr);
                current = json[pos];
            }
            else
            {
                // failed to skip json data.
                // check jsonState for details.
                // cannot find the end of the object
                break;
            }
        }
    }

    // end of object not found
    (*jsonState) = JSON_STATE_ERROR_OBJECT_MISSING_END;

    return 1;
}

/**********************************************************/
static const char* JsonSkip( // recursive
    const char* json,
    JsonState_t* jsonState)
{
    const char* ptr = json;
    char current = (*ptr);
    switch(current)
    {
        case '[':
        {
            // find closing ']'
            int dataLength = JsonArrayDataLength(ptr, jsonState); // recursive
            if(dataLength >= 2)
            {
                ptr += dataLength;
                SKIP_WHITESPACE(ptr);
                current = (*ptr);
                // ignoring all data after the end of the array.
                // no additional checks necessary
            }
            else
            {
                // array data length must be
                // at least 2 characters for the
                // start and end of the array.
                // check jsonState for details.
                // no change made to ptr
            }
        }
        break;
        case '{':
        {
            // find closing '}'
            int dataLength = JsonObjectDataLength(ptr, jsonState); // recursive
            if(dataLength >= 2)
            {
                ptr += dataLength;
                SKIP_WHITESPACE(ptr);
                current = (*ptr);
                // ignoring all data after the end of the object.
                // no additional checks necessary
            }
            else
            {
                // array data length must be
                // at least 2 characters for the
                // start and end of the array.
                // check jsonState for details.
                // no change made to ptr
            }
        }
        break;
        case '"':
        {
            // find closing '"'
            int dataLength = JsonStringDataLength(ptr, jsonState);
            if(dataLength >= 2)
            {
                ptr += dataLength;
                SKIP_WHITESPACE(ptr);
                current = (*ptr);
                // a string is only valid when
                // followed by a valid character
                if((current != ',') &&
                   (current != ']') &&
                   (current != '}') &&
                   (current != ':') &&
                   (current != '\0'))
                {
                    // a valid character was not found
                    (*jsonState) = JSON_STATE_ERROR_STRING_INVALID_FOLLOWER;
                    // pretend nothing happened
                    ptr = json;
                }
            }
            else
            {
                // string data length must be
                // at least 2 characters for the
                // start and end of the quoted string.
                // check jsonState for details.
                // no change made to ptr
            }
        }
        break;

        case ':':
        {
            // skip the ':' and the value
            int valueLength;
            const char* valuePtr;
            ptr++;
            SKIP_WHITESPACE(ptr);
            valueLength = JsonValueDataLength(ptr, jsonState); // recursive
            if(valueLength > 0)
            {
                ptr += valueLength;
                SKIP_WHITESPACE(ptr);
                current = (*ptr);
                // a value is only valid when
                // followed by a valid character
                if((current != ',') &&
                    (current != ']') &&
                    (current != '}') &&
                    (current != '\0'))
                {
                    // a valid character was not found
                    (*jsonState) = JSON_STATE_ERROR_VALUE_INVALID_FOLLOWER;
                    // pretend nothing happened
                    ptr = json;
                }
            }
            else
            {
                // failed to skip the value.
                // check jsonState for details.
                // pretend nothing happened
                ptr = json;
            }
        }
        break;

        case ',':
        {
            ptr++;
            SKIP_WHITESPACE(ptr);
            current = (*ptr);
            // a comma is only valid when
            // followed by a valid character
            if(current != '{' &&
               current != '[' &&
               current != '"')
            {
                // a valid character was not found
                (*jsonState) = JSON_STATE_ERROR_COMMA_INVALID_FOLLOWER;
                // pretend nothing happened
                ptr = json;
            }
        }
        break;

        default:
        {
            // whitespace or unexpected character
            if(IS_WHITESPACE(*ptr))
            {
                ptr++;
                SKIP_WHITESPACE(ptr);
                current = (*ptr);
            }
            else
            {
                (*jsonState) = JSON_STATE_ERROR_UNEXPECTED_CHAR;
            }
        }
        break;
    }

    return ptr;
}

/**********************************************************/
static const char* JsonSkipToKeyValue( // recursive
    const char* json,
    const char* key,
    JsonState_t* jsonState)
{
    char current;
    SKIP_WHITESPACE(json);
    current = (*json);
    while((current != '\0') &&
          (current != '}'))
    {
        int keyLength;

        if(current == ',')
        {
            // there are more keys
            json++;
            SKIP_WHITESPACE(json);
            current = (*json);
        }
        else
        {
            // maybe this is the first key
        }

        keyLength = JsonStringDataLength(json, jsonState);
        if(keyLength >= 2)
        {
            // found a key in quotes
            if(strncmp((json + 1), key, (keyLength - 2)) == 0)
            {
                // skip the key
                json += keyLength;
                SKIP_WHITESPACE(json);
                current = (*json);
                if(current == ':')
                {
                    json++;
                    SKIP_WHITESPACE(json);
                    current = (*json);
                    // now at value
                    break;
                }
                else
                {
                    // expected ':'
                    (*jsonState) = JSON_STATE_ERROR_OBJECT_MISSING_VALUE_START;
                    SKIP_TO_NULL(json);
                    break;
                }
            }
            else
            {
                // not the correct key.
                const char* ptr;

                // skip the key and the value
                json += keyLength;
                SKIP_WHITESPACE(json);
                ptr = json;
                json = JsonSkip(ptr, jsonState);
                if(json == ptr)
                {
                    // failed to skip json data.
                    // check jsonState for details.
                    SKIP_TO_NULL(json);
                    break;
                }
                current = (*json);
            }
        }
        else
        {
            // could not find a key
            (*jsonState) = JSON_STATE_ERROR_OBJECT_MISSING_KEY;
            SKIP_TO_NULL(json);
            break;
        }
    }

    return json;
}

/**********************************************************/
static const char* JsonSkipToNextValue( // recursive
    const char* json,
    const char** key,
    int* keyLength,
    JsonState_t* jsonState)
{
    char current;

    (*key) = NULL;
    (*keyLength) = 0;

    SKIP_WHITESPACE(json);
    current = (*json);
    while((current != '\0') &&
          (current != '}'))
    {
        if(current == ',')
        {
            // there are more keys
            json++;
            SKIP_WHITESPACE(json);
            current = (*json);
        }
        else
        {
            // maybe this is the first key
        }

        (*keyLength) = JsonStringDataLength(json, jsonState);
        if((*keyLength) >= 2)
        {
            // found a key in quotes
            (*key) = json;

            // skip the key
            json += (*keyLength);
            SKIP_WHITESPACE(json);
            current = (*json);
            if(current == ':')
            {
                json++;
                SKIP_WHITESPACE(json);
                current = (*json);
                // now at value
                break;
            }
            else
            {
                // expected ':'
                (*jsonState) = JSON_STATE_ERROR_OBJECT_MISSING_VALUE_START;
                SKIP_TO_NULL(json);
                break;
            }
        }
        else
        {
            // could not find a key
            (*jsonState) = JSON_STATE_ERROR_OBJECT_MISSING_KEY;
            SKIP_TO_NULL(json);
            break;
        }
    }

    return json;
}

/**********************************************************/
static const char* JsonArrayNextIndex( // recursive
    const char* json,
    int* currentIndex,
    JsonState_t* jsonState)
{
    char current;

    SKIP_WHITESPACE(json);
    current = (*json);
    if((current== ']') ||
       (current == '\0'))
    {
        // at end of array
        (*currentIndex) = INVALID_POSITION;
        return json;
    }

    // json is expected to be a non-whitespace
    // character after a ',' or '['
    if((*currentIndex) == INVALID_POSITION)
    {
        // index 0
        (*currentIndex) = 0;
    }
    else
    {
        // find the next index.
        const char* skipPtr = JsonSkip(json, jsonState); // recursive
        if(skipPtr != json)
        {
            if((*skipPtr) == ',')
            {
                // found the next index
                (*currentIndex) = (*currentIndex) + 1;
                json = (skipPtr + 1);
                SKIP_WHITESPACE(json);
                current = (*json);
            }
            else if(((*skipPtr) == ']') ||
                    ((*skipPtr) == '\0'))
            {
                // found the end of the array
                (*currentIndex) = INVALID_POSITION;
                json = skipPtr;
                current = (*json);
            }
            else
            {
                // unexpected character
                (*jsonState) = JSON_STATE_ERROR_ARRAY_MISSING_NEXT_OR_END_CHAR;
                SKIP_TO_NULL(json);
                (*currentIndex) = INVALID_POSITION;
            }
        }
        else
        {
            // failed to skip json data.
            // check jsonState for details.
            // cannot find the end of the array element
            SKIP_TO_NULL(json);
            (*currentIndex) = INVALID_POSITION;
        }
    }

    return json;
}

// MARK: --- RESUME ---

/**********************************************************/
static Result_t ResumeInit(
    Resume_t* me,
    ResumeOptions_t* resumeOptions)
{
    Section_t sectionIndex;

    if(resumeOptions)
    {
        me->m_ResumeOptions = (*resumeOptions);
    }
    else
    {
        // using default resume options
        me->m_ResumeOptions.m_ExtendedDisplayCount = DISPLAY_NONE;
        me->m_ResumeOptions.m_DisplayFormat = FORMAT_TEXT;
    }

    // using INVALID_POSITION to indicate no errors
    me->m_ResumeErrorInSection = INVALID_POSITION;

    // using ENTRY_STATE_UNKNOWN to indicate no
    // entry has been accessed
    me->m_ResumeErrorInEntry = ENTRY_STATE_UNKNOWN;

    // init hard-coded sections
    sectionIndex = 0;
    do
    {
        SectionData_t* sectionData = &me->m_ResumeSection[sectionIndex];
        SectionDataInit(sectionData);
        sectionIndex++;
    }
    while(sectionIndex < SECTION_COUNT);

    // start with no runtime sections
    me->m_ResumeSectionRuntime = NULL;

    Result_t result = ManagedEntriesInit(&me->m_ResumeAllEntries, RESUME_MAX_ENTRIES);
    if(result == RESULT_OK)
    {
        // select an initial section size that seems
        // reasonably large. a quarter of a page is more
        // than enough.
        int intSectionEntryCount = (RESUME_MAX_LINES_PER_PAGE / 4);

        // setup hard-coded section initial entries size
        sectionIndex = 0;
        do
        {
            Result_t resizeResult;
            SectionData_t* sectionData = &me->m_ResumeSection[sectionIndex];
            resizeResult = EntriesResize(
                &sectionData->m_SectionEntries,
                intSectionEntryCount);
            if(resizeResult == RESULT_ERROR)
            {
                // cannot set initial entries size
                me->m_ResumeErrorInSection = sectionIndex;
                result = RESULT_ERROR;
            }
            sectionIndex++;
        }
        while(sectionIndex < SECTION_COUNT);
    }
    else
    {
        // failed to init entries
    }

    return result;
}

/**********************************************************/
static Result_t ResumeOptionsIsHiddenSection(
    ResumeOptions_t* me,
    const char* sectionTitle)
{
    if(sectionTitle)
    {
        if(sectionTitle[0] != '\0')
        {
            int index = 0;
            while(index < me->m_HideSectionCount)
            {
                if(strcmp(sectionTitle, me->m_HideSectionName[index]) == 0)
                {
                    // resume option to hide this section is set
                    return RESULT_YES;
                }
                index++;
            }
        }
        else
        {
            // empty title.
            // always hide sections with no title
            return RESULT_YES;
        }
    }
    else
    {
        // title missing.
        // always hide invalid sections
        return RESULT_YES;
    }

    return RESULT_NO;
}

/**********************************************************/
static void ResumeDeInit(
    Resume_t* me)
{
    // cleanup runtime sections
    if(me->m_ResumeSectionRuntime)
    {
        SectionDataDeInit(me->m_ResumeSectionRuntime); // recursive
        me->m_ResumeSectionRuntime = NULL;
    }

    // clean up hard-coded sections
    int index = 0;
    do
    {
        SectionDataDeInit(&me->m_ResumeSection[index]); // recursive
        index++;
    }
    while(index < SECTION_COUNT);

    ManagedEntriesDeInit(&me->m_ResumeAllEntries);
}

/**********************************************************/
SectionData_t* ResumeInsertSection(
    Resume_t* me,
    Section_t section)
{
    SectionData_t* sectionData;
    
    if(section < SECTION_AT_RUNTIME)
    {
        // cannot insert here
        return NULL;
    }

    sectionData = (SectionData_t*)malloc(sizeof(SectionData_t));
    if(sectionData == NULL)
    {
        // failed to allocate section data
        return NULL;
    }

    if(section > SECTION_AT_RUNTIME)
    {
        SectionData_t* previousSectionData = ResumeGetSectionData(me, (section - 1));
        if(previousSectionData)
        {
            SectionDataInit(sectionData);

            // save the link to the existing section
            sectionData->m_SectionNext = previousSectionData->m_SectionNext;

            // insert the new section
            // after the previous section
            previousSectionData->m_SectionNext = sectionData;
        }
        else
        {
            // cannot insert here.
            // there is no previous runtime 
            // section to link to.
            free(sectionData);
            sectionData = NULL;
        }
    }
    else
    {
        SectionDataInit(sectionData);

        // placing this section at the start of the
        // linked-list of sections added at runtime
        if(me->m_ResumeSectionRuntime)
        {
            // save the link to the existing section
            sectionData->m_SectionNext = me->m_ResumeSectionRuntime;
        }
        
        // save the new section as the first
        me->m_ResumeSectionRuntime = sectionData;
    }

    return sectionData;
}

/**********************************************************/
SectionData_t* ResumeRemoveSection(
    Resume_t* me,
    Section_t section)
{
    SectionData_t* sectionData;

    if(section < SECTION_AT_RUNTIME)
    {
        // cannot remove this section
        return NULL;
    }

    if(section > SECTION_AT_RUNTIME)
    {
        sectionData = ResumeGetSectionData(me, section);
        if(sectionData)
        {
            // this is not effecient. the linked-list was
            // already searched once so a pointer to the
            // previous section could have been stored
            SectionData_t* previousSectionData = ResumeGetSectionData(me, (section - 1));
            if(previousSectionData)
            {
                // removing this section data from the
                // linked-list
                previousSectionData->m_SectionNext = sectionData->m_SectionNext;
                sectionData->m_SectionNext = NULL;
            }
            else
            {
                // this should not be possible.
                // ResumeGetSectionData() could not return a
                // valid pointer if there was no previous
                // section data at this section index
            }
        }
        else
        {
            // this section does not exist
        }
    }
    else if(me->m_ResumeSectionRuntime)
    {
        // removing the first runtime section
        sectionData = me->m_ResumeSectionRuntime;       
        me->m_ResumeSectionRuntime = sectionData->m_SectionNext;
        sectionData->m_SectionNext = NULL;
    }
    else
    {
        // no runtime section to remove
    }

    return sectionData;
}

/**********************************************************/
Section_t ResumeGetSectionCount(
    Resume_t* me)
{
    Section_t count = SECTION_COUNT;
    SectionData_t* sectionData = me->m_ResumeSectionRuntime;
    while(sectionData)
    {
        count++;
        sectionData = sectionData->m_SectionNext;
    }
    return count;
}

/**********************************************************/
SectionData_t* ResumeGetSectionData(
    Resume_t* me,
    Section_t section)
{
    SectionData_t* sectionData = NULL;

    if(section < SECTION_COUNT)
    {
        // hard-coded section
        sectionData = &me->m_ResumeSection[section];
    }
    else
    {
        section -= SECTION_AT_RUNTIME;
        if(section >= 0)
        {
            // runtime section
            sectionData = me->m_ResumeSectionRuntime;
            while(section > 0)
            {
                sectionData = sectionData->m_SectionNext;
                section--;
                if(sectionData == NULL)
                {
                    // section is missing
                    break;
                }
            }
        }
        else
        {
            // no section selected
        }
    }

    return sectionData;
}

/**********************************************************/
Section_t ResumeGetSectionIndex(
    Resume_t* me,
    SectionData_t* sectionData)
{
    Section_t section = 0;
    do
    {
        if(&me->m_ResumeSection[section] == sectionData)
        {
            return section;
        }
        section++;
    }
    while (section < SECTION_COUNT);
    
    SectionData_t* runtimeSection = me->m_ResumeSectionRuntime;
    while(runtimeSection)
    {
        if(runtimeSection == sectionData)
        {
            return section;
        }
        section++;
        runtimeSection = runtimeSection->m_SectionNext;
    }

    return INVALID_POSITION;
}

/**********************************************************/
static const char* ResumeGetSectionTitle(
    Resume_t* me,
    Section_t section)
{
    SectionData_t* sectionData = ResumeGetSectionData(me, section);
    if(sectionData)
    {
        return sectionData->m_SectionTitle;
    }

    // invalid section
    return g_EmptyText;
}

/**********************************************************/
static Result_t ResumeAddEntry(
    Resume_t* me,
    Section_t section,
    const char* text,
    const char* start,
    const char* end)
{
    Result_t result = RESULT_OK;
    SectionData_t* sectionData = ResumeGetSectionData(me, section);
    if(sectionData)
    {
        // set the text with practically no upper bounds
        result = SectionAddEntry(sectionData, me, text, MAX_STRING_LENGTH, start, end);
    }
    else
    {
        // section data is missing
        me->m_ResumeErrorInEntry = ENTRY_STATE_ERROR_SECTION_MISSING;
        me->m_ResumeErrorInSection = section;
        result = RESULT_ERROR;
    }

    return result;
}

/**********************************************************/
static Result_t SectionAddEntry(
    SectionData_t* me,
    Resume_t* resume,
    const char* text,
    int textLength,
    const char* start,
    const char* end)
{
    Result_t result = RESULT_OK;
    Entry_t* entry = ManagedEntriesAppendSubstr(&resume->m_ResumeAllEntries, text, textLength, start, end);
    if(entry)
    {
        result = SectionDataIncludeEntry(me, entry);
        if(result == RESULT_ERROR)
        {
            // the only way including an entry can fail
            // with an error is if the section could not
            // be resized to fit the entry
            resume->m_ResumeErrorInEntry = ENTRY_STATE_ERROR_RESIZE;
            resume->m_ResumeErrorInSection = ResumeGetSectionIndex(resume, me);
        }
    }
    else
    {
        // failed to create entry
        resume->m_ResumeErrorInEntry = resume->m_ResumeAllEntries.m_ManagedEntriesError;
        resume->m_ResumeErrorInSection = ResumeGetSectionIndex(resume, me);
        result = RESULT_ERROR;
    }

    return result;
}

/**********************************************************/
static void ResumePrintHtml(
    Resume_t* me)
{
    static const char dateRangeSeparator[] = " to ";
    static const int datePos[ENTRY_TIME_COUNT] =
        {
            0, // ENTRY_TIME_START
            (DATE_STRING_LENGTH + (sizeof(dateRangeSeparator)-1)) // ENTRY_TIME_END
        };

    // start date + separator + end date + space + null
    char dateString[DATE_STRING_LENGTH + (sizeof(dateRangeSeparator)-1) + DATE_STRING_LENGTH + 1 + 1];

    //
    // PRINT
    //
    // header
    //
    printf("<!doctype html>\r\n");
    printf("<html lang=\"en-us\">\r\n");
    printf("<head>\r\n");
    printf("    <title>%s - Resume</title>\r\n", g_ResumeFullName);
    printf("</head>\r\n");
    printf("<body>\r\n");
    printf("<p>\r\n");
    printf("%s<br/>\r\n", g_ResumeFullName);
    printf("%s<br/>\r\n", g_ResumeEmail);
    printf("%s, %s<br/>\r\n", g_ResumeCity, g_ResumeState);
    printf("%s<br/>\r\n", g_ResumeMobileNumber);
    printf("<a href=\"%s\">%s</a>\r\n", g_ResumeWebUrl, g_ResumeWebUrl);
    printf("</p>\r\n");
    printf("\r\n");

    // print all sections
    EntriesTraverse_t traverse;
    int sectionIndex = 0;
    SectionData_t* sectionData = ResumeGetSectionData(me, sectionIndex);
    while(sectionData)
    {
        if(sectionData->m_SectionDisplayMax != DISPLAY_NONE)
        {
            int dateStringStart = 0;
            int displayCount = 0;

            // adjust date string start position
            if((sectionData->m_SectionDateOption & DATE_OPTION_HIDE_ALL) == DATE_OPTION_HIDE_ALL)
            {
                // the date is always
                // hidden in this section
                dateStringStart = (sizeof(dateString) - 1);
            }
            else if((sectionData->m_SectionDateOption & DATE_OPTION_HIDE_START) == DATE_OPTION_HIDE_START)
            {
                // the start date is always
                // hidden in this section
                dateStringStart = datePos[ENTRY_TIME_END];
            }

            //
            // PRINT
            //
            // section title
            //
            printf("<hr/>\r\n");
            printf("<h1>%s</h1>\r\n", sectionData->m_SectionTitle);
            printf("<p>\r\n");

            EntriesTraverseInit(&traverse);
            Entry_t* entry = EntriesTraverseFirst(&traverse, &sectionData->m_SectionEntries, sectionData->m_SectionOrder, sectionData->m_SectionOrderEntryTime);
            while(entry)
            {
                // format the date string
                int datesShown = 0;
                datesShown += (int)(EntryTimeToString(entry, &dateString[datePos[ENTRY_TIME_START]], DATE_STRING_LENGTH, ENTRY_TIME_START, sectionData->m_SectionDateOption) > 0);
                datesShown += (int)(EntryTimeToString(entry, &dateString[datePos[ENTRY_TIME_END]], DATE_STRING_LENGTH, ENTRY_TIME_END, sectionData->m_SectionDateOption) > 0);
                if(datesShown == 2)
                {
                    // both dates are shown.
                    // include the separator
                    memcpy(&dateString[DATE_STRING_LENGTH], dateRangeSeparator, sizeof(dateRangeSeparator)-1);
                    dateString[sizeof(dateString)-2] = ' ';
                    dateString[sizeof(dateString)-1] = '\0';
                }
                else if(datesShown == 1)
                {
                    // only 1 or no date is shown.
                    if((sectionData->m_SectionDateOption & DATE_OPTION_HIDE_END) == DATE_OPTION_HIDE_END)
                    {
                        // entire end date is hidden.
                        // only the start date is shown.
                        dateString[DATE_STRING_LENGTH]   = ' ';
                        dateString[DATE_STRING_LENGTH + 1] = '\0';
                    }
                    else
                    {
                        // the end date is not hidden so
                        // the start date must be.
                        // fill the separator with spaces
                        memset(&dateString[DATE_STRING_LENGTH], ' ', sizeof(dateRangeSeparator)-1);
                        dateString[sizeof(dateString)-2] = ' ';
                        dateString[sizeof(dateString)-1] = '\0';
                    }
                }
                else
                {
                    // no dates shown
                    dateString[dateStringStart] = '\0';
                }

                //
                // PRINT
                //
                // date string and entry text
                //
                printf("%s%s<br/>\r\n", &dateString[dateStringStart], entry->m_EntryText);

                // check if more entries should be displayed
                if(sectionData->m_SectionDisplayMax != DISPLAY_ALL)
                {
                    displayCount++;
                    if(displayCount == sectionData->m_SectionDisplayMax)
                    {
                        // max display count reached.
                        // break out of this loop
                        break;
                    }
                }

                entry = EntriesTraverseNext(&traverse);
            }
            EntriesTraverseDeInit(&traverse);

            //
            // PRINT
            //
            // end of section
            //
            printf("</p>\r\n");
            printf("\r\n");
        }
        else
        {
            // this section is hidden
        }

        sectionIndex++;
        sectionData = ResumeGetSectionData(me, sectionIndex);
    }

    //
    // PRINT
    //
    // footer
    //
    printf("</body>\r\n");
    printf("</html>\r\n");
}

/**********************************************************/
static void ResumePrintPlainText(
    Resume_t* me)
{
    static const char dateRangeSeparator[] = " to ";
    static const int datePos[ENTRY_TIME_COUNT] =
        {
            0, // ENTRY_TIME_START
            (DATE_STRING_LENGTH + (sizeof(dateRangeSeparator)-1)) // ENTRY_TIME_END
        };

    // start date + separator + end date + space + null
    char dateString[DATE_STRING_LENGTH + (sizeof(dateRangeSeparator)-1) + DATE_STRING_LENGTH + 1 + 1];

    //
    // PRINT
    //
    // header
    //
    printf("=======================================\n");
    printf("%s\n", g_ResumeFullName);
    printf("%s\n", g_ResumeEmail);
    printf("%s, %s\n", g_ResumeCity, g_ResumeState);
    printf("%s\n", g_ResumeMobileNumber);
    printf("%s\n", g_ResumeWebUrl);
    printf("\n");

    // print all sections
    EntriesTraverse_t traverse;
    int sectionIndex = 0;
    SectionData_t* sectionData = ResumeGetSectionData(me, sectionIndex);
    while(sectionData)
    {
        int displayMax = sectionData->m_SectionDisplayMax;
        if(ResumeOptionsIsHiddenSection(&me->m_ResumeOptions, sectionData->m_SectionTitle) == RESULT_YES)
        {
            displayMax = 0;
        }

        if(sectionData->m_SectionDisplayMax != DISPLAY_NONE)
        {
            int dateStringStart = 0;
            int displayCount = 0;

            // apply resume options to display max
            if(me->m_ResumeOptions.m_ExtendedDisplayCount == DISPLAY_ALL)
            {
                displayMax = DISPLAY_ALL;
            }
            else
            {
                displayMax += me->m_ResumeOptions.m_ExtendedDisplayCount;
            }

            // adjust date string start position
            if((sectionData->m_SectionDateOption & DATE_OPTION_HIDE_ALL) == DATE_OPTION_HIDE_ALL)
            {
                // the date is always
                // hidden in this section
                dateStringStart = (sizeof(dateString) - 1);
            }
            else if((sectionData->m_SectionDateOption & DATE_OPTION_HIDE_START) == DATE_OPTION_HIDE_START)
            {
                // the start date is always
                // hidden in this section
                dateStringStart = datePos[ENTRY_TIME_END];
            }

            //
            // PRINT
            //
            // section title
            //
            printf("---------------------------------------\n");
            printf("%s\n", sectionData->m_SectionTitle);
            printf("---------------------------------------\n");

            EntriesTraverseInit(&traverse);
            Entry_t* entry = EntriesTraverseFirst(&traverse, &sectionData->m_SectionEntries, sectionData->m_SectionOrder, sectionData->m_SectionOrderEntryTime);
            while(entry)
            {
                // format the date string
                int datesShown = 0;
                datesShown += (int)(EntryTimeToString(entry, &dateString[datePos[ENTRY_TIME_START]], DATE_STRING_LENGTH, ENTRY_TIME_START, sectionData->m_SectionDateOption) > 0);
                datesShown += (int)(EntryTimeToString(entry, &dateString[datePos[ENTRY_TIME_END]], DATE_STRING_LENGTH, ENTRY_TIME_END, sectionData->m_SectionDateOption) > 0);
                if(datesShown == 2)
                {
                    // both dates are shown.
                    // include the separator
                    memcpy(&dateString[DATE_STRING_LENGTH], dateRangeSeparator, sizeof(dateRangeSeparator)-1);
                    dateString[sizeof(dateString)-2] = ' ';
                    dateString[sizeof(dateString)-1] = '\0';
                }
                else if(datesShown == 1)
                {
                    // only 1 or no date is shown.
                    if((sectionData->m_SectionDateOption & DATE_OPTION_HIDE_END) == DATE_OPTION_HIDE_END)
                    {
                        // entire end date is hidden.
                        // only the start date is shown.
                        dateString[DATE_STRING_LENGTH]   = ' ';
                        dateString[DATE_STRING_LENGTH + 1] = '\0';
                    }
                    else
                    {
                        // the end date is not hidden so
                        // the start date must be.
                        // fill the separator with spaces
                        memset(&dateString[DATE_STRING_LENGTH], ' ', sizeof(dateRangeSeparator)-1);
                        dateString[sizeof(dateString)-2] = ' ';
                        dateString[sizeof(dateString)-1] = '\0';
                    }
                }
                else
                {
                    // no dates shown
                    dateString[dateStringStart] = '\0';
                }

                //
                // PRINT
                //
                // date string and entry text
                //
                printf("     %s%s\n", &dateString[dateStringStart], entry->m_EntryText);

                // check if more entries should be displayed
                if(displayMax != DISPLAY_ALL)
                {
                    displayCount++;
                    if(displayCount == displayMax)
                    {
                        // max display count reached.
                        // break out of this loop
                        break;
                    }
                }

                entry = EntriesTraverseNext(&traverse);
            }
            EntriesTraverseDeInit(&traverse);

            //
            // PRINT
            //
            // end of section
            //
            printf("\n");
        }
        else
        {
            // this section is hidden
        }

        sectionIndex++;
        sectionData = ResumeGetSectionData(me, sectionIndex);
    }

    //
    // PRINT
    //
    // footer
    //
    printf("=======================================\n");
    printf("\n");
}

/**********************************************************/
static Result_t ResumeFillSectionsFromJsonArray(
    Resume_t* me,
    const char* json)
{
    Result_t result = RESULT_OK;

    SKIP_WHITESPACE(json);
    if(json[0] == '[')
    {
        // sections array
        JsonState_t jsonState = JSON_STATE_OK;
        int jsonArrayIndex = INVALID_POSITION;
        json = JsonArrayNextIndex((json + 1), &jsonArrayIndex, &jsonState); // recursive
        if(jsonState == JSON_STATE_OK)
        {
            while(jsonArrayIndex != INVALID_POSITION)
            {
                if(json[0] == '{')
                {
                    SectionData_t* sectionData = ResumeInsertSection(me, SECTION_AT_RUNTIME);
                    if(sectionData == NULL)
                    {
                        // cannot insert a new section
                        result = RESULT_ERROR;

                        // break out of this loop
                        break;
                    }
                    else
                    {
                        // a new section was created and
                        // initialized. its index is
                        // SECTION_AT_RUNTIME. if there
                        // was already a section at that
                        // index it can be found at
                        // sectionData->m_SectionNext
                    }

                    if(ResumeFillSectionFromJsonObject(me, sectionData, json) == RESULT_ERROR)
                    {
                        // cannot fill the section from
                        // the json object data
                        result = RESULT_ERROR;

                        // remove the empty section data 
                        // from the resume so it can be
                        // deinited without removing any
                        // sections that were already added
                        ResumeRemoveSection(me, SECTION_AT_RUNTIME);
                        SectionDataDeInit(sectionData);
                        free(sectionData);
                        sectionData = NULL;

                        // break out of this loop
                        break;
                    }
                    else
                    {
                        // section filled
                    }
                }
                else
                {
                    // not a json object.
                    // this could be an error, but allow
                    // this loop to try to skip to the next
                    // array element
                }

                // skp to the next json array element
                json = JsonArrayNextIndex(json, &jsonArrayIndex, &jsonState); // recursive
                if(jsonState != JSON_STATE_OK)
                {
                    return RESULT_ERROR;
                }
            }
            // done looping through all json array elements
        }
        else
        {
            // JsonArrayNextIndex() could not search for
            // the first array index within the json data.
            // check jsonState for the exact reason.
            result = RESULT_ERROR;
        }
    }
    else
    {
        // sections array should start with '['
        result = RESULT_ERROR;
    }

    return result;
}

/**********************************************************/
static Result_t ResumeFillSectionFromJsonObject(
    Resume_t* me,
    SectionData_t* sectionData,
    const char* json)
{
    Result_t result = RESULT_OK;

    SKIP_WHITESPACE(json);
    if(json[0] == '{')
    {
        // section object
        JsonState_t jsonState = JSON_STATE_OK;
        const char* key;
        int keyLength;
        const char* nextKey;
        int nextKeyLength;
        const char* value;
        int valueLength;

        json = JsonSkipToNextValue((json + 1), &key, &keyLength, &jsonState);
        while(keyLength > 2)
        {
            value = json;
            valueLength = JsonValueDataLength(json, &jsonState);
            if(valueLength == 0)
            {
                // value is missing
                result = RESULT_ERROR;
                break;
            }
            json = JsonSkipToNextValue((json + valueLength), &nextKey, &nextKeyLength, &jsonState);

            key++;
            keyLength -= 2;
            if(strncmp(RESUME_STRING_NAME, key, keyLength) == 0)
            {
                if(valueLength >= 2)
                {
                    if(SectionDataSetTitleSubstr(sectionData, value+1, valueLength-2) == RESULT_ERROR)
                    {
                        result = RESULT_ERROR;
                    }
                }
            }
            else if(strncmp(RESUME_STRING_DATE_ORDER, key, keyLength) == 0)
            {
                if(valueLength >= 2)
                {
                    if(strncmp(RESUME_STRING_DATE_END, value+1, valueLength-2) == 0)
                    {
                        sectionData->m_SectionOrderEntryTime = ENTRY_TIME_END;
                    }
                }
            }
            else if(strncmp(RESUME_STRING_DATE_OPTION, key, keyLength) == 0)
            {
                if(valueLength >= 2)
                {
                    if(strncmp(RESUME_STRING_SHOW_YEAR_ONLY, value+1, valueLength-2) == 0)
                    {
                        sectionData->m_SectionDateOption = DATE_OPTION_SHOW_YEAR_ONLY;
                    }
                }
            }
            else if(strncmp(RESUME_STRING_DISPLAY_MAX, key, keyLength) == 0)
            {
                int displayMax;
                if(sscanf(value, "%d", &displayMax) == 1)
                {
                    sectionData->m_SectionDisplayMax = displayMax;
                }
                else
                {
                    // parsing failed
                    result = RESULT_ERROR;
                }
            }
            else if(strncmp(RESUME_STRING_ORDER, key, keyLength) == 0)
            {
                if(valueLength >= 2)
                {
                    if(strncmp(RESUME_STRING_NEWEST_FIRST, value+1, valueLength-2) == 0)
                    {
                        sectionData->m_SectionOrder = ORDER_NEWEST_FIRST;
                    }
                    else if(strncmp(RESUME_STRING_OLDEST_FIRST, value+1, valueLength-2) == 0)
                    {
                        sectionData->m_SectionOrder = ORDER_OLDEST_FIRST;
                    }
                    else if(strncmp(RESUME_STRING_RANDOM, value+1, valueLength-2) == 0)
                    {
                        sectionData->m_SectionOrder = ORDER_RANDOM;
                    }
                }
            }
            else if(strncmp(RESUME_STRING_ENTRIES, key, keyLength) == 0)
            {
                if(ResumeFillEntriesFromJsonArray(me, sectionData, value) == RESULT_ERROR)
                {
                    result = RESULT_ERROR;
                }
            }

            key = nextKey;
            keyLength = nextKeyLength;
        }
        // done looping through all json object values
    }
    else
    {
        // section object should start with '{'
        result = RESULT_ERROR;
    }

    return result;
}

/**********************************************************/
static Result_t ResumeFillEntriesFromJsonArray(
    Resume_t* me, 
    SectionData_t* sectionData,
    const char* json)
{
    Result_t result = RESULT_OK;

    SKIP_WHITESPACE(json);
    if(json[0] == '[')
    {
        // entries array
        JsonState_t jsonState = JSON_STATE_OK;
        int jsonArrayIndex = INVALID_POSITION;
        json = JsonArrayNextIndex((json + 1), &jsonArrayIndex, &jsonState); // recursive
        if(jsonState == JSON_STATE_OK)
        {
            while(jsonArrayIndex != INVALID_POSITION)
            {
                if(json[0] == '{')
                {
                    // entry object
                    const char* nextJson;
                    const char* key;
                    int keyLength;
                    const char* nextKey;
                    int nextKeyLength;
                    const char* value;
                    int valueLength;
                    const char* text = NULL;
                    int textLength = 0;
                    const char* startDate = NULL;
                    const char* endDate = NULL;

                    nextJson = JsonSkipToNextValue((json + 1), &key, &keyLength, &jsonState);
                    while(keyLength > 2)
                    {
                        value = nextJson;
                        valueLength = JsonValueDataLength(nextJson, &jsonState);
                        if(valueLength == 0)
                        {
                            // value is missing
                            result = RESULT_ERROR;
                            break;
                        }
                        nextJson = JsonSkipToNextValue((nextJson + valueLength), &nextKey, &nextKeyLength, &jsonState);

                        key++;
                        keyLength -= 2;
                        if(strncmp(RESUME_STRING_TEXT, key, keyLength) == 0)
                        {
                            if(valueLength >= 2)
                            {
                                text = value + 1;
                                textLength = valueLength - 2;
                            }
                        }
                        else if(strncmp(RESUME_STRING_DATE_START, key, keyLength) == 0)
                        {
                            if(valueLength > 2)
                            {
                                startDate = value + 1;
                            }
                        }
                        else if(strncmp(RESUME_STRING_DATE_END, key, keyLength) == 0)
                        {
                            if(valueLength > 2)
                            {
                                endDate = value + 1;
                            }
                        }

                        key = nextKey;
                        keyLength = nextKeyLength;
                    }
                    // done looping through all json object values

                    if(SectionAddEntry(sectionData, me, text, textLength, startDate, endDate) == RESULT_ERROR)
                    {
                        result = RESULT_ERROR;
                    }
                }
                else
                {
                    // not a json object.
                    // this could be an error, but allow
                    // this loop to try to skip to the next
                    // array element
                }

                // skip to the next json array element
                json = JsonArrayNextIndex(json, &jsonArrayIndex, &jsonState); // recursive
                if(jsonState != JSON_STATE_OK)
                {
                    result = RESULT_ERROR;
                }
            }
            // done looping through all json array elements
        }
        else
        {
            // JsonArrayNextIndex() could not search for
            // the first array index within the json data.
            // check jsonState for the exact reason.
            result = RESULT_ERROR;
        }
    }
    else
    {
        // entries array should start with '['
        result = RESULT_ERROR;
    }

    return result;
}

/**********************************************************/
static Result_t ResumeFillHardCoded(
    Resume_t* me)
{
    if(ResumeFillSectionsFromHardCoded(me) != RESULT_OK)
    {
        return RESULT_ERROR;
    }

    if(ResumeFillEntriesFromHardCoded(me) != RESULT_OK)
    {
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

/**********************************************************/
static Result_t ResumeFillSectionsFromHardCoded(
    Resume_t* me)
{
    Result_t result = RESULT_OK;

    //
    // set title for each hard-coded section
    //
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_EXPERIENCE], "Experience") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_EXPERIENCE;
        result = RESULT_ERROR;
    }
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_WORK_HISTORY], "Work History") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_WORK_HISTORY;
        result = RESULT_ERROR;
    }
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_INTERESTS], "Interests") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_INTERESTS;
        result = RESULT_ERROR;
    }
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_EDUCATION], "Education") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_EDUCATION;
        result = RESULT_ERROR;
    }
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_TOOLS], "Tools") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_TOOLS;
        result = RESULT_ERROR;
    }
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_ACCOMPLISHMENTS], "Accomplishments") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_ACCOMPLISHMENTS;
        result = RESULT_ERROR;
    }
    if(SectionDataSetTitle(&me->m_ResumeSection[SECTION_PROJECTS], "Projects") == RESULT_ERROR)
    {
        me->m_ResumeErrorInSection = SECTION_PROJECTS;
        result = RESULT_ERROR;
    }

    //
    // set section sort options
    //
    // default order is ORDER_NEWEST_FIRST
    // default entry time is ENTRY_TIME_START
    me->m_ResumeSection[SECTION_EDUCATION].m_SectionOrderEntryTime   = ENTRY_TIME_END;
    me->m_ResumeSection[SECTION_INTERESTS].m_SectionOrder       = ORDER_RANDOM;
    me->m_ResumeSection[SECTION_TOOLS].m_SectionOrder           = ORDER_RANDOM;
    me->m_ResumeSection[SECTION_ACCOMPLISHMENTS].m_SectionOrder = ORDER_RANDOM;
    me->m_ResumeSection[SECTION_PROJECTS].m_SectionOrder        = ORDER_RANDOM;

    //
    // set max entries to display
    //
    // default is DISPLAY_ALL
    me->m_ResumeSection[SECTION_EXPERIENCE].m_SectionDisplayMax      = 6;
    me->m_ResumeSection[SECTION_INTERESTS].m_SectionDisplayMax       = 5;
    me->m_ResumeSection[SECTION_ACCOMPLISHMENTS].m_SectionDisplayMax = 5;
    me->m_ResumeSection[SECTION_PROJECTS].m_SectionDisplayMax        = 5;

    //
    // set date information to display
    //
    // default is DATE_OPTION_HIDE_ALL
    me->m_ResumeSection[SECTION_WORK_HISTORY].m_SectionDateOption = DATE_OPTION_SHOW_YEAR_ONLY;
    me->m_ResumeSection[SECTION_EXPERIENCE].m_SectionDateOption   = DATE_OPTION_SHOW_START_ONLY | DATE_OPTION_HIDE_START_DAY;
    me->m_ResumeSection[SECTION_EDUCATION].m_SectionDateOption    = DATE_OPTION_SHOW_END_ONLY | DATE_OPTION_SHOW_YEAR_ONLY;

    return result;
}

/**********************************************************/
static Result_t ResumeFillEntriesFromHardCoded(
    Resume_t* me)
{
    Result_t result = RESULT_OK;
    Section_t section;

    // using INVALID_POSITION to indicate no errors
    me->m_ResumeErrorInSection = INVALID_POSITION;

    // using ENTRY_STATE_OK to indicate no entry errors have occured
    me->m_ResumeErrorInEntry = ENTRY_STATE_OK;

    //
    // add entries to each section
    //

    section = SECTION_ACCOMPLISHMENTS;
    ResumeAddEntry(me, section, "Coldfire bare-metal Ethernet driver using ring-buffer and DMA",                                         NULL, NULL);
    ResumeAddEntry(me, section, "static memory TCP/IP library for Atmel and ported to Coldfire and AM335X (Sitara)",                     NULL, NULL);
    ResumeAddEntry(me, section, "FAT32 library with long file name support for Atmel and ported to Coldfire and AM335X (Sitara)",        NULL, NULL);
    ResumeAddEntry(me, section, "multi-screen touch panel GUI editor including integration with UDP data from controllers",              NULL, NULL);
    ResumeAddEntry(me, section, "Blackfin ucLinux kernel modules for SPI and SPORT (TDM) communication with two CS42448",                NULL, NULL);
    ResumeAddEntry(me, section, "custom board with TI CC3200 module for remote HVAC 24VAC and differential pressure monitoring",         NULL, NULL);
    ResumeAddEntry(me, section, "iOS app for local network communication and file transfer with HVAC monitor",                           NULL, NULL);
    ResumeAddEntry(me, section, "frame-accurate sequence timeline rendering with collapsible groups and interactive media scrubbing",    NULL, NULL);
    ResumeAddEntry(me, section, "communication protocol and audio status monitoring for on-board ride vehicle audio player",             NULL, NULL);
    ResumeAddEntry(me, section, "automatic detection of network modules and mounting of NFS shares for multichannel audio/video player", NULL, NULL);
    ResumeAddEntry(me, section, "remote firmware update capability for uBoot and Linux on Blackfin",                                     NULL, NULL);
    ResumeAddEntry(me, section, "multi-channel WAV and MP3 audio playback control and configuration software",                           NULL, NULL);
    ResumeAddEntry(me, section, "reliable transfer of image files over Modbus using sequenced data chunks with CRC16",                   NULL, NULL);
    ResumeAddEntry(me, section, "complete XML reader / writer with integrated motification history tracker",                             NULL, NULL);
    ResumeAddEntry(me, section, "closed over 500 customer and end-user support tickets",                                                 NULL, NULL);
    ResumeAddEntry(me, section, "integrated 3rd party lighting controller with train signals over the Tempe Salt River bridge",          NULL, NULL);

    section = SECTION_INTERESTS;
    ResumeAddEntry(me, section, "Video Games - Dr Mario",                 NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Astro Bot",                NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Stormworks",               NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Bloons TD 6",              NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Civilization VI",          NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Donut County",             NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Minecraft",                NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Teardown",                 NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Everybody's Golf",         NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Border Bots VR",           NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Subnautica",               NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Just Cause 3",             NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - TMNT: Shredder's Revenge", NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Subnautica",               NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Rogue Tower",              NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Subnautica",               NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Sanctum",                  NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Command & Conquer",        NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Homeworld",                NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Super Smash Brothers",     NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Mario Kart 8",             NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Portal",                   NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - LEGO City Undercover",     NULL, NULL);
    ResumeAddEntry(me, section, "Video Games - Worms Armageddon",         NULL, NULL);

    ResumeAddEntry(me, section, "Sports - Flag Football", NULL, NULL);
    ResumeAddEntry(me, section, "Sports - Basketball",    NULL, NULL);
    ResumeAddEntry(me, section, "Sports - Tae Kwon Do",   NULL, NULL);
    ResumeAddEntry(me, section, "Sports - Racquetball",   NULL, NULL);
    ResumeAddEntry(me, section, "Sports - Crossfit",      NULL, NULL);
    ResumeAddEntry(me, section, "Sports - Fun Run/Walk",  NULL, NULL);

    ResumeAddEntry(me, section, "Movies - Rat Race",                  NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Stargate",                  NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Robin Hood: Men in Tights", NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Despicable Me",             NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Down Periscope",            NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Crazy Rich Asians",         NULL, NULL);
    ResumeAddEntry(me, section, "Movies - The Last Samurai",          NULL, NULL);
    ResumeAddEntry(me, section, "Movies - LEGO Movie",                NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Tron: Legacy",              NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Mars Attacks",              NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Demolition Man",            NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Harry Potter",              NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Lord of the Rings",         NULL, NULL);
    ResumeAddEntry(me, section, "Movies - First Knight",              NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Gaurdians of the Galaxy",   NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Contact",                   NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Hotel Transylvania",        NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Idiocracy",                 NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Independence Day",          NULL, NULL);
    ResumeAddEntry(me, section, "Movies - John Carter",               NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Inside Out",                NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Jurassic Park",             NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Pitch Perfect",             NULL, NULL);
    ResumeAddEntry(me, section, "Movies - TMNT",                      NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Trolls",                    NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Valerian",                  NULL, NULL);
    ResumeAddEntry(me, section, "Movies - The Fifth Element",         NULL, NULL);
    ResumeAddEntry(me, section, "Movies - The Matrix",                NULL, NULL);
    ResumeAddEntry(me, section, "Movies - The Hunt for Red October",  NULL, NULL);
    ResumeAddEntry(me, section, "Movies - Groundhog Day",             NULL, NULL);

    ResumeAddEntry(me, section, "Music - Noisestorm",           NULL, NULL);
    ResumeAddEntry(me, section, "Music - Stray Kids",           NULL, NULL);
    ResumeAddEntry(me, section, "Music - Robert Miles",         NULL, NULL);
    ResumeAddEntry(me, section, "Music - Sofi Tucker",          NULL, NULL);
    ResumeAddEntry(me, section, "Music - They Might be Giants", NULL, NULL);
    ResumeAddEntry(me, section, "Music - BTS",                  NULL, NULL);
    ResumeAddEntry(me, section, "Music - Aespa",                NULL, NULL);
    ResumeAddEntry(me, section, "Music - Miami Sound Machine",  NULL, NULL);
    ResumeAddEntry(me, section, "Music - Vangelis",             NULL, NULL);
    ResumeAddEntry(me, section, "Music - Daft Punk",            NULL, NULL);
    ResumeAddEntry(me, section, "Music - Lionel Richie",        NULL, NULL);
    ResumeAddEntry(me, section, "Music - The Black Eyed Peas",  NULL, NULL);
    ResumeAddEntry(me, section, "Music - Olive",                NULL, NULL);
    ResumeAddEntry(me, section, "Music - Sade",                 NULL, NULL);

    section = SECTION_WORK_HISTORY;
    ResumeAddEntry(me, section, "Alcorn McBride",                       "2006-02-06", NULL);

    section = SECTION_EDUCATION;
    ResumeAddEntry(me, section, "Bachelor of Science - Computer Science - Stetson University", "1999-08-01", "2003-05-01");

    section = SECTION_EXPERIENCE;
    ResumeAddEntry(me, section, "add feature to adjust panel pc rotation by integrating a Powershell script with an existing Qt application",         "2024-01-01", NULL);
    ResumeAddEntry(me, section, "fix bug in Product File Creator where user defined spaces were being stripped from the final output",                "2024-03-01", NULL);
    ResumeAddEntry(me, section, "fix script import bugs that allowed script password to be bypassed",                                                 "2024-08-01", NULL);
    ResumeAddEntry(me, section, "fix script import bugs to identify when literals are used in place of variables for \"if\" events",                  "2024-08-01", NULL);
    ResumeAddEntry(me, section, "guide junior developer to integrate timecode conversion function into expression parser",                            "2024-09-01", NULL);
    ResumeAddEntry(me, section, "generate jenkins pipelines for RideAmp and VPage Utils projects for Visual Studio 2022",                             "2024-11-01", NULL);
    ResumeAddEntry(me, section, "allow expression parser to include variable names with special characters and spaces",                               "2023-04-01", NULL);
    ResumeAddEntry(me, section, "fix memory leak in one-shot sequences by tracing the code paths used to allocate each event",                        "2023-02-01", NULL);
    ResumeAddEntry(me, section, "organize product files and reformat all xml simplify compare and merge operations",                                  "2022-11-01", NULL);
    ResumeAddEntry(me, section, "allow copy and paste between WinScript Live version 5 and 6",                                                        "2022-12-01", NULL);
    ResumeAddEntry(me, section, "expand live mode sequence status to include pre-roll and looping states",                                            "2022-07-01", NULL);
    ResumeAddEntry(me, section, "allow winscriot live events view to split display of grid and timeline in the same frame",                           "2022-05-01", NULL);
    ResumeAddEntry(me, section, "modify timeline graphics to match WinScript live 6 mockups",                                                         "2022-03-01", NULL);
    ResumeAddEntry(me, section, "fix problem loading fonts that have the same name but different weights",                                            "2021-06-01", NULL);
    ResumeAddEntry(me, section, "redesign button action dialog to allow multiple actions for release and a separate action for press",                "2021-03-01", NULL);
    ResumeAddEntry(me, section, "simplify Visual Studio projects using property sheets",                                                              "2021-01-31", NULL);
    ResumeAddEntry(me, section, "improve GPS parser to support decimal degrees from GPRMC messages",                                                  "2013-10-01", NULL);
    ResumeAddEntry(me, section, "use PHP to load raw json data from a database then convert it for use with FusionCharts XT Javscript library",       "2019-12-01", NULL);
    ResumeAddEntry(me, section, "add feature to WinScript Live to track the active sequence and automatically display all variables in a watch list", "2024-05-01", NULL);
    ResumeAddEntry(me, section, "improve visibility into sequence scheduling by integrating a time database into the debug code",                     "2024-07-01", NULL);
    ResumeAddEntry(me, section, "update ShowTouch to work with latest OEM panel pc running Windows 10 IoT",                                           "2024-07-01", NULL);

    section = SECTION_TOOLS;
    ResumeAddEntry(me, section, "Git / github.com / bitbucket.org", NULL, NULL);
    ResumeAddEntry(me, section, "SVN",                              NULL, NULL);
    ResumeAddEntry(me, section, "Microsoft Visual Studio",          NULL, NULL);
    ResumeAddEntry(me, section, "Eclipse / ARM DS5",                NULL, NULL);
    ResumeAddEntry(me, section, "Qt Creator",                       NULL, NULL);
    ResumeAddEntry(me, section, "Corel Draw",                       NULL, NULL);
    ResumeAddEntry(me, section, "Adobe Photoshop",                  NULL, NULL);
    ResumeAddEntry(me, section, "Adobe After Effects",              NULL, NULL);
    ResumeAddEntry(me, section, "Wireshark / tcpdump",              NULL, NULL);
    ResumeAddEntry(me, section, "VS Code",                          NULL, NULL);
    ResumeAddEntry(me, section, "VMWare Workstation / Virtual Box", NULL, NULL);

    section = SECTION_PROJECTS;
    ResumeAddEntry(me, section, "bottle cap motion sensor using Arduino with C++",                                                        NULL, NULL);
    ResumeAddEntry(me, section, "digital audio recorder settings GUI using Qt for Windows and MacOS",                                     NULL, NULL);
    ResumeAddEntry(me, section, "\"Yuri on Ice\" themed ice-skating game using HTML5, Zim, and Javascript",                               NULL, NULL);
    ResumeAddEntry(me, section, "UDP tool using Qt and Npcap to monitor, send, and receive unicast, multicast, and broadcast datagrams.", NULL, NULL);
    ResumeAddEntry(me, section, "DMX512 visual data file editor using Qt",                                                                NULL, NULL);
    ResumeAddEntry(me, section, "WinMerge plugin to extract XML data from a proprietary archive file format",                             NULL, NULL);
    ResumeAddEntry(me, section, "GPS visualization tool for trigger zones and live positioning data",                                     NULL, NULL);
    ResumeAddEntry(me, section, "text based UDP protocol and gateway server for file system, SMTP, IMAP, and HTTPS access",               NULL, NULL);

    if(me->m_ResumeErrorInSection != INVALID_POSITION)
    {
        // an error occurred in a section
        result = RESULT_ERROR;
    }

    if(me->m_ResumeErrorInEntry != ENTRY_STATE_OK)
    {
        // an error occurred in an entry
        result = RESULT_ERROR;
    }

    return result;
}

//
// MARK: PROGRAM
//

/**********************************************************/
int main(
    int argc,
    char** argv)
{
    int exitCode = EXIT_OK;
    Resume_t resume;
    ResumeOptions_t resumeOptions = { 0 };
    unsigned int seed;

    // select a new random seed
    seed = (unsigned int)time(NULL);
    srand(seed);

    if(argc >= 2)
    {
        // check command line args.
        // skip program name
        int index = 1;
        do
        {
            char* arg = argv[index];
            if(arg)
            {
                if(strcmp(arg, "-h") == 0)
                {
                    // display help
                    MainPrintHelp(argv[0]);
                    return 0;
                }
                else if(strcmp(arg, "-v") == 0)
                {
                    MainPrintVersion();
                    return 0;
                }
                else if(strcmp(arg, "-H") == 0)
                {
                    // hidden section title expected
                    index++;
                    if(index < argc)
                    {
                        arg = argv[index];
                        if(arg)
                        {
                            if(resumeOptions.m_HideSectionCount < RESUME_OPTIONS_MAX_HIDDEN_SECTIONS)
                            {
                                resumeOptions.m_HideSectionName[resumeOptions.m_HideSectionCount] = arg;
                                resumeOptions.m_HideSectionCount++;
                            }
                            else
                            {
                                // out of resume options hidden sections
                            }
                        }
                        else
                        {
                            // invalid section name
                        }
                    }
                    else
                    {
                        // missing section name
                    }
                }
                else if(strcmp(arg, "-f") == 0)
                {
                    // format expected
                    index++;
                    if(index < argc)
                    {
                        arg = argv[index];
                        if(arg)
                        {
                            switch(arg[0])
                            {
                            case 'H': // fall-through
                            case 'h':
                            {
                                resumeOptions.m_DisplayFormat = FORMAT_HTML;
                            }
                            break;

                            case 'T': // fall-through
                            case 't':
                            {
                                resumeOptions.m_DisplayFormat = FORMAT_TEXT;
                            }
                            break;

                            default:
                            {
                                exitCode = EXIT_FORMAT_UNKNOWN_ARG;
                                printf("Error: unknown format arg \"%s\"\n", arg);
                            }
                            break;
                            }
                        }
                        else
                        {
                            exitCode = EXIT_FORMAT_ARG_MISSING;
                            printf("Error: format arg data is missing\n");
                        }
                    }
                    else
                    {
                        exitCode = EXIT_FORMAT_SELECTION_MISSING;
                        printf("Error: format selection is missing\n");
                    }
                }
                else if(strcmp(arg, "-a") == 0)
                {
                    resumeOptions.m_ExtendedDisplayCount = DISPLAY_ALL;
                }
                else if(strcmp(arg, "-e") == 0)
                {
                    // additional entry count expected
                    index++;
                    if(index < argc)
                    {
                        arg = argv[index];
                        if(arg)
                        {
                            int count;
                            if(sscanf(arg, "%d", &count) != 1)
                            {
                                // parsing failed
                                count = 0;
                            }

                            if((count >= 1) && (count <= RESUME_MAX_LINES_PER_PAGE))
                            {
                                if(resumeOptions.m_ExtendedDisplayCount != DISPLAY_ALL)
                                {
                                    resumeOptions.m_ExtendedDisplayCount = count;
                                }
                                else
                                {
                                    // display all option
                                    // already selected.
                                    // changes to this
                                    // option are ignored
                                }
                            }
                            else
                            {
                                exitCode = EXIT_EXTENDED_COUNT_INVALID;
                                printf("Error: invalid extended count \"%s\"\n", arg);
                            }
                        }
                        else
                        {
                            exitCode = EXIT_EXTENDED_COUNT_ARG_MISSING;
                            printf("Error: extended count arg data is missing\n");
                        }
                    }
                    else
                    {
                        exitCode = EXIT_EXTENDED_COUNT_MISSING;
                        printf("Error: extended count is missing\n");
                    }
                }
                else
                {
                    exitCode = EXIT_UNKNOWN_ARG;
                    printf("Error: unknown arg %d \"%s\"\n", (index + 1), arg);
                }
            }
            else
            {
                // ignore missing arg
            }
            index++;
        }
        while(index < argc);
    }
    else
    {
        // no command line args
    }

    if(exitCode != EXIT_OK)
    {
        // previous error
        printf("\nTo view help text, run: %s -h\n", argv[0]);
        return exitCode;
    }

    if(ResumeInit(&resume, &resumeOptions) == RESULT_OK)
    {
        Result_t fillResult = ResumeFillHardCoded(&resume);
        if(fillResult != RESULT_ERROR)
        {
            fillResult = ResumeFillSectionsFromJsonArray(&resume, g_ResumeJsonData);
        }

        if(fillResult != RESULT_ERROR)
        {
            switch(resume.m_ResumeOptions.m_DisplayFormat)
            {
            case FORMAT_HTML:
            {
                ResumePrintHtml(&resume);
            }
            break;

            case FORMAT_TEXT: // fall-through
            default:
            {
                ResumePrintPlainText(&resume);
            }
            break;
            }
        }
        else
        {
            exitCode = EXIT_FILL_ERROR;
            if(resume.m_ResumeErrorInSection != INVALID_POSITION)
            {
                const char* sectionTitle = ResumeGetSectionTitle(&resume, resume.m_ResumeErrorInSection);
                printf("Error: resume could not be filled at section \"%s\" [%s]\n", sectionTitle, g_EntryStateLookup[resume.m_ResumeErrorInEntry]);
            }
            else
            {
                printf("Error: resume could not be filled [%s]\n", g_EntryStateLookup[resume.m_ResumeErrorInEntry]);
            }
        }
    }
    else
    {
        exitCode = EXIT_RESUME_INIT_ERROR;
        if(resume.m_ResumeErrorInSection != INVALID_POSITION)
        {
            const char* sectionTitle = ResumeGetSectionTitle(&resume, resume.m_ResumeErrorInSection);
            printf("Error: failed to initialize resume data at section \"%s\"\n", sectionTitle);
        }
        else
        {
            printf("Error: failed to initialize resume data\n");
        }
    }
    ResumeDeInit(&resume);

    return exitCode;
}
