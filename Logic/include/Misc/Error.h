#ifndef ERROR_H
#define ERROR_H



// Print & Handle Possible Errors
#define _SAFE_EXECUTION

#define _ERROR(Result) if (Result != _OK) { printf(GetErrorString(Result)); printf("In File: %s\n Line: %d, Time: %s\n", __FILE__, __LINE__, __TIME__); }

enum _RESULT
{
    _OK,
    _NULLPTR,
    _INVALID_INDEX, 
    _FILE_NOT_FOUND, 
    _JUST_FOR_YOU
};

inline char* GetErrorString(_RESULT result)
{
    switch (result)
    {
    case _OK:                           return "\n- OK.\n"; break;
    case _NULLPTR:                      return "\n- Invalid pointer was used as a parameter.\n"; break;
    case _INVALID_INDEX:                return "\n- Index is out of bounds.\n"; break;
    case _FILE_NOT_FOUND:               return "\n- File not found.\n"; break;
    case _JUST_FOR_YOU:                 return "\n- I see what you're doing.\n"; break;
    default:                            return "\n- Unknown Error.\n"; break;
    }

    return "Compiler Error?\n";
}

#endif // !ERROR_H