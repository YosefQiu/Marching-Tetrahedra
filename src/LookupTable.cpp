#include "LookupTable.h"
#include <Log.hpp>

LookupTable::lookupTbl LookupTable::EvaluateCase(int A, int B, int C, int D)
{
	lookupTbl info;
    std::string str = GenerateBinaryCode(A, B, C, D);
    info.binaryCode = str;
    info.caseNumber = std::stoull(str, 0, 2);

    std::unordered_set<int> validCaseNumbers = {0, 3, 8, 10, 11, 16, 17, 19, 20, 21, 23, 24, 25, 26, 27};

  
    int a_index = 0, b_index = 1, c_index = 2, d_index = 3;
    switch (info.caseNumber)
    {
    case  0: info.AIndex = a_index; info.BIndex = b_index; info.CIndex = c_index; info.DIndex = d_index; break;
    case  3: info.AIndex = d_index; info.BIndex = a_index; info.CIndex = b_index; info.DIndex = c_index; break;
    case  8: info.AIndex = c_index; info.BIndex = a_index; info.CIndex = b_index; info.DIndex = d_index; break;
    case 10: info.AIndex = a_index; info.BIndex = b_index; info.CIndex = c_index; info.DIndex = d_index; break;
    case 11: info.AIndex = a_index; info.BIndex = b_index; info.CIndex = c_index; info.DIndex = d_index; break;
    case 16: info.AIndex = b_index; info.BIndex = a_index; info.CIndex = c_index; info.DIndex = d_index; break;
    case 17: info.AIndex = a_index; info.BIndex = c_index; info.CIndex = b_index; info.DIndex = d_index; break;
    case 19: info.AIndex = a_index; info.BIndex = c_index; info.CIndex = b_index; info.DIndex = d_index; break;
    case 20: info.AIndex = a_index; info.BIndex = d_index; info.CIndex = b_index; info.DIndex = c_index; break;
    case 21: info.AIndex = a_index; info.BIndex = b_index; info.CIndex = c_index; info.DIndex = d_index; break;
    case 23: info.AIndex = b_index; info.BIndex = c_index; info.CIndex = a_index; info.DIndex = d_index; break;
    case 24: info.AIndex = a_index; info.BIndex = d_index; info.CIndex = b_index; info.DIndex = c_index; break;
    case 25: info.AIndex = b_index; info.BIndex = d_index; info.CIndex = a_index; info.DIndex = c_index; break;
    case 26: info.AIndex = c_index; info.BIndex = d_index; info.CIndex = a_index; info.DIndex = b_index; break;
    case 27: info.AIndex = a_index; info.BIndex = b_index; info.CIndex = c_index; info.DIndex = d_index; break;
    default:
        info.AIndex = -1; info.BIndex = -1; info.CIndex = -1; info.DIndex = -1;
        break;
    }
    return info;
}

std::string LookupTable::GenerateBinaryCode(int a, int b, int c, int d)
{
    std::string binaryCode = "nnnnn";

    // b ∈ {0, 1} 
    binaryCode[0] = b == a ? '0' : '1';

    // c ∈ {0,1,2} 
    if (c == a) 
    {
        binaryCode[1] = '0';
        binaryCode[2] = '0';
    } 
    else if (c == b) 
    {
        binaryCode[1] = '0';
        binaryCode[2] = '1';
    } else 
    {
        binaryCode[1] = '1';
        binaryCode[2] = '0';
    }

    // d ∈ {0,1,2,3} 
    if (d == a) 
    {
        binaryCode[3] = '0';
        binaryCode[4] = '0';
    } 
    else if (d == b) 
    {
        binaryCode[3] = '0';
        binaryCode[4] = '1';
    } 
    else if (d == c) 
    {
        binaryCode[3] = '1';
        binaryCode[4] = '0';
    } 
    else 
    {
        binaryCode[3] = '1';
        binaryCode[4] = '1';
    }

    if (std::find(binaryCode.begin(), binaryCode.end(), 'n') != binaryCode.end())
    {
        Debug("LOOKUPTABLE::produced an unexpected value %s %d %d %d %d", binaryCode.c_str(), a, b, c, d);
    }

    return binaryCode;
}
