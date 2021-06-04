#ifndef CONTROLLER_H
#define CONTROLLER_H

class QPainter;
namespace MaskConfig {

class Controller
{
public:
    Controller();
    virtual ~Controller() {}


    virtual int enter() {return 0;}
    virtual int leave() {return 0;}
    virtual int resize(int w, int h) {return 0;}
    virtual int mouseMove(int posX, int posY) {return 0;}
    virtual int mousePress(int posX,int posY, unsigned int button, unsigned int modifier = 0) { button_ = button; modifier_ = modifier; return 0;}
    virtual int mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier = 0) {button_ = 0; modifier = 0; return 0;}
    virtual int keyUp(const unsigned int& key, unsigned int modifier = 0) {return 0;}
    virtual int keyDown(const unsigned int& key, unsigned int modifier = 0) {return 0;}
    virtual int wheel(const float& delta) {return 0;}

    virtual int paint(QPainter& painter) {return 0;}

    int getButton()  const {return button_;}
    int getModifier() const {return modifier_;}
protected:
    unsigned int button_;
    unsigned int modifier_;

};
}
#endif // CONTROLLER_H
