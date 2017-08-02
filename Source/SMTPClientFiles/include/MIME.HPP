// MIME.HPP
// provides C to CPP interface for SMTP protocol

// CPowerFtp is the C++ class that provides FTP processing

#ifndef MIME_HPP_DEFINED
#define MIME_HPP_DEFINED

#include <stdio.h>

class CMime 
{
private:
    int numBits1;
    int eof;
    WORD bc;            
    
    PBYTE pbBuf;
    PBYTE outBuf;
    PBYTE mBuf,omBuf;
    int outcount;
    char holdbuf[2048];
    
    HFILE hInfile;
    HFILE hOutfile;
public:
    void Encode(LPCSTR Infile, LPCSTR Outfile);
    void Decode(LPCSTR Infile, LPCSTR Outfile);

	// Constructor
	CMime();

	// Destructor
	~CMime(void);

protected:
//    unsigned long ReadBits() ;

    void WriteChar(char c);
    void PadMimeOutput(void);
	int  ReadChar(void);
    void WriteBits(unsigned bits, int n);
};

#endif // MIME_HPP_DEFINED
