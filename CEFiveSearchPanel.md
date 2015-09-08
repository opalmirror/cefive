# Search Panel #
The Search Panel is the interface to the Search Engine.  The user can enter a search query, start and cancel a search and view the first 100 results of a search with the Search Panel.

## Search Modes ##
### Immediate Value ###
_Immediate Value Mode is not currently implemented._
An Immediate Value search is used to find instructions within text or code segments that refer to or contain a specified immediate value.  The User enters the value to find as an immediate value.  Every instance of the immediate encountered in an instruction is marked as a result.

### Exact Value ###
An Exact Value search is used to find instances of an exact byte, word or doubleword in memory.  The User enters the value to find and the size of the value.  Every instance of the value is marked as a result.

### Text ###
_Text Mode is not currently implemented._
A Text Mode search is used to find character strings in memory.  The User enters the text string to find.  Every instance of the text string is marked as a result.

## Search Range ##
The Range of memory to search can be set in the Range Start and End fields.  These fields specify the starting address and ending address of the search and default to the starting and ending address of memory.  The Search Engine will not search outside of the specified range.