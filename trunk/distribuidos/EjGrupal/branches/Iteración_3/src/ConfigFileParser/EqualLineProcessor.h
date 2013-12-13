#ifndef EQUALLINEPROCESSOR_H
#define	EQUALLINEPROCESSOR_H

#include "ILineProcessor.h"

class EqualLineProcessor : public ILineProcessor {
public:
    EqualLineProcessor();
    void addToChain(ILineProcessor::ptrLineProcessor lineProcessor);
    std::pair<std::string, std::string> processLine(std::string line);
    ILineProcessor::ptrLineProcessor getNextLineProcessor();
    virtual ~EqualLineProcessor();
    
private:
    EqualLineProcessor(const EqualLineProcessor &);
    int numberOfEqualsInLine(std::string line);
    std::pair<std::string, std::string> pairOfLineWithOneEqual(std::string line);
    
    ILineProcessor::ptrLineProcessor nextLineProcessor_;

};

#endif	/* EQUALLINEPROCESSOR_H */

