#ifndef _NRAASSERT_H_
#define _NRAASSERT_H_

#include <iostream>
#include <string>

class nraAssertion;

#define NRA_ASSERTION(type, desc, mesg, exp)						    \
    throw nraAssertion(type, #desc"("exp")", mesg, __FILE__, __LINE__)

#ifdef NRA_ASSERTIONS_ON
    #define NRA_REQUIRE(exp, message)							    \
        if(!(exp))									    \
        {										    \
            NRA_ASSERTION(nraAssertion::Require, Require, message, #exp);		    \
        }
    #define NRA_ENSURE(exp, message)							    \
        if(!(exp))									    \
        {										    \
            NRA_ASSERTION(nraAssertion::Ensure, Ensure, message, #exp);			    \
        }
    #define NRA_IMPLIES(exp1, exp2, message)						    \
        if((exp1))									    \
        {										    \
            if(!(exp2))									    \
            {										    \
                NRA_ASSERTION(nraAssertion::Implies, Implies, message, #exp1 "," #exp2);    \
            }										    \
        }
    #define NRA_INVARIANT								    \
        protected: virtual void nraInvariant() const					    \
        {
    #define NRA_END_INVARIANT								    \
        }
    #define NRA_CHECK_INVARIANT								    \
        nraInvariant()
    #define NRA_ASSERT(exp, message)							    \
        if (!(exp))									    \
        {										    \
            NRA_ASSERTION(nraAssertion::Assert, Assert, message, #exp);			    \
        }
    #define NRA_USES_OLD(Type)								    \
        Type NRA_OLD = *this
#else
    #define NRA_REQUIRE(exp, message)
    #define NRA_ENSURE(exp, message)
    #define NRA_IMPLIES(exp1, exp2, message)
    #define NRA_INVARIANT
    #define NRA_END_INVARIANT
    #define NRA_CHECK_INVARIANT
    #define NRA_ASSERT(exp, message)
    #define NRA_USES_OLD(Type)
#endif






class nraAssertion
{

public:
    enum AssertionType { Require, Ensure, Check, Assert, Implies};
    
    nraAssertion(AssertionType eType, const char* pReason, const char* pMessage, const char* pFile, int iLine)
    {
        m_tAssertionType = eType; 
        m_AssertionReason = pReason;
        m_AssertionMessage = pMessage;
        m_AssertionFile = pFile;
        m_iAssertionLine = iLine;
    };
    
    nraAssertion(const nraAssertion& Orig)
    {
        m_tAssertionType = Orig.GetType(); 
        m_AssertionReason = Orig.GetReason();
        m_AssertionMessage = Orig.GetMessage();
        m_AssertionFile = Orig.GetFile();
        m_iAssertionLine = Orig.GetLine();
    };

    virtual ~nraAssertion() {}

    nraAssertion& operator= (const nraAssertion& Rhs) 
    {
        if(this != &Rhs)
        {
            m_tAssertionType = Rhs.GetType(); 
            m_AssertionReason = Rhs.GetReason();
            m_AssertionMessage = Rhs.GetMessage();
            m_AssertionFile = Rhs.GetFile();
            m_iAssertionLine = Rhs.GetLine();
        }
        return *this;
    };
    
    friend std::ostream& operator<< (std::ostream& s, const nraAssertion& Assertion)
    {
        s << "Message: " << Assertion.GetMessage() << std::endl;
        s << "Reason: " << Assertion.GetReason() << " in" << std::endl;
        s << "File: " << Assertion.GetFile();
        s << ", Line: " << Assertion.GetLine() << std::endl;
        return s;
    };
    
    inline std::string GetReason() const { return m_AssertionReason; };
    inline std::string GetMessage() const { return m_AssertionMessage; }; 
    inline std::string GetFile() const { return m_AssertionFile; };
    inline int GetLine() const { return m_iAssertionLine; };
    inline AssertionType GetType() const { return m_tAssertionType; };
    
private:
    nraAssertion::AssertionType m_tAssertionType;
    std::string m_AssertionReason;
    std::string m_AssertionMessage;
    std::string m_AssertionFile;
    int m_iAssertionLine;

};
        
#endif



