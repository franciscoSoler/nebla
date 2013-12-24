#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

class EventHandler {
public:
  // Hook method for the signal hook method.
  virtual int handleSignal(int signum) = 0;
};

#endif /* EVENT_HANDLER_H_ */ 
