#include "Signal.hpp"
#include <iostream>

class Button{
    public:
    Signal <> on_click;
    void Click()
    {
        on_click.emit();
    }
};

class Message{
    public:
    void display() const{
        std::cout << "Hello World!" << std:: endl;
    }
};

int main()
{
    Button button;
    Message message;

    button.on_click.connect_member(&message, &Message::display);
    button.Click();
    return 0;
}