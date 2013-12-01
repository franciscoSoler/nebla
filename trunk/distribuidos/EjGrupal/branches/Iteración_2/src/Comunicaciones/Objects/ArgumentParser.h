#ifndef ARGUMENTPARSER_H
#define	ARGUMENTPARSER_H

class ArgumentParser {
public:
    ArgumentParser(int argc, char** argv);
    int parseArgument(int index, int & argument);
    int parseArgument(int index, long & argument);
    int parseArgument(int index, char argument[], int argumentSize);
    
private:
    bool isIndexValid(int index) {
        if (index > argc_ || index < 1) {
            return false;
        }
        return true;
    }
private:
    int argc_;
    char** argv_;
};

#endif	/* ARGUMENTPARSER_H */

