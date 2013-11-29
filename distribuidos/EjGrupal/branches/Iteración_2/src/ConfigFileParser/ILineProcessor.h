#ifndef ILINEPROCESSOR_H
#define	ILINEPROCESSOR_H

#include <string>

class ILineProcessor {
public: 
    //typedef std::auto_ptr<ILineProcessor> ptrLineProcessor;
    typedef ILineProcessor* ptrLineProcessor;
    
    virtual void addToChain(ptrLineProcessor lineProcessor) = 0;
    virtual std::pair<std::string, std::string> processLine(std::string line) = 0;
    virtual ptrLineProcessor getNextLineProcessor() = 0;
    virtual ~ILineProcessor() {};
};



#endif	/* ILINEPROCESSOR_H */

