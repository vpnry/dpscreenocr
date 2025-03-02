
#pragma once

#include "backend/screenshot.h"
#include "geometry.h"


namespace dpso::backend {


class NullScreenshot : public Screenshot {
public:
    explicit NullScreenshot(const Rect& rect);

    int getWidth() const override;
    int getHeight() const override;

    void getGrayscaleData(std::uint8_t* buf, int pitch) const override;
private:
    int w;
    int h;
};


}
