#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:
    virtual void Update(float);
    virtual void UserUpdate(float);
    virtual void destroy();
    virtual ~Object();
};
#endif
