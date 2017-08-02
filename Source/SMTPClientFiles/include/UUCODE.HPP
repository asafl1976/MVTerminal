// UUCODE.HPP
// lets test to make sure this works correctly
// jeff steele 8/31/95
 

#ifndef UUCODE_HPP_DEFINED
#define UUCODE_HPP_DEFINED

#include <stdio.h>

class CUucode 
{
public:
    void Encode(LPCSTR Infile, LPCSTR Outfile);
    void Decode(LPCSTR Infile, LPCSTR Outfile);

	// Constructor
	CUucode();

	// Destructor
	~CUucode(void);


protected:
    void outdec(char *p, HFILE ofile);
    void indec(char *bp, HFILE ofile, int n);
private:
    int numBits1;
    int eof;
    int bc;            
    
    PBYTE pbBuf;
    PBYTE outBuf;
    PBYTE mBuf,omBuf;
    int outcount;
    char holdbuf[80];
    
    HFILE hInfile;
    HFILE hOutfile;

};

#endif // UUCODE_HPP_DEFINED
