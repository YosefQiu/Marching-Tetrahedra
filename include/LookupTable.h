#pragma once
#include "ggl.h"

class LookupTable
{
public:
    LookupTable() = default;
    virtual ~LookupTable() = default;
public:
    struct lookupTbl
    {
        int caseNumber;
        int labelNumber;
        std::string binaryCode;
        int AIndex, BIndex, CIndex, DIndex;
    };
    static lookupTbl EvaluateCase(int A, int B, int C, int D);
    static std::string GenerateBinaryCode(int a, int b, int c, int d);
};