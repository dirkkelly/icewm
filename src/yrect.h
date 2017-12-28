#ifndef __YRECT_H
#define __YRECT_H

#ifndef INT_MAX
#include <limits.h>
#endif

class YRect {
public:
    YRect() : xx(0), yy(0), ww(0), hh(0) { }
    YRect(int x, int y, unsigned w, unsigned h)
        :xx(x), yy(y), ww(w), hh(h)
    {
        PRECONDITION(ww < INT_MAX);
        PRECONDITION(hh < INT_MAX);
    }

    int x() const { return xx; }
    int y() const { return yy; }
    unsigned width() const { return ww; }
    unsigned height() const { return hh; }
    unsigned pixels() const { return ww * hh; }

    void setRect(int x, int y, unsigned w, unsigned h) {
        xx = x;
        yy = y;
        ww = w;
        hh = h;
    }

    // does the same as gdk_rectangle_union
    void unionRect(int x, int y, unsigned width, unsigned height) {
        int mx = min(xx, x), w = int(max(xx + ww, x + width));
        int my = min(yy, y), h = int(max(yy + hh, y + height));
        setRect(mx, my, w - mx, h - my);
    }

    YRect intersect(const YRect& r) {
        int x = max(xx, r.xx), w = int(min(xx + ww, r.xx + r.ww));
        int y = max(yy, r.yy), h = int(min(yy + hh, r.yy + r.hh));
        if (x < w && y < h)
            return YRect(x, y, unsigned(w - x), unsigned(h - y));
        return YRect();
    }

private:
    int xx, yy;
    unsigned ww, hh;
};

#endif

// vim: set sw=4 ts=4 et:
