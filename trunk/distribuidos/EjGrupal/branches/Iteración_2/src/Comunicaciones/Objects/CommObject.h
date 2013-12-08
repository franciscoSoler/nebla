#ifndef COMMOBJECT_H
#define COMMOBJECT_H

#include <string>

class CommObject {
public:
    CommObject(std::string CommName);
    virtual ~CommObject();
    std::string getCommName() const;
    void setCommName(std::string name);

protected:
    void findCommId(std::string key);

protected:
    std::string commName_;
    int commId_;
};

#endif // COMMOBJECT_H
